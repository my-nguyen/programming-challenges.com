#include <iostream>
#include <vector>
#include <algorithm> // sort()
using namespace std;

class Expenses
{
private:
  float average();
  vector<float> data;

public:
  Expenses()
  {
    cout << fixed;
    cout.precision(2);
  }

  void add(float expense)
  {
    data.push_back(expense);
  }

  void sort()
  {
    std::sort(data.begin(), data.end());
  }

  float exchange();
  friend ostream& operator<<(ostream& out, Expenses& expenses);
};

float Expenses::exchange()
{
  float total = 0.0f;
  float avg = average();
  for (vector<float>::iterator it = data.begin(); it != data.end(); it++)
    if (*it >= avg)
      break;
    else
      total += avg - *it;
  return total;
}

float Expenses::average()
{
  float total = 0.0f;
  for (vector<float>::iterator it = data.begin(); it != data.end(); it++)
    total += *it;

  return total / data.size();
}

ostream& operator<<(ostream& out, Expenses& expenses)
{
  for (vector<float>::iterator it = expenses.data.begin(); it != expenses.data.end(); it++)
    out << *it << " ";
  out << endl;
  return out;
}

vector<Expenses> input()
{
  vector<Expenses> list;
  while (true)
  {
    // read in the number of students on this Expenses
    int count;
    cin >> count;

    // end of input
    if (count == 0)
      break;
    else
    {
      // read in the expense of each student in this Expenses
      Expenses expenses;
      for (int i = 0; i < count; i++)
      {
        float expense;
        cin >> expense;
        expenses.add(expense);
      }

      list.push_back(expenses);
    }
  }
  return list;
}

void output(vector<Expenses> list)
{
  for (vector<Expenses>::iterator it = list.begin(); it != list.end(); it++)
  {
    // sort all the expenses, to facilitate calculating the average and the
    // exchange amount per student
    it->sort();
    // print output in the format required
    cout << "$" << it->exchange() << endl;
  }
}

int main()
{
  vector<Expenses> list = input();
  output(list);
}
