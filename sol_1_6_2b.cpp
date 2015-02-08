// C++ solution a: use vector instead of C-style array
// and public methods
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;


const int DEBUG_OFF     = 0;
const int DEBUG_LIGHT   = 1;
const int DEBUG_COPIOUS = 2;


int _debug = DEBUG_OFF;


class Field
{
public:
    Field();
   ~Field();
    void lines(int);
    void columns(int);
    void input(const vector<string>&);
    void sweep();
    void printInput();
    void printRealInput();
    void printOutput();

private:
    bool isMine(char);

private:
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
    vector<string> _input;

    // _output contains the mine indicia as well as the calculated value of
    // each of the cell in the field. This doesn't need the special extra 2
    // lines and 2 columns, as opposed to _input
    vector<string> _output;
};


Field::Field() : _input(), _output()
{
}


Field::~Field()
{
}


void Field::lines(int lines)
{
    _input.resize(lines+2);
    _output.resize(lines);
}


void Field::columns(int columns)
{
    if (_input.size() != 0)
    {
       for (vector<string>::iterator i = _input.begin();
            i != _input.end(); i++)
           i->resize(columns+2, '+');

       for (vector<string>::iterator i = _output.begin();
            i != _output.end(); i++)
           i->resize(columns);
    }
}


void Field::input(const vector<string>& input)
{
    // copy data from a vector of strings into an internal vector of strings
    // while making sure to surround all borders of such internal vector with
    // characters '+'
    for (int i = 0; i < input.size(); i++)
        _input[i+1].replace(1, _input[i+1].size()-2, input[i]);
}


void Field::printInput()
{
    if (_debug >= DEBUG_LIGHT)
    {
        // make sure to print the field contained within the surrounding cells
        // that contain '+'
        cout << _input.size()-2 << " " << _input[0].size()-2 << endl;
        for (int i = 1; i < _input.size()-1; i++)
        {
            for (int j = 1; j < _input[i].size()-1; j++)
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
        cout << _input.size()-2 << " " << _input[0].size()-2 << endl;
        for (int i = 0; i < _input.size(); i++)
            cout << _input[i] << endl;
    }
}


void Field::printOutput()
{
    // printing _output is straightforward, unlike printing _input
    for (int i = 0; i < _output.size(); i++)
        cout << _output[i] << endl;
}


bool Field::isMine(char value)
{
    // does this cell contain a mine?
    return (value == '*') ? 1 : 0;
}


void Field::sweep()
{
    for (int i = 0; i < _input.size()-2; i++)
    {
        for (int j = 0; j < _input[i].size()-2; j++)
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


int input(const char* input_file, vector<Field>& fields)
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
        success = false;
    }
    else
    {
        while (true)
        {
            // make sure to read BOTH the 2 integers AND the EOL character
            char tmp[20];
            file.getline(tmp, 20);
            istringstream tmpstream(tmp);
            int tmplines, tmpcolumns;
            tmpstream >> tmplines >> tmpcolumns;

            // lines = 0 and columns = 0 mean the end of input
            if (tmplines == 0 || tmpcolumns == 0)
                break;
            else
            {
                Field tmpfield;
                tmpfield.lines(tmplines);
                tmpfield.columns(tmpcolumns);

                // read the next few lines (determined by tmplines),
                // each line containing several characters (determined
                // by tmpcolumns), where each character is either
                // a mine ('*') or a safe square ('.')
                vector<string> tmpinput(tmplines);
                for (int i = 0; i < tmplines; i++)
                {
                    tmpinput[i].resize(tmpcolumns);
                    getline(file, tmpinput[i]);
                }
                tmpfield.input(tmpinput);
                fields.push_back(tmpfield);
            }
        }
        file.close();


        // debug prints
        for (int i = 0; i < fields.size(); i++)
            fields[i].printInput();
        for (int i = 0; i < fields.size(); i++)
            fields[i].printRealInput();

        success = true;
    }

    return success;
}


void output(vector<Field>& fields)
{
    // for each cell in the field, sweep the surrounding cells for
    // mines and store the number of mines in the cell
    for (int i = 0; i < fields.size(); i++)
        fields[i].sweep();


    // print output in the format required
    for (int i = 0; i < fields.size(); i++)
    {
        if (i != 0)
            cout << endl;
        cout << "Field #" << i+1 << ":" << endl;
        fields[i].printOutput();
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
        vector<Field> fields;
        if (input(input_file, fields) == true)
            output(fields);
    }
}
