# this class represents a list of integers
class Numbers
  attr_accessor :list

  def initialize
    @list = []
  end

  def to_s
    builder = ""
    builder << @list.size().to_s
    @list.each do |i|
      builder << " " << i.to_s
    end
    builder << "\n"
  end
end

# this class represents a list of boolean flags
class Flags
  attr_accessor :list

  def initialize(size)
    @list = []
    size.times do
      @list << false
    end
  end

  def to_s
    buider = ""
    builder << "Flags entries:\n"
    @list.each do |bool|
      builder << bool.to_s << " "
    end
    builder << "\n"
  end

  # this method scans the array for all unmarked flags and returns true if all
  # flags are marked, and false if any flag is unmarked. note that it goes thru
  # the entire array looking for all unmarked flags and it doesn't just return
  # upon the first unmarked flag
  def scan
    result = true
    1.upto(@list.size-1) do |i|
      if (@list[i] == false)
        # puts("Entry #{i} unmarked")
        result = false
      end
    end

    result
  end
end

# this method determines whether a sequence of integers is a jolly jumper
def is_jolly(numbers)
  # a sequence having only one integer is a jolly jumper
  if (numbers.list.size == 1)
    return true
  else
    # create an array where each entry (between 1 and n-1) indicates whether
    # it has been represented in the Numbers sequence
    flags = Flags.new(numbers.list.size)

    # for each absolute difference, mark it in the visited array
    (numbers.list.size-1).times do |i|
      difference = (numbers.list[i]-numbers.list[i+1]).abs
      if (difference <= numbers.list.size)
        flags.list[difference] = true
        # puts("Marking #{difference}")
      else
        # puts("#{difference} is out of bound")
      end
    end

    flags.scan()
  end
end

def input
  list = []
  while (line = gets)
  # read the number of test cases
    tokens = line.split
    # read the first number, which is the size of how many more integers to
    # follow
    count = tokens[0]

    # read the sequence of integers that follow the size
    current = Numbers.new
    count.to_i.times do |j|
      current.list << tokens[j+1].to_i
    end
    list << current
  end

  list
end

def output(list)
  list.each do |numbers|
    puts(is_jolly(numbers) ? "Jolly" : "Not jolly")
  end
end

output(input())
