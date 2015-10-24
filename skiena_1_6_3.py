import sys

class Expenses:
  def __init__(self):
    self.data = []

  def add(self, expense):
    self.data.append(expense)

  def sort(self):
    self.data.sort()

  def exchange(self):
    total = 0.0
    avg = self.average()
    for expense in self.data:
      if (expense < avg):
        total += avg - expense
      else:
        break
    # return a string, formatted Ruby style
    return "%.2f" % total

  def average(self):
    return sum(self.data) / len(self.data)

  # Convert all float expenses into string format, for use by puts
  def str(self):
    string = ""
    for expense in self.data:
      string += "#{expense} "
    string += "\n"
    return string

def input():
  list = []
  while (True):
    # read in the number of students on this Expenses
    count = int(sys.stdin.readline())

    # end of input
    if (count == 0):
      break
    else:
      # read in the expense of each student in this Expenses
      expenses = Expenses()
      for i in range(count):
        expenses.add(float(sys.stdin.readline()))

      expenses.sort()
      list.append(expenses)
  return list

def output(list):
  for expenses in list:
    print "$", expenses.exchange()

output(input())
