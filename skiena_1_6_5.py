import sys
import abc
#from abc import abstractmethod

class Image:
  # WHITE = '.'
  # WHITE = "0"
  # this method retunrs the color of the pixel at (column, row)
  def get(self, column, row):
    return self.data[row-1][column-1]

  # this method colors the pixel at (column, row) in color
  def set(self, column, row, color):
    self.data[row-1][column-1] = color

  # this constructor creates a new column_count x row_count image with all
  # pixels initially colored white 'O'
  def __init__(self, column_count, row_count):
    self.data = []
    for i in range(row_count):
      self.data.append(["O"] * column_count)

  # this method clears the image by setting all pixels white '0'. The size
  # remains unchanged
  def clear(self):
    for i in range(len(self.data)):
      for j in range(len(self.data[i])):
        self.data[i][j] = "O"

  def sort(self, left, right):
    if (left > right):
      tmp = left
      left = right
      right = tmp

  # this method draws a vertical segment of color in column, between row1 and
  # row2 inclusive.
  def vertical(self, column, row1, row2, color):
    # sort row1 and row2, for the benefit of the following for loop
    self.sort(row1, row2)
    for i in range(row1-1, row2):
      self.data[i][column-1] = color
    # print "vertical: col:%d, row1:%d, row2:%d, col:%s" %(column, row1, row2, color)
    # print self.str(),

  # this method draws a horizontal segment of color in row , between column1
  # and column2 inclusive.
  def horizontal(self, column1, column2, row, color):
    # sort column1 and column2, for the benefit of the following for loop
    self.sort(column1, column2)
    for i in range(column1-1, column2):
      self.data[row-1][i] = color
    # print "horizontal: col1:%d, col2:%d, row:%d, col:%s" %(column1, column2, row, color)
    # print self.str(),

  # this method draws a filled rectangle with color, where (column1, row1) is
  # the upper-left and (column2, row2) the lower right corner.
  def draw(self, column1, row1, column2, row2, color):
    # sort row1, row2, column1 and column2 for the benefit of the for loops
    self.sort(row1, row2)
    self.sort(column1, column2)
    for i in range(row1-1, row2):
      for j in range(column1-1, column2):
        self.data[i][j] = color

  # this method fills a region with color, where region is defined as follows.
  # Pixel at (column, row) belongs to region. Any other pixel which is the same
  # color as pixel at (column, row) and shares a common side with any pixel in
  # region also belongs to this region.
  def fill(self, column, row, color):
    old_color = self.get(column, row)
    self.fill_internal(column, row, old_color, color)
    # print "fill: col:%d, row:%d, old:%s, new:%s" %(column, row, old_color, color)
    # print self.str(),

  # internal recursive method, used by the other fill()
  # since Ruby doesn't allow method overloading
  def fill_internal(self, column, row, old_color, new_color):
    if (row >= 1 and row <= len(self.data)) and\
       (column >= 1 and column <= len(self.data[0])) and\
       (self.data[row-1][column-1] == old_color):
      self.data[row-1][column-1] = new_color
      self.fill_internal(column-1, row, old_color, new_color)
      self.fill_internal(column+1, row, old_color, new_color)
      self.fill_internal(column, row-1, old_color, new_color)
      self.fill_internal(column, row+1, old_color, new_color)

  # this method writes the file name in MSDOS 8.3 format followed by the
  # contents of the current image
  def save(self, filename):
    file = open(filename, "w")
    for row in self.data:
      file.write("".join(row))
      file.write("\n")
    file.close()
    print "%s\n%s" %(filename, self.str()),

  def str(self):
    builder = ""
    for row in self.data:
      builder += "".join(row)
      builder += "\n"
    return builder

# base class; all subclasses will have the specific functions
class Command(object):
  # There's only one image, created by the I command. all commands such as
  # C, L, V, H, K, F, S operate on this single image.
  image = None

  def __init__(self, tokens):
    self.tokens = tokens

  # virtual method, to be overriden by all subclasses
  @abc.abstractmethod
  def execute(self): pass

class Initialize(Command):
  def __init__(self, tokens):
    super(Initialize, self).__init__(tokens)

  def execute(self):
    column_count = int(self.tokens[1])
    row_count = int(self.tokens[2])
    Command.image = Image(column_count, row_count)

class Clear(Command):
  def __init__(self, tokens):
    super(Clear, self).__init__(tokens)

  def execute(self):
    Command.image.clear()

class Color(Command):
  def __init__(self, tokens):
    super(Color, self).__init__(tokens)

  def execute(self):
    column = int(self.tokens[1])
    row = int(self.tokens[2])
    color = self.tokens[3]
    Command.image.set(column, row, color)

class Vertical(Command):
  def __init__(self, tokens):
    super(Vertical, self).__init__(tokens)

  def execute(self):
    column = int(self.tokens[1])
    row1 = int(self.tokens[2])
    row2 = int(self.tokens[3])
    color = self.tokens[4]
    Command.image.vertical(column, row1, row2, color)

class Horizontal(Command):
  def __init__(self, tokens):
    super(Horizontal, self).__init__(tokens)

  def execute(self):
    column1 = int(self.tokens[1])
    column2 = int(self.tokens[2])
    row = int(self.tokens[3])
    color = self.tokens[4]
    Command.image.horizontal(column1, column2, row, color)

class Draw(Command):
  def __init__(self, tokens):
    super(Draw, self).__init__(tokens)

  def execute(self):
    column1 = int(self.tokens[1])
    column2 = int(self.tokens[2])
    row1 = int(self.tokens[3])
    row2 = int(self.tokens[4])
    color = self.tokens[5]
    Command.image.draw(column1, column2, row1, row2, color)

class Fill(Command):
  def __init__(self, tokens):
    super(Fill, self).__init__(tokens)

  def execute(self):
    column_count = int(self.tokens[1])
    row_count = int(self.tokens[2])
    color = self.tokens[3]
    Command.image.fill(column_count, row_count, color)

class Save(Command):
  def __init__(self, tokens):
    super(Save, self).__init__(tokens)

  def execute(self):
    filename = self.tokens[1]
    Command.image.save(filename)

def input():
  list = []
  while (True):
    # read and parse all tokens in each line
    tokens = sys.stdin.readline().split()
    # decipher the character code
    letter = tokens[0]
    # terminate program
    if (letter == "X"):
      break
    else:
      # only process valid commands
      if (letter in "ICLVHKFS"):
        if (letter == "I"):
          command = Initialize(tokens)
        elif (letter == "C"):
          command = Clear(tokens)
        elif (letter == "L"):
          command = Color(tokens)
        elif (letter == "V"):
          command = Vertical(tokens)
        elif (letter == "H"):
          command = Horizontal(tokens)
        elif (letter == "K"):
          command = Draw(tokens)
        elif (letter == "F"):
          command = Fill(tokens)
        elif (letter == "S"):
          command = Save(tokens)
        list.append(command)
  return list

def outputs(list):
  for command in list:
    command.execute()

outputs(input())
