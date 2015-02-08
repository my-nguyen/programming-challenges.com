#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "defs.h"
using namespace std;


int _debug = DEBUG_OFF;


class Image
{
public:
    void create(int columns, int rows);
    void clear();
    void color(int column, int row, char couleur);
    void vertical(int column, int row1, int row2, char color);
    void horizontal(int column1, int column2, int row, char color);
    void rectangle(int column1, int row1, int column2, int row2, char color);
    void fill(int column, int row, char old_color, char new_color);
    void save(char* filename);
    char at(int column, int row);

    friend ostream& operator<<(ostream& stream, const Image& image);

private:
    int  _rows;
    int  _columns;
    char _pixels[80][80];
};


ostream& operator<<(ostream& stream, const Image& image)
{
    for (int x = 0; x < image._rows; x++)
        cout << image._pixels[x] << endl;
}


// char C_WHITE = '.';
char C_WHITE = 'O';

// Create a new M x N image with all pixels initially colored white (.)
void Image::create(int columns, int rows)
{
    _columns = columns;
    _rows = rows;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            _pixels[i][j] = C_WHITE;
        _pixels[i][columns] = '\0';
    }

    if (_debug >= DEBUG_LIGHT)
        cout << "I " << columns << " " << rows << endl << *this;
}


// Clear the table by setting all pixels white (.). The size remains unchanged.
void Image::clear()
{
    for (int i = 0; i < _rows; i++)
    {
        for (int j = 0; j < _columns; j++)
            _pixels[i][j] = C_WHITE;
    }

    if (_debug >= DEBUG_LIGHT)
        cout << "C" << endl << *this;
}


// Return the color of the pixel at (column, row)
char Image::at(int column, int row)
{
    if (_debug >= DEBUG_COPIOUS)
        cout << "pixel at (" << column << ", " << row << "): '"
             << _pixels[row-1][column-1] << "'" << endl;
    return _pixels[row-1][column-1];
}


// Colors the pixel (X, Y ) in color (C)
void Image::color(int column, int row, char couleur)
{
    _pixels[row-1][column-1] = couleur;

    if (_debug >= DEBUG_LIGHT)
        cout << "L " << column << " " << row << " " << couleur << endl << *this;
}


void sort(int* one, int* two)
{
    if (*one > *two)
    {
        int tmp = *one;
        *one = *two;
        *two = tmp;
    }
}


// Draw a vertical segment of color (C) in column X, between the rows Y1 and Y2
// inclusive.
void Image::vertical(int column, int row1, int row2, char color)
{
    // sort row1 and row2, for the benefit of the following for loop
    sort(&row1, &row2);
    for (int i = row1-1; i < row2; i++)
        _pixels[i][column-1] = color;

    if (_debug >= DEBUG_LIGHT)
        cout << "V " << column << " " << row1 << " " << row2 << " "
             << color << endl << *this;
}


// Draw a horizontal segment of color (C) in the row Y, between the columns X1
// and X2 inclusive.
void Image::horizontal(int column1, int column2, int row, char color)
{
    // sort column1 and column2, for the benefit of the following for loop
    sort(&column1, &column2);
    for (int i = column1-1; i < column2; i++)
        _pixels[row-1][i] = color;

    if (_debug >= DEBUG_LIGHT)
        cout << "H " << column1 << " " << column2 << " " << row << " "
             << color << endl << *this;
}


// Draw a filled rectangle of color C, where (X1, Y1) is the upper-left and
// (X2, Y2) the lower right corner.
void Image::rectangle(int column1, int row1, int column2, int row2, char color)
{
    // sort row1, row2, column1 and column2 for the benefit of the for loops
    sort(&row1, &row2);
    sort(&column1, &column2);
    for (int i = row1-1; i < row2; i++)
    {
        for (int j = column1-1; j < column2; j++)
            _pixels[i][j] = color;
    }

    if (_debug >= DEBUG_LIGHT)
        cout << "K " << column1 << " " << row1 << " " << column2 << " "
             << row2 << " " << color << endl << *this;
}


// Fill the region R with the color C, where R is defined as follows. Pixel
// (X, Y) belongs to R. Any other pixel which is the same color as pixel (X, Y)
// and shares a common side with any pixel in R also belongs to this region.
void Image::fill(int column, int row, char old_color, char new_color)
{
    if (row < 1 || row > _rows ||
        column < 1 || column > _columns ||
        _pixels[row-1][column-1] != old_color)
        ;
    else
    {
        _pixels[row-1][column-1] = new_color;
        fill(column-1, row, old_color, new_color);
        fill(column+1, row, old_color, new_color);
        fill(column, row-1, old_color, new_color);
        fill(column, row+1, old_color, new_color);
    }
}


// Write the file name in MSDOS 8.3 format followed by the contents of the
// current image
void Image::save(char* filename)
{
    ofstream file(filename);
    for (int i = 0; i < _rows; i++)
    {
        _pixels[i][_columns] = '\0';
        file << _pixels[i] << endl;
    }
    file.close();

    cout  << filename << endl << *this;
}


struct Command
{
public:
    Command() {}
    Command(const Command& other) { *this = other; }
    void execute();
    Command& operator=(const Command& other);

public:
    char _command;
    int  _x1;
    int  _x2;
    int  _y1;
    int  _y2;
    char _color;
    char _filename[30];
};


Command& Command::operator=(const Command& other)
{
    if (this != &other)
    {
        _command = other._command;
        _x1 = other._x1;
        _x2 = other._x2;
        _y1 = other._y1;
        _y2 = other._y2;
        _color = other._color;
        strcpy(_filename, other._filename);
    }
    return *this;
}


void Command::execute()
{
    static Image image;

    if (_debug >= DEBUG_COPIOUS)
        cout << "Command " << _command << endl;

    switch (_command)
    {
    case 'I':
        image.create(_x1, _y1);
        break;
    case 'C':
        image.clear();
        break;
    case 'L':
        image.color(_x1, _y1, _color);
        break;
    case 'V':
        image.vertical(_x1, _y1, _y2, _color);
        break;
    case 'H':
        image.horizontal(_x1, _x2, _y1, _color);
        break;
    case 'K':
        image.rectangle(_x1, _y1, _x2, _y2, _color);
        break;
    case 'F':
        image.fill(_x1, _y1, image.at(_x1, _y1), _color);

        // debug print is done outside Image::fill(), unlike other Image
        // methods, because Image::fill() is recursive
        if (_debug >= DEBUG_LIGHT)
        {
            cout << "F " << _x1 << " " << _y1 << " " << _color << endl;
            cout << image;
        }
        break;
    case 'S':
        image.save(_filename);
        break;
    case 'X':
        break;
    }
}


bool input(const char* input_file, Command* list[], int& count)
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
        // read the commmands and their corresponding parameters until
        // end of input
        while (true)
        {
            char line[80];
            file.getline(line, 80);
            if (_debug >= DEBUG_LIGHT)
                cout << line << endl;

            Command tmp;
            tmp._command = line[0];
            switch(tmp._command)
            {
            case 'I':
                sscanf(line+2, "%d %d", &tmp._x1, &tmp._y1);
                break;
            case 'C':
                break;
            case 'L':
                sscanf(line+2, "%d %d %c",
                       &tmp._x1, &tmp._y1, &tmp._color);
                break;
            case 'V':
                sscanf(line+2, "%d %d %d %c",
                       &tmp._x1, &tmp._y1, &tmp._y2, &tmp._color);
                break;
            case 'H':
                sscanf(line+2, "%d %d %d %c",
                       &tmp._x1, &tmp._x2, &tmp._y1, &tmp._color);
                break;
            case 'K':
                sscanf(line+2, "%d %d %d %d %c", &tmp._x1, &tmp._y1,
                       &tmp._x2, &tmp._y2, &tmp._color);
                break;
            case 'F':
                sscanf(line+2, "%d %d %c",
                       &tmp._x1, &tmp._y1, &tmp._color);
                break;
            case 'S':
                sscanf(line+2, "%s", tmp._filename);
                break;
            case 'X':
                break;
            }


            if (tmp._command == 'X')
                break;
            else
            {
                list[count] = new Command(tmp);
                count++;
            }
        }
        file.close();
        success = true;
    }

    return success;
}


void output(Command* list[], int count)
{
    for (int i = 0; i < count; i++)
        list[i]->execute();
}


int main(int argc, char** argv)
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
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
        }
    }

    if (input_file == 0 || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
    else
    {
        Command* list[MAX_CASES];
        int count = 0;  // how many actual entries in the array

        if (input(input_file, list, count) == true)
            output(list, count);

        for (int i = 0; i < count; i++)
            delete list[i];
    }
}
