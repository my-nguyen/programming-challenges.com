import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;


class sol_1_6_1 {
   private static int cycle_length(int n) {
      int length = 1;
      int tmp = n;

      while (n != 1)
      {
         if ((n % 2) == 1) // odd number
            n = n * 3 + 1;
         else           // even number
            n = n / 2;

         length++;       // keep track of cycle length
      }

      return length;
   }


   // read input from STDIN. each line must consist only of two integers.
   // the first integer is the lower bound and the second is the upper bound
   private static void input(ArrayList<Pair> list) throws FileNotFoundException {
      // read the 2 integers from file into a pair
      Scanner scanner = new Scanner(System.in);
      while (true) {
         if (scanner.hasNext()) {  // store the pair into list and keep going
            Pair pair = new Pair();
            pair._first = scanner.nextInt();
            pair._second = scanner.nextInt();
            list.add(pair);
         }
         else
            break; // exit loop if end of file is reached
      }
      scanner.close();
   }


   // process the actual entries in the array
   private static void output(ArrayList<Pair> list) {
      // for each array entry (pair of lower and upper bounds), do this:
      // for each integer from the lower bound to the upper bound, inclusive
      // of both bounds, get the cycle length for that integer, then obtain
      // the maximum value of all such cycle lengths
      for (int i = 0; i < list.size(); i++) {
         int max = 0;
         for (int j = list.get(i)._first; j <= list.get(i)._second; j++)
         {
            int tmp = cycle_length(j);
            if (tmp > max)
               max = tmp;
         }

         System.out.print(list.get(i)._first + " " + list.get(i)._second +
                      " " + max + "\n");
      }
   }


   public static void main(String[] args) throws FileNotFoundException {
      ArrayList<Pair> list = new ArrayList<Pair>();
      input(list);
      output(list);
   }
}


class Pair {
   int _first;
   int _second;
}
