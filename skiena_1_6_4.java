import java.util.*;

class LCD
{
  public LCD(int base, int number)
  {
    size = base;
    // convert int to a string of digits
    digits = Integer.toString(number);
  }

  // the size based on which to print a digit, with number of columns = size+2,
  // and number of rows = 2*size + 3
  int size;
  // a string of all digits, e.g. "12345", or "67890"
  String digits;

  // array of Print's, each in charge of returning one character based on the
  // row, column, digit (array index) and size
  static Print[] prints = new Print[10];
  static
  {
    // prints[0], which is Print0 is in charge of printing letter '0', etc
    prints[0] = new Print0();
    prints[1] = new Print1();
    prints[2] = new Print2();
    prints[3] = new Print3();
    prints[4] = new Print4();
    prints[5] = new Print5();
    prints[6] = new Print6();
    prints[7] = new Print7();
    prints[8] = new Print8();
    prints[9] = new Print9();
  }

  // due to the problem requirements, a string of digits (which is a number),
  // is printed as follows: at each row, print all the characters that form a
  // digit at that row, followed by a blank, then repeat the process for others.
  // For example, the string 84 of size 3 would be printed as:
  // at row 0: " ---       "
  // at row 1: "|   | |   |"
  // at row 2: "|   | |   |"
  // at row 3: "|   | |   |"
  // at row 4: " ---   --- "
  // at row 5: "|   |     |"
  // at row 6: "|   |     |"
  // at row 7: "|   |     |"
  // at row 8: " ---       "
  // note how this satisfies the requirement of size + 2 columns and 2size + 3 rows
  public String toString()
  {
    StringBuilder builder = new StringBuilder();

    // print row by row
    for (int row = 0; row < 2*size+3; row++)
    {
      // print all digits in each row
      for (int i = 0; i < digits.length(); i++)
      {
        // blank column after each digit
        if (i != 0)
          builder.append(" ");

        // for each digit
        for (int column = 0; column < size+2; column++)
        {
          // convert each digit from char to int
          int digit = Character.getNumericValue(digits.charAt(i));
          // using the int digit as an index to an array of prints, append just
          // one character forming the digit based on size and position(row, column)
          builder.append(prints[digit].toChar(size, row, column));
        }
      }
      builder.append(System.lineSeparator());
    }
    // one blank line after each string of digits
    builder.append(System.lineSeparator());

    return builder.toString();
  }

  // practice Java functor
  public interface Print
  {
    public char toChar(int size, int row, int column);
  }

  // all Print? classes must be static to allow instantiation in the static
  // block in class LCD
  static class Print0 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else if (row == size+1)
        return ' ';
      else
      {
        if (column == 0 || column == size+1)
          return '|';
        else
          return ' ';
      }
    }
  }

  static class Print1 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
        return ' ';
      else
      {
        if (column == size+1)
          return '|';
        else
          return ' ';
      }
    }
  }

  static class Print2 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (row <= size)
        {
          if (column == size+1)
            return '|';
          else
            return ' ';
        }
        else
        {
          if (column == 0)
            return '|';
          else
            return ' ';
        }
      }
    }
  }

  static class Print3 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (column == size+1)
          return '|';
        else
          return ' ';
      }
    }
  }

  static class Print4 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == 2*size+2)
        return ' ';
      else if (row == size+1)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (row <= size)
        {
          if (column == 0 || column == size+1)
            return '|';
          else
            return ' ';
        }
        else
        {
          if (column == size+1)
            return '|';
          else
            return ' ';
        }
      }
    }
  }

  static class Print5 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (row <= size)
        {
          if (column == 0)
            return '|';
          else
            return ' ';
        }
        else
        {
          if (column == size+1)
            return '|';
          else
            return ' ';
        }
      }
    }
  }

  static class Print6 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (row <= size)
        {
          if (column == 0)
            return '|';
          else
            return ' ';
        }
        else
        {
          if (column == 0 || column == size+1)
            return '|';
          else
            return ' ';
        }
      }
    }
  }

  static class Print7 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else if (row == size+1 || row == 2*size+2)
        return ' ';
      else
      {
        if (column == size+1)
          return '|';
        else
          return ' ';
      }
    }
  }

  static class Print8 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (column == 0 || column == size+1)
          return '|';
        else
          return ' ';
      }
    }
  }

  static class Print9 implements Print
  {
    public char toChar(int size, int row, int column)
    {
      if (row == 0 || row == size+1 || row == 2*size+2)
      {
        if (column == 0 || column == size+1)
          return ' ';
        else
          return '-';
      }
      else
      {
        if (row <= size)
        {
          if (column == 0 || column == size+1)
            return '|';
          else
            return ' ';
        }
        else
        {
          if (column == size+1)
            return '|';
          else
            return ' ';
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
      int size = scanner.nextInt();
      int digits = scanner.nextInt();

      // end of input
      if (size == 0 || digits == 0)
        break;
      else
      {
        LCD lcd = new LCD(size, digits);
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
