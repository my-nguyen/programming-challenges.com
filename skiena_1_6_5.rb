# WHITE = '.'
WHITE = 'O'
class Image
  # this method retunrs the color of the pixel at (column, row)
  def get(column, row)
    @data[row-1][column-1]
  end

  # this method colors the pixel at (column, row) in color
  def set(column, row, color)
    @data[row-1][column-1] = color
  end

  # this constructor creates a new column_count x row_count image with all
  # pixels initially colored white 'O'
  def initialize(column_count, row_count)
    @data = Array.new
    row_count.times do |i|
      @data << (WHITE * column_count)
    end
  end

  # this method clears the image by setting all pixels white '0'. The size
  # remains unchanged
  def clear
    @data.each_index do |i|
      @data[i].gsub!(/./, WHITE)
    end
  end

  def sort(left, right)
    if (left > right)
      left, right = right, left
    end
  end

  # this method draws a vertical segment of color in column, between row1 and
  # row2 inclusive.
  def vertical(column, row1, row2, color)
    # sort row1 and row2, for the benefit of the following for loop
    sort(row1, row2)
    ((row1-1)..(row2-1)).each do |i|
      @data[i][column-1] = color
    end
    # puts("vertical, col:#{column}, row1:#{row1}, row2:#{row2}, col:#{color}")
    # puts self
  end

  # this method draws a horizontal segment of color in row , between column1
  # and column2 inclusive.
  def horizontal(column1, column2, row, color)
    # sort column1 and column2, for the benefit of the following for loop
    sort(column1, column2)
    ((column1-1)..(column2-1)).each do |i|
      @data[row-1][i] = color
    end
    # puts("horizontal, col1:#{column1}, col2:#{column2}, row:#{row}, col:#{color}")
    # puts self
  end

  # this method draws a filled rectangle with color, where (column1, row1) is
  # the upper-left and (column2, row2) the lower right corner.
  def draw(column1, row1, column2, row2, color)
    # sort row1, row2, column1 and column2 for the benefit of the for loops
    sort(row1, row2)
    sort(column1, column2)
    ((row1-1)..(row2-1)).each do |i|
      ((column1-1)..(column2-1)).each do |j|
        @data[i][j] = color
      end
    end
  end

  # this method fills a region with color, where region is defined as follows.
  # Pixel at (column, row) belongs to region. Any other pixel which is the same
  # color as pixel at (column, row) and shares a common side with any pixel in
  # region also belongs to this region.
  def fill(column, row, color)
    old_color = get(column, row)
    fill_internal(column, row, old_color, color)
    # puts("fill, col:#{column}, row:#{row}, col:#{color}")
    # puts self
  end

  # internal recursive method, used by the other fill()
  # since Ruby doesn't allow method overloading
  def fill_internal(column, row, old_color, new_color)
    if (row >= 1 && row <= @data.size) &&
       (column >= 1 && column <= @data[0].length) &&
       (@data[row-1][column-1] == old_color)
      @data[row-1][column-1] = new_color
      fill_internal(column-1, row, old_color, new_color)
      fill_internal(column+1, row, old_color, new_color)
      fill_internal(column, row-1, old_color, new_color)
      fill_internal(column, row+1, old_color, new_color)
    end
  end

  # this method writes the file name in MSDOS 8.3 format followed by the
  # contents of the current image
  def save(filename)
    open(filename, 'w') do |file|
      @data.each do |row|
        file.puts(row)
      end
    end

    puts("#{filename}\n#{to_s}")
  end

  def to_s
    builder = ""
    @data.each do |row|
      builder << row << "\n"
    end
    builder
  end
end

# base class; all subclasses will have the specific functions
class Command
  # There's only one image, created by the I command. all commands such as
  # C, L, V, H, K, F, S operate on this single image.
  @@image = nil

  def initialize(tokens)
    @tokens = tokens
  end

  # virtual method, to be overriden by all subclasses
  def execute
  end
end

class Initialize < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    column_count = @tokens[1].to_i
    row_count = @tokens[2].to_i
    @@image = Image.new(column_count, row_count)
  end
end

class Clear < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    @@image.clear
  end
end

class Color < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    column = @tokens[1].to_i
    row = @tokens[2].to_i
    color = @tokens[3]
    @@image.set(column, row, color)
  end
end

class Vertical < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    column = @tokens[1].to_i
    row1 = @tokens[2].to_i
    row2 = @tokens[3].to_i
    color = @tokens[4]
    @@image.vertical(column, row1, row2, color)
  end
end

class Horizontal < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    column1 = @tokens[1].to_i
    column2 = @tokens[2].to_i
    row = @tokens[3].to_i
    color = @tokens[4]
    @@image.horizontal(column1, column2, row, color)
  end
end

class Draw < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    column1 = @tokens[1].to_i
    column2 = @tokens[2].to_i
    row1 = @tokens[3].to_i
    row2 = @tokens[4].to_i
    color = @tokens[5]
    @@image.draw(column1, column2, row1, row2, color)
  end
end

class Fill < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    column_count = @tokens[1].to_i
    row_count = @tokens[2].to_i
    color = @tokens[3]
    @@image.fill(column_count, row_count, color)
  end
end

class Save < Command
  def initialize(tokens)
    super(tokens)
  end

  def execute
    filename = @tokens[1]
    @@image.save(filename)
  end
end

def input
  # vector<Command*> instead of vector<Command> because Command is an abstract
  # base class which can't be instantiated.
  list = []
  while (true)
    # read and parse all tokens in each line
    tokens = gets.split
    # decipher the character code
    letter = tokens[0]
    # terminate program
    if (letter == "X")
      break
    else
      # only process valid commands
      if ("ICLVHKFS".include?(letter))
        case letter[0]
        when 'I'
          command = Initialize.new(tokens)
        when 'C'
          command = Clear.new(tokens)
        when 'L'
          command = Color.new(tokens)
        when 'V'
          command = Vertical.new(tokens)
        when 'H'
          command = Horizontal.new(tokens)
        when 'K'
          command = Draw.new(tokens)
        when 'F'
          command = Fill.new(tokens)
        when 'S'
          command = Save.new(tokens)
        end

        list << command
      end
    end
  end
  list
end

def output(list)
  list.each do |command|
    command.execute
  end
end

output(input())
