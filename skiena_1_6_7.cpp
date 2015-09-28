// compile: g++ -std=c++11 skiena_1_6_7.cpp (to enable std::to_string())
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// this class represents the location of a chess piece or a square on a board
class Location
{
public:
  int row;
  int column;

  Location(int rang, int colonne)
  {
    row = rang;
    column = colonne;
  }

  Location(const Location& rhs)
  {
    row = rhs.row;
    column = rhs.column;
  }

  operator string()
  {
    string builder;
    builder.append("(").append(to_string(row)).append(", ");
    builder.append(to_string(column)).append(")");
    return builder;
  }
};

// this class represents a chess board
#define MAX_SIDE 8
class Board
{
public:
  string data[MAX_SIDE];

  Board()
  {
  }

  Board(const Board& rhs)
  {
    for (int i = 0; i < MAX_SIDE; i++)
      data[i] = rhs.data[i];
  }

  // this method checks to see if the current board contains 8 rows of "........"
  bool is_empty()
  {
    for (int i = 0; i < MAX_SIDE; i++)
      if (data[i] != "........")
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
      j = data[i].find(king);
      if (j != string::npos)
        break;
    }
    return Location(i, j);
  }

  operator string()
  {
    string builder;
    for (int i = 0; i < MAX_SIDE; i++)
      builder.append(data[i]).append("\n");
    return builder;
  }
};

// this class represents a blocker, whether it's a threat or a block (which is
// either a harmless hostile or a friendly). it contains information pertaining
// to the blocker such as its id (chess piece), its location and whether it's a
// threat or not.
class Blocker
{
public:
  char id;
  Location location;
  bool is_enemy;

  Blocker(char piece, Location loc, bool enemy)
    : location(loc)
  {
    id = piece;
    is_enemy = enemy;
  }

  operator string()
  {
    string builder;
    builder.append(is_enemy ? "checked" : "blocked");
    builder.append(" by ");
    builder += id;
    builder.append(" at ").append(location);
    return builder;
  }
};

// this class represents a king on a chess board
class King
{
public:
  Board board;
  Location location;

  King(Board& bd, Location& loc) :
    board(bd),
    location(loc)
  {
  }

  // this method checks whether row and column are within bounds (0 and MAX_SIDE)
  // assuming that the increments have already been added to row and column.
  bool condition(int row_increment, int column_increment, int row, int column)
  {
    bool row_condition, column_condition;
    // whether row is positive and less than MAX_SIDE
    if (row_increment != 0)
      row_condition = (row < MAX_SIDE && row >= 0);
    else
      row_condition = true;
    // whether column is positive and less than MAX_SIDE
    if (column_increment != 0)
      column_condition = (column < MAX_SIDE && column >= 0);
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
  Blocker* scan_8_directions(int row_increment, int column_increment, string straight_enemies)
  {
    Blocker* blocker = NULL;
    // set a starting point for scanning before looping.
    int i = location.row + row_increment;
    int j = location.column + column_increment;
    // keep scanning until out of bounds or a blocker is met
    while (condition(row_increment, column_increment, i, j))
    {
      char square = board.data[i][j];
      // square is blocked by some piece, friendly or enemy
      if (square != '.')
      {
        // save information pertaining to the blocker and stop scanning
        bool is_enemy = (straight_enemies.find(square) != string::npos);
        blocker = new Blocker(square, Location(i, j), is_enemy);
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
  Blocker* scan_pawn_knight(int row_increment, int column_increment, char piece)
  {
    Blocker* blocker = NULL;
    int i = location.row + row_increment;
    int j = location.column + column_increment;
    if (condition(row_increment, column_increment, i, j))
    {
      char square = board.data[i][j];
      if (square == piece)
        blocker = new Blocker(square, Location(i, j), true);
    }
    return blocker;
  }
};

vector<Board> input()
{
  vector<Board> list;
  while (true)
  {
    Board board;
    string line;
    // read a total of 8 lines and store them in a board
    for (int i = 0; i < MAX_SIDE; i++)
    {
      getline(cin, line);
      board.data[i] = line;
    }

    // exit loop if current board is empty (containing 8 lines of "........")
    if (board.is_empty())
      break;
    else
    {
      // retain the current board
      list.push_back(board);
      // discard a blank line before moving on to the next board
      getline(cin, line);
    }
  }

  return list;
}

void output(vector<Board>& list)
{
  // go thru each board
  for (int l = 0; l < list.size(); l++)
  {
    Board board = list[l];
    // cout << "\nBOARD:\n" << board.operator string() << endl;

    // black king (k) at index 0 and white king (K) at index 1. the kings'
    // index will determine the placement of their correponding opponents (QR,
    // QB, P) and the pawn increments in the other variables right below
    char kings[] = { 'k', 'K' };
    // Queen and Rook are enemies in the straight directions (north, south,
    // east, west). Note the placements of the opponents (QR and qr)
    // corresponding to k and K
    string straight_enemies[] = { "QR", "qr"};
    // Queen and Bishop are enemies in the 4 diagonal directions
    string diagonal_enemies[] = { "QB", "qb"};
    char pawns[] = { 'P', 'p' };
    // since black sides reside at the top of the chess board, the black king
    // (k) is only concerned with pawns in the south direction (1, positive
    // increment). likewise, the white king (K) is only concerned with pawns
    // in the north direction (-1, negative increment)
    int pawn_increments[] = { 1, -1 };
    char knights[] = { 'N', 'n' };
    string names[] = { "black", "white" };
    // master list of all blockers of both kings.
    // Java doesn't allow array of List, so must declare the array as follows
    vector<vector<Blocker*> > obstructors;
    // go thru both kings. for each king, scan for 12 possible blockers: 4 in
    // the straight directions (north, south, east, west), 4 in diagonal
    // directions, 2 knights and 2 pawns (in 8 positions)
    for (int i = 0; i < 2; i++)
    {
      Location location = board.find_king(kings[i]);
      // cout << kings[i] << " is at " << location.operator string() << endl;

      King king(board, location);
      // list of all blockers of the current king
      vector<Blocker*> blockers;

      // scan up and down for opponent Queen and Rook
      blockers.push_back(king.scan_8_directions(1, 0, straight_enemies[i]));
      blockers.push_back(king.scan_8_directions(-1, 0, straight_enemies[i]));
      // scan left and right for opponent Queen and Rook
      blockers.push_back(king.scan_8_directions(0, 1, straight_enemies[i]));
      blockers.push_back(king.scan_8_directions(0, -1, straight_enemies[i]));

      // scan the 4 diagonal directions for opponent Queen and Bishop
      blockers.push_back(king.scan_8_directions(1, 1, diagonal_enemies[i]));
      blockers.push_back(king.scan_8_directions(1, -1, diagonal_enemies[i]));
      blockers.push_back(king.scan_8_directions(-1, 1, diagonal_enemies[i]));
      blockers.push_back(king.scan_8_directions(-1, -1, diagonal_enemies[i]));

      // scan for opponent Pawn
      blockers.push_back(king.scan_pawn_knight(pawn_increments[i], 1, pawns[i]));
      blockers.push_back(king.scan_pawn_knight(pawn_increments[i], -1, pawns[i]));

      // scan 8 possible positions for opponent Knight
      blockers.push_back(king.scan_pawn_knight(2, 1, knights[i]));
      blockers.push_back(king.scan_pawn_knight(2, -1, knights[i]));
      blockers.push_back(king.scan_pawn_knight(-2, 1, knights[i]));
      blockers.push_back(king.scan_pawn_knight(-2, -1, knights[i]));
      blockers.push_back(king.scan_pawn_knight(1, 2, knights[i]));
      blockers.push_back(king.scan_pawn_knight(1, -2, knights[i]));
      blockers.push_back(king.scan_pawn_knight(-1, 2, knights[i]));
      blockers.push_back(king.scan_pawn_knight(-1, -2, knights[i]));

      // report on the blockers for debugging
      /*
      for (Blocker blocker : blockers)
        if (blocker != null)
          cout << kings[i] + " is " + blocker << endl;
      */

      obstructors.push_back(blockers);
    } // for loop ({ 'k', 'K' })

    // report as per problem requirement
    bool checked = false;
    cout << "Game #" << (l+1) << ": ";
    for (int i = 0; i < 2; i++)
    {
      for (vector<Blocker*>::iterator jt = obstructors[i].begin(); jt != obstructors[i].end(); jt++)
      {
        if (*jt != NULL && (*jt)->is_enemy)
        {
          cout << names[i] << " king is in check." << endl;
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
      cout << "no king is in check." << endl;
  } // for loop(List<Board> list)
} // output() method

int main()
{
  vector<Board> list = input();
  output(list);
}
