class LCD
  # function that returns one character to display on the screen based on
  # the base, row and column, utilized to print a full digit
  def zero(base, row, column)
    char = ' '
    if (row == 0 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    elsif (row == base+1)
      char = ' '
    else
      if (column == 0 || column == base+1)
        char = '|'
      else
        char = ' '
      end
    end
  end

  def one(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      char = ' '
    else
      if (column == base+1)
        char = '|'
      else
        char = ' '
      end
    end
  end

  def two(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (row <= base)
        if (column == base+1)
          char = '|'
        else
          char = ' '
        end
      else
        if (column == 0)
          char = '|'
        else
          char = ' '
        end
      end
    end
    return char
  end

  def three(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (column == base+1)
        char = '|'
      else
        char = ' '
      end
    end
    return char
  end

  def four(base, row, column)
    char = ' '
    if (row == 0 || row == 2*base+2)
      char = ' '
    elsif (row == base+1)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (row <= base)
        if (column == 0 || column == base+1)
          char = '|'
        else
          char = ' '
        end
      else
        if (column == base+1)
          char = '|'
        else
          char = ' '
        end
      end
    end
  end

  def five(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (row <= base)
        if (column == 0)
          char = '|'
        else
          char = ' '
        end
      else
        if (column == base+1)
          char = '|'
        else
          char = ' '
        end
      end
    end
  end

  def six(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (row <= base)
        if (column == 0)
          char = '|'
        else
          char = ' '
        end
      else
        if (column == 0 || column == base+1)
          char = '|'
        else
          char = ' '
        end
      end
    end
  end

  def seven(base, row, column)
    char = ' '
    if (row == 0)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    elsif (row == base+1 || row == 2*base+2)
      char = ' '
    else
      if (column == base+1)
        char = '|'
      else
        char = ' '
      end
    end
  end

  def eight(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (column == 0 || column == base+1)
        char = '|'
      else
        char = ' '
      end
    end
  end

  def nine(base, row, column)
    char = ' '
    if (row == 0 || row == base+1 || row == 2*base+2)
      if (column == 0 || column == base+1)
        char = ' '
      else
        char = '-'
      end
    else
      if (row <= base)
        if (column == 0 || column == base+1)
          char = '|'
        else
          char = ' '
        end
      else
        if (column == base+1)
          char = '|'
        else
          char = ' '
        end
      end
    end
  end

  # class variable: array of pointers to method zero, one, two, ... nine
  @@prints = []
  attr_accessor :base, :digits

  def initialize(base, digits)
    # initialize class variable
    if @@prints.empty?
      @@prints << method(:zero) << method(:one) << method(:two) << method(:three)
      @@prints << method(:four) << method(:five) << method(:six)
      @@prints << method(:seven) << method(:eight) << method(:nine)
    end

    # initialize instance variables
    @base = base
    @digits = digits
  end

  def to_s
    string = ""
    # base determines the size of the multi-digit number to display:
    # rows is 2*base+3, and columns is base+2
    # Furthermore, i is a digit within the number
    (0..(2*@base+2)).each do |row|
      (0..@digits.size-1).each do |i|
        if i != 0
          string << " "
        end

        (0..base+1).each do |column|
          string << @@prints[@digits[i].to_i].call(@base, row, column)
        end
      end
      string << "\n"
    end
    string << "\n"
  end
end

def input(filename)
  cases = []
  File.open(filename.to_s) do |file|
    while true
      # Read one line from a file, then extract the 2 numbers from the line,
      # and store them in an array
      numbers = file.gets.split(" ").collect { |string| string.to_i }
      if (numbers[0] == 0) && (numbers[1] == 0)
        file.close();
        break
      else
        cases << LCD.new(numbers[0], numbers[1].to_s)
      end
    end
  end
  return cases
end

def output(tests)
  tests.each do |test|
    puts(test)
  end
end

if ARGV.empty?
  puts("Usage: Must pass input file as command-line argument")
else
  output(input(ARGV[0]))
end
