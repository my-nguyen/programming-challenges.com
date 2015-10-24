#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class LCD
{
public:
  LCD(int base, int number);
  operator string();

public:
  // the size based on which to print a digit, with number of columns = size+2,
  // and number of rows = 2*size + 3
  int size;
  // a string of all digits, e.g. "12345", or "67890"
  string digits;

private:
  // array of Print's, each in charge of returning one character based on the
  // row, column, digit (array index) and size
  static char (*prints[10])(int, int, int);

  static void static_init();
  static char print0(int size, int row, int column);
  static char print1(int size, int row, int column);
  static char print2(int size, int row, int column);
  static char print3(int size, int row, int column);
  static char print4(int size, int row, int column);
  static char print5(int size, int row, int column);
  static char print6(int size, int row, int column);
  static char print7(int size, int row, int column);
  static char print8(int size, int row, int column);
  static char print9(int size, int row, int column);
};

char (*LCD::prints[10])(int, int, int);

void LCD::static_init()
{
  if (prints[0] == 0)
  {
    prints[0] = &print0;
    prints[1] = &print1;
    prints[2] = &print2;
    prints[3] = &print3;
    prints[4] = &print4;
    prints[5] = &print5;
    prints[6] = &print6;
    prints[7] = &print7;
    prints[8] = &print8;
    prints[9] = &print9;
  }
}

LCD::LCD(int base, int number)
{
  static_init();

  size = base;
  // convert int to a string of digits
  stringstream stream;
  stream << number;
  digits = stream.str();
}

// due to the problem requirements, a string of digits (which is a number),
// is printed as follows: at each row, print all the characters that form a
// digit at that row, followed by a blank, then repeat the process for others.
// For example, the string 84 of size 3 would be printed as:
// at row 0: " ---       "
// at row 1: "|   | |   |"
// at row 2: "|   | |   |"
// at row 3: "|   | |   |"
// at row 4: " ---   --- "
// at row 5: "|   |     |"
// at row 6: "|   |     |"
// at row 7: "|   |     |"
// at row 8: " ---       "
// note how this satisfies the requirement of size + 2 columns and 2size + 3 rows
LCD::operator string()
{
  string builder;

  // print row by row
  for (int row = 0; row < 2*size+3; row++)
  {
    // print all digits in each row
    for (int i = 0; i < digits.size(); i++)
    {
      // blank column after each digit
      if (i != 0)
        builder += " ";

      // for each digit
      for (int column = 0; column < size+2; column++)
      {
        // convert each digit from char to int
        int index = digits[i] - '0';
        // using the int digit as an index to an array of prints, append just
        // one character forming the digit based on size and position(row, column)
        builder += prints[index](size, row, column);
      }
    }
    builder += '\n';
  }
  // one blank line after each string of digits
  builder += '\n';

  return builder;
}

// all Print? classes must be static to allow instantiation in the static
// block in class LCD
char LCD::print0(int size, int row, int column)
{
  if (row == 0 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else if (row == size+1)
    return ' ';
  else
  {
    if (column == 0 || column == size+1)
      return '|';
    else
      return ' ';
  }
}

char LCD::print1(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
    return ' ';
  else
  {
    if (column == size+1)
      return '|';
    else
      return ' ';
  }
}

char LCD::print2(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == 0)
        return '|';
      else
        return ' ';
    }
  }
}

char LCD::print3(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (column == size+1)
      return '|';
    else
      return ' ';
  }
}

char LCD::print4(int size, int row, int column)
{
  if (row == 0 || row == 2*size+2)
    return ' ';
  else if (row == size+1)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0 || column == size+1)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char LCD::print5(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char LCD::print6(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == 0 || column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char LCD::print7(int size, int row, int column)
{
  if (row == 0)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else if (row == size+1 || row == 2*size+2)
    return ' ';
  else
  {
    if (column == size+1)
      return '|';
    else
      return ' ';
  }
}

char LCD::print8(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (column == 0 || column == size+1)
      return '|';
    else
      return ' ';
  }
}

char LCD::print9(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0 || column == size+1)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

vector<LCD> input()
{
  vector<LCD> list;
  while (true)
  {
    // read the 2 integers
    int size, number;
    cin >> size >> number;

    // end of input
    if (size == 0 || number == 0)
      break;
    else
    {
      LCD lcd(size, number);
      list.push_back(lcd);
    }
  }
  return list;
}

void output(vector<LCD> list)
{
  for (int i = 0; i < list.size(); i++)
    cout << list[i].operator string();
}

int main()
{
  output(input());
}
