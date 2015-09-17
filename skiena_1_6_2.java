import java.util.*;

class skiena_1_6_2
{
  // this method reads data from STDIN. The first line of each field contains 2
  // integers which stand for the number of lines and columns of the field. Each
  // of the next n lines contains exactly m characters, representing the field.
  // Safe squares are denoted by "." and mine squares by "*". The first field
  // line where n = m = 0 represents the end of input and should not be processed.
  static List<Field> input()
  {
    List<Field> list = new ArrayList<>();
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

        // create and retain a field of lines
        list.add(new Field(lines));
      }
    }

    scanner.close();
    return list;
  }

  // this method sweeps thru a list of Fields; for each Field it prints out the
  // mine fields as well as fields which contain the total number of mines in
  // the surrounding 8 cells
  static void output(List<Field> list)
  {
    // use traditional for loop instead of the advanced for loop because index
    // is needed in printing
    for (int i = 0; i < list.size(); i++)
    {
      // for each cell in the field, sweep the surrounding cells for mines and
      // store the number of mines in the cell
      list.get(i).sweep();

      // print output in the format required
      System.out.println();
      System.out.print("Field #" + (i+1) + ":\n");
      list.get(i).print_output();
    }
  }

  public static void main(String[] args)
  {
    List<Field> list = input();
    output(list);
  }
}

class Field
{
  // _input has 2 more lines and 2 more columns than the actual number of
  // lines and columns read from input file. The extra lines and columns will
  // be initialized to '+'. This serves to facilitate calculating the number
  // of mines in the surrounding cells of any particular cell. So, if the
  // field from input file is:
  //		*...
  //		....
  //		.*..
  //		....
  // Then the internal _input field looks something like this:
  //		++++++
  //		+*...+
  //		+....+
  //		+.*..+
  //		++++++
  List<String> _input;

  // _output contains the mine indicia as well as the calculated value of each
  // of the cell in the field. This doesn't need the special extra 2 lines and 2
  // columns, as opposed to _input
  List<String> _output;

  // this method creates a new string having the desired length, and padd the
  // contents of such string with blank characters "+"
  String blanks(int size)
  {
    char[] array = new char[size];
    Arrays.fill(array, '+');
    return new String(array);
  }

  // this constructor initializes internal data from a list of strings
  public Field(List<String> data)
  {
    // create a new ArrayList
    _input = new ArrayList<>();

    // add a blank string at the beginning
    _input.add(blanks(data.get(0).length() + 2));

    // store the date while padding a blank at both the beginning and the end
    for (String string : data)
    {
      StringBuilder builder = new StringBuilder();
      builder.append('+').append(string).append('+');
      _input.add(builder.toString());
    }

    // add a blank string to the end
    _input.add(blanks(data.get(0).length() + 2));
  }

  // this method prints only the field contained within the surrounding cells
  // containing '+'
  public void print_input()
  {
    for (int i = 1; i < _input.size()-1; i++)
    {
      for (int j = 1; j < _input.get(i).length()-1; j++)
        System.out.print(_input.get(i).charAt(j));
      System.out.println();
    }
  }

  // this method prints everything in _input
  public void print_internal()
  {
    for (String line : _input)
      System.out.println(line);
  }

  // this method prints everything in _output
  public void print_output()
  {
    for (String line : _output)
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
    return mined(_input.get(i-1).charAt(j-1)) +
           mined(_input.get(i-1).charAt(j)) +
           mined(_input.get(i-1).charAt(j+1)) +
           mined(_input.get(i).charAt(j-1)) +
           mined(_input.get(i).charAt(j+1)) +
           mined(_input.get(i+1).charAt(j-1)) +
           mined(_input.get(i+1).charAt(j)) +
           mined(_input.get(i+1).charAt(j+1));
  }

  // this method builds the internal _output field, by recording the mines where
  // they are present in _input, and also the number of mines in the surrounding
  // 8 cells, for each cell
  public void sweep()
  {
    // delay creation of _output field until now
    _output = new ArrayList<>();

    // remember: _input has 2 extra lines
    for (int i = 1; i < _input.size()-1; i++)
    {
      StringBuilder builder = new StringBuilder();
      // each line in _input has 2 extra columns (at beginning and at end)
      for (int j = 1; j < _input.get(i).length()-1; j++)
      {
        // if this input cell contains a mine, record the mine in the output cell
        if (_input.get(i).charAt(j) == '*')
          builder.append('*');
        // otherwise, calculate the number of mines in the surrounding 8 cells
        // and record that number in the output cell
        else
        {
          int mine_count = Character.getNumericValue('0') + mine_total(i, j);
          builder.append(Character.forDigit(mine_count, 10));
        }
      }
      // retain the string
      _output.add(builder.toString());
    }
  }
}
