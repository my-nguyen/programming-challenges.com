import sys

# this class represents a list of integers
class Numbers:
  def __init__(self):
    self.list = []

  def str(self):
    builder = ""
    builder += str(len(self.list))
    for i in self.list:
      builder += " "
      builder += i.str()
    builder += "\n"
    return builder

# this class represents a list of boolean flags
class Flags:
  def __init__(self, size):
    self.list = [False] * size

  def str(self):
    buider = ""
    builder += "Flags entries:\n"
    for bool in self.list:
      builder += str(bool)
      builder += " "
    builder += "\n"
    return builder

  # this method scans the array for all unmarked flags and returns true if all
  # flags are marked, and false if any flag is unmarked. note that it goes thru
  # the entire array looking for all unmarked flags and it doesn't just return
  # upon the first unmarked flag
  def scan(self):
    result = True
    for i in range(1, len(self.list)):
      if (not self.list[i]):
        # puts("Entry #{i} unmarked")
        result = False
    return result

# this method determines whether a sequence of integers is a jolly jumper
def is_jolly(numbers):
  # a sequence having only one integer is a jolly jumper
  if (len(numbers.list) == 1):
    return True
  else:
    # create an array where each entry (between 1 and n-1) indicates whether
    # it has been represented in the Numbers sequence
    flags = Flags(len(numbers.list))

    # for each absolute difference, mark it in the visited array
    for i in range(len(numbers.list)-1):
      difference = abs(numbers.list[i]-numbers.list[i+1])
      if (difference <= len(numbers.list)):
        flags.list[difference] = True
        # puts("Marking #{difference}")
    return flags.scan()

def input():
  list = []
  for line in sys.stdin:
  # read the number of test cases
    tokens = line.split()
    # read the first number, which is the size of how many more integers to
    # follow
    count = int(tokens[0])

    # read the sequence of integers that follow the size
    current = Numbers()
    for j in range(count):
      current.list.append(int(tokens[j+1]))
    list.append(current)
  return list

def output(list):
  for numbers in list:
    print "Jolly" if is_jolly(numbers) else "Not jolly"

output(input())
