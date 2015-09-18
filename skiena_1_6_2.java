import java.util.*;

class skiena_1_6_2
{
  // this method reads data from STDIN. The first line of each field contains 2
  // integers which stand for the number of lines and columns of the field. Each
  // of the next n lines contains exactly m characters, representing the field.
  // Safe squares are denoted by "." and mine squares by "*". The first field
  // line where n = m = 0 represents the end of input and should not be processed.
  static List<input_t> input()
  {
    List<input_t> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (true)
    {
      // read 2 integers
      int line_count = scanner.nextInt();
      int column_count = scanner.nextInt();
      // throw away the EOL, to prepare for the next line of text
      scanner.nextLine();

      // end of input
      if (line_count == 0 || column_count == 0)
        break;
      else
      {
        List<String> lines = new ArrayList<>();
        // read the next line_count lines
        for (int i = 0; i < line_count; i++)
          lines.add(scanner.nextLine());

        // create and retain a input_t of lines
        list.add(new input_t(lines));
      }
    }

    scanner.close();
    return list;
  }

  // this method sweeps thru a list of Fields; for each Field it prints out the
  // mine fields as well as fields which contain the total number of mines in
  // the surrounding 8 cells
  static void output(List<input_t> list)
  {
    // use traditional for loop instead of the advanced for loop because index
    // is needed in printing
    for (int i = 0; i < list.size(); i++)
    {
      // for each cell in the field, sweep the surrounding cells for mines and
      // store the number of mines in the cell
      output_t output = new output_t(list.get(i));

      // print output in the format required
      System.out.println();
      System.out.println("Field #" + (i+1) + ":");
      System.out.print(output);
    }
  }

  public static void main(String[] args)
  {
    List<input_t> list = input();
    output(list);
  }
}

class input_t
{
  // data has 2 more lines and 2 more columns than the actual number of
  // lines and columns read from input file. The extra lines and columns will
  // be initialized to '+'. This serves to facilitate calculating the number
  // of mines in the surrounding cells of any particular cell. So, if the
  // field from input file is:
  //		*...
  //		....
  //		.*..
  //		....
  // Then the internal data field looks something like this:
  //		++++++
  //		+*...+
  //		+....+
  //		+.*..+
  //		++++++
  List<String> data;

  // this method creates a new string having the desired length, and padd the
  // contents of such string with blank characters "+"
  String blanks(int size)
  {
    char[] array = new char[size];
    Arrays.fill(array, '+');
    return new String(array);
  }

  // this constructor initializes internal data from a list of strings
  public input_t(List<String> list)
  {
    // create a new ArrayList
    data = new ArrayList<>();

    // add a blank string at the beginning
    data.add(blanks(list.get(0).length() + 2));

    // store the date while padding a blank at both the beginning and the end
    for (String string : list)
    {
      StringBuilder builder = new StringBuilder();
      builder.append('+').append(string).append('+');
      data.add(builder.toString());
    }

    // add a blank string to the end
    data.add(blanks(list.get(0).length() + 2));
  }

  // this method prints only the field contained within the surrounding cells
  // containing '+'
  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (int i = 1; i < data.size()-1; i++)
    {
      for (int j = 1; j < data.get(i).length()-1; j++)
        builder.append(data.get(i).charAt(j));
      builder.append('\n');
    }
    return builder.toString();
  }

  // this method prints everything in data
  public void print()
  {
    for (String line : data)
      System.out.println(line);
  }

  // this method returns whether this cell contain a mine
  int mined(char value)
  {
    return (value == '*') ? 1 : 0;
  }

  // this method returns the total number of mines in the surrounding 8 cells
  int mine_total(int i, int j)
  {
    return mined(data.get(i-1).charAt(j-1)) +
           mined(data.get(i-1).charAt(j)) +
           mined(data.get(i-1).charAt(j+1)) +
           mined(data.get(i).charAt(j-1)) +
           mined(data.get(i).charAt(j+1)) +
           mined(data.get(i+1).charAt(j-1)) +
           mined(data.get(i+1).charAt(j)) +
           mined(data.get(i+1).charAt(j+1));
  }
}

class output_t
{
  // this method builds the internal data field, by recording the mines where
  // they are present in data, and also the number of mines in the surrounding
  // 8 cells, for each cell
  public output_t(input_t field)
  {
    data = new ArrayList<>();

    // remember: data has 2 extra lines
    for (int i = 1; i < field.data.size()-1; i++)
    {
      StringBuilder builder = new StringBuilder();
      // each line in data has 2 extra columns (at beginning and at end)
      for (int j = 1; j < field.data.get(i).length()-1; j++)
      {
        // if this input cell contains a mine, record the mine in the output cell
        if (field.data.get(i).charAt(j) == '*')
          builder.append('*');
        // otherwise, calculate the number of mines in the surrounding 8 cells
        // and record that number in the output cell
        else
        {
          int mine_count = Character.getNumericValue('0') + field.mine_total(i, j);
          builder.append(Character.forDigit(mine_count, 10));
        }
      }
      // retain the string
      data.add(builder.toString());
    }
  }

  public String toString()
  {
    // this method prints everything in data
    StringBuilder builder = new StringBuilder();
    for (String line : data)
      builder.append(line).append('\n');
    return builder.toString();
  }

  // data contains the mine indicia as well as the calculated value of each
  // of the cell in the field. This doesn't need the special extra 2 lines and 2
  // columns, as opposed to data
  private List<String> data;
}
