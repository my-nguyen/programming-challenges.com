class Base
  def to_s
    string = ""
    @data.each do |row|
      string << row << "\n"
    end
    string
  end
end

class Input < Base
  attr_accessor :data

  # this constructor initializes internal data from a list of strings
  def initialize(list)
    @data = []

    # add a blank string at the beginning
    @data << blanks(list[0].length + 2)

    # store the date while padding a blank at both the beginning and the end
    list.each do |string|
      tmp = ""
      tmp << "+" << string << "+"
      @data << tmp
    end

    # add a blank string to the end
    @data << blanks(list[0].length + 2)
  end

  # this method returns the total number of mines in the surrounding 8 cells
  def mine_total(i, j)
    return mined(@data[i-1][j-1]) +
           mined(@data[i-1][j]) +
           mined(@data[i-1][j+1]) +
           mined(@data[i][j-1]) +
           mined(@data[i][j+1]) +
           mined(@data[i+1][j-1]) +
           mined(@data[i+1][j]) +
           mined(@data[i+1][j+1]);
  end

private
  # this method creates a new string having the desired length, and padd the
  # contents of such string with blank characters "+"
  def blanks(size)
    "+" * size
  end

  # this method prints only the input_t contained within the surrounding cells
  # containing '+'
  def print_original
    1.upto(@data.length-2) do |i|
      1.upto(@data[i].length-2) do |j|
        print @data[i][j]
      end
      puts
    end
  end

  # this method returns whether this cell contain a mine
  def mined(value)
    (value == '*') ? 1 : 0
  end
end

class Output < Base
  # this method builds the internal data input_t, by recording the mines where
  # they are present in data, and also the number of mines in the surrounding
  # 8 cells, for each cell
  def initialize(input)
    @data = []
    # remember: data has 2 extra lines
    1.upto(input.data.length-2) do |i|
      builder = ""
      # each line in data has 2 extra columns (at beginning and at end)
      1.upto(input.data[i].length-2) do |j|
        # if this input cell contains a mine, record the mine in the output cell
        if (input.data[i][j] == '*')
          builder << "*"
        # otherwise, calculate the number of mines in the surrounding 8 cells
        # and record that number in the output cell
        else
          builder << ('0'.ord + input.mine_total(i, j)).chr
        end
      end
      # retain the string
      @data << builder
    end
  end
end

# this method reads data from STDIN. The first line of each input_t contains 2
# integers which stand for the number of lines and columns of the input_t. Each
# of the next n lines contains exactly m characters, representing the input_t.
# Safe squares are denoted by "." and mine squares by "*". The first input_t
# line where n = m = 0 represents the end of input and should not be processed.
def input
  list = []
  while (true)
    # read 2 integers
    # also throw away the EOL, to prepare for the next line of text
    counts = gets.split.map(&:to_i)
    line_count = counts[0]
    column_count = counts[1]

    # end of input
    if (line_count == 0 || column_count == 0)
      break;
    else
      lines = []
      # read the next line_count lines
      line_count.times do
        lines << gets.chomp!
      end

      # create and retain a input_t of lines
      list << Input.new(lines)
    end
  end

  list
end

# this method sweeps thru a list of input_ts; for each input_t it prints out the
# mine input_ts as well as input_ts which contain the total number of mines in
# the surrounding 8 cells
def output(list)
  # use traditional for loop instead of the advanced for loop because index
  # is needed in printing
  0.upto(list.size-1) do |i|
    # for each cell in the input_t, sweep the surrounding cells for mines and
    # store the number of mines in the cell
    output = Output.new(list[i])

    # print output in the format required
    puts
    puts("Field ##{i+1}:")
    puts output
  end
end

list = input
output(list)
