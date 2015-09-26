class Instructions
  attr_accessor :data

  def initialize
    @data = Hash.new
  end

  def to_s
    builder = ""
    @data.each do |key, value|
      builder << key.to_s << ": " << value << "\n"
    end
    builder
  end

  def get(index)
    @data.key?(index) ? @data[index] : "000"
  end
end

class Registers
  attr_accessor :data
  MAX_REGISTERS = 10
  def initialize
    # data = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
    @data = Array.new(10) { |e| e = 0 }
  end

  def to_s
    builder = ""
    @data.each do |element|
      builder << element.to_s << " "
    end
    builder
  end
end

def input
  # read number of cases
  case_count = gets.to_i

  # skip the blank line following the number of cases
  gets

  list = []
  case_count.times do |i|
    instructions = Instructions.new
    index = 0

    # read until end of input
    while (line = gets)
      line.chomp!
      # empty line means the start of a new set of instructions
      if (line.length == 0)
        break
      else
        # fetch the 3-digit instruction
        instructions.data[index] = line
        index += 1
      end
    end
    # retain the current set of instructions
    list << instructions
  end
  list
end

MAX_LOOPS = 10
def output(list)
  list.each do |instructions|
    # puts("Instructions:\n#{instructions}")
    # snapshot of instructions
    goto_register_2 = 999
    goto_count = 0

    registers = Registers.new
    # location of the current instruction word
    location = 0
    # the number of executed instructions
    execution_count = 0
    # because of the switch statement inside the while loop, a break won't
    # cause an exit from the while loop; a boolean flag is used instead.
    halt = false
    while (halt == false)
      # current instruction word based on location
      instruction = instructions.get(location)
      # puts("location: #{location}, instruction: #{instruction}, registers: #{registers}")
      # the digits at location 0, 1, and 2 inside the current instruction instruction
      zero = instruction[0].ord - '0'.ord
      one = instruction[1].ord - '0'.ord
      two = instruction[2].ord - '0'.ord
      case (zero)
      # special and troublesome case
      when 0
        # puts("\tgoto::instruction: #{instruction} , register[#{one}]: #{registers.data[one]}, register[#{two}]: #{registers.data[two]}")
        # if register at 2 does not contain 0
        if (registers.data[two] != 0)
          # if the location in register at 1 is the same as the current
          # location, then we have an infinite loop. In that case, just go to
          # the next instruction
          if (location == registers.data[one])
            # puts("\tlocation: #{location}, instruction: #{instruction}, register[#{one}]: #{registers.data[one]}")
            location += 1
          # if the content of register at 2 hasn't changed between the last
          # GOTO instruction (0xx) and now, then we have another infinite
          # loop. In that case, halt the program
          elsif (goto_register_2 == registers.data[two])
            # puts("\thalt::current location: #{location}, next location: #{registers.data[one]}, snapshot == register[#{two}]: #{registers.data[two]}")
            halt = true
          elsif (goto_count >= MAX_LOOPS)
            # puts("\thalt::current location: #{location}, next location: #{registers.data[one]}, goto_count: #{goto_count}")
            halt = true
            execution_count -= goto_count
          # only go to the location in register at 1 if everything is ok
          else
            # puts("\tsnapshot::current location: #{location}, next location: #{registers.data[one]}, saving register[#{two}]: #{registers.data[two]}")
            goto_register_2 = registers.data[two]
            goto_count += 1
            # go to the location in register at 1
            location = registers.data[one]
          end
        # if the next location is beyond the current set of instructions,
        # halt the program
        elsif (!instructions.data.key?(location+1))
          # puts("\thalt::next location #{location+1} not in RAM")
          halt = true
        # otherwise, go to the next location
        else
          location += 1
        end
      when 1
        # 100 means halt
        if (one == 0 && two == 0)
          # puts("\texit(100)")
          halt = true
        # all other 1xx instructions are invalid, so ignore them and move on
        # to the next instruction
        else
          location += 1
        end
      when 2
        # set register at 1 to the value of 2
        registers.data[one] = two
        location += 1
      when 3
        # add the value of 2 to register at 1
        registers.data[one] = (registers.data[one] + two) % 1000
        location += 1
      when 4
        # multiply register at 1 by the value of 2
        registers.data[one] = (registers.data[one] * two) % 1000
        location += 1
      when 5
        # set register at 1 to the value of register 2
        registers.data[one] = registers.data[two]
        location += 1
      when 6
        # add the value of register at 2 to register at 1
        registers.data[one] = (registers.data[one] + registers.data[two]) % 1000
        location += 1
      when 7
        # multiply register at 1 by the value of register at 2
        registers.data[one] = (registers.data[one] * registers.data[two]) % 1000
        location += 1
      when 8
        # set register at 1 to the value in RAM whose address is in register
        # at 2. So: first, fetch the content of register at 2, which is an
        # address. then, go to the address location in RAM and fetch the
        # instruction there and store it in register at 1.
        registers.data[one] = instructions.get(registers.data[two]).to_i
        location += 1
      when 9
        # set the value in RAM whose address is in register at 2 to that of
        # register at 1. So first, go to the address location in RAM
        # contained in register at 2, then change the instruction at that
        # location to the content of register at 1
        instructions.data[registers.data[two]] = "%03d" % registers.data[one]
        # puts("\tRAM-write::instruction: #{instruction}, register[#{two}](address): #{registers.data[two]}, register[#{one}](value): #{registers.data[one]}")
        # sprintf(instructions.instructions[registers.data[two]], "%03d", registers.data[one]);
        location += 1
      end
      execution_count += 1
    end

    # print the number of executed instructions, as required
    puts("#{execution_count}")
    puts
  end
end

list = input
output(list)
