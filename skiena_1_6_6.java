import java.util.*;

class Instructions
{
  // "data" is a field of 1000 instruction words, each containing 3 digits.
  //   consideration for selecting a type for "instructions":
  // (1) a vector of char[] was rejected because a vector element must be
  //   an object type to allow copying and assignment, whereas a char[]
  //   is not an object
  // (2) a vector of String was rejected because the field is a fixed-size
  //   list of 1000 elements, as opposed to the dynamical allocation nature of a
  //   vector
  // (3) an array of char[] was rejected because of the inconvenience in
  //   converting to and from char[] and String
  // (4) an array of 1000 String's was rejected because input doesn't fill all
  //   1000 instructions, thus causing waste
  // (5) a map from Integer to String was chosen because we only save the
  //   instructions actually created
  Map<Integer, String> data = new HashMap<Integer, String>();

  public Instructions()
  {
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (Integer key : data.keySet())
      // builder.append(data.get(key)).append("\n");
      builder.append(key).append(": ").append(data.get(key)).append("\n");
    return builder.toString();
  }

  public String get(int key)
  {
    return data.containsKey(key)? data.get(key) : "000";
  }
}

class Registers
{
  final int MAX_REGISTERS = 10;
  public int[] data = new int[MAX_REGISTERS];

  public Registers()
  {
    for (int i = 0; i < MAX_REGISTERS; i++)
      data[i] = 0;
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (int i = 0; i < MAX_REGISTERS; i++)
      builder.append(data[i] + " ");
    return builder.toString();
  }
}

class skiena_1_6_6
{
  static List<Instructions> input()
  {
    Scanner scanner = new Scanner(System.in);
    // number of cases
    int case_count = Integer.parseInt(scanner.nextLine());
    // skip the blank line following the number of cases
    scanner.nextLine();

    List<Instructions> list = new ArrayList<>();
    for (int i = 0; i < case_count; i++)
    {
      Instructions instructions = new Instructions();
      int index = 0;

      // read until end of input
      while (scanner.hasNext())
      {
        String line = scanner.nextLine();
        // empty line means the start of a new set of instructions
        if (line.length() == 0)
          break;
        else
        {
          // fetch the 3-digit instruction
          instructions.data.put(index, line);
          index++;
        }
      }
      // retain the current set of instructions
      list.add(instructions);
    }
    scanner.close();
    return list;
  }

  static void output(List<Instructions> list)
  {
    for (Instructions instructions : list)
    {
      // System.out.println("Instructions:\n" + instructions);
      int goto_register_2 = 999;
      int goto_count = 0;

      Registers registers = new Registers();
      // location of the current instruction instruction
      int location = 0;
      // the number of executed instructions
      int execution_count = 0;
      final int MAX_LOOPS = 10;
      // because of the switch statement inside the while loop, a break won't
      // cause an exit from the while loop; a boolean flag is used instead.
      boolean halt = false;
      while (halt == false)
      {
        // current instruction based on location
        String instruction = instructions.get(location);
        // System.out.println("location: " + location + ", instruction: " + instruction + ", registers: " + registers);
        // the digits at location 0, 1, and 2 inside the current instruction
        int zero = Character.getNumericValue(instruction.charAt(0));
        int one = Character.getNumericValue(instruction.charAt(1));
        int two = Character.getNumericValue(instruction.charAt(2));
        switch (zero)
        {
        // special and troublesome case
        case 0:
          // System.out.println("\tgoto::instruction: " + instruction + ", register[" + one + "]: " + registers.data[one] + ", register[" + two + "]: " + registers.data[two]);
          // if register at 2 does not contain 0
          if (registers.data[two] != 0)
          {
            // if the location in register at 1 is the same as the current
            // location, then we have an infinite loop. In that case, just go to
            // the next instruction
            if (location == registers.data[one])
            {
              // System.out.println("\tlocation: " + location + ", instruction: " + instruction + ", register[" + one + "]: " + registers.data[one]);
              location++;
            }
            // if the content of register at 2 hasn't changed between the last
            // GOTO instruction (0xx) and now, then we have another infinite
            // loop. In that case, halt the program
            else if (goto_register_2 == registers.data[two])
            {
              // System.out.println("\thalt::current location: " + location + ", next location: " + registers.data[one] + ", snapshot == register[" + two + "]: " + registers.data[two]);
              halt = true;
            }
            else if (goto_count >= MAX_LOOPS)
            {
              // System.out.println("\thalt::current location: " + location + ", next location: " + registers.data[one] + ", goto_count: " + goto_count);
              halt = true;
              execution_count -= goto_count;
            }
            // only go to the location in register at 1 if everything is ok
            else
            {
              // System.out.println("\tsnapshot::current location: " + location + ", next location: " + registers.data[one] + ", saving register[" + two + "]: " + registers.data[two]);
              goto_register_2 = registers.data[two];
              goto_count++;

              // go to the location in register at 1
              location = registers.data[one];
            }
          }
          // if the next location is beyond the current set of instructions,
          // halt the program
          else if (!instructions.data.containsKey(location+1))
          {
            // System.out.println("\thalt::next location (" + (location+1) + ") not in RAM");
            halt = true;
          }
          // otherwise, go to the next location
          else
            location++;
          break;
        case 1:
          // 100 means halt
          if (one == 0 && two == 0)
          {
            // System.out.println("\texit(100)");
            halt = true;
          }
          // all other 1xx instructions are invalid, so ignore them and move on
          // to the next instruction
          else
            location++;
          break;
        case 2:
          // set register at 1 to the value of 2
          registers.data[one] = two;
          location++;
          break;
        case 3:
          // add the value of 2 to register at 1
          registers.data[one] = (registers.data[one] + two) % 1000;
          location++;
          break;
        case 4:
          // multiply register at 1 by the value of 2
          registers.data[one] = (registers.data[one] * two) % 1000;
          location++;
          break;
        case 5:
          // set register at 1 to the value of register 2
          registers.data[one] = registers.data[two];
          location++;
          break;
        case 6:
          // add the value of register at 2 to register at 1
          registers.data[one] = (registers.data[one] + registers.data[two]) % 1000;
          location++;
          break;
        case 7:
          // multiply register at 1 by the value of register at 2
          registers.data[one] = (registers.data[one] * registers.data[two]) % 1000;
          location++;
          break;
        case 8:
          // set register at 1 to the value in Instructions whose address is in register
          // at 1. So: first, fetch the content of register at 2, which is an
          // address. then, go to the address location in Instructions and fetch the
          // instruction there and store it in register at 1.
          registers.data[one] = Integer.parseInt(instructions.get(registers.data[two]));
          location++;
          break;
        case 9:
          // set the value in Instructions whose address is in register at 2 to that of
          // register at 1. So first, go to the address location in Instructions
          // contained in register at 2, then change the instruction at that location
          // to the content of register at 1
          instructions.data.put(registers.data[two], String.format("%03d", registers.data[one]));
          // System.out.println("\tRAM-write::instruction: " + instruction + ", register[" + two + "](address): " + registers.data[two] + ", register[" + one + "](value): " + registers.data[one]);
          location++;
          break;
        }
        // keep track of the number of executed instructions
        execution_count++;
      }

      // print the number of executed instructions, as required
      System.out.print(execution_count + "\n\n");
    }
  }

  public static void main(String[] args)
  {
    output(input());
  }
}
