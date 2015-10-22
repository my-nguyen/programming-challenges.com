#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

// #define WHITE '.'
#define WHITE 'O'
class Image
{
  // since we need the ability to re-write characters in each row (string) of
  // the data, and since Java String is immutable (can't be re-written),
  // char[] is chosen instead of String
  vector<string> data;

public:
  Image()
  {
  }
  // this constructor creates a new column_count x row_count image with all
  // pixels initially colored white 'O'
  void initialize(int column_count, int row_count)
  {
    data.resize(row_count);
    for (int i = 0; i < row_count; i++)
      data[i].resize(column_count, WHITE);
  }
  // this method clears the image by setting all pixels white '0'. The size
  // remains unchanged
  void clear()
  {
    for (int i = 0; i < data.size(); i++)
      for (int j = 0; j < data[i].size(); j++)
        data[i][j] = WHITE;
  }
  // this method draws a vertical segment of color in column, between row1 and
  // row2 inclusive.
  void vertical(int column, int row1, int row2, char color)
  {
    // sort row1 and row2, for the benefit of the following for loop
    sort(row1, row2);
    for (int i = row1-1; i < row2; i++)
      data[i][column-1] = color;
  }
  // this method draws a horizontal segment of color in row , between column1
  // and column2 inclusive.
  void horizontal(int column1, int column2, int row, char color)
  {
    // sort column1 and column2, for the benefit of the following for loop
    sort(column1, column2);
    for (int i = column1-1; i < column2; i++)
      data[row-1][i] = color;
  }
  // this method draws a filled rectangle with color, where (column1, row1) is
  // the upper-left and (column2, row2) the lower right corner.
  void draw(int column1, int row1, int column2, int row2, char color)
  {
    // sort row1, row2, column1 and column2 for the benefit of the for loops
    sort(row1, row2);
    sort(column1, column2);
    for (int i = row1-1; i < row2; i++)
      for (int j = column1-1; j < column2; j++)
        data[i][j] = color;
  }
  // this method fills a region with color, where region is defined as follows.
  // Pixel at (column, row) belongs to region. Any other pixel which is the same
  // color as pixel at (column, row) and shares a common side with any pixel in
  // region also belongs to this region.
  void fill(int column, int row, char color)
  {
    char old_color = get(column, row);
    fill(column, row, old_color, color);
  }
  // this method writes the file name in MSDOS 8.3 format followed by the
  // contents of the current image
  void save(string& filename)
  {
    ofstream stream(filename.c_str());
    for (int i = 0; i < data.size(); i++)
        stream << data[i] << endl;
    stream.close();

    cout  << filename << endl << operator string();
  }
  operator string()
  {
    string builder;
    for (int i = 0; i < data.size(); i++)
    {
      for (int j = 0; j < data[i].size(); j++)
        builder += data[i][j];
      builder.append("\n");
    }
    return builder;
  }
  // this method retunrs the color of the pixel at (column, row)
  char get(int column, int row) { return data[row-1][column-1]; }
  // this method colors the pixel at (column, row) in color
  void set(int column, int row, char color) { data[row-1][column-1] = color; }

private:
  void sort(int& left, int& right)
  {
    if (left > right)
    {
      int tmp = left;
      left = right;
      right = tmp;
    }
  }
  // internal recursive method, used by the other fill()
  void fill(int column, int row, char old_color, char new_color)
  {
    if (row >= 1 && row <= data.size() &&
      column >= 1 && column <= data[0].size() &&
      data[row-1][column-1] == old_color)
    {
      data[row-1][column-1] = new_color;

      fill(column-1, row, old_color, new_color);
      fill(column+1, row, old_color, new_color);
      fill(column, row-1, old_color, new_color);
      fill(column, row+1, old_color, new_color);
    }
  }
};

// base class; all subclasses will have the specific functions
class Command
{
protected:
  // There's only one image, created by the I command. all commands such as
  // C, L, V, H, K, F, S operate on this single image.
  static Image image;
  istringstream sstream;

public:
  Command(const string& line) : sstream(line) {}
  // virtual method, to be overriden by all subclasses
  virtual void execute() = 0;
};

Image Command::image;

struct Initialize: public Command
{
  Initialize(const string& line) : Command(line) {}
  void execute()
  {
    int column_count, row_count;
    sstream >> column_count >> row_count;
    image.initialize(column_count, row_count);
  }
};

struct Clear: public Command
{
  Clear(const string& line) : Command(line) {}
  void execute() { image.clear(); }
};

struct Color: public Command
{
  Color(const string& line) : Command(line) {}
  void execute()
  {
    int column, row;
    char color;
    sstream >> column >> row >> color;
    image.set(column, row, color);
  }
};

struct Vertical: public Command
{
  Vertical(const string& line) : Command(line) {}
  void execute()
  {
    int column, row1, row2;
    char color;
    sstream >> column >> row1 >> row2 >> color;
    image.vertical(column, row1, row2, color);
  }
};

struct Horizontal: public Command
{
  Horizontal(const string& line) : Command(line) {}
  void execute()
  {
    int column1, column2, row;
    char color;
    sstream >> column1 >> column2 >> row >> color;
    image.horizontal(column1, column2, row, color);
  }
};

struct Draw: public Command
{
  Draw(const string& line) : Command(line) {}
  void execute()
  {
    int column1, column2, row1, row2;
    char color;
    sstream >> column1 >> column2 >> row1 >> row2 >> color;
    image.draw(column1, column2, row1, row2, color);
  }
};

struct Fill: public Command
{
  Fill(const string& line) : Command(line) {}
  void execute()
  {
    int column, row;
    char color;
    sstream >> column >> row >> color;
    image.fill(column, row, color);
  }
};

struct Save: public Command
{
  Save(const string& line) : Command(line) {}
  void execute()
  {
    string filename;
    sstream >> filename;
    image.save(filename);
  }
};

vector<Command*> input()
{
  // vector<Command*> instead of vector<Command> because Command is an abstract
  // base class which can't be instantiated.
  vector<Command*> list;
  while (true)
  {
    Command* command = NULL;
    // read and parse all tokens in each line
    char line[80];
    cin.getline(line, 80);
    // decipher the character code
    char letter = line[0];
    // terminate program
    if (letter == 'X')
      break;
    else
    {
      // only process valid commands
      if (string("ICLVHKFS").find(letter) != string::npos)
      {
        switch(letter)
        {
        case 'I':
          command = new Initialize(line+2);
          break;
        case 'C':
          command = new Clear(line+2);
          break;
        case 'L':
          command = new Color(line+2);
          break;
        case 'V':
          command = new Vertical(line+2);
          break;
        case 'H':
          command = new Horizontal(line+2);
          break;
        case 'K':
          command = new Draw(line+2);
          break;
        case 'F':
          command = new Fill(line+2);
          break;
        case 'S':
          command = new Save(line+2);
          break;
        }

        list.push_back(command);
      }
    }
  }
  return list;
}

void output(vector<Command*>& list)
{
  for (vector<Command*>::iterator it = list.begin(); it != list.end(); it++)
    (*it)->execute();
}

void cleanup(vector<Command*>& list)
{
  for (vector<Command*>::iterator it = list.begin(); it != list.end(); it++)
    delete (*it);
}

int main()
{
  vector<Command*> list = input();
  output(list);
  cleanup(list);
}
