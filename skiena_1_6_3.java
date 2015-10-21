import java.util.*;

class Expenses
{
  public List<Float> data = new ArrayList<>();

  public void add(float expense)
  {
    data.add(expense);
  }

  public void sort()
  {
    Collections.sort(data);
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (float expense : data)
      builder.append(expense).append(" ");
    builder.append("\n");

    return builder.toString();
  }

  float average()
  {
    float total = 0.0f;
    for (float expense : data)
      total += expense;

    return total / data.size();
  }

  public float exchange()
  {
    float total = 0.0f;
    float avg = average();
    for (float expense : data)
      if (expense >= avg)
        break;
      else
        total += avg - expense;
    return total;
  }
}

class skiena_1_6_3
{
  static List<Expenses> input()
  {
    List<Expenses> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (true)
    {
      // read in the number of students on this Expenses
      int count = scanner.nextInt();

      // end of input
      if (count == 0)
        break;
      else
      {
        // read in the expense of each student in this Expenses
        Expenses expenses = new Expenses();
        for (int i = 0; i < count; i++)
        {
          float expense = scanner.nextFloat();
          expenses.add(expense);
        }

        list.add(expenses);
      }
    }
    scanner.close();
    return list;
  }

  static void output(List<Expenses> list)
  {
    for (Expenses expenses : list)
    {
      // sort all the expenses, to facilitate calculating the average and the
      // exchange amount per student
      expenses.sort();
      // print output in the format required
      System.out.printf("$%.2f\n", expenses.exchange());
    }
  }

  public static void main(String[] args)
  {
    output(input());
  }
}
