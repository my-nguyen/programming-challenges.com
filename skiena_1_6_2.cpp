#include <iostream>
#include <vector>
#include <string>
using namespace std;

// this class is a base class for the 2 classes PaddedField and CalculatedField.
// it declares a common data field, and a common to_string method.
class Field
{
protected:
  vector<string> data;

public:
  string to_string()
  {
    string builder;
    for (int i = 0; i < data.size(); i++)
      builder.append(data[i]).append("\n");
    return builder;
  }
};

// this class contains the data field as read from input, with 2 extra lines and
// 2 extra columns added as 4 borders around the actual data field. the borders
// serve to facilitate calculating the number of mines in the 8 surrounding
// cells of any cell. So, if the field from input file is:
//		*...
//		....
//		.*..
//		....
// Then the internal data field looks like this:
//		++++++
//		+*...+
//		+....+
//		+.*..+
//		++++++
class PaddedField: public Field
{
friend class CalculatedField;
public:
  // this constructor retains input data from a list of String's by padding the
  // 4 borders as described above.
  PaddedField(vector<string>& list)
  {
    // add a blank string at the beginning
    data.push_back(blanks(list[0].length() + 2));
    // store all input strings while padding a blank at both the beginning and
    // the end of each string
    for (int i = 0; i < list.size(); i++)
      data.push_back("+" + list[i] + "+");
    // add a blank string to the end
    data.push_back(blanks(list[0].length() + 2));
  }

  // this method prints the data field exactly as read from input (with the 4
  // borders omitted)
  string to_string()
  {
    string builder;
    for (int i = 1; i < data.size()-1; i++)
    {
      for (int j = 1; j < data[i].length()-1; j++)
        builder += data[i][j];
      builder += '\n';
    }
    return builder;
  }

private:
  // this method returns whether this cell contain a mine
  int is_mined(char value)
  {
    return (value == '*') ? 1 : 0;
  }

  // this method returns the total number of mines in the surrounding 8 cells
  int mine_total(int i, int j)
  {
    return is_mined(data[i-1][j-1]) +
           is_mined(data[i-1][j]) +
           is_mined(data[i-1][j+1]) +
           is_mined(data[i][j-1]) +
           is_mined(data[i][j+1]) +
           is_mined(data[i+1][j-1]) +
           is_mined(data[i+1][j]) +
           is_mined(data[i+1][j+1]);
  }

  // this utility method returns a string whose content is filled with blank
  // characters "+"
  string blanks(int size)
  {
    return string(size, '+');
  }
};

// this class represents the field whose every cell contains the calculated
// number of mines in its surrounding 8 cells.
class CalculatedField: public Field
{
public:
  // this method builds the internal data field, by recording the mines where
  // they are present in data, and also the number of mines in the surrounding
  // 8 cells, for each cell
  CalculatedField(PaddedField& field)
  {
    // remember: data has 2 extra lines
    for (int i = 1; i < field.data.size()-1; i++)
    {
      string builder;
      // each line in data has 2 extra columns (at beginning and at end)
      for (int j = 1; j < field.data[i].length()-1; j++)
      {
        // if this input cell contains a mine, record the mine in the output cell
        if (field.data[i][j] == '*')
          builder += '*';
        // otherwise, calculate the number of mines in the surrounding 8 cells
        // and record that number in the output cell
        else
          builder += '0' + field.mine_total(i, j);
      }
      // retain the string
      data.push_back(builder);
    }
  }
};

// this method reads data from STDIN. The first line of each field contains 2
// integers which stand for the number of lines and columns of the field. Each
// of the next n lines contains exactly m characters, representing the field.
// Safe squares are denoted by "." and mine squares by "*". The first field
// line where n = m = 0 represents the end of input and should not be processed.
vector<PaddedField> input()
{
  vector<PaddedField> list;
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
      list.push_back(PaddedField(lines));
    }
  }

  return list;
};

// this method sweeps thru a list of Fields; for each Field it prints out the
// mine fields as well as fields which contain the total number of mines in
// the surrounding 8 cells
void output(vector<PaddedField> list)
{
  // use traditional for loop instead of the advanced for loop because index
  // is needed in printing
  for (int i = 0; i < list.size(); i++)
  {
    // for each cell in the input_t, sweep the surrounding cells for mines and
    // store the number of mines in the cell
    CalculatedField output(list[i]);

    // print output in the format required
    cout << endl;
    cout << "Field #" << (i+1) << ":" << endl;
    cout << output.to_string();
  }
}

int main()
{
  output(input());
}
