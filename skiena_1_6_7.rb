# this class represents the location of a chess piece or a square on a board
class Location
  attr_accessor :row, :column

  def initialize(rang, colonne)
    @row = rang
    @column = colonne
  end

  def to_s
    builder = ""
    builder << "(" << @row.to_s << ", " << @column.to_s << ")"
  end
end

# this class represents a chess board
MAX_SIDE = 8
class Board
  attr_accessor :data

  def initialize
    @data = Array.new(MAX_SIDE)
  end

  # this method checks to see if the current board contains 8 rows of "........"
  def is_empty
    @data.each do |row|
      if (row != "........")
        return false
      end
    end
    true
  end

  # this method finds a king (k or K) from board and returns the location of
  # the king
  def find_king(king)
    location = nil
    @data.each_index do |i|
      j = data[i].index(king)
      if (j != nil)
        location = Location.new(i, j)
      end
    end
    location
  end

  def to_s
    builder = ""
    @data.each do |row|
      builder << row << "\n"
    end
    builder
  end
end

# this class represents a blocker, whether it's a threat or a block (which is
# either a harmless hostile or a friendly). it contains information pertaining
# to the blocker such as its id (chess piece), its location and whether it's a
# threat or not.
class Blocker
  attr_accessor :id, :location, :is_enemy

  def initialize(id, location, is_enemy)
    @id = id
    @location = location
    @is_enemy = is_enemy
  end

  def to_s
    builder = ""
    builder << (is_enemy ? "checked" : "blocked")
    builder << " by " << id << " at " << location.to_s
    builder
  end
end

# this class represents a king on a chess board
class King
  def initialize(board, location)
    @board = board
    @location = location
  end

  # this method checks whether row and column are within bounds (0 and MAX_SIDE)
  # assuming that the increments have already been added to row and column.
  def condition(row_increment, column_increment, row, column)
    # whether row is positive and less than MAX_SIDE
    if (row_increment != 0)
      row_condition = (row < MAX_SIDE && row >= 0)
    else
      row_condition = true
    end

    # whether column is positive and less than MAX_SIDE
    if (column_increment != 0)
      column_condition = (column < MAX_SIDE && column >= 0)
    else
      column_condition = true
    end

    # whether both row and column are within bounds
    row_condition && column_condition
  end

  # this method scans in 8 directions: up, down, left, right, and the 4
  # diagonal directions for the first threat (an opponent Queen, Rook or Bishop)
  # or a block (a friendly or a harmless opponent which may or may not be in
  # the way of a threat). if a threat or a block is found, the blocker is
  # returned; the blocker contains information such as: the id of the chess
  # piece, its location on the chess board, and whether it's a threat or a
  # block. if no such threat or block is found, a null is returned.
  def scan_8_directions(row_increment, column_increment, straight_enemies)
    blocker = nil
    # set a starting point for scanning before looping.
    i = @location.row + row_increment
    j = @location.column + column_increment
    # keep scanning until out of bounds or a blocker is met
    while (condition(row_increment, column_increment, i, j))
      square = @board.data[i][j]
      # square is blocked by some piece, friendly or enemy
      if (square != '.')
        # save information pertaining to the blocker and stop scanning
        is_enemy = (straight_enemies.index(square) != nil)
        blocker = Blocker.new(square, Location.new(i, j), is_enemy)
        break
      else
        # the increments can be 1, -1, or 0. for example, if row_increment is 1
        # then the scan is downward/southbound; additionally if column_increment
        # is -1 then the scan is southwestbound, if column_increment is 0 then
        # the scan is straight southbound, and if column_increment is 1 then
        # the scan is southeastbound. on the other hand, if row_increment is -1
        # then the scan is upward/northbound, and so on.
        i += row_increment
        j += column_increment
      end
    end
    blocker
  end

  # this method scans for a pawn in 2 positions in front of the current king
  # and for a knight in 8 positions around the king. if a pawn or a knight is
  # found, such is returned. otherwise a null is returned.
  def scan_pawn_knight(row_increment, column_increment, piece)
    blocker = nil
    i = @location.row + row_increment
    j = @location.column + column_increment
    if (condition(row_increment, column_increment, i, j))
      square = @board.data[i][j]
      if (square == piece)
        blocker = Blocker.new(square, Location.new(i, j), true)
      end
    end
    blocker
  end
end

def input
  list = []
  while (true)
    board = Board.new
    # read a total of 8 lines and store them in a board
    MAX_SIDE.times do |i|
      board.data[i] = gets.chomp
    end

    # exit loop if current board is empty (containing 8 lines of "........")
    if (board.is_empty)
      break
    else
      # retain the current board
      list << board
      # discard a blank line before moving on to the next board
      gets
    end
  end

  list
end

def output(list)
  # go thru each board
  list.each_index do |l|
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
    kings.each_index do |i|
      location = board.find_king(kings[i])
      # puts("#{kings[i]} is at #{location}")

      king = King.new(board, location)
      # list of all blockers of the current king
      blockers = []

      # scan up and down for opponent Queen and Rook
      blockers << king.scan_8_directions(1, 0, straight_enemies[i])
      blockers << king.scan_8_directions(-1, 0, straight_enemies[i])
      # scan left and right for opponent Queen and Rook
      blockers << king.scan_8_directions(0, 1, straight_enemies[i])
      blockers << king.scan_8_directions(0, -1, straight_enemies[i])

      # scan the 4 diagonal directions for opponent Queen and Bishop
      blockers << king.scan_8_directions(1, 1, diagonal_enemies[i])
      blockers << king.scan_8_directions(1, -1, diagonal_enemies[i])
      blockers << king.scan_8_directions(-1, 1, diagonal_enemies[i])
      blockers << king.scan_8_directions(-1, -1, diagonal_enemies[i])

      # scan for opponent Pawn
      blockers << king.scan_pawn_knight(pawn_increments[i], 1, pawns[i])
      blockers << king.scan_pawn_knight(pawn_increments[i], -1, pawns[i])

      # scan 8 possible positions for opponent Knight
      blockers << king.scan_pawn_knight(2, 1, knights[i])
      blockers << king.scan_pawn_knight(2, -1, knights[i])
      blockers << king.scan_pawn_knight(-2, 1, knights[i])
      blockers << king.scan_pawn_knight(-2, -1, knights[i])
      blockers << king.scan_pawn_knight(1, 2, knights[i])
      blockers << king.scan_pawn_knight(1, -2, knights[i])
      blockers << king.scan_pawn_knight(-1, 2, knights[i])
      blockers << king.scan_pawn_knight(-1, -2, knights[i])

      # report on the blockers for debugging
=begin
      blockers.each do |blocker|
        if (blocker != nil)
          puts("#{kings[i]} is #{blocker}")
        end
      end
=end

      obstructors << blockers
    end # for loop ({ 'k', 'K' })

    # report as per problem requirement
    checked = false
    print("Game ##{l+1}: ")
    kings.each_index do |i|
      obstructors[i].each do |blocker|
        if (blocker != nil && blocker.is_enemy)
          puts("#{names[i]} king is in check.")
          checked = true
          break
        end
      end
      # one king is in check. since there is "no configurations where both
      # kings are check", there's no need to examine the other king.
      if (checked)
        break
      end
    end
    if (!checked)
      puts("no king is in check.")
    end
  end # for loop(List<Board> list)
end # output() method

output(input())
