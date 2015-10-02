#include <iostream>
#include <fstream>
#include <cstring> // strcpy(), strtok()
#include <cstdlib> // atoi()
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;


const int MAX_CANDIDATES = 20;
const int MAX_NAME       = 80;
const int MAX_BALLOTS    = 400;


class NameList
{
public:
    char _list[MAX_CANDIDATES][MAX_NAME];
    int  _count;

    void print();
};


// print this list of names
void NameList::print()
{
    for (int i = 0; i < _count; i++)
        cout << _list[i] << endl;
}


class Ballot
{
public:
    int _list[MAX_CANDIDATES];
    int _count;


    Ballot() { reset(); }
    Ballot(char* line);
    Ballot(const Ballot& that) { *this = that; }
    Ballot& operator=(const Ballot& that);

    void reset() { _count = 0; }
    void print();
    void shiftLeft();
};


// initialize this ballot with integers extracted from a string
Ballot::Ballot(char* line)
{
    // use a temporary copy to preserve the input line
    char tmp[MAX_LINE];
    strcpy(tmp, line);
    char* token = strtok(tmp, " ");
    _count = 0;
    while (token != 0)
    {
        _list[_count] = atoi(token);
        _count++;
        token = strtok(0, " \n");
    }
}


Ballot& Ballot::operator=(const Ballot& that)
{
    for (int i = 0; i < that._count; i++)
        _list[i] = that._list[i];
    _count = that._count;
}


// print this ballot
void Ballot::print()
{
    for (int i = 0; i < _count; i++)
        cout << _list[i] << " ";
    cout << endl;
}


// shift all integers in this ballot to the left, so that the leftmost integer
// becomes the rightmost integer, and all other integers move one position
// to the left
void Ballot::shiftLeft()
{
    int out = _list[0];
    for (int i = 0; i < _count-1; i++)
        _list[i] = _list[i+1];
    _list[_count-1] = out;
}


class CandidateList;
class BallotList
{
public:
    Ballot _list[MAX_BALLOTS];
    int    _count;


    BallotList() { reset(); }
    BallotList(const BallotList& that) { *this = that; }
    BallotList& operator=(const BallotList& that);

    void reset() { _count = 0; }
    void add(Ballot* ballot);
    void add(BallotList* that);
    void add(char* line);
    void print();
    void shiftLeft(CandidateList* candidates);
};


// initialize this list of ballots with another
BallotList& BallotList::operator=(const BallotList& that)
{
    for (int i = 0; i < that._count; i++)
        _list[i] = that._list[i];
    _count = that._count;

    return *this;
}


// add one ballot into this list of ballots
void BallotList::add(Ballot* ballot)
{
    _list[_count] = *ballot;
    _count++;
}


// go through the list of ballots and record all the ballots whose first
// choice is the current candidate
void BallotList::add(BallotList* that)
{
    for (int i = 0; i < that->_count; i++)
        if (that->_list[i]._list[0] == _list[0]._list[0])
            add(that->_list+i);
}


// add one ballot into this list of ballots, based on string contents
void BallotList::add(char* line)
{
    _list[_count] = Ballot(line);
    _count++;
}


// print all ballots in this list
void BallotList::print()
{
    for (int i = 0; i < _count; i++)
    {
        cout << "BALLOT " << i << ": ";
        _list[i].print();
    }
}


class Candidate
{
public:
    char       _name[MAX_NAME];
    bool       _active;
    BallotList _ballots;


    Candidate() {}
    Candidate(char* name);
    Candidate(const Candidate& that) { *this = that; }
    Candidate& operator=(const Candidate& that);

    void addBallots(BallotList* ballots);
    void print();
};


class CandidateList
{
public:
    Candidate _list[MAX_CANDIDATES];
    int       _count;


    CandidateList() { _count = 0; }
    void print();
    bool findWinner(int num_ballots);
};


// shift all ballots in this list to the left
// this method is defined after the declaration of CandidateList, to avoid
// a forward declaration
void BallotList::shiftLeft(CandidateList* candidates)
{
    for (int i = 0; i < _count; i++)
    {
        int index = _list[i]._list[0] - 1;
        while (candidates->_list[index]._active == false)
        {
            _list[i].shiftLeft();
            index = _list[i]._list[0] - 1;
        }
    }
}


class Input
{
public:
    NameList   _names;
    BallotList _ballots;


    void print();
};


// print contents of the current test case, including all candidates names
// and all ballots
void Input::print()
{
    cout << _names._count << " CANDIDATES" << endl;
    _names.print();
    cout << _ballots._count << " BALLOTS" << endl;
    _ballots.print();
}


// To get an idea as to how much memory this program consumes, here's
// a breakdown of the sizes of the various structs:
//     sizeof(NameList): 1604
//     sizeof(Ballot): 84
//     sizeof(BallotList): 33604
//     sizeof(Input): 35208
//     sizeof(Candidate): 33688
//     sizeof(CandidateList): 673764
//     sizeof(IndexList): 84
// So an array of 100 (MAX_CASES) of such cases will cause a seg fault.
// So I need to scale the size of the list from 100 to 10 cases at the most
// Input list[MAX_CASES];
class InputList
{
public:
    Input _list[10];
    int   _count;
};


// initialize the current candidate with a name, a status, and a list of ballots
Candidate::Candidate(char* name)
{
    strcpy(_name, name);
    _active = true;
}


Candidate& Candidate::operator=(const Candidate& that)
{
    strcpy(_name, that._name);
    _active = that._active;
    _ballots = that._ballots;
}


// add a list of ballots into this candidate
void Candidate::addBallots(BallotList* ballots)
{
    if (_active)
        _ballots.add(ballots);
}


// print contents of the current candidate
void Candidate::print()
{
    cout << _name << ", active " << (_active ? "true" : "false") <<
            ", votes " << _ballots._count << endl;
}


// print all candidates in this list
void CandidateList::print()
{
    for (int i = 0; i < _count; i++)
        _list[i].print();
}


// from among this list of candidates, find a candidate whose total ballot
// count is more than 50% of the total ballot count
bool CandidateList::findWinner(int num_ballots)
{
    for (int i = 0; i < _count; i++)
        if (_list[i]._ballots._count / (float)num_ballots >= .5)
        {
            cout << _list[i]._name << endl;
            return true;
        }
    if (_debug >= DEBUG_LIGHT)
        cout << "NO WINNER" << endl;

    return false;
}


class IndexList
{
public:
    int _list[MAX_CANDIDATES];
    int _count;


    void print(CandidateList* candidates);
};


// print the name and ballot count of all the losing candidates
void IndexList::print(CandidateList* candidates)
{
    cout << _count << " LOSERS:" << endl;
    for (int i = 0; i < _count; i++)
    {
        Candidate* candidate = candidates->_list + _list[i];
        candidate->print();
        candidate->_ballots.print();
    }
}


bool input(char* input_file, InputList* inputs)
{
    bool success;
    ifstream file(input_file);
    if (file.is_open() == false)
    {
        cout << "Cannot open file " << input_file << endl;
        success = false;
    }
    else
    {
        // read the number of test cases
        char line[MAX_LINE];
        file.getline(line, MAX_LINE);
        inputs->_count = atoi(line);


        // read a blank line
        file.getline(line, MAX_LINE);


        for (int i = 0; i < inputs->_count; i++)
        {
            // read the count of the candidates
            Input* current = inputs->_list + i;
            file.getline(line, MAX_LINE);
            current->_names._count = atoi(line);


            // read all candidates' names
            for (int j = 0; j < current->_names._count; j++)
            {
                file.getline(line, MAX_LINE);
                strcpy(current->_names._list[j], line);
            }


            // read all ballots, one line at a time. stop when EOF or
            // empty line is fetched
            while (file.getline(line, MAX_LINE) && strlen(line))
                current->_ballots.add(line);
        }

        file.close();
        success = true;

        if (_debug >= DEBUG_LIGHT)
        {
            cout << "READ " << inputs->_count << " CASES" << endl;
            for (int i = 0; i < inputs->_count; i++)
            {
                cout << "CASE " << i << endl;
                inputs->_list[i].print();
            }
        }
    }

    return success;
}


void output(InputList* inputs)
{
    for (int i = 0; i < inputs->_count; i++)
    {
        if (_debug >= DEBUG_LIGHT)
            cout << "PROCESSING CASE " << i << endl;
        CandidateList candidates;
        Input* input = inputs->_list + i;


        // go through the list of candidates' names
        for (int j = 0; j < input->_names._count; j++)
        {
            // save the input candidate's name
            Candidate* candidate = candidates._list+j;
            *candidate = Candidate(input->_names._list[j]);


            // go through the list of ballots and record all the ballots
            // whose first choice is the current candidate
            for (int k = 0; k < input->_ballots._count; k++)
                if (input->_ballots._list[k]._list[0] == j+1)
                    candidate->_ballots.add(input->_ballots._list+k);
            if (_debug >= DEBUG_LIGHT)
            {
                cout << "CANDIDATE " << candidate->_name << ", with "
                     << candidate->_ballots._count << " ballots:" << endl;
                candidate->_ballots.print();
            }


            // save the current candidate
            candidates._count++;
        }
        if (_debug >= DEBUG_LIGHT)
        {
            cout << "ORIGINAL CANDIDATES:" << endl;
            candidates.print();
        }


        // while unable to find the winner
        while (candidates.findWinner(input->_ballots._count) == false)
        {
            // find the losers
            int min_votes = input->_ballots._count;
            IndexList indices;
            for (int k = 0; k < candidates._count; k++)
            {
                if (candidates._list[k]._active)
                {
                    if (candidates._list[k]._ballots._count < min_votes)
                    {
                        indices._list[0] = k;
                        indices._count = 1;
                        min_votes = candidates._list[k]._ballots._count;
                    }
                    else if (candidates._list[k]._ballots._count == min_votes)
                    {
                        indices._list[indices._count] = k;
                        indices._count++;
                    }
                }
            }
            if (_debug >= DEBUG_LIGHT)
                indices.print(&candidates);


            // deactive the losers
            for (int k = 0; k < indices._count; k++)
                candidates._list[indices._list[k]]._active = false;


            for (int k = 0; k < indices._count; k++)
            {
                // from the list of ballots of the deactivated losers,
                // promote candidates from second choice to first choice
                int idx = indices._list[k];
                BallotList* ballots = &candidates._list[idx]._ballots;
                ballots->shiftLeft(&candidates);
                if (_debug >= DEBUG_LIGHT)
                {
                    cout << candidates._list[idx]._name
                         << ", POST-SHIFT:" << endl;
                    ballots->print();
                }


                // add all ballots of the newly-promoted first choice
                // candidates to the existing first-choicers
                for (int l = 0; l < candidates._count; l++)
                    candidates._list[l].addBallots(ballots);
                if (_debug >= DEBUG_COPIOUS)
                {
                    cout << "CANDIDATES, POST-ADD:" << endl;
                    candidates.print();
                }


                // reset the ballot count for the current deactivated loser
                ballots->reset();
            }
            if (_debug >= DEBUG_LIGHT)
            {
                cout << "REVISED CANDIDATES:" << endl;
                candidates.print();
            }
        }
    }
}


int main(int argc, char** argv)
{
    char opt;
    char* input_file = NULL;

    while ((opt = getopt(argc, argv, "i:d:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            input_file = optarg;
            break;
        case 'd':
            _debug = atoi(optarg);
            break;
        default:
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]"
                 << endl;
        }
    }

    if (input_file == NULL || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
    else
    {
        InputList inputs;
        if (input(input_file, &inputs))
            output(&inputs);

        if (_debug >= DEBUG_COPIOUS)
        {
            cout <<"sizeof(NameList): " << sizeof(NameList) << endl;
            cout <<"sizeof(Ballot): " << sizeof(Ballot) << endl;
            cout <<"sizeof(BallotList): " << sizeof(BallotList) << endl;
            cout <<"sizeof(Input): " << sizeof(Input) << endl;
            cout <<"sizeof(InputList): " << sizeof(InputList) << endl;
            cout <<"sizeof(Candidate): " << sizeof(Candidate) << endl;
            cout <<"sizeof(CandidateList): " << sizeof(CandidateList) << endl;
            cout <<"sizeof(IndexList): " << sizeof(IndexList) << endl;
        }
    }

    return 0;
}