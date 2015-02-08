#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


const int DEBUG_OFF     = 0;
const int DEBUG_LIGHT   = 1;
const int DEBUG_COPIOUS = 2;


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
    int    _base;
    string _digits;
};


void LCD::digits(int number)
{
    ostringstream stream;
    stream << number;
    _digits = stream.str();
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
             << ", digits length: " << _digits.size() << endl;

    for (int row = 0; row < 2*_base+3; row++)
    {
        for (int i = 0; i < _digits.size(); i++)
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


bool input(const char* input_file, vector<LCD>& lcds)
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
                LCD tmp;
                tmp.base(base);
                tmp.digits(digits);

                lcds.push_back(tmp);
            }
        }
        file.close();

        success = true;
    }

    return success;
}


void output(vector<LCD>& lcds)
{
    // process the actual entries in the array
    for (int i = 0; i < lcds.size(); i++)
        cout << lcds[i];
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
        vector<LCD> list;
        if (input(input_file, list))
            output(list);
    }
}
