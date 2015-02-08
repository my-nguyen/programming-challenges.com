import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;


class sol_1_6_6 {
   // the following 2 members representing a snapshot of all 1000 instructions
   // and of all 10 registers, respectively, cannot be declared as static
   // local variables to method output(), even though they're only used inside
   // that method, because Java doesn't allow static local variables. Thus
   // the two variables must become class members. And because they're
   // referenced by output(), which is a static method, these members become
   // static as well
   // snapshot of all 1000 instructions
   static int[] snap_instr = new int[1000];
   // snapshot of all 10 registers
   static Registers snap_reg = new Registers();

   private static void input(ArrayList<RAM> list) throws FileNotFoundException {
      // read the commmands and their corresponding parameters until
      // end of input
      Scanner scanner = new Scanner(System.in);
      int i = Integer.parseInt(scanner.nextLine());

      while (scanner.hasNext()) {
         String line = scanner.nextLine();
         // blank line means the beginning of a new set (case) of input
         if (line.length() == 0) {
            RAM record = new RAM();
            record.reset();
            list.add(record);
         }
         else {
            // fetch the 3-digit instruction
            RAM record = list.get(list.size()-1);
            record._locations[record._count] = line;
            record._count++;
         }
      }
      scanner.close();
   }


   private static void output(ArrayList<RAM> list) throws FileNotFoundException {
      Registers registers = new Registers();
      for (int i = 0; i < list.size(); i++)
      {
         registers.clear();
         int location = 0;
         int count = 0;
         boolean halt = false;
         while (halt == false)
         {
            int zero = list.get(i)._locations[location].charAt(0) - '0';
            int one = list.get(i)._locations[location].charAt(1) - '0';
            int two = list.get(i)._locations[location].charAt(2) - '0';
            switch (zero)
            {
            case 0:
               // if register at two does not contain 0, go to the location in
               // register at one
               if (registers._list[two] != 0) {
                  // if the location in register at one is the same as the
                  // current location, then we have an infinite loop.
                  // In that case, go to the next instruction
                  if (location == registers._list[one])
                  {
                     location++;
                  }

                  // if the instruction at the location contained in register
                  // at one hasn't changed between the last GOTO instruction
                  // (0xx) and now, then we have another indefinite loop.
                  // In that case, halt the program
                  else if (snap_instr[registers._list[one]] ==
                         Integer.parseInt(list.get(i)._locations[registers._list[one]]) &&
                        !Registers.approaching_zero(snap_reg._list[two],
                                            registers._list[two]))
                  {
                     halt = true;
                  }

                  // only go to the location in register at one if everything
                  // is ok
                  else {
                     // take a snapshot of the instruction at the location
                     // contained in register at one
                     snap_instr[registers._list[one]] =
                        Integer.parseInt(list.get(i)._locations[registers._list[one]]);

                     // take a snapshot of all registers
                     snap_reg = (Registers)registers.clone();

                     // go to the location in register at one
                     location = registers._list[one];
                  }
               }

               // if the current location is beyond the current set of
               // instructions, halt the program
               else if (location >= list.get(i)._count &&
                      list.get(i)._locations[location].equals("000")) {
                  halt = true;
               }

               // otherwise, go to the next location
               else
                  location++;
               count++;
               break;
            case 1:
               // 100 means halt
               if (one == 0 && two == 0)
                  halt = true;
               // otherwise, if instruction is 1xx, ignore the instruction
               else
                  ;
               count++;
               location++;
               break;
            case 2:
               // set register at one to two
               registers._list[one] = two;
               count++;
               location++;
               break;
            case 3:
               // add two to register at one
               registers._list[one] = (registers._list[one] + two) % 1000;
               count++;
               location++;
               break;
            case 4:
               // multiply register at one by two
               registers._list[one] = (registers._list[one] * two) % 1000;
               count++;
               location++;
               break;
            case 5:
               // set register at one to the value of register at two
               registers._list[one] = registers._list[two];
               count++;
               location++;
               break;
            case 6:
               // add the value of register at two to register at one
               registers._list[one] = (registers._list[one] +
                                 registers._list[two]) % 1000;
               count++;
               location++;
               break;
            case 7:
               // multiply register at one by the value of register at two
               registers._list[one] = (registers._list[one] *
                                 registers._list[two]) % 1000;
               count++;
               location++;
               break;
            case 8:
               // set register at one to the value in RAM whose address is
               // in register at two
               registers._list[one] =
                  Integer.parseInt(list.get(i)._locations[registers._list[two]]);
               count++;
               location++;
               break;
            case 9:
               // set the value in RAM whose address is in register at two
               // to that of register at one
               {
                  String before =
                           list.get(i)._locations[registers._list[two]];
                  String after = String.format("%03d",
                                        registers._list[one]);
                  list.get(i)._locations[registers._list[two]] = after;
                  count++;
                  location++;
                  break;
               }
            }
         }

         // print the number of executed instructions, as required
         System.out.print(count + "\n\n");
      }
   }


   public static void main(String[] args) throws FileNotFoundException {
      ArrayList<RAM> list = new ArrayList<RAM>();
      input(list);
      output(list);
   }
}


class RAM {
   // Considerations for implementing the field _locations. Facts: the field
   // represents a _locations address of 1000 instruction words, each word
   // comprising 3 digits. Even if the instruction words given in input don't
   // fill all 1000 address locations, the unused portion must still be
   // available and initialized to "000", because the instruction 9xx will
   // attempt to override a word instruction at address so-and-so with some
   // value that is beyond the number of words from input.
   // (1) a vector of char[] was rejected because a vector element must be
   //    an object type to allow copying and assignment, whereas a char[] 
   //    is not an object
   // (2) a vector of string was rejected because the field is a fixed-size
   //    list, as opposed to the dynamical allocation nature of a vector
   // (3) an array of char[] was rejected because of the inconvenience in
   //    converting to and from char[] and String
   // (4) an array of string[] was chosen because of the convenience afforded
   //    by the String class in assigning, format printing, etc.

   public String[] _locations = new String[1000];
   public int  _count;


   public void reset() {
      for (int i = 0; i < 1000; i++)
         _locations[i] = "000";
      _count = 0;
   }
}


class Registers implements Cloneable {
   public int[] _list = new int[10];


   public void clear() {
      for (int i = 0; i < 10; i++)
         _list[i] = 0;
   }


   public String toString() {
      StringBuilder builder = new StringBuilder();
      builder.append("REGS ");
      for (int i = 0; i < 10; i++)
         builder.append(_list[i] + " ");
      return builder.toString();
   }


   // must accompany equals(), per Java specification. I just copied from
   // examples of implementation on the net, without understanding why
   public int hashCode() {
      final int multiplier = 31;
      int hash = 7;
      for (int i = 0; i < 10; i++)
         hash = hash * multiplier + _list[i];
      return hash;
   }

   // Compares the array of integers this to the array other. Returns whether
   // this list of registers is equal to that of another list
   // this method is the equivalence of C++ operator==()
   public boolean equals(Object other) {
      if (other == null)
         return false;
      if (other == this)
         return true;
      if (other.getClass() != getClass())
         return false;

      Registers that = (Registers)other;
      boolean result = true;
      for (int i = 0; i < 10; i++)
      {
         if (_list[i] != that._list[i])
         {
            result = false;
            break;
         }
      }
      return result;
   }


   // Copies to array of integers pointed by source into the array pointed by
   // destination. destination is returned
   // this method is the equivalence of C++ assignment operator
   public Object clone() {
      Registers reg = new Registers();
      reg._list = (int[])_list.clone();
      return reg;
   }


   // Whether within the next 10 times the current value will become zero
   public static boolean approaching_zero(int previous, int current) {
      int diff = current - previous;
      int tmp = current;
      for (int i = 0; i < 10 && current != 0; i++)
         current += diff;
      return current == 0;
   }
}
