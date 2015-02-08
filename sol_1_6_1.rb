# ruby sol_1_6_1.rb in_1_6_1 | tr -d '\r' | diff out_1_6_1 -
#
# Calculate the cycle length for a given number n
def cycle_length(n)
  length = 1
  while n != 1
    # odd number
    if (n % 2) == 1
      n = n * 3 + 1
    # even number
    else
      n = n / 2
    end

    length += 1
  end

  return length;
end


def input(filename)
  pairs = []
  # read input from a file.
  File.open(filename.to_s).each do |line|
    pair = line.split(" ")
    # exit loop if end of file is reached
    if pair.size == 0
      break
    else
      # Each line must consist only of two integers. The first integer is
      # the lower bound and the second is the upper.
      # Store the pair into an array and keep going
      pairs << [pair[0].to_i, pair[1].to_i]
    end
  end

  return pairs
end


def output(pairs)
  pairs.each do |pair|
    max = 00
    # For each array entry (a pair of lower and upper bounds), do this:
    # for each integer between the lower bound and the upper bound,
    # inclusive of both bounds, get the cycle length for that integer,
    # then obtain the maximum value of all such cycle lengths
    (pair[0]..pair[1]).each do |integer|
      length = cycle_length(integer)
      if max < length
        max = length
      end
    end

    puts("#{pair[0]} #{pair[1]} #{max}")
  end
end

if ARGV.empty?
  puts("Usage: Must pass input file as command-line argument")
else
  output(input(ARGV[0]))
end
