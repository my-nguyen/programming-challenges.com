import java.util.*;
import java.io.*;

class skiena_1_6_5
{
  static List<Command> input()
  {
    List<Command> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (true)
    {
      Command command = null;
      // read and parse all tokens in each line
      String line = scanner.nextLine();
      String[] tokens = line.split(" ");
      // decipher the character code
      char letter = tokens[0].charAt(0);
      // terminate program
      if (letter == 'X')
        break;
      else
      {
        switch(letter)
        {
        case 'I':
          command = new Initialize(tokens);
          break;
        case 'C':
          command = new Clear(tokens);
          break;
        case 'L':
          command = new Color(tokens);
          break;
        case 'V':
          command = new Vertical(tokens);
          break;
        case 'H':
          command = new Horizontal(tokens);
          break;
        case 'K':
          command = new Draw(tokens);
          break;
        case 'F':
          command = new Fill(tokens);
          break;
        case 'S':
          command = new Save(tokens);
          break;
        }
      }

      if (command != null)
        list.add(command);
    }
    scanner.close();
    return list;
  }

  static void output(List<Command> list)
  {
    for (Command command : list)
      command.execute();
  }

  public static void main(String[] args)
  {
    List<Command> list = input();
    output(list);
  }
}

// base class; all subclasses will have the specific functions
class Command
{
  // There's only one image, created by the I command. all commands such as
  // C, L, V, H, K, F, S operate on this single image.
  static Image image = null;

  // virtual method, to be overriden by all subclasses
  void execute()
  {
  }
}

class Initialize extends Command
{
  int column_count;
  int row_count;

  public Initialize(String[] tokens)
  {
    column_count = Integer.parseInt(tokens[1]);
    row_count = Integer.parseInt(tokens[2]);
  }

  void execute()
  {
    image = new Image(column_count, row_count);
  }
}

class Clear extends Command
{
  public Clear(String[] tokens)
  {
  }

  void execute()
  {
    image.clear();
  }
}

class Color extends Command
{
  int column;
  int row;
  char color;

  public Color(String[] tokens)
  {
    column = Integer.parseInt(tokens[1]);
    row = Integer.parseInt(tokens[2]);
    color = tokens[3].charAt(0);
  }

  void execute()
  {
    image.set(column, row, color);
  }
}

class Vertical extends Command
{
  int column;
  int row1;
  int row2;
  char color;

  public Vertical(String[] tokens)
  {
    column = Integer.parseInt(tokens[1]);
    row1 = Integer.parseInt(tokens[2]);
    row2 = Integer.parseInt(tokens[3]);
    color = tokens[4].charAt(0);
  }

  void execute()
  {
    image.vertical(column, row1, row2, color);
  }
}

class Horizontal extends Command
{
  int column1;
  int column2;
  int row;
  char color;

  public Horizontal(String[] tokens)
  {
    column1 = Integer.parseInt(tokens[1]);
    column2 = Integer.parseInt(tokens[2]);
    row = Integer.parseInt(tokens[3]);
    color = tokens[4].charAt(0);
  }

  void execute()
  {
    image.horizontal(column1, column2, row, color);
  }
}

class Draw extends Command
{
  int column1;
  int column2;
  int row1;
  int row2;
  char color;

  public Draw(String[] tokens)
  {
    column1 = Integer.parseInt(tokens[1]);
    column2 = Integer.parseInt(tokens[2]);
    row1 = Integer.parseInt(tokens[3]);
    row2 = Integer.parseInt(tokens[4]);
    color = tokens[5].charAt(0);
  }

  void execute()
  {
    image.draw(column1, column2, row1, row2, color);
  }
}

class Fill extends Command
{
  int column;
  int row;
  char color;

  public Fill(String[] tokens)
  {
    column = Integer.parseInt(tokens[1]);
    row = Integer.parseInt(tokens[2]);
    color = tokens[3].charAt(0);
  }

  void execute()
  {
    image.fill(column, row, color);
  }
}

class Save extends Command
{
  String filename;

  public Save(String[] tokens)
  {
    filename = tokens[1];
  }

  void execute()
  {
    image.save(filename);
  }
}

class Image
{
  // since we need the ability to re-write characters in each row (string) of
  // the data, and since Java String is immutable (can't be re-written),
  // char[] is chosen instead of String
  List<char[]> data;
  // final char WHITE = '.';
  final char WHITE = 'O';

  // this constructor creates a new column_count x row_count image with all
  // pixels initially colored white 'O'
  Image(int column_count, int row_count)
  {
    data = new ArrayList<>();
    for (int i = 0; i < row_count; i++)
    {
      char[] row = new char[column_count];
      for (int j = 0; j < column_count; j++)
        row[j] = WHITE;
      data.add(row);
    }
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (int i = 0; i < data.size(); i++)
    {
      for (int j = 0; j < data.get(i).length; j++)
        builder.append(data.get(i)[j]);
      builder.append("\n");
    }
    return builder.toString();
  }

  // this method clears the image by setting all pixels white '0'. The size
  // remains unchanged
  void clear()
  {
    for (int i = 0; i < data.size(); i++)
      for (int j = 0; j < data.get(i).length; j++)
        data.get(i)[j] = WHITE;
  }

  // this method retunrs the color of the pixel at (column, row)
  char get(int column, int row)
  {
    return data.get(row-1)[column-1];
  }

  // this method colors the pixel at (column, row) in color
  void set(int column, int row, char color)
  {
    data.get(row-1)[column-1] = color;
  }

  void sort(int left, int right)
  {
    List<Integer> list = Arrays.asList(left, right);
    Collections.sort(list);
    left = list.get(0);
    right = list.get(1);
  }

  // this method draws a vertical segment of color in column, between row1 and
  // row2 inclusive.
  void vertical(int column, int row1, int row2, char color)
  {
    // sort row1 and row2, for the benefit of the following for loop
    sort(row1, row2);
    for (int i = row1-1; i < row2; i++)
      data.get(i)[column-1] = color;
  }

  // this method draws a horizontal segment of color in row , between column1
  // and column2 inclusive.
  void horizontal(int column1, int column2, int row, char color)
  {
    // sort column1 and column2, for the benefit of the following for loop
    sort(column1, column2);
    for (int i = column1-1; i < column2; i++)
      data.get(row-1)[i] = color;
  }

  // this method draws a filled rectangle with color, where (column1, row1) is
  // the upper-left and (column2, row2) the lower right corner.
  void draw(int column1, int row1, int column2, int row2, char color)
  {
    // sort row1, row2, column1 and column2 for the benefit of the for loops
    sort(row1, row2);
    sort(column1, column2);
    for (int i = row1-1; i < row2; i++)
      for (int j = column1-1; j < column2; j++)
        data.get(i)[j] = color;
  }

  // this method fills a region with color, where region is defined as follows.
  // Pixel at (column, row) belongs to region. Any other pixel which is the same
  // color as pixel at (column, row) and shares a common side with any pixel in
  // region also belongs to this region.
  void fill(int column, int row, char color)
  {
    char old_color = get(column, row);
    fill(column, row, old_color, color);
  }

  // internal recursive method, used by the other fill()
  void fill(int column, int row, char old_color, char new_color)
  {
    if (row >= 1 && row <= data.size() &&
      column >= 1 && column <= data.get(0).length &&
      data.get(row-1)[column-1] == old_color)
    {
      data.get(row-1)[column-1] = new_color;

      fill(column-1, row, old_color, new_color);
      fill(column+1, row, old_color, new_color);
      fill(column, row-1, old_color, new_color);
      fill(column, row+1, old_color, new_color);
    }
  }

  // this method writes the file name in MSDOS 8.3 format followed by the
  // contents of the current image
  void save(String filename)
  {
    try
    {
      PrintWriter writer = new PrintWriter(filename);
      writer.write(toString());
      writer.close();
    } catch(FileNotFoundException e)
    {
      System.out.println("Exception: " + e);
    }

    System.out.print(filename + "\n" + this);
  }
}
