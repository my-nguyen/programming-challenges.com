// C++ solution b: use vector and pair instead of C-style array
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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


bool input(const char* input_file, vector<pair<int, int> >& pairs)
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
            // read the 2 integers from file into a pair
            pair<int, int> tmp;
            file >> tmp.first >> tmp.second;

            if (file.eof()) // exit loop if end of file is reached
                break;
            else         // otherwise, store the pair into vector and keep going
            {
                if (_debug >= DEBUG_LIGHT)
                    cout << tmp.first << " " << tmp.second << endl;
                pairs.push_back(tmp);
            }
        }
        file.close();
        success = true;
    }

    return success;
}


void output(const vector<pair<int, int> > pairs)
{
    // process the actual entries in the array
    for (vector<pair<int, int> >::const_iterator i = pairs.begin();
         i != pairs.end(); i++)
    {
        // for each array entry (pair of lower and upper bounds), do
        // this: for each integer from the lower bound to the upper
        // bound, inclusive of both bounds, get the cycle length for
        // that integer, then obtain the maximum value of all such
        // cycle lengths
        int max = 0;
        for (int j = i->first; j <= i->second; j++)
        {
            int tmp = cycle_length(j);
            if (tmp > max)
                max = tmp;
        }

        cout << i->first << " " << i->second << " " << max << endl;
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
        // each vector element is 2 integers: the lower and the upper bounds
        vector<pair<int, int> > pairs;
        if (input(input_file, pairs) == true)
            output(pairs);
    }
}
