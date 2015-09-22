#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;


class LCD
{
public:
    LCD() {}
   ~LCD() {}
    void base(int base) { _base = base; }
    void digits(int);

    friend ostream& operator<<(ostream& stream, const LCD& lcd);

private:
    void print() const;

private:
    int  _base;
    char _digits[10];
};


// convert an integer into a string and store the value in _digits
void LCD::digits(int number)
{
    sprintf(_digits, "%d", number);
}


void print_zero(int base, int row, int column)
{
    if (row == 0 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else if (row == base+1)
        cout << " ";
    else
    {
        if (column == 0 || column == base+1)
            cout << "|";
        else
            cout << " ";
    }
}


void print_one(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
        cout << " ";
    else
    {
        if (column == base+1)
            cout << "|";
        else
            cout << " ";
    }
}


void print_two(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (row <= base)
        {
            if (column == base+1)
                cout << "|";
            else
                cout << " ";
        }
        else
        {
            if (column == 0)
                cout << "|";
            else
                cout << " ";
        }
    }
}


void print_three(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (column == base+1)
            cout << "|";
        else
            cout << " ";
    }
}


void print_four(int base, int row, int column)
{
    if (row == 0 || row == 2*base+2)
        cout << " ";
    else if (row == base+1)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (row <= base)
        {
            if (column == 0 || column == base+1)
                cout << "|";
            else
                cout << " ";
        }
        else
        {
            if (column == base+1)
                cout << "|";
            else
                cout << " ";
        }
    }
}


void print_five(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (row <= base)
        {
            if (column == 0)
                cout << "|";
            else
                cout << " ";
        }
        else
        {
            if (column == base+1)
                cout << "|";
            else
                cout << " ";
        }
    }
}


void print_six(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (row <= base)
        {
            if (column == 0)
                cout << "|";
            else
                cout << " ";
        }
        else
        {
            if (column == 0 || column == base+1)
                cout << "|";
            else
                cout << " ";
        }
    }
}


void print_seven(int base, int row, int column)
{
    if (row == 0)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else if (row == base+1 || row == 2*base+2)
        cout << " ";
    else
    {
        if (column == base+1)
            cout << "|";
        else
            cout << " ";
    }
}


void print_eight(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (column == 0 || column == base+1)
            cout << "|";
        else
            cout << " ";
    }
}


void print_nine(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            cout << " ";
        else
            cout << "-";
    }
    else
    {
        if (row <= base)
        {
            if (column == 0 || column == base+1)
                cout << "|";
            else
                cout << " ";
        }
        else
        {
            if (column == base+1)
                cout << "|";
            else
                cout << " ";
        }
    }
}


void LCD::print() const
{
    // define and initialize an array of pointer to prints
    static void (*prints[10])(int, int, int);
    static bool init = false;

    if (init == false)
    {
        prints[0] = &print_zero;
        prints[1] = &print_one;
        prints[2] = &print_two;
        prints[3] = &print_three;
        prints[4] = &print_four;
        prints[5] = &print_five;
        prints[6] = &print_six;
        prints[7] = &print_seven;
        prints[8] = &print_eight;
        prints[9] = &print_nine;

        init = true;
    }


    if (_debug >= DEBUG_COPIOUS)
        cout << "base: " << _base << ", digits: " << _digits
             << ", digits length: " << strlen(_digits) << endl;

    for (int row = 0; row < 2*_base+3; row++)
    {
        for (int i = 0; i < strlen(_digits); i++)
        {
            if (i != 0)
                cout << " ";
            for (int column = 0; column < _base+2; column++)
            {
                int digit = _digits[i] - '0';
                (*prints[digit])(_base, row, column);
            }
        }
        cout << endl;
    }
    cout << endl;
}

 
ostream& operator<<(ostream& stream, const LCD& lcd)
{
    lcd.print();
}


bool input(const char* input_file, LCD* lcds[], int& count)
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
        // read the 2 integers from file into one array element until end
        // of input
        while (true)
        {
            // read 2 integers
            int base, digits;
            file >> base >> digits;
            if (_debug >= DEBUG_COPIOUS)
               cout << base << " " << digits << endl;

            // both integers being zero means the end of input
            if (base == 0 || digits == 0)
                break;
            else
            {
                LCD* tmp = new LCD;
                tmp->base(base);
                tmp->digits(digits);

                lcds[count] = tmp;
                count++;
            }
        }
        file.close();
        success = true;
    }

    return success;
}


void output(LCD* list[], int count)
{
    // process the actual entries in the array
    for (int i = 0; i < count; i++)
        cout << *list[i];
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
        // each test case is one line of input, with each line having two
        // numbers representing the base and the digits to be displayed
        LCD* list[MAX_CASES];
        int count = 0;  // how many actual entries in the array

        if (input(input_file, list, count) == true)
            output(list, count);

        for (int i = 0; i < count; i++)
            delete list[i];
    }
}
