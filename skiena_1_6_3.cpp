#include <iostream>
#include <vector>
#include <algorithm> // sort()
using namespace std;

class Expenses
{
private:
  float average()
  {
    float total = 0.0f;
    for (int i = 0; i < data.size(); i++)
      total += data[i];

    return total / data.size();
  }

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

  float exchange()
  {
    float total = 0.0f;
    float avg = average();
    for (int i = 0; i < data.size(); i++)
      if (data[i] >= avg)
        break;
      else
        total += avg - data[i];
    return total;
  }

  friend ostream& operator<<(ostream& out, Expenses& expenses)
  {
    for (int i = 0; i < expenses.data.size(); i++)
      out << expenses.data[i] << " ";
    out << endl;
    return out;
  }
};

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
  for (int i = 0; i < list.size(); i++)
  {
    // sort all the expenses, to facilitate calculating the average and the
    // exchange amount per student
    list[i].sort();
    // print output in the format required
    cout << "$" << list[i].exchange() << endl;
  }
}

int main()
{
  output(input());
}
