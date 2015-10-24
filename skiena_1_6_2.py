import sys

# this class is a base class for the 2 classes PaddedField and CalculatedField.
# it declares a common data field, and a common toString method.
class Field:
  def str(self):
    string = ""
    for row in self.data:
      string += row
      string += "\n"
    string = string[:-1]
    return string

# this class contains the data field as read from input, with 2 extra lines and
# 2 extra columns added as 4 borders around the actual data field. the borders
# serve to facilitate calculating the number of mines in the 8 surrounding
# cells of any cell. So, if the field from input file is:
#		*...
#		....
#		.*..
#		....
# Then the internal data field looks like this:
#		++++++
#		+*...+
#		+....+
#		+.*..+
#		++++++
class PaddedField(Field):
  # this constructor initializes internal data from a list of strings
  def __init__(self, list):
    self.data = []

    # add a blank string at the beginning
    self.data.append(self.blanks(len(list[0]) + 2))

    # store the data while padding a blank at both the beginning and the end
    for string in list:
      tmp = ""
      tmp += "+"
      tmp += string
      tmp += "+"
      self.data.append(tmp)

    # add a blank string to the end
    self.data.append(self.blanks(len(list[0]) + 2))

  # this method returns the total number of mines in the surrounding 8 cells
  def mine_total(self, i, j):
    return self.is_mined(self.data[i-1][j-1]) + \
           self.is_mined(self.data[i-1][j]) + \
           self.is_mined(self.data[i-1][j+1]) + \
           self.is_mined(self.data[i][j-1]) + \
           self.is_mined(self.data[i][j+1]) + \
           self.is_mined(self.data[i+1][j-1]) + \
           self.is_mined(self.data[i+1][j]) + \
           self.is_mined(self.data[i+1][j+1])

  # this method creates a new string having the desired length, and padd the
  # contents of such string with blank characters "+"
  def blanks(self, size):
    return "+" * size

  # this method prints the data field exactly as read from input (with the 4
  # borders omitted)
  def to_s2():
    builder = ""
    for i in range(1, data.size()-2):
      for j in range(1, data[i].size()-2):
        builder += data[i][j]
      builder += '\n'
    return builder

  # this method returns whether this cell contain a mine
  def is_mined(self, value):
    return 1 if (value == '*') else 0

# this class represents the field whose every cell contains the calculated
# number of mines in its surrounding 8 cells.
class CalculatedField(Field):
  # this method builds the internal data input_t, by recording the mines where
  # they are present in data, and also the number of mines in the surrounding
  # 8 cells, for each cell
  def __init__(self, input):
    self.data = []
    # remember: data has 2 extra lines
    for i in range(1, len(input.data)-1):
      builder = ""
      # each line in data has 2 extra columns (at beginning and at end)
      for j in range(1, len(input.data[i])-2):
        # if this input cell contains a mine, record the mine in the output cell
        if (input.data[i][j] == '*'):
          builder += "*"
        # otherwise, calculate the number of mines in the surrounding 8 cells
        # and record that number in the output cell
        else:
          builder += chr(input.mine_total(i, j) + ord('0'))
      # retain the string
      self.data.append(builder)

# this method reads data from STDIN. The first line of each input_t contains 2
# integers which stand for the number of lines and columns of the input_t. Each
# of the next n lines contains exactly m characters, representing the input_t.
# Safe squares are denoted by "." and mine squares by "*". The first input_t
# line where n = m = 0 represents the end of input and should not be processed.
def input():
  list = []
  while (True):
    line = sys.stdin.readline()
    # read 2 integers
    # also throw away the EOL, to prepare for the next line of text
    counts = map(int, line.split())
    line_count = counts[0]
    column_count = counts[1]

    # end of input
    if (line_count == 0 or column_count == 0):
      break
    else:
      lines = []
      # read the next line_count lines
      for i in range(line_count):
        lines.append(sys.stdin.readline())

      # create and retain a input_t of lines
      list.append(PaddedField(lines))

  return list

# this method sweeps thru a list of input_ts; for each input_t it prints out the
# mine input_ts as well as input_ts which contain the total number of mines in
# the surrounding 8 cells
def output(list):
  # use traditional for loop instead of the advanced for loop because index
  # is needed in printing
  for i in range(len(list)):
    # for each cell in the input_t, sweep the surrounding cells for mines and
    # store the number of mines in the cell
    output = CalculatedField(list[i])

    # print output in the format required
    print
    print "Field #%d:" %(i+1)
    print output.str()

output(input())
