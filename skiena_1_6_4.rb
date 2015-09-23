class LCD
  def self.print0(size, row, column)
    if (row == 0 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    elsif (row == size+1)
      return ' '
    else
      if (column == 0 || column == size+1)
        return '|'
      else
        return ' '
      end
    end
  end

  def self.print1(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      return ' '
    else
      if (column == size+1)
        return '|'
      else
        return ' '
      end
    end
  end

  def self.print2(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (row <= size)
        if (column == size+1)
          return '|'
        else
          return ' '
        end
      else
        if (column == 0)
          return '|'
        else
          return ' '
        end
      end
    end
  end

  def self.print3(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (column == size+1)
        return '|'
      else
        return ' '
      end
    end
  end

  def self.print4(size, row, column)
    if (row == 0 || row == 2*size+2)
      return ' '
    elsif (row == size+1)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (row <= size)
        if (column == 0 || column == size+1)
          return '|'
        else
          return ' '
        end
      else
        if (column == size+1)
          return '|'
        else
          return ' '
        end
      end
    end
  end

  def self.print5(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (row <= size)
        if (column == 0)
          return '|'
        else
          return ' '
        end
      else
        if (column == size+1)
          return '|'
        else
          return ' '
        end
      end
    end
  end

  def self.print6(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (row <= size)
        if (column == 0)
          return '|'
        else
          return ' '
        end
      else
        if (column == 0 || column == size+1)
          return '|'
        else
          return ' '
        end
      end
    end
  end

  def self.print7(size, row, column)
    if (row == 0)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    elsif (row == size+1 || row == 2*size+2)
      return ' '
    else
      if (column == size+1)
        return '|'
      else
        return ' '
      end
    end
  end

  def self.print8(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (column == 0 || column == size+1)
        return '|'
      else
        return ' '
      end
    end
  end

  def self.print9(size, row, column)
    if (row == 0 || row == size+1 || row == 2*size+2)
      if (column == 0 || column == size+1)
        return ' '
      else
        return '-'
      end
    else
      if (row <= size)
        if (column == 0 || column == size+1)
          return '|'
        else
          return ' '
        end
      else
        if (column == size+1)
          return '|'
        else
          return ' '
        end
      end
    end
  end

  @@prints = []
  def self.static_init()
    if (@@prints.empty?)
      @@prints << method(:print0)
      @@prints << method(:print1)
      @@prints << method(:print2)
      @@prints << method(:print3)
      @@prints << method(:print4)
      @@prints << method(:print5)
      @@prints << method(:print6)
      @@prints << method(:print7)
      @@prints << method(:print8)
      @@prints << method(:print9)
    end
  end

  def initialize(base, number)
    self.class.static_init

    @size = base
    # convert int to a string of digits
    @digits = number.to_s
  end

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
  def to_s()
    builder = ""

    # print row by row
    (2*@size+3).times do |row|
      # print all digits in each row
      @digits.length.times do |i|
        # blank column after each digit
        builder << " " if (i != 0)

        # for each digit
        (@size+2).times do |column|
          # convert each digit from char to int
          index = @digits[i].to_i
          # using the int digit as an index to an array of prints, append just
          # one character forming the digit based on size and position(row, column)
          builder << @@prints[index].call(@size, row, column)
        end
      end
      builder << "\n"
    end
    # one blank line after each string of digits
    builder << "\n"

    builder
  end
end

def input
  list = Array.new
  while (true)
    # read the 2 integers
    counts = gets.split.map(&:to_i)
    size = counts[0]
    number = counts[1]

    # end of input
    if (size == 0 || number == 0)
      break
    else
      lcd = LCD.new(size, number)

      list << lcd
    end
  end
  list
end

def output(list)
  list.each do |lcd|
    puts lcd
  end
end

list = input
output(list)
