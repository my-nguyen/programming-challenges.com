// C++ solution a: use C-style array
#include <iostream>
#include <fstream>
#include <sstream>
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;


int cycle_length(int n)
{
    int length = 1;
    int tmp = n;

    if (_debug >= DEBUG_COPIOUS)
        cout << n;

    while (n != 1)
    {
        if ((n % 2) == 1) /* odd number */
           n = n * 3 + 1;
        else              /* even number */
           n = n / 2;

        if (_debug >= DEBUG_COPIOUS)
            cout << " " << n;
        length++;         /* keep track of cycle length */
    }
    if (_debug >= DEBUG_COPIOUS)
        cout << endl;

    if (_debug >= DEBUG_LIGHT)
        cout << "cycle length of " << tmp << " is " << length << endl;

    return length;
}


bool input(char* input_file, pair<int, int>* pairs[], int& count)
{
    // read input from a file. each line must consist only of two integers.
    // the first integer is the lower bound and the second is the upper bound
    bool success;
    ifstream file(input_file);
    if (file.is_open() == false)
    {
        cout << "Cannot open file " << input_file << endl;
        success = false;
    }
    else
    {
        while (true)
        {
            // read the 2 integers from file into one element
            pair<int, int> tmp;
            file >> tmp.first >> tmp.second;

            if (file.eof()) // exit loop if end of file is reached
                break;
            else            // otherwise, store the pair and keep going
            {
                pair<int, int>* tmp2 = new pair<int, int>(tmp);
                if (_debug >= DEBUG_LIGHT)
                    cout << tmp2->first << " " << tmp2->second << endl;

                pairs[count] = tmp2;
                count++;
            }
        }
        file.close();
        success = true;
    }

    return success;
}


void output(pair<int, int>* pairs[], int count)
{
    // process the actual entries
    for (int i = 0; i < count; i++)
    {
        // for each entry (pair of lower and upper bounds), do this: for each
        // integer from the lower bound to the upper bound, inclusive of both
        // bounds, get the cycle length for that integer, then obtain the
        // maximum value of all such cycle lengths
        int max = 0;
        for (int j = pairs[i]->first; j <= pairs[i]->second; j++)
        {
            int tmp = cycle_length(j);
            if (tmp > max)
                max = tmp;
        }

        cout << pairs[i]->first << " " << pairs[i]->second
             << " " << max << endl;
    }
}


int main(int argc, char** argv)
{
    char opt;
    char* input_file = 0;

    while ((opt = getopt(argc, argv, "i:d:")) != -1)
    {
        istringstream sstream;
        switch (opt)
        {
        case 'i':
            input_file = optarg;
            break;
        case 'd':
            sstream.str(optarg);
            sstream >> _debug;
            break;
        default:
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]\n";
        }
    }

    if (input_file == 0)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]\n";
    else
    {
        // each test case is a pair of 2 integers
        pair<int, int>* pairs[MAX_CASES];
        int count = 0;             // how many actual entries in the array

        if (input(input_file, pairs, count) == true)
            output(pairs, count);

        // deallocate dynamic memories
        for (int i = 0; i < count; i++)
            delete pairs[i];
    }
}
