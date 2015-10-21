import java.util.*;

// simulate a C-like struct
class Bound
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
  static List<Bound> input()
  {
    List<Bound> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (scanner.hasNext())
    {
      Bound bound = new Bound();
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
  static void process(List<Bound> list)
  {
    // for each array entry of lower and upper bounds, get the cycle length for
    // each integer within those bounds, then obtain the maximum value of all
    // such cycle lengths
    for (Bound bound : list)
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
    process(input());
  }
}
