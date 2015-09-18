#include <iostream>
#include <vector>
#include <string>
using namespace std;

class input_t
{
  friend class output_t;

public:
  input_t(const vector<string>& data);

private:
  void printdata();
  void print_internal();
  string blanks(int size);
  int mined(char value) const;
  int mine_total(int i, int j) const;

private:
  // data has 2 more lines and 2 more columns than the actual number of
  // lines and columns read from input file. The extra lines and columns will
  // be initialized to '+'. This serves to facilitate calculating the number
  // of mines in the surrounding cells of any particular cell. So, if the
  // input_t from input file is:
  //		*...
  //		....
  //		.*..
  //		....
  // Then the internal data input_t looks something like this:
  //		++++++
  //		+*...+
  //		+....+
  //		+.*..+
  //		++++++
  vector<string> data;
};

// this method creates a new string having the desired length, and padd the
// contents of such string with blank characters "+"
string input_t::blanks(int size)
{
  return string(size, '+');
}

// this constructor initializes internal data from a list of strings
input_t::input_t(const vector<string>& list)
{
  // add a blank string at the beginning
  data.push_back(blanks(list[0].length() + 2));

  // store the date while padding a blank at both the beginning and the end
  for (vector<string>::const_iterator it = list.begin(); it != list.end(); it++)
  {
    data.push_back(string("+") + *it + string("+"));
  }

  // add a blank string to the end
  data.push_back(blanks(list[0].length() + 2));
}

// this method prints only the input_t contained within the surrounding cells
// containing '+'
void input_t::printdata()
{
  for (int i = 1; i < data.size()-1; i++)
  {
    for (int j = 1; j < data[i].length()-1; j++)
      cout << data[i][j];
    cout << endl;
  }
}

// this method prints everything in data
void input_t::print_internal()
{
  for (vector<string>::iterator it = data.begin(); it != data.end(); it++)
    cout << *it << endl;
}

// this method returns whether this cell contain a mine
int input_t::mined(char value) const
{
  return (value == '*') ? 1 : 0;
}

// this method returns the total number of mines in the surrounding 8 cells
int input_t::mine_total(int i, int j) const
{
  return mined(data[i-1][j-1]) +
         mined(data[i-1][j]) +
         mined(data[i-1][j+1]) +
         mined(data[i][j-1]) +
         mined(data[i][j+1]) +
         mined(data[i+1][j-1]) +
         mined(data[i+1][j]) +
         mined(data[i+1][j+1]);
}

class output_t
{
public:
  output_t(const input_t& input);
  friend ostream& operator<<(ostream& out, output_t& field);

private:
  // data contains the mine indicia as well as the calculated value of each
  // of the cell in the input_t. This doesn't need the special extra 2 lines and 2
  // columns, as opposed to data
  vector<string> data;
};

// this method builds the internal data input_t, by recording the mines where
// they are present in data, and also the number of mines in the surrounding
// 8 cells, for each cell
output_t::output_t(const input_t& input)
{
  // remember: data has 2 extra lines
  for (int i = 1; i < input.data.size()-1; i++)
  {
    string builder;
    // each line in data has 2 extra columns (at beginning and at end)
    for (int j = 1; j < input.data[i].length()-1; j++)
    {
      // if this input cell contains a mine, record the mine in the output cell
      if (input.data[i][j] == '*')
        builder.append("*");
      // otherwise, calculate the number of mines in the surrounding 8 cells
      // and record that number in the output cell
      else
      {
        builder += '0' + input.mine_total(i, j);
      }
    }
    // retain the string
    data.push_back(builder);
  }
}

// this method prints everything in data
ostream& operator<<(ostream& out, output_t& field)
{
  for (vector<string>::const_iterator it = field.data.begin(); it != field.data.end(); it++)
    out << *it << endl;
  return out;
}

// this method reads data from STDIN. The first line of each input_t contains 2
// integers which stand for the number of lines and columns of the input_t. Each
// of the next n lines contains exactly m characters, representing the input_t.
// Safe squares are denoted by "." and mine squares by "*". The first input_t
// line where n = m = 0 represents the end of input and should not be processed.
vector<input_t> input()
{
  vector<input_t> list;
  while (true)
  {
    // read 2 integers
    int line_count, column_count;
    cin >> line_count;
    cin >> column_count;
    // throw away the EOL, to prepare for the next line of text
    char tmp[20];
    cin.getline(tmp, 20);

    // end of input
    if (line_count == 0 || column_count == 0)
      break;
    else
    {
      vector<string> lines;
      // read the next line_count lines
      for (int i = 0; i < line_count; i++)
      {
        string line;
        getline(cin, line);
        lines.push_back(line);
      }

      // create and retain a input_t of lines
      list.push_back(input_t(lines));
    }
  }

  return list;
}

// this method sweeps thru a list of input_ts; for each input_t it prints out the
// mine input_ts as well as input_ts which contain the total number of mines in
// the surrounding 8 cells
void output(vector<input_t>& list)
{
  // use traditional for loop instead of the advanced for loop because index
  // is needed in printing
  for (int i = 0; i < list.size(); i++)
  {
    // for each cell in the input_t, sweep the surrounding cells for mines and
    // store the number of mines in the cell
    output_t output(list[i]);

    // print output in the format required
    cout << endl;
    cout << "Field #" << (i+1) << ":" << endl;
    cout << output;
  }
}

int main()
{
  vector<input_t> list = input();
  output(list);
}
