import java.util.*;

// this class represents the location of a chess piece or a square on a board
class Location
{
  int row;
  int column;

  Location(int rang, int colonne)
  {
    row = rang;
    column = colonne;
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append("(").append(Integer.toString(row)).append(", ");
    builder.append(Integer.toString(column)).append(")");
    return builder.toString();
  }
}

// this class represents a chess board
class Board
{
  static final int MAX_SIDE = 8;
  String[] data = new String[MAX_SIDE];

  // this method checks to see if the current board contains 8 rows of "........"
  boolean is_empty()
  {
    for (String string : data)
      if (!string.equals("........"))
        return false;
    return true;
  }

  // this method finds a king (k or K) from board and returns the location of
  // the king
  Location find_king(char king)
  {
    int i, j = 0;
    for (i = 0; i < MAX_SIDE; i++)
    {
      j = data[i].indexOf(king);
      if (j != -1)
        break;
    }
    return new Location(i, j);
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (String row : data)
      builder.append(row).append("\n");
    return builder.toString();
  }
}

// this class represents a blocker, whether it's a threat or a block (which is
// either a harmless hostile or a friendly). it contains information pertaining
// to the blocker such as its id (chess piece), its location and whether it's a
// threat or not.
class Blocker
{
  char id;
  Location location;
  boolean is_enemy;

  Blocker(char pi, Location loc, boolean enemy)
  {
    id = pi;
    location = loc;
    is_enemy = enemy;
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append(is_enemy ? "checked" : "blocked");
    builder.append(" by ").append(id).append(" at ").append(location);
    return builder.toString();
  }
}

// this class represents a king on a chess board
class King
{
  Board board;
  Location location;

  King(Board bd, Location loc)
  {
    board = bd;
    location = loc;
  }

  // this method checks whether row and column are within bounds (0 and MAX_SIDE)
  // assuming that the increments have already been added to row and column.
  boolean condition(int row_increment, int column_increment, int row, int column)
  {
    boolean row_condition, column_condition;
    // whether row is positive and less than MAX_SIDE
    if (row_increment != 0)
      row_condition = (row < Board.MAX_SIDE && row >= 0);
    else
      row_condition = true;
    // whether column is positive and less than MAX_SIDE
    if (column_increment != 0)
      column_condition = (column < Board.MAX_SIDE && column >= 0);
    else
      column_condition = true;

    // whether both row and column are within bounds
    return row_condition && column_condition;
  }

  // this method scans in 8 directions: up, down, left, right, and the 4
  // diagonal directions for the first threat (an opponent Queen, Rook or Bishop)
  // or a block (a friendly or a harmless opponent which may or may not be in
  // the way of a threat). if a threat or a block is found, the blocker is
  // returned; the blocker contains information such as: the id of the chess
  // piece, its location on the chess board, and whether it's a threat or a
  // block. if no such threat or block is found, a null is returned.
  Blocker scan_8_directions(int row_increment, int column_increment, String straight_enemies)
  {
    Blocker blocker = null;
    // set a starting point for scanning before looping.
    int i = location.row + row_increment;
    int j = location.column + column_increment;
    // keep scanning until out of bounds or a blocker is met
    while (condition(row_increment, column_increment, i, j))
    {
      char square = board.data[i].charAt(j);
      // square is blocked by some piece, friendly or enemy
      if (square != '.')
      {
        // save information pertaining to the blocker and stop scanning
        boolean is_enemy = (straight_enemies.indexOf(square) != -1);
        blocker = new Blocker(square, new Location(i, j), is_enemy);
        break;
      }
      else
      {
        // the increments can be 1, -1, or 0. for example, if row_increment is 1
        // then the scan is downward/southbound; additionally if column_increment
        // is -1 then the scan is southwestbound, if column_increment is 0 then
        // the scan is straight southbound, and if column_increment is 1 then
        // the scan is southeastbound. on the other hand, if row_increment is -1
        // then the scan is upward/northbound, and so on.
        i += row_increment;
        j += column_increment;
      }
    }
    return blocker;
  }

  // this method scans for a pawn in 2 positions in front of the current king
  // and for a knight in 8 positions around the king. if a pawn or a knight is
  // found, such is returned. otherwise a null is returned.
  Blocker scan_pawn_knight(int row_increment, int column_increment, char piece)
  {
    Blocker blocker = null;
    int i = location.row + row_increment;
    int j = location.column + column_increment;
    if (condition(row_increment, column_increment, i, j))
    {
      char square = board.data[i].charAt(j);
      if (square == piece)
        blocker = new Blocker(square, new Location(i, j), true);
    }
    return blocker;
  }
}

class skiena_1_6_7
{
  static List<Board> input()
  {
    List<Board> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    while (true)
    {
      Board board = new Board();
      // read a total of 8 lines and store them in a board
      for (int i = 0; i < Board.MAX_SIDE; i++)
        board.data[i] = scanner.nextLine();

      // exit loop if current board is empty (containing 8 lines of "........")
      if (board.is_empty())
        break;
      else
      {
        // retain the current board
        list.add(board);
        // discard a blank line before moving on to the next board
        scanner.nextLine();
      }
    }

    scanner.close();
    return list;
  }

  static void output(List<Board> list)
  {
    // go thru each board
    for (int l = 0; l < list.size(); l++)
    {
      Board board = list.get(l);
      // System.out.print("\nBOARD:\n" + board);

      // black king (k) at index 0 and white king (K) at index 1. the kings'
      // index will determine the placement of their correponding opponents (QR,
      // QB, P) and the pawn increments in the other variables right below
      char[] kings = { 'k', 'K' };
      // Queen and Rook are enemies in the straight directions (north, south,
      // east, west). Note the placements of the opponents (QR and qr)
      // corresponding to k and K
      String[] straight_enemies = { "QR", "qr"};
      // Queen and Bishop are enemies in the 4 diagonal directions
      String[] diagonal_enemies = { "QB", "qb"};
      char[] pawns = { 'P', 'p' };
      // since black sides reside at the top of the chess board, the black king
      // (k) is only concerned with pawns in the south direction (1, positive
      // increment). likewise, the white king (K) is only concerned with pawns
      // in the north direction (-1, negative increment)
      int[] pawn_increments = { 1, -1 };
      char[] knights = { 'N', 'n' };
      String[] names = { "black", "white" };
      // master list of all blockers of both kings.
      // Java doesn't allow array of List, so must declare the array as follows
      List<List<Blocker>> obstructors = new ArrayList<List<Blocker>>();
      // go thru both kings. for each king, scan for 12 possible blockers: 4 in
      // the straight directions (north, south, east, west), 4 in diagonal
      // directions, 2 knights and 2 pawns (in 8 positions)
      for (int i = 0; i < kings.length; i++)
      {
        Location location = board.find_king(kings[i]);
        // System.out.println(kings[i] + " is at " + location);

        King king = new King(board, location);
        // list of all blockers of the current king
        List<Blocker> blockers = new ArrayList<>();

        // scan up and down for opponent Queen and Rook
        blockers.add(king.scan_8_directions(1, 0, straight_enemies[i]));
        blockers.add(king.scan_8_directions(-1, 0, straight_enemies[i]));
        // scan left and right for opponent Queen and Rook
        blockers.add(king.scan_8_directions(0, 1, straight_enemies[i]));
        blockers.add(king.scan_8_directions(0, -1, straight_enemies[i]));

        // scan the 4 diagonal directions for opponent Queen and Bishop
        blockers.add(king.scan_8_directions(1, 1, diagonal_enemies[i]));
        blockers.add(king.scan_8_directions(1, -1, diagonal_enemies[i]));
        blockers.add(king.scan_8_directions(-1, 1, diagonal_enemies[i]));
        blockers.add(king.scan_8_directions(-1, -1, diagonal_enemies[i]));

        // scan for opponent Pawn
        blockers.add(king.scan_pawn_knight(pawn_increments[i], 1, pawns[i]));
        blockers.add(king.scan_pawn_knight(pawn_increments[i], -1, pawns[i]));

        // scan 8 possible positions for opponent Knight
        blockers.add(king.scan_pawn_knight(2, 1, knights[i]));
        blockers.add(king.scan_pawn_knight(2, -1, knights[i]));
        blockers.add(king.scan_pawn_knight(-2, 1, knights[i]));
        blockers.add(king.scan_pawn_knight(-2, -1, knights[i]));
        blockers.add(king.scan_pawn_knight(1, 2, knights[i]));
        blockers.add(king.scan_pawn_knight(1, -2, knights[i]));
        blockers.add(king.scan_pawn_knight(-1, 2, knights[i]));
        blockers.add(king.scan_pawn_knight(-1, -2, knights[i]));

        // report on the blockers for debugging
        /*
        for (Blocker blocker : blockers)
          if (blocker != null)
            System.out.println(kings[i] + " is " + blocker);
        */

        obstructors.add(blockers);
      } // for loop ({ 'k', 'K' })

      // report as per problem requirement
      boolean checked = false;
      System.out.print("Game #" + (l+1) + ": ");
      for (int i = 0; i < kings.length; i++)
      {
        for (Blocker blocker : obstructors.get(i))
        {
          if (blocker != null && blocker.is_enemy)
          {
            System.out.println(names[i] + " king is in check.");
            checked = true;
            break;
          }
        }
        // one king is in check. since there is "no configurations where both
        // kings are check", there's no need to examine the other king.
        if (checked)
          break;
      }
      if (!checked)
        System.out.println("no king is in check.");
    } // for loop(List<Board> list)
  } // output() method

  public static void main(String[] args)
  {
    List<Board> list = input();
    output(list);
  }
}
