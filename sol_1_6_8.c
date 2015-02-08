#include <stdio.h>
#include <string.h>
#include <unistd.h> // optarg
#include "defs.h"


int _debug = DEBUG_OFF;


typedef int bool;
#define false 0
#define true  1


#define MAX_CANDIDATES 20
#define MAX_NAME       80
#define MAX_BALLOTS    400


struct NameList
{
    char _list[MAX_CANDIDATES][MAX_NAME];
    int  _count;
};
typedef struct NameList NameList;


struct Ballot
{
    int _list[MAX_CANDIDATES];
    int _count;
};
typedef struct Ballot Ballot;


struct BallotList
{
    Ballot _list[MAX_BALLOTS];
    int    _count;
};
typedef struct BallotList BallotList;


struct Input
{
    NameList   _names;
    BallotList _ballots;
};
typedef struct Input Input;


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
struct InputList
{
    Input _list[10];
    int   _count;
};
typedef struct InputList InputList;


struct Candidate
{
    char        _name[MAX_NAME];
    bool        _active;
    BallotList _ballots;
};
typedef struct Candidate Candidate;


struct CandidateList
{
    Candidate _list[MAX_CANDIDATES];
    int       _count;
};
typedef struct CandidateList CandidateList;


struct IndexList
{
    int _list[MAX_CANDIDATES];
    int _count;
};
typedef struct IndexList IndexList;


// print this list of names
void nl_print(NameList* names)
{
    int i;
    for (i = 0; i < names->_count; i++)
        printf("%s\n", names->_list[i]);
}


// initialize this ballot with integers extracted from a string
void b_init_from_string(Ballot* ballot, char* line)
{
    // use a temporary copy to preserve the input line
    char tmp[MAX_LINE];
    strcpy(tmp, line);
    char* token = strtok(tmp, " ");
    ballot->_count = 0;
    while (token != 0)
    {
        ballot->_list[ballot->_count] = atoi(token);
        ballot->_count++;
        token = strtok(0, " \n");
    }
}


// initialize this ballot with another ballot
void b_init_from_ballot(Ballot* this, Ballot* that)
{
    int i;
    for (i = 0; i < that->_count; i++)
        this->_list[i] = that->_list[i];
    this->_count = that->_count;
}


// print this ballot
void b_print(Ballot* ballot)
{
    int i;
    for (i = 0; i < ballot->_count; i++)
        printf("%d ", ballot->_list[i]);
    printf("\n");
}


// shift all integers in this ballot to the left, so that the leftmost integer
// becomes the rightmost integer, and all other integers move one position
// to the left
void b_shift_left(Ballot* ballot)
{
    int out = ballot->_list[0];
    int i;
    for (i = 0; i < ballot->_count-1; i++)
        ballot->_list[i] = ballot->_list[i+1];
    ballot->_list[ballot->_count-1] = out;
}


// initialize this list of ballots with another
void bl_init(BallotList* this, BallotList* that)
{
    if (that == 0)
        this->_count = 0;
    else
    {
        int i;
        for (i = 0; i < that->_count; i++)
            b_init_from_ballot(this->_list+i, that->_list+i);
        this->_count = that->_count;
    }
}


// add one ballot into this list of ballots
void bl_add_ballot(BallotList* list, Ballot* ballot)
{
    b_init_from_ballot(list->_list+list->_count, ballot);
    list->_count++;
}


// go through the list of ballots and record all the ballots whose first
// choice is the current candidate
void bl_add_ballots(BallotList* this, BallotList* that)
{
    int i;
    for (i = 0; i < that->_count; i++)
        if (that->_list[i]._list[0] == this->_list[0]._list[0])
            bl_add_ballot(this, that->_list+i);
}


// add one ballot into this list of ballots, based on string contents
void bl_add_string(BallotList* list, char* line)
{
    b_init_from_string(list->_list+list->_count, line);
    list->_count++;
}


// print all ballots in this list
void bl_print(BallotList* ballots)
{
    int i;
    for (i = 0; i < ballots->_count; i++)
    {
        printf("BALLOT %d: ", i);
        b_print(ballots->_list + i);
    }
}


// shift all ballots in this list to the left
void bl_shift_left(BallotList* ballots, CandidateList* candidates)
{
    int i;
    for (i = 0; i < ballots->_count; i++)
    {
        int index = ballots->_list[i]._list[0] - 1;
        while (candidates->_list[index]._active == false)
        {
            b_shift_left(ballots->_list+i);
            index = ballots->_list[i]._list[0] - 1;
        }
    }
}


// print contents of the current test case, including all candidates names
// and all ballots
void cs_print(Input* record)
{
    printf("%d CANDIDATES\n", record->_names._count);
    nl_print(&record->_names);
    printf("%d BALLOTS\n", record->_ballots._count);
    bl_print(&record->_ballots);
}


// initialize the current candidate with a name, a status, and a list of ballots
void cn_init(Candidate* candidate, char* name)
{
    strcpy(candidate->_name, name);
    candidate->_active = true;
    bl_init(&candidate->_ballots, 0);
}


// add a list of ballots into this candidate
void cn_add_ballots(Candidate* candidate, BallotList* ballots)
{
    if (candidate->_active)
        bl_add_ballots(&candidate->_ballots, ballots);
}


// print contents of the current candidate
void cn_print(Candidate* candidate)
{
    printf("%s, active %s, votes %d\n",
           candidate->_name,
           (candidate->_active ? "true" : "false"),
           candidate->_ballots._count);
}


// initialize this list of candidates
void cl_init(CandidateList* list)
{
    list->_count = 0;
}


// print all candidates in this list
void cl_print(CandidateList* candidates)
{
    int i;
    for (i = 0; i < candidates->_count; i++)
        cn_print(candidates->_list + i);
}


// from among this list of candidates, find a candidate whose total ballot
// count is more than 50% of the total ballot count
bool cl_find_winner(CandidateList* candidates, int num_ballots)
{
    int i;
    for (i = 0; i < candidates->_count; i++)
        if (candidates->_list[i]._ballots._count / (float)num_ballots >= .5)
        {
            printf("%s\n", candidates->_list[i]._name);
            return true;
        }
    if (_debug >= DEBUG_LIGHT)
        printf("NO WINNER\n");

    return false;
}


// print the name and ballot count of all the losing candidates
void il_print(IndexList* indices, CandidateList* candidates)
{
    printf("%d LOSERS:\n", indices->_count);
    int i;
    for (i = 0; i < indices->_count; i++)
    {
        Candidate* candidate = candidates->_list + indices->_list[i];
        cn_print(candidate);
        bl_print(&candidate->_ballots);
    }
}


bool input(char* input_file, InputList* inputs)
{
    bool success;
    FILE* file = fopen(input_file, "r");
    if (file == 0)
    {
        printf("Cannot open file %s\n", input_file);
        success = false;
    }
    else
    {
        // read the number of test cases
        char line[MAX_LINE];
        fgets(line, MAX_LINE, file);
        inputs->_count = atoi(line);


        // read a blank line
        fgets(line, MAX_LINE, file);


        int i;
        for (i = 0; i < inputs->_count; i++)
        {
            // read the count of the candidates
            Input* current = inputs->_list + i;
            fgets(line, MAX_LINE, file);
            current->_names._count = atoi(line);


            // read all candidates' names
            int j;
            for (j = 0; j < current->_names._count; j++)
            {
                fgets(line, MAX_LINE, file);
                strncpy(current->_names._list[j], line, strlen(line)-1);
            }


            // read all ballots, one line at a time. stop when EOF or
            // empty line is fetched
            bl_init(&current->_ballots, 0);
            while (fgets(line, MAX_LINE, file) != 0 && line[0] != '\n')
                bl_add_string(&current->_ballots, line);
        }

        fclose(file);
        success = true;

        if (_debug >= DEBUG_LIGHT)
        {
            printf("READ %d CASES\n", inputs->_count);
            int i;
            for (i = 0; i < inputs->_count; i++)
            {
                printf("CASE %d\n", i);
                cs_print(inputs->_list + i);
            }
        }
    }

    return success;
}


void output(InputList* inputs)
{
    int i;
    for (i = 0; i < inputs->_count; i++)
    {
        if (_debug >= DEBUG_LIGHT)
            printf("PROCESSING CASE %d\n", i);
        CandidateList candidates;
        cl_init(&candidates);
        Input* input = inputs->_list + i;


        // go through the list of candidates' names
        int j;
        for (j = 0; j < input->_names._count; j++)
        {
            // save the input candidate's name
            Candidate* candidate = candidates._list+j;
            cn_init(candidate, input->_names._list[j]);


            // go through the list of ballots and record all the ballots
            // whose first choice is the current candidate
            int k;
            for (k = 0; k < input->_ballots._count; k++)
                if (input->_ballots._list[k]._list[0] == j+1)
                   bl_add_ballot(&candidate->_ballots, input->_ballots._list+k);
            if (_debug >= DEBUG_LIGHT)
            {
                printf("CANDIDATE %s, with %d ballots:\n", candidate->_name,
                       candidate->_ballots._count);
                bl_print(&candidate->_ballots);
            }


            // save the current candidate
            candidates._count++;
        }
        if (_debug >= DEBUG_LIGHT)
        {
            printf("ORIGINAL CANDIDATES:\n");
            cl_print(&candidates);
        }


        // while unable to find the winner
        while (cl_find_winner(&candidates, input->_ballots._count) == false)
        {
            // find the losers
            int min_votes = input->_ballots._count;
            IndexList indices;
            int k;
            for (k = 0; k < candidates._count; k++)
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
                il_print(&indices, &candidates);


            // deactive the losers
            for (k = 0; k < indices._count; k++)
                candidates._list[indices._list[k]]._active = false;


            for (k = 0; k < indices._count; k++)
            {
                // from the list of ballots of the deactivated losers,
                // promote candidates from second choice to first choice
                int idx = indices._list[k];
                BallotList* ballots = &candidates._list[idx]._ballots;
                bl_shift_left(ballots, &candidates);
                if (_debug >= DEBUG_LIGHT)
                {
                    printf("%s, POST-SHIFT:\n",
                           candidates._list[idx]._name);
                    bl_print(ballots);
                }


                // add all ballots of the newly-promoted first choice
                // candidates to the existing first-choicers
                int l;
                for (l = 0; l < candidates._count; l++)
                    cn_add_ballots(candidates._list+l, ballots);
                if (_debug >= DEBUG_COPIOUS)
                {
                    printf("CANDIDATES, POST-ADD:\n");
                    cl_print(&candidates);
                }


                // reset the ballot count for the current deactivated loser
                bl_init(ballots, 0);
            }
            if (_debug >= DEBUG_LIGHT)
            {
                printf("REVISED CANDIDATES:\n");
                cl_print(&candidates);
            }
        }
    }
}


void main(int argc, char** argv)
{
    char opt;
    char* input_file = 0;

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
            printf("Usage: %s -i input_file [-d 0|1|2]\n", argv[0]);
        }
    }

    if (input_file == 0 || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        printf("Usage: %s -i input_file [-d 0|1|2]\n", argv[0]);
    else
    {
        InputList inputs;
        if (input(input_file, &inputs))
            output(&inputs);

        if (_debug >= DEBUG_COPIOUS)
        {
            printf("sizeof(NameList): %d\n", sizeof(NameList));
            printf("sizeof(Ballot): %d\n", sizeof(Ballot));
            printf("sizeof(BallotList): %d\n", sizeof(BallotList));
            printf("sizeof(Input): %d\n", sizeof(Input));
            printf("sizeof(InputList): %d\n", sizeof(InputList));
            printf("sizeof(Candidate): %d\n", sizeof(Candidate));
            printf("sizeof(CandidateList): %d\n", sizeof(CandidateList));
            printf("sizeof(IndexList): %d\n", sizeof(IndexList));
        }
    }
}
