import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;


class Image {
    private ArrayList<char[]> _pixels;
    private final char WHITE = '.';


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

        if (Debug._level >= Debug.LIGHT)
            System.out.print("I " + columns + " " + rows + "\n" + this);
    }


    // Clear the table by setting all pixels white (.). The size remains
    // unchanged.
    public void clear() {
        for (int i = 0; i < _pixels.size(); i++)
            for (int j = 0; j < _pixels.get(i).length; j++)
                _pixels.get(i)[j] = WHITE;

        if (Debug._level >= Debug.LIGHT)
            System.out.print("C" + "\n" + this);
    }


    // Return the color of the pixel at (column, row)
    char get(int column, int row) {
        if (Debug._level >= Debug.COPIOUS)
            System.out.print("pixel at (" + column + ", " + row + "): '" +
                             _pixels.get(row-1)[column-1] + "'\n");
        return _pixels.get(row-1)[column-1];
    }


    // Colors the pixel (X, Y ) in color (C)
    void color(int column, int row, char couleur) {
        _pixels.get(row-1)[column-1] = couleur;

        if (Debug._level >= Debug.LIGHT)
            System.out.print("L " + column + " " + row + " " +
                             couleur + "\n" + this);
    }


    // Draw a vertical segment of color (C) in column X, between the rows Y1
    // and Y2 inclusive.
    void vertical(int column, int row1, int row2, char color) {
        // sort row1 and row2, for the benefit of the following for loop
        Sort sort = new Sort(row1, row2);
        for (int i = sort._low-1; i < sort._high; i++)
            _pixels.get(i)[column-1] = color;

        if (Debug._level >= Debug.LIGHT)
            System.out.print("V " + column + " " + row1 + " " +
                             row2 + " " + color + "\n" + this);
    }


    // Draw a horizontal segment of color (C) in the row Y, between the columns
    // X1 and X2 inclusive.
    void horizontal(int column1, int column2, int row, char color) {
        // sort column1 and column2, for the benefit of the following for loop
        Sort sort = new Sort(column1, column2);
        for (int i = sort._low-1; i < sort._high; i++)
            _pixels.get(row-1)[i] = color;

        if (Debug._level >= Debug.LIGHT)
            System.out.print("H " + column1 + " " + column2 + " " +
                             row + " " + color + "\n" + this);
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

        if (Debug._level >= Debug.LIGHT)
            System.out.print("K " + column1 + " " + row1 + " " + column2 +
                             " " + row2 + " " + color + "\n" + this);
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

        if (Debug._level >= Debug.LIGHT)
            System.out.print("S " + filename + "\n" + this);
    }
}
