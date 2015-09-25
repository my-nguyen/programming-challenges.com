import java.util.*;

class RAM
{
  // "words" is a field of 1000 instruction words, each containing 3 digits.
  // Even when the instruction words given in input don't fill all 1000 address
  // locations, the unused portion must still be available and initialized to
  // "000", because the instruction 9xx will attempt to override a word
  // instruction at address so-and-so with some value that is beyond the number
  // of words from input. consideration for selecting a type for "words":
  // (1) a vector of char[] was rejected because a vector element must be
  //   an object type to allow copying and assignment, whereas a char[]
  //   is not an object
  // (2) a vector of String was rejected because the field is a fixed-size
  //   list of 1000 elements, as opposed to the dynamical allocation nature of a
  //   vector
  // (3) an array of char[] was rejected because of the inconvenience in
  //   converting to and from char[] and String
  // (4) an array of String was chosen because of the convenience in assigning,
  //   format printing, etc.
  final int MAX_WORDS = 1000;
  public String[] words = new String[MAX_WORDS];

  // count of all instruction words read from input
  public int count;

  public RAM()
  {
    for (int i = 0; i < MAX_WORDS; i++)
      words[i] = "000";
    count = 0;
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (int i = 0; i < count; i++)
      builder.append(words[i]).append("\n");
    return builder.toString();
  }
}

// this class implements Cloneable so that it can support cloning, for use in
// taking snapshots
class Registers implements Cloneable
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

  // this method makes a copy of itself and returns the destination
  // it's the equivalence of C++ assignment operator
  public Object clone()
  {
    Registers registers = new Registers();
    registers.data = (int[])data.clone();
    return registers;
  }

  // this method tells whether within the next 10 read's the current value will
  // become zero. this is a hack (10 was chosen at random) to prevent an
  // infinite loop.
  static boolean approaching_zero(int previous, int current)
  {
    int diff = current - previous;
    int tmp = current;
    for (int i = 0; i < 10 && current != 0; i++)
      current += diff;
    return current == 0;
  }
}

class skiena_1_6_6
{
  static List<RAM> input()
  {
    Scanner scanner = new Scanner(System.in);
    // number of cases
    int case_count = Integer.parseInt(scanner.nextLine());
    // skip the blank line following the number of cases
    scanner.nextLine();

    List<RAM> list = new ArrayList<>();
    for (int i = 0; i < case_count; i++)
    {
      RAM ram = new RAM();

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
          ram.words[ram.count] = line;
          ram.count++;
        }
      }
      // retain the current set of instructions
      list.add(ram);
    }
    scanner.close();
    return list;
  }

  static void output(List<RAM> list)
  {
    for (RAM ram : list)
    {
      // snapshot of all 1000 instructions and 10 registers
      int[] snap_words = new int[1000];
      Registers snap_registers = new Registers();
      boolean snap_taken = false;

      Registers registers = new Registers();
      // location of the current instruction word
      int location = 0;
      // the number of executed instructions
      int execution_count = 0;
      // because of the switch statement inside the while loop, a break won't
      // cause an exit from the while loop; a boolean flag is used instead.
      boolean halt = false;
      while (halt == false)
      {
        // current instruction word based on location
        String word = ram.words[location];
        // the digits at location 0, 1, and 2 inside the current instruction word
        int zero = Character.getNumericValue(word.charAt(0));
        int one = Character.getNumericValue(word.charAt(1));
        int two = Character.getNumericValue(word.charAt(2));
        switch (zero)
        {
        // special and troublesome case
        case 0:
          // if register at 2 does not contain 0
          if (registers.data[two] != 0)
          {
            // if the location in register at 1 is the same as the current
            // location, then we have an infinite loop. In that case, just go to
            // the next instruction
            if (location == registers.data[one])
            {
              location++;
            }
            // if the instruction at the location contained in register at 1
            // hasn't changed between the last GOTO instruction (0xx) and now,
            // then we have another indefinite loop. In that case, halt the program
            else if (snap_taken &&
              snap_words[registers.data[one]] == Integer.parseInt(ram.words[registers.data[one]]) &&
              !Registers.approaching_zero(snap_registers.data[two], registers.data[two]))
            {
              halt = true;
            }
            // only go to the location in register at 1 if everything is ok
            else
            {
              // take a snapshot of the instruction at the location contained in
              // register at 1
              snap_words[registers.data[one]] =
                Integer.parseInt(ram.words[registers.data[one]]);
              // take a snapshot of all registers
              snap_registers = (Registers)registers.clone();
              snap_taken = true;

              // go to the location in register at 1
              location = registers.data[one];
            }
          }
          // if the current location is beyond the current set of instructions,
          // halt the program
          else if (location >= ram.count && ram.words[location].equals("000"))
            halt = true;
          // otherwise, go to the next location
          else
            location++;
          break;
        case 1:
          // 100 means halt
          if (one == 0 && two == 0)
            halt = true;
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
          // set register at 1 to the value in RAM whose address is in register
          // at 1. So: first, fetch the content of register at 2, which is an
          // address. then, go to the address location in RAM and fetch the
          // instruction there and store it in register at 1.
          registers.data[one] = Integer.parseInt(ram.words[registers.data[two]]);
          location++;
          break;
        case 9:
          // set the value in RAM whose address is in register at 2 to that of
          // register at 1. So first, go to the address location in RAM
          // contained in register at 2, then change the word at that location
          // to the content of register at 1
          ram.words[registers.data[two]] = String.format("%03d", registers.data[one]);
          location++;
          break;
        }
        execution_count++;
      }

      // print the number of executed instructions, as required
      System.out.print(execution_count + "\n\n");
    }
  }

  public static void main(String[] args)
  {
    List<RAM> list = input();
    output(list);
  }
}
