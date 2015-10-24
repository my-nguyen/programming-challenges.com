import sys

class Instructions:
  def __init__(self):
    self.data = dict()

  def str(self):
    builder = ""
    for key, value in self.data.iteritems():
      builder += str(key)
      builder += ": "
      builder += value
      builder += "\n"
    return builder

  def get(self, index):
    return self.data[index] if (index in self.data) else "000"

class Registers:
  MAX_REGISTERS = 10
  def __init__(self):
    # data = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
    self.data = [0] * 10

  def str(self):
    builder = ""
    for element in self.data:
      builder += str(element)
      builder += " "
    return builder

def input():
  # read number of cases
  case_count = int(sys.stdin.readline())

  # skip the blank line following the number of cases
  sys.stdin.readline()

  list = []
  for i in range(case_count):
    instructions = Instructions()
    index = 0

    # read until end of input
    for line in sys.stdin:
      line = line.rstrip('\n')
      # empty line means the start of a new set of instructions
      if (len(line) == 0):
        break
      else:
        # fetch the 3-digit instruction
        instructions.data[index] = line[:]
        index += 1
    # retain the current set of instructions
    list.append(instructions)
  return list

MAX_LOOPS = 10
def output(list):
  for instructions in list:
    # print("Instructions:\n%s" %instructions)
    # snapshot of instructions
    goto_register_2 = 999
    goto_count = 0

    registers = Registers()
    # location of the current instruction word
    location = 0
    # the number of executed instructions
    execution_count = 0
    # because of the switch statement inside the while loop, a break won't
    # cause an exit from the while loop; a boolean flag is used instead.
    halt = False
    while (not halt):
      # current instruction word based on location
      instruction = instructions.get(location)
      # print("location: %d, instruction: %s, registers: %s" %(location, instruction, registers))
      # the digits at location 0, 1, and 2 inside the current instruction instruction
      zero = ord(instruction[0]) - ord('0')
      one = ord(instruction[1]) - ord('0')
      two = ord(instruction[2]) - ord('0')
      # special and troublesome case
      if (zero == 0):
        # puts("\tgoto::instruction: #{instruction} , register[#{one}]: #{registers.data[one]}, register[#{two}]: #{registers.data[two]}")
        # if register at 2 does not contain 0
        if (registers.data[two] != 0):
          # if the location in register at 1 is the same as the current
          # location, then we have an infinite loop. In that case, just go to
          # the next instruction
          if (location == registers.data[one]):
            # puts("\tlocation: #{location}, instruction: #{instruction}, register[#{one}]: #{registers.data[one]}")
            location += 1
          # if the content of register at 2 hasn't changed between the last
          # GOTO instruction (0xx) and now, then we have another infinite
          # loop. In that case, halt the program
          elif (goto_register_2 == registers.data[two]):
            # puts("\thalt::current location: #{location}, next location: #{registers.data[one]}, snapshot == register[#{two}]: #{registers.data[two]}")
            halt = True
          elif (goto_count >= MAX_LOOPS):
            # puts("\thalt::current location: #{location}, next location: #{registers.data[one]}, goto_count: #{goto_count}")
            halt = True
            execution_count -= goto_count
          # only go to the location in register at 1 if everything is ok
          else:
            # puts("\tsnapshot::current location: #{location}, next location: #{registers.data[one]}, saving register[#{two}]: #{registers.data[two]}")
            goto_register_2 = registers.data[two]
            goto_count += 1
            # go to the location in register at 1
            location = registers.data[one]
        # if the next location is beyond the current set of instructions,
        # halt the program
        elif (location+1 not in instructions.data):
          # puts("\thalt::next location #{location+1} not in RAM")
          halt = True
        # otherwise, go to the next location
        else:
          location += 1
      elif (zero == 1):
        # 100 means halt
        if (one == 0 and two == 0):
          # puts("\texit(100)")
          halt = True
        # all other 1xx instructions are invalid, so ignore them and move on
        # to the next instruction
        else:
          location += 1
      elif (zero == 2):
        # set register at 1 to the value of 2
        registers.data[one] = two
        location += 1
      elif (zero == 3):
        # add the value of 2 to register at 1
        registers.data[one] = (registers.data[one] + two) % 1000
        location += 1
      elif (zero == 4):
        # multiply register at 1 by the value of 2
        registers.data[one] = (registers.data[one] * two) % 1000
        location += 1
      elif (zero == 5):
        # set register at 1 to the value of register 2
        registers.data[one] = registers.data[two]
        location += 1
      elif (zero == 6):
        # add the value of register at 2 to register at 1
        registers.data[one] = (registers.data[one] + registers.data[two]) % 1000
        location += 1
      elif (zero == 7):
        # multiply register at 1 by the value of register at 2
        registers.data[one] = (registers.data[one] * registers.data[two]) % 1000
        location += 1
      elif (zero == 8):
        # set register at 1 to the value in RAM whose address is in register
        # at 2. So: first, fetch the content of register at 2, which is an
        # address. then, go to the address location in RAM and fetch the
        # instruction there and store it in register at 1.
        registers.data[one] = int(instructions.get(registers.data[two]))
        location += 1
      elif (zero == 9):
        # set the value in RAM whose address is in register at 2 to that of
        # register at 1. So first, go to the address location in RAM
        # contained in register at 2, then change the instruction at that
        # location to the content of register at 1
        instructions.data[registers.data[two]] = "%03d" % registers.data[one]
        # puts("\tRAM-write::instruction: #{instruction}, register[#{two}](address): #{registers.data[two]}, register[#{one}](value): #{registers.data[one]}")
        # sprintf(instructions.instructions[registers.data[two]], "%03d", registers.data[one]);
        location += 1
      execution_count += 1

    # print the number of executed instructions, as required
    print execution_count
    print

output(input())
