import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Arrays;


class sol_1_6_7 {
   private static void input(ArrayList<Board> list) throws FileNotFoundException {
      // read the commmands and their corresponding parameters until
      // end of input
      Scanner scanner = new Scanner(System.in);
      while (true) {
         Board tmp = new Board();
         String line = null;
         for (int i = 0; i < Board.MAX_ROWS; i++) {
            // read a line from file, discard the trailing newline, then
            // store the line in the current board
            line = scanner.nextLine();
            tmp._list[i] = Arrays.copyOf(line.toCharArray(), line.length());
         }

         // exit loop if last board was empty
         if (tmp.empty())
            break;

         // discard the blank line before moving on to the next board
         else {
            list.add(tmp);
            line = scanner.nextLine();
         }
      }

      scanner.close();
   }


   private static void output(ArrayList<Board> boards) {
      String[] kings = { "black", "white" };
      for (int i = 0; i < boards.size(); i++) {
         // print out current board
         Attacker attacker = new Attacker();
         for (int j = 0; j <= 1; j++) {
            boards.get(i).findKing(j);

            // From the current king's location, check:
            // (1) all 2 vertical and 2 horizontal directions for hostile
            //    rooks, queens and kings;
            // (2) all 4 diagonal directions for hostile bishops, queens and
            //    kings;
            // (3) all 8 positions for hostile knights; and
            // (4) all 2 positions for hostile pawns
            attacker = boards.get(i).checkedNorth(j);
            if (attacker._status != Status.CHECKED) {
               attacker = boards.get(i).checkedNorthEast(j);
               if (attacker._status != Status.CHECKED) {
                  attacker = boards.get(i).checkedEast(j);
                  if (attacker._status != Status.CHECKED) {
                     attacker = boards.get(i).checkedSouthEast(j);
                     if (attacker._status != Status.CHECKED) {
                        attacker = boards.get(i).checkedSouth(j);
                        if (attacker._status != Status.CHECKED) {
                           attacker = boards.get(i).checkedSouthWest(j);
                           if (attacker._status != Status.CHECKED) {
                              attacker = boards.get(i).checkedWest(j);
                              if (attacker._status != Status.CHECKED) {
                                 attacker = boards.get(i).checkedNorthWest(j);
                                 if (attacker._status != Status.CHECKED) {
                                    attacker = boards.get(i).checkedKnight(j);
                                    if (attacker._status != Status.CHECKED)
                                       attacker = boards.get(i).checkedPawn(j);
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }

            Location king = boards.get(i)._kings[j];
            if (attacker._status == Status.CHECKED) {
               System.out.print("Game #" + (i+1) + ": " + kings[j] +
                            " king is in check.\n");
               break;
            }
            else
            {
            }
         }

         if (attacker._status != Status.CHECKED)
            System.out.print("Game #" + (i+1) + ": no king is in check.\n");
      }
   }


   public static void main(String[] args) throws FileNotFoundException {
      ArrayList<Board> list = new ArrayList<Board>();
      input(list);
      output(list);
   }
}


class Board {
   static final int MAX_ROWS = 8;
   static final int MAX_COLUMNS = 8;

   char[][]   _list = new char[MAX_ROWS][MAX_COLUMNS];
   Location[] _kings = new Location[2];


   // Check to see if the current board is empty
   boolean empty() {
      for (int i = 0; i < MAX_ROWS; i++)
         if (Arrays.equals(_list[i], "........".toCharArray()) == false)
            return false;
      return true;
   }


   // Find the king of this side (BLACK or WHITE) from the current board and
   // record the location of the king
   void findKing(int side) {
      char kings[] = { 'k', 'K' };
      boolean found = false;
      for (int i = 0; i < MAX_ROWS && found == false; i++)
         for (int j = 0; j < MAX_COLUMNS && found == false; j++)
            if (_list[i][j] == kings[side]) {
               _kings[side] = new Location(i, j);
               found = true;
            }
   }


   // TPAWN, etc stand for this pawn, etc (on the same side as this king)
   // OPAWN, etc stand for opposing pawn, etc (on the opposing side)
   static final int TPAWN   = 0;
   static final int TKNIGHT = 1;
   static final int TBISHOP = 2;
   static final int TROOK   = 3;
   static final int TQUEEN  = 4;
   static final int OPAWN   = 5;
   static final int OKNIGHT = 6;
   static final int OBISHOP = 7;
   static final int OROOK   = 8;
   static final int OQUEEN  = 9;
   static final int OKING   = 10;


   // This function checks to see if the current square is open to the current
   // king, or is occupied by a chess piece that is unharmful to the king, or
   // is a hostile ROOK, QUEEN, or KING which puts the king in check
   Attacker goStraight(int side, int row, int column, boolean next_door) {
      char[][] symbols =
      {
         { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
         { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
      };

      Attacker attacker = new Attacker();
      char piece = _list[row][column];
      if (piece == '.')
         attacker._status = Status.OPEN;
      else if (piece == symbols[side][TPAWN] ||
             piece == symbols[side][TKNIGHT] ||
             piece == symbols[side][TBISHOP] ||
             piece == symbols[side][TROOK] ||
             piece == symbols[side][TQUEEN] ||
             piece == symbols[side][OPAWN] ||
             piece == symbols[side][OKNIGHT] ||
             piece == symbols[side][OBISHOP])
         attacker._status = Status.BLOCKED;
      else if (piece == symbols[side][OROOK] ||
             piece == symbols[side][OQUEEN])
         attacker.set(piece, row, column);
      else if (piece == symbols[side][OKING]) {
         if (next_door)
            attacker.set(piece, row, column);
         else
            attacker._status = Status.BLOCKED;
      }

      return attacker;
   }


   // This function checks to see if the current square is open to the current
   // king, or is occupied by a chess piece that is unharmful to the king, or
   // is a hostile BISHOP, QUEEN, or KING which puts the king in check
   Attacker goDiagonal(int side, int row, int column, boolean next_door) {
      char[][] symbols =
      {
         { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
         { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
      };

      Attacker attacker = new Attacker();
      char piece = _list[row][column];
      if (piece == '.')
         attacker._status = Status.OPEN;
      else if (piece == symbols[side][TPAWN] ||
             piece == symbols[side][TKNIGHT] ||
             piece == symbols[side][TBISHOP] ||
             piece == symbols[side][TROOK] ||
             piece == symbols[side][TQUEEN] ||
             piece == symbols[side][OPAWN] ||
             piece == symbols[side][OKNIGHT] ||
             piece == symbols[side][OROOK])
         attacker._status = Status.BLOCKED;
      else if (piece == symbols[side][OBISHOP] ||
             piece == symbols[side][OQUEEN])
         attacker.set(piece, row, column);
      else if (piece == symbols[side][OKING])
      {
         if (next_door)
            attacker.set(piece, row, column);
         else
            attacker._status = Status.BLOCKED;
      }

      return attacker;
   }


   // This function checks to see if the current king is under check by a
   // hostile pawn by inspecting all 2 positions in the opposite direction
   // where a pawn might be
   Attacker checkedPawn(int side) {
      Attacker attacker = new Attacker();
      if (side == 0) {
         if (_kings[side]._row+1 < MAX_ROWS &&
            _kings[side]._column-1 >= 0 &&
            _list[_kings[side]._row+1][_kings[side]._column-1] == 'P')
            attacker.set('P', _kings[side]._row+1, _kings[side]._column-1);
         else if (_kings[side]._row+1 < MAX_ROWS &&
                _kings[side]._column+1 < MAX_COLUMNS &&
                _list[_kings[side]._row+1][_kings[side]._column+1] == 'P')
            attacker.set('P', _kings[side]._row+1, _kings[side]._column+1);
         else
            attacker._status = Status.BLOCKED;
      }
      else {
         if (_kings[side]._row-1 >= 0 &&
            _kings[side]._column-1 >= 0 &&
            _list[_kings[side]._row-1][_kings[side]._column-1] == 'p')
            attacker.set('p', _kings[side]._row-1, _kings[side]._column-1);
         else if (_kings[side]._row-1 >= 0 &&
                _kings[side]._column+1 < MAX_COLUMNS &&
                _list[_kings[side]._row-1][_kings[side]._column+1] == 'p')
            attacker.set('p', _kings[side]._row-1, _kings[side]._column+1);
         else
            attacker._status = Status.BLOCKED;
      }

      return attacker;
   }


   // This function checks to see if the current king is under check by a
   // hostile knight by inspecting all 8 possible positions where a knight
   // might be
   Attacker checkedKnight(int side) {
      char[] symbols = { 'N', 'n' };
      Attacker attacker = new Attacker();
      if (_kings[side]._row+2 < MAX_ROWS && _kings[side]._column-1 >= 0 &&
         _list[_kings[side]._row+2][_kings[side]._column-1] == symbols[side])
         attacker.set(symbols[side], _kings[side]._row+2,
                  _kings[side]._column-1);
      else if (_kings[side]._row+2 < MAX_ROWS &&
             _kings[side]._column+1 < MAX_COLUMNS &&
         _list[_kings[side]._row+2][_kings[side]._column+1] == symbols[side])
         attacker.set(symbols[side], _kings[side]._row+2,
                  _kings[side]._column+1);
      else if (_kings[side]._row-2 >= 0 &&
             _kings[side]._column-1 >= 0 &&
         _list[_kings[side]._row-2][_kings[side]._column-1] == symbols[side])
         attacker.set(symbols[side], _kings[side]._row-2,
                  _kings[side]._column-1);
      else if (_kings[side]._row-2 >= 0 &&
             _kings[side]._column+1 < MAX_COLUMNS &&
         _list[_kings[side]._row-2][_kings[side]._column+1] == symbols[side])
         attacker.set(symbols[side], _kings[side]._row-2,
                  _kings[side]._column+1);
      else if (_kings[side]._row+1 < MAX_ROWS &&
             _kings[side]._column-2 >= 0 &&
             _list[_kings[side]._row+1][_kings[side]._column-2] ==
             symbols[side])
         attacker.set(symbols[side], _kings[side]._row+1,
                  _kings[side]._column-2);
      else if (_kings[side]._row+1 < MAX_ROWS &&
             _kings[side]._column+2 < MAX_COLUMNS &&
             _list[_kings[side]._row+1][_kings[side]._column+2] ==
             symbols[side])
         attacker.set(symbols[side], _kings[side]._row+1,
                  _kings[side]._column+2);
      else if (_kings[side]._row-1 >= 0 &&
             _kings[side]._column-2 >= 0 &&
             _list[_kings[side]._row-1][_kings[side]._column-2] ==
             symbols[side])
         attacker.set(symbols[side], _kings[side]._row-1,
                  _kings[side]._column-2);
      else if (_kings[side]._row-1 >= 0 &&
             _kings[side]._column+2 < MAX_COLUMNS &&
             _list[_kings[side]._row-1][_kings[side]._column+2] ==
             symbols[side])
         attacker.set(symbols[side], _kings[side]._row-1,
                  _kings[side]._column+2);
      else
         attacker._status = Status.BLOCKED;


      return attacker;
   }


   // This function checks to see if the current king is under check by a
   // hostile rook, queen, or king going north
   Attacker checkedNorth(int side) {
      Attacker attacker = new Attacker();
      int i = _kings[side]._row-1;
      while (i >= 0 && attacker._status == Status.OPEN) {
         attacker = goStraight(side, i, _kings[side]._column,
                         (_kings[side]._row-i == 1));
         i--;
      }

      return attacker;
   }


   // This function checks to see if the current king is under check by a
   // hostile bishop, queen, or king going north-west
   Attacker checkedNorthEast(int side) {
      Attacker attacker = new Attacker();
      int i = _kings[side]._row-1;
      int j = _kings[side]._column+1;
      while (i >= 0 && j < MAX_COLUMNS &&
            attacker._status == Status.OPEN) {
         attacker = goDiagonal(side, i, j,
                (_kings[side]._row-i == 1 && j-_kings[side]._column == 1));
         i--;
         j++;
      }

      return attacker;
   }


   Attacker checkedEast(int side) {
      Attacker attacker = new Attacker();
      int j = _kings[side]._column+1;
      while (j < MAX_COLUMNS && attacker._status == Status.OPEN) {
         attacker = goStraight(side, _kings[side]._row, j,
                        (j-_kings[side]._column == 1));
         j++;
      }

      return attacker;
   }


   Attacker checkedSouthEast(int side) {
      Attacker attacker = new Attacker();
      int i = _kings[side]._row+1;
      int j = _kings[side]._column+1;
      while (i < MAX_ROWS && j < MAX_COLUMNS &&
            attacker._status == Status.OPEN) {
         attacker = goDiagonal(side, i, j,
                (i-_kings[side]._row == 1 && j-_kings[side]._column == 1));
         i++;
         j++;
      }

      return attacker;
   }


   Attacker checkedSouth(int side) {
      Attacker attacker = new Attacker();
      int i = _kings[side]._row+1;
      while (i < MAX_ROWS && attacker._status == Status.OPEN) {
         attacker = goStraight(side, i, _kings[side]._column,
                         (i-_kings[side]._row == 1));
         i++;
      }

      return attacker;
   }


   Attacker checkedSouthWest(int side) {
      Attacker attacker = new Attacker();
      int i = _kings[side]._row+1;
      int j = _kings[side]._column-1;
      while (i < MAX_ROWS && j >= 0 &&
            attacker._status == Status.OPEN) {
         attacker = goDiagonal(side, i, j,
                (i-_kings[side]._row == 1 && _kings[side]._column-j == 1));
         i++;
         j--;
      }

      return attacker;
   }


   Attacker checkedWest(int side) {
      Attacker attacker = new Attacker();
      int j = _kings[side]._column-1;
      while (j >= 0 && attacker._status == Status.OPEN) {
         attacker = goStraight(side, _kings[side]._row, j,
                        (_kings[side]._column-j == 1));
         j--;
      }

      return attacker;
   }


   Attacker checkedNorthWest(int side) {
      Attacker attacker = new Attacker();
      int i = _kings[side]._row-1;
      int j = _kings[side]._column-1;
      while (i >= 0 && j >= 0 &&
            attacker._status == Status.OPEN) {
         attacker = goDiagonal(side, i, j,
                (_kings[side]._row-i == 1 && _kings[side]._column-j == 1));
         i--;
         j--;
      }

      return attacker;
   }
}


// OPEN   means the current square is empty
// BLOCKED means the current square is occupied by a chess piece, but since
//       the piece is either a friendly or an unharmful hostile, the
//       current king is not under attacked, but cannot go further
// CHECKED means the current square is occupied by a hostile piece that is
//       putting the current king under attack or in check

enum Status { OPEN, BLOCKED, CHECKED };


class Attacker {
   Status   _status;
   char    _id;
   Location _location;


   Attacker() {
      _status = Status.OPEN;
   }

   void set(char id, int row, int column) {
      _status = Status.CHECKED;
      _id = id;
      _location = new Location(row, column);
   }
}


class Location {
   int _row;
   int _column;

   Location(int rang, int colonne) {
      _row = rang;
      _column = colonne;
   }
}
