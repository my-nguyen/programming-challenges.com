import java.util.*;

// simulate a C-like struct
class bound_t
{
  int lower;
  int upper;
}

class skiena_1_6_1
{
  // this method returns the cycle length of a given number
  static int cycle_length(int number)
  {
    int length = 1;

    while (number != 1)
    {
      if ((number % 2) == 1) // odd number
        number = number * 3 + 1;
      else              // even number
        number /= 2;

      length++;         // keep track of cycle length
    }

    return length;
  }

  // this method reads input and returns a list of pairs of lower and upper bounds
  static List<bound_t> input()
  {
    List<bound_t> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (scanner.hasNext())
    {
      bound_t bound = new bound_t();
      // read the lower and upper bounds
      bound.lower = scanner.nextInt();
      bound.upper = scanner.nextInt();

      // retain the 2 bounds
      list.add(bound);
    }
    scanner.close();
    return list;
  }

  // this method returns the maximum of all cycle lengths
  static void process(List<bound_t> list)
  {
    // for each array entry of lower and upper bounds, get the cycle length for
    // each integer within those bounds, then obtain the maximum value of all
    // such cycle lengths
    for (bound_t bound : list)
    {
      int max = 0;
      for (int j = bound.lower; j <= bound.upper; j++)
      {
        int tmp = cycle_length(j);
        if (tmp > max)
          max = tmp;
      }

      System.out.println(bound.lower + " " + bound.upper + " " + max);
    }
  }

  public static void main(String[] args)
  {
    List<bound_t> list = input();
    process(list);
  }
}
