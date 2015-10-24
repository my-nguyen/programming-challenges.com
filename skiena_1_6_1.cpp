#include <iostream>
#include <vector>
using namespace std;

struct Bound
{
  int lower;
  int upper;
};

// this method returns the cycle length of a given number
int cycle_length(int number)
{
  int length = 1;

  while (number != 1)
  {
    if ((number % 2) == 1) // odd number
      number = number * 3 + 1;
    else              // even number
      number /= 2;

    length++;         // keep track of cycle length
  }

  return length;
}

// this method reads input and returns a list of pairs of lower and upper bounds
vector<Bound> input()
{
  vector<Bound> list;
  Bound bound;
  while (cin >> bound.lower)
  {
    // read the 2 integers from file into a pair
    cin >> bound.upper;
    list.push_back(bound);
  }
  return list;
}

// this method returns the maximum of all cycle lengths
void process(vector<Bound> list)
{
  // for each array entry of lower and upper bounds, get the cycle length for
  // each integer within those bounds, then obtain the maximum value of all
  // such cycle lengths
  for (int i = 0; i < list.size(); i++)
  {
    int max = 0;
    for (int j = list[i].lower; j <= list[i].upper; j++)
    {
      int tmp = cycle_length(j);
      if (tmp > max)
        max = tmp;
    }

    cout << list[i].lower << " " << list[i].upper << " " << max << endl;
  }
}

int main()
{
  process(input());
}
