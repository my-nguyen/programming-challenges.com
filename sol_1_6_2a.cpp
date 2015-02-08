// C++ solution a: use C-style array, while replacing the struct with a class
// and public methods
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;


class Field
{
public:
    Field();
   ~Field();
    void lines(int lins);
    void columns(int cols);
    void input(char iput[][MAX_LINE]);
    void sweep();
    void printInput();
    void printRealInput();
    void printOutput();

private:
    bool isMine(char value);

private:
    // these 2 fields are self-explanatory
    int  _lines;
    int  _columns;

    // _input has 2 more lines and 2 more columns than the actual number of
    // lines and columns read from input file. The extra lines and columns will
    // be initialized to '+'. This serves to facilitate calculating the number
    // of mines in the surrounding cells of any particular cell. So, if the
    // field from input file is:
    //		*...
    //		....
    //		.*..
    //		....
    // Then the internal _input field looks something like this:
    //		++++++
    //		+*...+
    //		+....+
    //		+.*..+
    //		++++++
    char** _input;

    // _output contains the mine indicia as well as the calculated value of
    // each of the cell in the field. This doesn't need the special extra 2
    // lines and 2 columns, as opposed to _input
    char** _output;
};


Field::Field()
{
    _lines = 0;
    _columns = 0;
    _input = 0;
    _output = 0;
}


Field::~Field()
{
    for (int i = 0; i < _lines+2; i++)
        delete[] _input[i];
    delete[] _input;


    for (int i = 0; i < _lines; i++)
        delete[] _output[i];
    delete[] _output;
}


void Field::lines(int lines)
{
    _lines = lines;
    _input = new char* [lines+2];
    _output = new char* [lines];
}


void Field::columns(int columns)
{
    _columns = columns;
    if (_lines != 0)
    {
        for (int i = 0; i < _lines+2; i++)
        {
            _input[i] = new char [columns+3];
            for (int j = 0; j < columns+2; j++)
                _input[i][j] = '+';
            _input[i][columns+2] = '\0';
        }

        for (int i = 0; i < _lines; i++)
        {
            _output[i] = new char [columns+1];
            _output[i][columns] = '\0';
        }
    }
}


void Field::input(char input[][MAX_LINE])
{
    // copy data from a 2-dimensional array of characters into an internal
    // 2-dimensional array while making sure to surround all borders of such
    // internal array with characters '+'
    for (int i = 0; i < _lines; i++)
        strncpy(_input[i+1]+1, input[i], _columns);
}


void Field::printInput()
{
    if (_debug >= DEBUG_LIGHT)
    {
        // make sure to print the field contained within the surrounding cells
        // that contain '+'
        cout << _lines << " " << _columns << endl;
        for (int i = 1; i < _lines+1; i++)
        {
            for (int j = 1; j < _columns+1; j++)
                cout << _input[i][j];
            cout << endl;
        }
    }
}


void Field::printRealInput()
{
    if (_debug >= DEBUG_COPIOUS)
    {
        // print everything in _input
        cout << _lines << " " << _columns << endl;
        for (int i = 0; i < _lines+2; i++)
            cout << _input[i] << endl;
    }
}


void Field::printOutput()
{
    // printing _output is straightforward, unlike printing _input
    for (int i = 0; i < _lines; i++)
        cout << _output[i] << endl;
}


bool Field::isMine(char value)
{
    // does this cell contain a mine?
    return (value == '*') ? 1 : 0;
}


void Field::sweep()
{
    for (int i = 0; i < _lines; i++)
    {
        for (int j = 0; j < _columns; j++)
        {
            // if this input cell contains a mine, record the mine in the
            // output cell
            if (_input[i+1][j+1] == '*')
                _output[i][j] = '*';

            // otherwise, calculate the number of mines in the surrounding 8
            // cells and record that number in the output cell
            else
            {
                int total = isMine(_input[i][j]) +
                            isMine(_input[i][j+1]) +
                            isMine(_input[i][j+2]) +
                            isMine(_input[i+1][j]) +
                            isMine(_input[i+1][j+2]) +
                            isMine(_input[i+2][j]) +
                            isMine(_input[i+2][j+1]) +
                            isMine(_input[i+2][j+2]);
                _output[i][j] = '0' + total;
            }
        }
    }
}


int input(const char* input_file, Field* fields[], int& count)
{
    // read input from a file. The first line of each field contains 2
    // integers which stand for the number of lines and columns of the
    // field. Each of the next n lines contains exactly m characters,
    // representing the field. Safe squares are denoted by "." and mine
    // squares by "*". The first field line where n = m = 0 represents the
    // end of input and should not be processed.
    int success;
    ifstream file(input_file);
    if (file.is_open() == false)
    {
        cout << "Cannot open file " << input_file << endl;
        success = 0;
    }
    else
    {
        while (true)
        {
            // make sure to read BOTH the 2 integers AND the EOL character
            char tmp[MAX_LINE];
            file.getline(tmp, MAX_LINE);
            istringstream tmpstream(tmp);
            int tmplines, tmpcolumns;
            tmpstream >> tmplines >> tmpcolumns;

            // lines = 0 and columns = 0 mean the end of input
            if (tmplines == 0 || tmpcolumns == 0)
                break;
            else
            {
                Field* tmpfield = new Field;
                tmpfield->lines(tmplines);
                tmpfield->columns(tmpcolumns);

                // read the next few lines (determined by tmplines),
                // each line containing several characters (determined
                // by tmpcolumns), where each character is either
                // a mine ('*') or a safe square ('.')
                char tmpinput[80][MAX_LINE];
                for (int i = 0; i < tmplines; i++)
                    file.getline(tmpinput[i], MAX_LINE);
                tmpfield->input(tmpinput);

                fields[count] = tmpfield;
                count++;
            }
        }
        file.close();


        // debug prints
        for (int i = 0; i < count; i++)
            fields[i]->printInput();
        for (int i = 0; i < count; i++)
            fields[i]->printRealInput();


        success = 1;
    }

    return success;
}


void output(Field* fields[], int count)
{
    // for each cell in the field, sweep the surrounding cells for
    // mines and store the number of mines in the cell
    for (int i = 0; i < count; i++)
        fields[i]->sweep();


    // print output in the format required
    for (int i = 0; i < count; i++)
    {
        if (i != 0)
            cout << endl;
        cout << "Field #" << i+1 << ":" << endl;
        fields[i]->printOutput();
    }
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
        // each test case is one field, each with a number of rows and columns
        Field* fields[MAX_CASES];
        int count = 0;

        if (input(input_file, fields, count) == true)
            output(fields, count);

        // clean up memory allocation
        for (int i = 0; i < count; i++)
            delete fields[i];
    }
}
