#include <iostream>
#include <fstream>
#include <cstring> // strcpy(), strtok()
#include <cstdlib> // atoi()
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;


class Input
{
public:
    int* _list;
    int  _size;

    Input();
    Input(int size);
    Input& operator=(const Input& that);
   ~Input();
    void print();
};


Input::Input()
{
    _list = 0;
    _size = 0;
}


Input::Input(int size)
{
    _list = new int[size];
    _size = size;
}


Input& Input::operator=(const Input& that)
{
    _list = new int[that._size];
    for (int i = 0; i < that._size; i++)
        _list[i] = that._list[i];
    _size = that._size;

    return *this;
}


Input::~Input()
{
    if (_list)
        delete _list;
}


void Input::print()
{
    cout << _size;
    for (int i = 0; i < _size; i++)
        cout << " " << _list[i];
    cout << endl;
}


class InputList
{
public:
    Input _list[MAX_CASES];
    int   _size;

    InputList() { _size = 0; }
   ~InputList() {}
};


class Difference
{
public:
    bool _list[1000];
    int  _size;


    Difference(int size);
    void print();
    bool scan();
};


Difference::Difference(int size)
{
    for (int i = 0; i < size; i++)
        _list[i] = false;
    _size = size;
}


void Difference::print()
{
    cout << "Difference entries:" << endl;
    for (int i = 1; i < _size; i++)
        cout << _list[i] << " ";
    cout << endl;
}


// scan the visited array to see if any number was not represented
bool Difference::scan()
{
    for (int i = 1; i < _size; i++)
        if (_list[i] == false)
        {
            if (_debug >= DEBUG_COPIOUS)
                cout << "Entry " << i << " not represented" << endl;
            return false;
        }

    return true;
}


bool is_jolly(Input* input)
{
    // a sequence having only one integer is a jolly jumper
    if (input->_size == 1)
        return true;
    else
    {
        // create an array whose entries (between 1 and n-1) signify whether
        // such a value has been represented in the input sequence
        Difference diff(input->_size);


        // for each absolute difference, mark it in the visited array
        for (int i = 0; i < input->_size-1; i++)
        {
            int abs_diff = abs(input->_list[i]-input->_list[i+1]);
            if (_debug >= DEBUG_COPIOUS)
                cout << "Absolute difference between " << input->_list[i]
                     << " and " << input->_list[i+1] << " is " << abs_diff
                     << endl;

            if (abs_diff <= input->_size)
            {
                diff._list[abs_diff] = true;
                if (_debug >= DEBUG_COPIOUS)
                    cout << "Marking entry " << abs_diff << " as represented"
                         << endl;
            }
            else
                if (_debug >= DEBUG_COPIOUS)
                    cout << "Entry " << abs_diff << " out of bound" << endl;
        }

        if (_debug >= DEBUG_COPIOUS)
            diff.print();
        return diff.scan();
    }
}


bool input(char* input_file, InputList* input)
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
        // read the first number, which is the size of how many more integers
        // to follow
        int size;
        int i;
        for (i = 0; file >> size; i++)
        {
            // read the sequence of integers that follow the size
            Input current(size);
            for (int j = 0; j < size; j++)
                file >> current._list[j];
            input->_list[i] = current;


            if (_debug >= DEBUG_LIGHT)
            {
                for (int j = 0; j < size; j++)
                    cout << " " << current._list[j];
                cout << endl;
            }
        }

        input->_size = i;
        file.close();
        success = true;
    }

    return success;
}


void output(InputList* input)
{
    for (int i = 0; i < input->_size; i++)
    {
        if (_debug >= DEBUG_LIGHT)
            input->_list[i].print();

        if (is_jolly(&input->_list[i]))
            cout << "Jolly" << endl;
        else
            cout << "Not jolly" << endl;
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
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
        }
    }

    if (input_file == NULL || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
    else
    {
        InputList cases;
        if (input(input_file, &cases) != 0)
            output(&cases);
    }

    return 0;
}
