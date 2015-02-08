// C++ solution b: vector
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> // sort()
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;



class Trip
{
public:
    Trip();
   ~Trip();
    void sort();
    float average();
    float exchange();

    friend ostream& operator<<(ostream& stream, const Trip& trip);

private:
    void print() const;

public:
    vector<float> _expenses; // list of each student's expense
};


Trip::Trip()
{
    // to print out exactly 2 digits after the decimal point
    cout << fixed;
    cout.precision(2);
}


Trip::~Trip()
{
}


void Trip::sort()
{
    ::sort(_expenses.begin(), _expenses.end());
}


void Trip::print() const
{
    for (vector<float>::const_iterator i = _expenses.begin();
         i != _expenses.end(); i++)
        cout << *i << " ";
    cout << endl;
}


float Trip::average()
{
    float total = 0.0;
    for (vector<float>::iterator i = _expenses.begin();
         i != _expenses.end(); i++)
        total += *i;

    if (_debug >= DEBUG_COPIOUS)
        cout << "AVERAGE: " << (total / _expenses.size()) << endl;
    return total / _expenses.size();
}


float Trip::exchange()
{
    float total = 0.0;
    float avg = average();
    for (vector<float>::iterator i = _expenses.begin();
         i != _expenses.end() && *i < avg; i++)
        total += avg - *i;
    return total;
}


ostream& operator<<(ostream& stream, const Trip& trip)
{
    trip.print();
}


bool input(const char* input_file, vector<Trip>& trips)
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
        while (true)
        {
            // read in the number of students on this trip
            int tmp;
            file >> tmp;

            // number of students equal zero means end of input
            if (tmp == 0)
                break;
            else
            {
                // read in the expense of each student in this trip
                Trip tmptrip;
                if (_debug >= DEBUG_COPIOUS)
                    cout << "number of students: " << tmp << endl;
                for (int i = 0; i < tmp; i++)
                {
                    float tmpexp;
                    file >> tmpexp;
                    tmptrip._expenses.push_back(tmpexp);
                    if (_debug >= DEBUG_COPIOUS)
                        cout << "student " << i << ", expense: "
                             << tmpexp << endl;
                }

                trips.push_back(tmptrip);
            }
        }
        file.close();


        if (_debug >= DEBUG_COPIOUS)
            cout << "READ " << trips.size() << " trips" << endl;

        success = true;
    }
    return success;
}


void output(vector<Trip> trips)
{
   // sort all the expenses, to facilitate calculating the average
   // and the exchange amount per student
   for (vector<Trip>::iterator i = trips.begin(); i != trips.end(); i++)
   {
        if (_debug >= DEBUG_COPIOUS)
            cout << "PRE-SORT: " << *i;
        i->sort();
        if (_debug >= DEBUG_COPIOUS)
            cout << "POST-SORT: " << *i;
   }


   // print output in the format required
   for (vector<Trip>::iterator i = trips.begin(); i != trips.end(); i++)
        cout << "$" << i->exchange() << endl;
}


int main(int argc, char** argv)
{
    char opt;
    char* input_file = 0;

    while ((opt = getopt(argc, argv, "i:d:")) != -1)
    {
        istringstream tmpstream;
        switch (opt)
        {
        case 'i':
            input_file = optarg;
            break;
        case 'd':
            tmpstream.str(optarg);
            tmpstream >> _debug;
            break;
        default:
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
        }
    }

    if (input_file == 0 || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
    else
    {
        vector<Trip> trips;
        if (input(input_file, trips) == true)
            output(trips);
    }
}
