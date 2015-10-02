import java.util.*;

// this class represents a list of integers
class Numbers
{
  List<Integer> list = new ArrayList<>();

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append(Integer.toString(list.size()));
    for (int i : list)
      builder.append(" ").append(Integer.toString(i));
    builder.append("\n");
    return builder.toString();
  }
}

// this class represents a list of boolean flags
class Flags
{
  List<Boolean> list = new ArrayList<>();

  Flags(int size)
  {
    for (int i = 0; i < size; i++)
      list.add(false);
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append("Flags entries:\n");
    for (boolean bool : list)
      builder.append(bool).append(" ");
    builder.append("\n");
    return builder.toString();
  }

  // this method scans the array for all unmarked flags and returns true if all
  // flags are marked, and false if any flag is unmarked. note that it goes thru
  // the entire array looking for all unmarked flags and it doesn't just return
  // upon the first unmarked flag
  boolean scan()
  {
    boolean result = true;
    for (int i = 1; i < list.size(); i++)
      if (list.get(i) == false)
      {
        // System.out.print("Entry " + i + " unmarked\n");
        result = false;
      }

    return result;
  }
}

class skiena_2_8_1
{
  // this method determines whether a sequence of integers is a jolly jumper
  static boolean is_jolly(Numbers numbers)
  {
    // a sequence having only one integer is a jolly jumper
    if (numbers.list.size() == 1)
      return true;
    else
    {
      // create an array where each entry (between 1 and n-1) indicates whether
      // it has been represented in the Numbers sequence
      Flags flags = new Flags(numbers.list.size());

      // for each absolute difference, mark it in the visited array
      for (int i = 0; i < numbers.list.size()-1; i++)
      {
        int difference = Math.abs(numbers.list.get(i)-numbers.list.get(i+1));
        if (difference <= numbers.list.size())
        {
          flags.list.set(difference, true);
          // System.out.println("Marking " + difference);
        }
        else
        {
          // System.out.println(difference + " is out of bound");
        }
      }

      return flags.scan();
    }
  }

  static List<Numbers> input()
  {
    List<Numbers> list = new ArrayList<>();
    // read the number of test cases
    Scanner scanner = new Scanner(System.in);
    for (int i = 0; scanner.hasNextInt(); i++)
    {
      // read the first number, which is the size of how many more integers to
      // follow
      int count = scanner.nextInt();

      // read the sequence of integers that follow the size
      Numbers current = new Numbers();
      for (int j = 0; j < count; j++)
        current.list.add(scanner.nextInt());
      list.add(current);
    }

    scanner.close();
    return list;
  }

  static void output(List<Numbers> list)
  {
    for (Numbers numbers : list)
      System.out.println(is_jolly(numbers) ? "Jolly" : "Not jolly");
  }

  public static void main(String[] args)
  {
    List<Numbers> list = input();
    output(list);
  }
}
