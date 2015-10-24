import sys

# this method returns the cycle length of a given number
def cycle_length(number):
  length = 1
  while (number != 1):
    # odd number
    if ((number % 2) == 1):
      number = number * 3 + 1
    # even number
    else:
      number /= 2
    length += 1         # keep track of cycle length
  return length

# this method reads input and returns a list of pairs of lower and upper bounds
def input():
  result = []
  for line in sys.stdin:
    # read the 2 integers from file into a pair
    result.append(map(int, line.split()))
  return result

# this method returns the maximum of all cycle lengths
def process(array):
  # for each array entry of lower and upper bounds, get the cycle length for
  # each integer within those bounds, then obtain the maximum value of all
  # such cycle lengths
  for bounds in array:
    max = 0
    for j in range(bounds[0], bounds[1]+1):
      tmp = cycle_length(j)
      if (tmp > max):
        max = tmp

    print bounds[0], bounds[1], max

process(input())
