import java.util.*;

// this class is a base class for the 2 classes PaddedField and CalculatedField.
// it declares a common data field, and a common toString method.
class Field
{
  List<String> data = new ArrayList<>();

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (String line : data)
      builder.append(line).append('\n');
    return builder.toString();
  }
}

// this class contains the data field as read from input, with 2 extra lines and
// 2 extra columns added as 4 borders around the actual data field. the borders
// serve to facilitate calculating the number of mines in the 8 surrounding
// cells of any cell. So, if the field from input file is:
//		*...
//		....
//		.*..
//		....
// Then the internal data field looks like this:
//		++++++
//		+*...+
//		+....+
//		+.*..+
//		++++++
class PaddedField extends Field
{
  // this constructor retains input data from a list of String's by padding the
  // 4 borders as described above.
  PaddedField(List<String> list)
  {
    // add a blank string at the beginning
    data.add(blanks(list.get(0).length() + 2));
    // store all input strings while padding a blank at both the beginning and
    // the end of each string
    for (String string : list)
      data.add("+" + string + "+");
    // add a blank string to the end
    data.add(blanks(list.get(0).length() + 2));
  }

  // this method prints the data field exactly as read from input (with the 4
  // borders omitted)
  public String toString2()
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

  // this method returns whether this cell contain a mine
  int is_mined(char value)
  {
    return (value == '*') ? 1 : 0;
  }

  // this method returns the total number of mines in the surrounding 8 cells
  int mine_total(int i, int j)
  {
    return is_mined(data.get(i-1).charAt(j-1)) +
           is_mined(data.get(i-1).charAt(j)) +
           is_mined(data.get(i-1).charAt(j+1)) +
           is_mined(data.get(i).charAt(j-1)) +
           is_mined(data.get(i).charAt(j+1)) +
           is_mined(data.get(i+1).charAt(j-1)) +
           is_mined(data.get(i+1).charAt(j)) +
           is_mined(data.get(i+1).charAt(j+1));
  }

  // this utility method returns a string whose content is filled with blank
  // characters "+"
  String blanks(int size)
  {
    char[] array = new char[size];
    Arrays.fill(array, '+');
    return new String(array);
  }
}

// this class represents the field whose every cell contains the calculated
// number of mines in its surrounding 8 cells.
class CalculatedField extends Field
{
  // this method builds the internal data field, by recording the mines where
  // they are present in data, and also the number of mines in the surrounding
  // 8 cells, for each cell
  CalculatedField(PaddedField field)
  {
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
}

class skiena_1_6_2
{
  // this method reads data from STDIN. The first line of each field contains 2
  // integers which stand for the number of lines and columns of the field. Each
  // of the next n lines contains exactly m characters, representing the field.
  // Safe squares are denoted by "." and mine squares by "*". The first field
  // line where n = m = 0 represents the end of input and should not be processed.
  static List<PaddedField> input()
  {
    List<PaddedField> list = new ArrayList<>();
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
        // create and retain a PaddedField of lines
        list.add(new PaddedField(lines));
      }
    }

    scanner.close();
    return list;
  }

  // this method sweeps thru a list of Fields; for each Field it prints out the
  // mine fields as well as fields which contain the total number of mines in
  // the surrounding 8 cells
  static void output(List<PaddedField> list)
  {
    // use traditional for loop instead of the advanced for loop because index
    // is needed in printing
    for (int i = 0; i < list.size(); i++)
    {
      // for each cell in the field, sweep the surrounding cells for mines and
      // store the number of mines in the cell
      CalculatedField output = new CalculatedField(list.get(i));

      // print output in the format required
      System.out.println();
      System.out.println("Field #" + (i+1) + ":");
      System.out.print(output);
    }
  }

  public static void main(String[] args)
  {
    output(input());
  }
}
