#include <iostream>
#include <vector>
#include <cmath>  // abs()
using namespace std;

// this class represents a list of integers
class Numbers
{
public:
  vector<int> list;

  operator string()
  {
    string builder;
    builder.append(to_string(list.size()));
    for (int i = 0; i < list.size(); i++)
      builder.append(" ").append(to_string(list[i]));
    builder.append("\n");
    return builder;
  }
};

class Flag
{
public:
  bool flag;

  Flag(bool fl)
  {
    flag = fl;
  }

  operator string()
  {
    return (flag ? "true" : "false");
  }
};

// this class represents a list of boolean flags
class Flags
{
public:
  vector<Flag> list;

  Flags(int size)
  {
    for (int i = 0; i < size; i++)
      list.push_back(Flag(false));
  }

  operator string()
  {
    string builder;
    builder.append("Flags entries:\n");
    for (int i = 0; i < list.size(); i++)
      builder.append(list[i].operator string()).append(" ");
    builder.append("\n");
    return builder;
  }

  // this method scans the array for all unmarked flags and returns true if all
  // flags are marked, and false if any flag is unmarked. note that it goes thru
  // the entire array looking for all unmarked flags and it doesn't just return
  // upon the first unmarked flag
  bool scan()
  {
    bool result = true;
    for (int i = 1; i < list.size(); i++)
      if (list[i].flag == false)
      {
        // cout << "Entry " + i + " unmarked" << endl;
        result = false;
      }

    return result;
  }
};

// this method determines whether a sequence of integers is a jolly jumper
bool is_jolly(Numbers numbers)
{
  // a sequence having only one integer is a jolly jumper
  if (numbers.list.size() == 1)
    return true;
  else
  {
    // create an array where each entry (between 1 and n-1) indicates whether
    // it has been represented in the Numbers sequence
    Flags flags(numbers.list.size());

    // for each absolute difference, mark it in the visited array
    for (int i = 0; i < numbers.list.size()-1; i++)
    {
      int difference = abs(numbers.list[i]-numbers.list[i+1]);
      if (difference <= numbers.list.size())
      {
        flags.list[difference] = true;
        // cout << "Marking " << difference << endl;
      }
      else
      {
        // cout << difference << " is out of bound" << endl;
      }
    }

    return flags.scan();
  }
}

vector<Numbers> input()
{
  vector<Numbers> list;
  // read the first number, which is the size of how many more integers to follow
  int count;
  while (cin >> count)
  {
    // read the sequence of integers that follow the size
    Numbers current;
    for (int j = 0; j < count; j++)
    {
      int number;
      cin >> number;
      current.list.push_back(number);
    }
    list.push_back(current);
  }

  return list;
}

void output(vector<Numbers>& list)
{
  for (int i = 0; i < list.size(); i++)
    cout << (is_jolly(list[i]) ? "Jolly" : "Not jolly") << endl;
}

int main()
{
  vector<Numbers> list = input();
  output(list);
}
