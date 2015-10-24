import sys

class LCD:
  @staticmethod
  def print0(size, row, column):
    if (row == 0 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    elif (row == size+1):
      return ' '
    else:
      if (column == 0 or column == size+1):
        return '|'
      else:
        return ' '

  @staticmethod
  def print1(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      return ' '
    else:
      if (column == size+1):
        return '|'
      else:
        return ' '

  @staticmethod
  def print2(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (row <= size):
        if (column == size+1):
          return '|'
        else:
          return ' '
      else:
        if (column == 0):
          return '|'
        else:
          return ' '

  @staticmethod
  def print3(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (column == size+1):
        return '|'
      else:
        return ' '

  @staticmethod
  def print4(size, row, column):
    if (row == 0 or row == 2*size+2):
      return ' '
    elif (row == size+1):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (row <= size):
        if (column == 0 or column == size+1):
          return '|'
        else:
          return ' '
      else:
        if (column == size+1):
          return '|'
        else:
          return ' '

  @staticmethod
  def print5(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (row <= size):
        if (column == 0):
          return '|'
        else:
          return ' '
      else:
        if (column == size+1):
          return '|'
        else:
          return ' '

  @staticmethod
  def print6(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (row <= size):
        if (column == 0):
          return '|'
        else:
          return ' '
      else:
        if (column == 0 or column == size+1):
          return '|'
        else:
          return ' '

  @staticmethod
  def print7(size, row, column):
    if (row == 0):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    elif (row == size+1 or row == 2*size+2):
      return ' '
    else:
      if (column == size+1):
        return '|'
      else:
        return ' '

  @staticmethod
  def print8(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (column == 0 or column == size+1):
        return '|'
      else:
        return ' '

  @staticmethod
  def print9(size, row, column):
    if (row == 0 or row == size+1 or row == 2*size+2):
      if (column == 0 or column == size+1):
        return ' '
      else:
        return '-'
    else:
      if (row <= size):
        if (column == 0 or column == size+1):
          return '|'
        else:
          return ' '
      else:
        if (column == size+1):
          return '|'
        else:
          return ' '

  prints = []
  @staticmethod
  def static_init():
    if (not LCD.prints):
      LCD.prints.append(LCD.print0)
      LCD.prints.append(LCD.print1)
      LCD.prints.append(LCD.print2)
      LCD.prints.append(LCD.print3)
      LCD.prints.append(LCD.print4)
      LCD.prints.append(LCD.print5)
      LCD.prints.append(LCD.print6)
      LCD.prints.append(LCD.print7)
      LCD.prints.append(LCD.print8)
      LCD.prints.append(LCD.print9)

  def __init__(self, base, number):
    self.static_init()

    self.size = base
    # convert int to a string of digits
    self.digits = str(number)

  # due to the problem requirements, a string of digits (which is a number),
  # is printed as follows: at each row, print all the characters that form a
  # digit at that row, followed by a blank, then repeat the process for others.
  # For example, the string 84 of size 3 would be printed as:
  # at row 0: " ---       "
  # at row 1: "|   | |   |"
  # at row 2: "|   | |   |"
  # at row 3: "|   | |   |"
  # at row 4: " ---   --- "
  # at row 5: "|   |     |"
  # at row 6: "|   |     |"
  # at row 7: "|   |     |"
  # at row 8: " ---       "
  # note how this satisfies the requirement of size + 2 columns and 2size + 3 rows
  def str(self):
    builder = ""

    # print row by row
    for row in range(2*self.size + 3):
      # print all digits in each row
      for i in range(len(self.digits)):
        # blank column after each digit
        if (i != 0):
          builder += " "

        # for each digit
        for column in range(self.size+2):
          # convert each digit from char to int
          index = int(self.digits[i])
          # using the int digit as an index to an array of prints, append just
          # one character forming the digit based on size and position(row, column)
          builder += LCD.prints[index](self.size, row, column)
      builder += "\n"
    return builder

def input():
  list = []
  while (True):
    # read the 2 integers
    counts = map(int, sys.stdin.readline().split())
    size = counts[0]
    number = counts[1]

    # end of input
    if (size == 0 or number == 0):
      break
    else:
      lcd = LCD(size, number)
      list.append(lcd)
  return list

def output(list):
  for lcd in list:
    print lcd.str()

output(input())
