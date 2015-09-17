# this method returns the cycle length of a given number
def cycle_length(number)
  length = 1
  while (number != 1)
    if ((number % 2) == 1) # odd number
      number = number * 3 + 1
    else              # even number
      number /= 2
    end
    length += 1         # keep track of cycle length
  end

  length
end

# this method reads input and returns a list of pairs of lower and upper bounds
def input
  list = []
  while (line = gets)
    # read the 2 integers from file into a pair
    list << line.split.map(&:to_i)
  end
  list
end

# this method returns the maximum of all cycle lengths
def process(list)
  # for each array entry of lower and upper bounds, get the cycle length for
  # each integer within those bounds, then obtain the maximum value of all
  # such cycle lengths
  list.each do |bound|
    max = 0
    bound[0].upto(bound[1]) do |j|
      tmp = cycle_length(j)
      max = tmp if (tmp > max)
    end

    puts("#{bound[0]} #{bound[1]} #{max}")
  end
end

list = input
process(list)
