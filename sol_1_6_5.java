import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.ArrayList;


class sol_1_6_5 {
   private static void input(ArrayList<Command> list) throws FileNotFoundException {
      Scanner scanner = new Scanner(System.in);
      while (true) {
         String line = scanner.nextLine();
         String[] tokens = line.split(" ");
         Command tmp = new Command();
         tmp._command = tokens[0].charAt(0);
         switch(tmp._command)
         {
         case 'I':
            tmp._x1 = Integer.parseInt(tokens[1]);
            tmp._y1 = Integer.parseInt(tokens[2]);
            break;
         case 'C':
            break;
         case 'L':
            tmp._x1 = Integer.parseInt(tokens[1]);
            tmp._y1 = Integer.parseInt(tokens[2]);
            tmp._color = tokens[3].charAt(0);
            break;
         case 'V':
            tmp._x1 = Integer.parseInt(tokens[1]);
            tmp._y1 = Integer.parseInt(tokens[2]);
            tmp._y2 = Integer.parseInt(tokens[3]);
            tmp._color = tokens[4].charAt(0);
            break;
         case 'H':
            tmp._x1 = Integer.parseInt(tokens[1]);
            tmp._x2 = Integer.parseInt(tokens[2]);
            tmp._y1 = Integer.parseInt(tokens[3]);
            tmp._color = tokens[4].charAt(0);
            break;
         case 'K':
            tmp._x1 = Integer.parseInt(tokens[1]);
            tmp._y1 = Integer.parseInt(tokens[2]);
            tmp._x2 = Integer.parseInt(tokens[3]);
            tmp._y2 = Integer.parseInt(tokens[4]);
            tmp._color = tokens[5].charAt(0);
            break;
         case 'F':
            tmp._x1 = Integer.parseInt(tokens[1]);
            tmp._y1 = Integer.parseInt(tokens[2]);
            tmp._color = tokens[3].charAt(0);
            break;
         case 'S':
            tmp._filename = tokens[1];
            break;
         case 'X':
            break;
         }


         if (tmp._command == 'X')
            break;
         else
            list.add(tmp);
      }
      scanner.close();
   }


   private static void output(ArrayList<Command> list) throws FileNotFoundException {
      for (int i = 0; i < list.size(); i++)
         list.get(i).execute();
   }


   public static void main(String[] args) throws FileNotFoundException {
      ArrayList<Command> list = new ArrayList<Command>();
      input(list);
      output(list);
   }
}


class Command {
   public char   _command;
   public int   _x1;
   public int   _x2;
   public int   _y1;
   public int   _y2;
   public char   _color;
   public String _filename;


   // There's only one image, created by the one command "I" (all commands
   // such as C, L, V, H, K, F, S work on this single image). So, let's make
   // the image static. Since image is only referenced in method
   // Command.execute(), let's make image private.
   // In a future implementation where there are many images, then we need
   // to declare the member as ArrayList<Image>
   private static Image _image = new Image();


   void execute() throws FileNotFoundException {
      switch (_command)
      {
      case 'I':
         _image.create(_x1, _y1);
         break;
      case 'C':
         _image.clear();
         break;
      case 'L':
         _image.color(_x1, _y1, _color);
         break;
      case 'V':
         _image.vertical(_x1, _y1, _y2, _color);
         break;
      case 'H':
         _image.horizontal(_x1, _x2, _y1, _color);
         break;
      case 'K':
         _image.rectangle(_x1, _y1, _x2, _y2, _color);
         break;
      case 'F':
         _image.fill(_x1, _y1, _image.get(_x1, _y1), _color);
         break;
      case 'S':
         _image.save(_filename);
         break;
      case 'X':
         break;
      }
   }
}


class Image {
   private ArrayList<char[]> _pixels;
   // private final char WHITE = '.';
   private final char WHITE = 'O';


   public String toString() {
      StringBuilder builder = new StringBuilder();
      for (int i = 0; i < _pixels.size(); i++) {
         for (int j = 0; j < _pixels.get(i).length; j++)
            builder.append(_pixels.get(i)[j]);
         builder.append("\n");
      }
      return builder.toString();
   }


   // Create a new M x N image with all pixels initially colored white (.)
   public void create(int columns, int rows) {
      _pixels = new ArrayList<char[]>();
      for (int i = 0; i < rows; i++) {
         char[] string = new char[columns];
         for (int j = 0; j < columns; j++)
            string[j] = WHITE;
         _pixels.add(string);
      }
   }


   // Clear the table by setting all pixels white (.). The size remains
   // unchanged.
   public void clear() {
      for (int i = 0; i < _pixels.size(); i++)
         for (int j = 0; j < _pixels.get(i).length; j++)
            _pixels.get(i)[j] = WHITE;
   }


   // Return the color of the pixel at (column, row)
   char get(int column, int row) {
      return _pixels.get(row-1)[column-1];
   }


   // Colors the pixel (X, Y ) in color (C)
   void color(int column, int row, char couleur) {
      _pixels.get(row-1)[column-1] = couleur;
   }


   // Draw a vertical segment of color (C) in column X, between the rows Y1
   // and Y2 inclusive.
   void vertical(int column, int row1, int row2, char color) {
      // sort row1 and row2, for the benefit of the following for loop
      Sort sort = new Sort(row1, row2);
      for (int i = sort._low-1; i < sort._high; i++)
         _pixels.get(i)[column-1] = color;
   }


   // Draw a horizontal segment of color (C) in the row Y, between the columns
   // X1 and X2 inclusive.
   void horizontal(int column1, int column2, int row, char color) {

      // sort column1 and column2, for the benefit of the following for loop
      Sort sort = new Sort(column1, column2);
      for (int i = sort._low-1; i < sort._high; i++)
         _pixels.get(row-1)[i] = color;
   }


   // Draw a filled rectangle of color C, where (X1, Y1) is the upper-left and
   // (X2, Y2) the lower right corner.
   void rectangle(int column1, int row1, int column2, int row2, char color) {
      // sort row1, row2, column1 and column2 for the benefit of the for loops
      Sort sort1 = new Sort(row1, row2);
      Sort sort2 = new Sort(column1, column2);
      for (int i = sort1._low-1; i < sort1._high; i++)
         for (int j = sort2._low-1; j < sort2._high; j++)
            _pixels.get(i)[j] = color;
   }


   // Fill the region R with the color C, where R is defined as follows. Pixel
   // (X, Y) belongs to R. Any other pixel which is the same color as pixel
   // (X, Y) and shares a common side with any pixel in R also belongs to this
   // region.
   void fill(int column, int row, char old_color, char new_color) {
      if (row < 1 || row > _pixels.size() ||
         column < 1 || column > _pixels.get(0).length ||
         _pixels.get(row-1)[column-1] != old_color)
         ;
      else
      {
         _pixels.get(row-1)[column-1] = new_color;

         fill(column-1, row, old_color, new_color);
         fill(column+1, row, old_color, new_color);
         fill(column, row-1, old_color, new_color);
         fill(column, row+1, old_color, new_color);
      }
   }


   // Write the file name in MSDOS 8.3 format followed by the contents of the
   // current image
   void save(String filename) throws FileNotFoundException {
      PrintWriter writer = new PrintWriter(new File(filename));
      writer.write(toString());
      writer.close();

      System.out.print(filename + "\n" + this);
   }
}


class Sort {
   public int _low;
   public int _high;

   public Sort(int one, int two) {
      if (one > two)
      {
         _low = two;
         _high = one;
      }
      else
      {
         _low = one;
         _high = two;
      }
   }
}
