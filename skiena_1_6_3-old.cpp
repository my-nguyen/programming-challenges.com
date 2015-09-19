// C++ solution a: C-style array
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>  // qsort()
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;

const int MAX_STUDENTS = 1000;


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
    static int float_cmp(const void* one, const void* two);
    void print() const;

public:
    float _expenses[MAX_STUDENTS]; // list of each student's expense
    int   _count;                  // actual number of students
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


int Trip::float_cmp(const void* one, const void* two)
{
    return *((const int*)one) - *((const int*)two);
}


void Trip::sort()
{
    qsort(_expenses, _count, sizeof(float), float_cmp);
}


void Trip::print() const
{
    for (int i = 0; i < _count; i++)
        cout << _expenses[i] << " ";
    cout << endl;
}


float Trip::average()
{
    float total = 0.0;
    for (int i = 0; i < _count; i++)
        total += _expenses[i];

    if (_debug >= DEBUG_COPIOUS)
        cout << "AVERAGE: " << (total / _count) << endl;
    return total / _count;
}


float Trip::exchange()
{
    float total = 0.0;
    float avg = average();
    for (int i = 0; i < _count && _expenses[i] < avg; i++)
        total += avg - _expenses[i];
    return total;
}


ostream& operator<<(ostream& stream, const Trip& trip)
{
    trip.print();
}


bool input(const char* input_file, Trip* trips[], int& count)
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
                Trip* tmptrip = new Trip;
                tmptrip->_count = tmp;
                if (_debug >= DEBUG_COPIOUS)
                    cout << "number of students: " << tmp << endl;
                for (int i = 0; i < tmp; i++)
                {
                    file >> tmptrip->_expenses[i];
                    if (_debug >= DEBUG_COPIOUS)
                        cout << "student " << i << ", expense: "
                             << tmptrip->_expenses[i] << endl;
                }

                trips[count] = tmptrip;
                count++;
            }
        }
        file.close();


        if (_debug >= DEBUG_COPIOUS)
            cout << "READ " << count << " trips" << endl;

        success = true;
    }

    return success;
}


void output(Trip* trips[], int count)
{
    // sort all the expenses, to facilitate calculating the average
    // and the exchange amount per student
    for (int i = 0; i < count; i++)
    {
        if (_debug >= DEBUG_COPIOUS)
            cout << "PRE-SORT: " << trips[i];
        trips[i]->sort();
        if (_debug >= DEBUG_COPIOUS)
            cout << "POST-SORT: " << trips[i];
    }


    // print output in the format required
    for (int i = 0; i < count; i++)
        cout << "$" << trips[i]->exchange() << endl;
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
        // each test case is one trip, each trip having up to 1000 students
        // and a maximum expense of $10,000
        Trip* trips[MAX_CASES];
        int count = 0;    // number of trips

        if (input(input_file, trips, count) == true)
            output(trips, count);

        // clean up memory allocation
        for (int i = 0; i < count; i++)
            delete trips[i];
    }
}
