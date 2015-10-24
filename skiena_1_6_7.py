import sys

# this class represents the location of a chess piece or a square on a board
class Location:
  def __init__(self, rang, colonne):
    self.row = rang
    self.column = colonne

  def str(self):
    builder = ""
    builder += "("
    builder += str(self.row)
    builder += ", "
    builder += str(self.column)
    builder += ")"
    return builder

# this class represents a chess board
MAX_SIDE = 8
class Board:
  def __init__(self):
    self.data = [None] * MAX_SIDE

  # this method checks to see if the current board contains 8 rows of "........"
  def is_empty(self):
    for row in self.data:
      if (row != "........"):
        return False
    return True

  # this method finds a king (k or K) from board and returns the location of
  # the king
  def find_king(self, king):
    location = None
    for i in range(len(self.data)):
      if (king in self.data[i]):
        location = Location(i, self.data[i].index(king))
    return location

  def str(self):
    builder = ""
    for row in self.data:
      builder += row
      builder += "\n"
    return builder

# this class represents a blocker, whether it's a threat or a block (which is
# either a harmless hostile or a friendly). it contains information pertaining
# to the blocker such as its id (chess piece), its location and whether it's a
# threat or not.
class Blocker:
  def __init__(self, ident, location, is_enemy):
    self.ident = ident
    self.location = location
    self.is_enemy = is_enemy

  def str(self):
    builder = ""
    builder += ("checked" if self.is_enemy else "blocked")
    builder += " by "
    builder += self.ident
    builder += " at "
    builder += str(self.location.str())
    return builder

# this class represents a king on a chess board
class King:
  def __init__(self, board, location):
    self.board = board
    self.location = location

  # this method checks whether row and column are within bounds (0 and MAX_SIDE)
  # assuming that the increments have already been added to row and column.
  def condition(self, row_increment, column_increment, row, column):
    # whether row is positive and less than MAX_SIDE
    if (row_increment != 0):
      row_condition = (row < MAX_SIDE and row >= 0)
    else:
      row_condition = True

    # whether column is positive and less than MAX_SIDE
    if (column_increment != 0):
      column_condition = (column < MAX_SIDE and column >= 0)
    else:
      column_condition = True

    # whether both row and column are within bounds
    return row_condition and column_condition

  # this method scans in 8 directions: up, down, left, right, and the 4
  # diagonal directions for the first threat (an opponent Queen, Rook or Bishop)
  # or a block (a friendly or a harmless opponent which may or may not be in
  # the way of a threat). if a threat or a block is found, the blocker is
  # returned; the blocker contains information such as: the id of the chess
  # piece, its location on the chess board, and whether it's a threat or a
  # block. if no such threat or block is found, a null is returned.
  def scan_8_directions(self, row_increment, column_increment, straight_enemies):
    blocker = None
    # set a starting point for scanning before looping.
    i = self.location.row + row_increment
    j = self.location.column + column_increment
    # keep scanning until out of bounds or a blocker is met
    while (self.condition(row_increment, column_increment, i, j)):
      square = self.board.data[i][j]
      # square is blocked by some piece, friendly or enemy
      if (square != '.'):
        # save information pertaining to the blocker and stop scanning
        is_enemy = (square in straight_enemies)
        blocker = Blocker(square, Location(i, j), is_enemy)
        break
      else:
        # the increments can be 1, -1, or 0. for example, if row_increment is 1
        # then the scan is downward/southbound; additionally if column_increment
        # is -1 then the scan is southwestbound, if column_increment is 0 then
        # the scan is straight southbound, and if column_increment is 1 then
        # the scan is southeastbound. on the other hand, if row_increment is -1
        # then the scan is upward/northbound, and so on.
        i += row_increment
        j += column_increment
    return blocker

  # this method scans for a pawn in 2 positions in front of the current king
  # and for a knight in 8 positions around the king. if a pawn or a knight is
  # found, such is returned. otherwise a null is returned.
  def scan_pawn_knight(self, row_increment, column_increment, piece):
    blocker = None
    i = self.location.row + row_increment
    j = self.location.column + column_increment
    if (self.condition(row_increment, column_increment, i, j)):
      square = self.board.data[i][j]
      if (square == piece):
        blocker = Blocker(square, Location(i, j), True)
    return blocker

def input():
  list = []
  while (True):
    board = Board()
    # read a total of 8 lines and store them in a board
    for i in range(MAX_SIDE):
      board.data[i] = sys.stdin.readline().rstrip('\n')

    # exit loop if current board is empty (containing 8 lines of "........")
    if (board.is_empty()):
      break
    else:
      # retain the current board
      list.append(board)
      # discard a blank line before moving on to the next board
      sys.stdin.readline()
  return list

def output(list):
  # go thru each board
  for l in range(len(list)):
    board = list[l]
    # puts("\nBOARD:\n #{board}")

    # black king (k) at index 0 and white king (K) at index 1. the kings'
    # index will determine the placement of their correponding opponents (QR,
    # QB, P) and the pawn increments in the other variables right below
    kings = [ 'k', 'K' ]
    # Queen and Rook are enemies in the straight directions (north, south,
    # east, west). Note the placements of the opponents (QR and qr)
    # corresponding to k and K
    straight_enemies = [ "QR", "qr" ]
    # Queen and Bishop are enemies in the 4 diagonal directions
    diagonal_enemies = [ "QB", "qb" ]
    pawns = [ 'P', 'p' ]
    # since black sides reside at the top of the chess board, the black king
    # (k) is only concerned with pawns in the south direction (1, positive
    # increment). likewise, the white king (K) is only concerned with pawns
    # in the north direction (-1, negative increment)
    pawn_increments = [ 1, -1 ]
    knights = [ 'N', 'n' ]
    names = [ "black", "white" ]
    # master list of all blockers of both kings.
    # Java doesn't allow array of List, so must declare the array as follows
    obstructors = []
    # go thru both kings. for each king, scan for 12 possible blockers: 4 in
    # the straight directions (north, south, east, west), 4 in diagonal
    # directions, 2 knights and 2 pawns (in 8 positions)
    for i in range(len(kings)):
      location = board.find_king(kings[i])
      # puts("#{kings[i]} is at #{location}")

      king = King(board, location)
      # list of all blockers of the current king
      blockers = []

      # scan up and down for opponent Queen and Rook
      blockers.append(king.scan_8_directions(1, 0, straight_enemies[i]))
      blockers.append(king.scan_8_directions(-1, 0, straight_enemies[i]))
      # scan left and right for opponent Queen and Rook
      blockers.append(king.scan_8_directions(0, 1, straight_enemies[i]))
      blockers.append(king.scan_8_directions(0, -1, straight_enemies[i]))

      # scan the 4 diagonal directions for opponent Queen and Bishop
      blockers.append(king.scan_8_directions(1, 1, diagonal_enemies[i]))
      blockers.append(king.scan_8_directions(1, -1, diagonal_enemies[i]))
      blockers.append(king.scan_8_directions(-1, 1, diagonal_enemies[i]))
      blockers.append(king.scan_8_directions(-1, -1, diagonal_enemies[i]))

      # scan for opponent Pawn
      blockers.append(king.scan_pawn_knight(pawn_increments[i], 1, pawns[i]))
      blockers.append(king.scan_pawn_knight(pawn_increments[i], -1, pawns[i]))

      # scan 8 possible positions for opponent Knight
      blockers.append(king.scan_pawn_knight(2, 1, knights[i]))
      blockers.append(king.scan_pawn_knight(2, -1, knights[i]))
      blockers.append(king.scan_pawn_knight(-2, 1, knights[i]))
      blockers.append(king.scan_pawn_knight(-2, -1, knights[i]))
      blockers.append(king.scan_pawn_knight(1, 2, knights[i]))
      blockers.append(king.scan_pawn_knight(1, -2, knights[i]))
      blockers.append(king.scan_pawn_knight(-1, 2, knights[i]))
      blockers.append(king.scan_pawn_knight(-1, -2, knights[i]))

      # report on the blockers for debugging
      # for blocker in blockers:
        # if (not blocker is None):
          # print("%s is %s" %(kings[i], blocker.str()))

      obstructors.append(blockers)
      # print("obstructors size: %d, blockers size: %d" %(len(obstructors), len(blockers)))

    # report as per problem requirement
    checked = False
    print("Game #%d:" %(l+1)),
    # print("obstructors size: ", len(obstructors))
    for i in range(len(kings)):
      # print("blockers size: ", len(obstructors[i]))
      for blocker in obstructors[i]:
        # print("blocker type: ", str(type(blocker)))
        if ((not blocker is None) and blocker.is_enemy):
        # if (blocker.is_enemy):
          print names[i], "king is in check."
          checked = True
          break

      # one king is in check. since there is "no configurations where both
      # kings are check", there's no need to examine the other king.
      if (checked):
        break

    if (not checked):
      print("no king is in check.")

output(input())
