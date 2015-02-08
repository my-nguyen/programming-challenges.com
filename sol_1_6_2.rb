# ruby sol_1_6_2.rb in_1_6_2 | tr -d '\r' | diff out_1_6_2 -
class Field
  # @input has 2 more lines and 2 more columns than the actual number of
  # lines and columns read from input file. The extra lines and columns will
  # be initialized to '+'. This serves to facilitate calculating the number
  # of mines in the surrounding cells of any particular cell. So, if the
  # field from input file is:
  #		*...
  #		....
  #		.*..
  #		....
  # Then the internal @input field looks something like this:
  #		++++++
  #		+*...+
  #		+....+
  #		+.*..+
  #		++++++
  #
  # @output contains the mine indicia as well as the calculated value of
  # each of the cell in the field. This doesn't need the special extra 2
  # lines and 2 columns, as opposed to @input

  def initialize(lines)
    # copy data from an array of strings into an internal array of strings
    # while making sure to surround all borders of such internal array with
    # padding characters '+'
    padding = "".rjust(lines[0].length+2, "+")
    @input = []
    @input << padding
    lines.each { |line| @input << "+" + line + "+" }
    @input << padding

    @output = []
  end

  # Convert input to a string, where input is the internal representation of
  # the cells as read
  def input_to_s
    string = ""
    @input.each do |line|
      string << line + "\n"
    end
    return string
  end

  # Convert output to a string, to ease a call to puts()
  def output_to_s
    string = ""
    @output.each do |line|
      string << line + "\n"
    end
    return string
  end

  # does this cell contain a mine?
  def count(char)
    return (char == '*' ? 1 : 0)
  end

  def sweep()
    (0..@input.size-3).each do |i|
      output_row = ""
      (0..@input[i].size-3).each do |j|

        # if this input cell contains a mine, record the mine in the output cell
        if (@input[i+1][j+1] == '*')
          output_row << '*'

        # otherwise, calculate the number of mines in the surrounding 8 cells
        # and record that number in the output cell
        else
          total = count(@input[i][j]) + count(@input[i][j+1]) +
                  count(@input[i][j+2]) + count(@input[i+1][j]) +
                  count(@input[i+1][j+2]) + count(@input[i+2][j]) +
                  count(@input[i+2][j+1]) + count(@input[i+2][j+2])
          output_row << ('0'.ord + total).chr
        end
      end

      # Save all the recordings into an output row
      @output[i] = output_row
    end
  end
end

def input(filename)
  # Read input from a file. The first line of each field contains 2 integers
  # which stand for the number of lines and columns of the field. Each of
  # the next n lines contains exactly m characters, representing the field.
  # Safe squares are denoted by "." and mine squares by "*". The first
  # field line where n = m = 0 represents the end of input and should not
  # be processed.
  fields = []
  File.open(filename.to_s) do |file|
    while true
      # Read one line from a file, then extract the 2 numbers from the line,
      # and store them in an array
      numbers = file.gets.split(" ").collect { |string| string.to_i }
      if (numbers[0] == 0) && (numbers[1] == 0)
        file.close();
        break
      else
        # read the next few lines (determined by numbers[0]), each line
        # containing several characters (determined by numbers[1]), where
        # each character is either a mine ('*') or a safe square ('.')
        lines = []
        (0..numbers[0]-1).each do
          lines << file.gets.chomp!
        end

        # Convert all the lines just read into a Field object before
        # appending the object into the array fields.
        fields << Field.new(lines)
      end
    end
  end

  return fields
end

def output(fields)
  # For each cell in each field, sweep the surrounding 8 cells for mines
  # and store the number of mines in the cell
  fields.each do |field|
    field.sweep
  end

  # Print the mines just recorded, as required
  fields.each_index do |i|
    # Skip one line for each new field
    if i != 0
      puts
    end

    puts("Field ##{i+1}:")
    puts(fields[i].output_to_s)
  end
end

if ARGV.empty?
  puts("Usage: Must pass input file as command-line argument")
else
  output(input(ARGV[0]))
end
