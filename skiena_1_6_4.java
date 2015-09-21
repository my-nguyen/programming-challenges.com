import java.util.*;

class LCD
{
  // the size based on which to print a digit, with number of columns = base+2,
  // and number of rows = 2*base + 3
  int base;
  // a string of all digits, e.g. "12345", or "67890"
  String digits;

  public String toString()
  {
    Printer printer = new Printer();

    for (int row = 0; row < 2*base+3; row++)
    {
      // print all digits
      for (int i = 0; i < digits.length(); i++)
      {
        String tmp = printer.builder.toString();
        // blank column after each digit
        if (i != 0)
          printer.builder.append(" ");

        // for each digit
        for (int column = 0; column < base+2; column++)
        {
          // convert each digit from char to int
          int digit = Character.getNumericValue(digits.charAt(i));
          // using the int digit as an index to an array of functors, append
          // just one character forming the digit with base and at position(row, column)
          printer.functors[digit].execute(base, row, column);
        }
      }
      printer.builder.append(System.lineSeparator());
    }
    // one blank line after each string of digits
    printer.builder.append(System.lineSeparator());

    return printer.builder.toString();
  }
}

class Printer
{
  Functor[] functors = null;
  StringBuilder builder = new StringBuilder();

  public Printer()
  {
    if (functors == null)
    {
      // array of functors, each in charge of appending one character to a
      // StringBuilder based on array index, row, column, and base
      functors = new Functor[10];

      // functors[0], which is Functor0 is in charge of printing letter '0', etc
      functors[0] = new Functor0();
      functors[1] = new Functor1();
      functors[2] = new Functor2();
      functors[3] = new Functor3();
      functors[4] = new Functor4();
      functors[5] = new Functor5();
      functors[6] = new Functor6();
      functors[7] = new Functor7();
      functors[8] = new Functor8();
      functors[9] = new Functor9();
    }
  }

  public interface Functor
  {
    public void execute(int base, int row, int column);
  }

  class Functor0 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else if (row == base+1)
        builder.append(" ");
      else
      {
        if (column == 0 || column == base+1)
          builder.append("|");
        else
          builder.append(" ");
      }
    }
  }

  class Functor1 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
        builder.append(" ");
      else
      {
        if (column == base+1)
          builder.append("|");
        else
          builder.append(" ");
      }
    }
  }

  class Functor2 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (row <= base)
        {
          if (column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
        else
        {
          if (column == 0)
            builder.append("|");
          else
            builder.append(" ");
        }
      }
    }
  }

  class Functor3 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (column == base+1)
          builder.append("|");
        else
          builder.append(" ");
      }
    }
  }

  class Functor4 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == 2*base+2)
        builder.append(" ");
      else if (row == base+1)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (row <= base)
        {
          if (column == 0 || column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
        else
        {
          if (column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
      }
    }
  }

  class Functor5 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (row <= base)
        {
          if (column == 0)
            builder.append("|");
          else
            builder.append(" ");
        }
        else
        {
          if (column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
      }
    }
  }

  class Functor6 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (row <= base)
        {
          if (column == 0)
            builder.append("|");
          else
            builder.append(" ");
        }
        else
        {
          if (column == 0 || column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
      }
    }
  }

  class Functor7 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else if (row == base+1 || row == 2*base+2)
        builder.append(" ");
      else
      {
        if (column == base+1)
          builder.append("|");
        else
          builder.append(" ");
      }
    }
  }

  class Functor8 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (column == 0 || column == base+1)
          builder.append("|");
        else
          builder.append(" ");
      }
    }
  }

  class Functor9 implements Functor
  {
    public void execute(int base, int row, int column)
    {
      String tmp = builder.toString();
      if (row == 0 || row == base+1 || row == 2*base+2)
      {
        if (column == 0 || column == base+1)
          builder.append(" ");
        else
          builder.append("-");
      }
      else
      {
        if (row <= base)
        {
          if (column == 0 || column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
        else
        {
          if (column == base+1)
            builder.append("|");
          else
            builder.append(" ");
        }
      }
    }
  }
}

class skiena_1_6_4
{
  static List<LCD> input()
  {
    List<LCD> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (true)
    {
      // read the 2 integers
      int base = scanner.nextInt();
      int digits = scanner.nextInt();

      // end of input
      if (base == 0 || digits == 0)
        break;
      else
      {
        LCD lcd = new LCD();
        lcd.base = base;
        lcd.digits = Integer.toString(digits);
        list.add(lcd);
      }
    }
    scanner.close();
    return list;
  }

  static void output(List<LCD> list)
  {
    for (LCD lcd : list)
      System.out.print(lcd);
  }

  public static void main(String[] args)
  {
    List<LCD> list = input();
    output(list);
  }
}
