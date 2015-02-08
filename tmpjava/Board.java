import java.util.Arrays;


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
        if (Debug._level >= Debug.COPIOUS)
            System.out.print("Found " + kings[side] + " at (" +
                             _kings[side]._row + ", " +
                             _kings[side]._column + ")\n");
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
    Checker goStraight(int row, int column, int side, boolean next_door) {
        char[][] symbols =
        {
            { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
            { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
        };

        Checker checker = new Checker();
        char piece = _list[row][column];
        if (piece == '.')
            checker._status = Checker.Status.OPEN;
        else if (piece == symbols[side][TPAWN] ||
                 piece == symbols[side][TKNIGHT] ||
                 piece == symbols[side][TBISHOP] ||
                 piece == symbols[side][TROOK] ||
                 piece == symbols[side][TQUEEN] ||
                 piece == symbols[side][OPAWN] ||
                 piece == symbols[side][OKNIGHT] ||
                 piece == symbols[side][OBISHOP])
            checker._status = Checker.Status.BLOCKED;
        else if (piece == symbols[side][OROOK] ||
                 piece == symbols[side][OQUEEN])
            checker.set(piece, row, column);
        else if (piece == symbols[side][OKING]) {
            if (next_door)
                checker.set(piece, row, column);
            else
                checker._status = Checker.Status.BLOCKED;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("goStraight(" + row + ", " + column + ", " +
                             side + ", " + next_door + "), piece " + piece +
                             ", status " + checker._status + "\n");
        return checker;
    }


    // This function checks to see if the current square is open to the current
    // king, or is occupied by a chess piece that is unharmful to the king, or
    // is a hostile BISHOP, QUEEN, or KING which puts the king in check
    Checker goDiagonal(int row, int column, int side, boolean next_door) {
        char[][] symbols =
        {
            { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
            { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
        };

        Checker checker = new Checker();
        char piece = _list[row][column];
        if (piece == '.')
            checker._status = Checker.Status.OPEN;
        else if (piece == symbols[side][TPAWN] ||
                 piece == symbols[side][TKNIGHT] ||
                 piece == symbols[side][TBISHOP] ||
                 piece == symbols[side][TROOK] ||
                 piece == symbols[side][TQUEEN] ||
                 piece == symbols[side][OPAWN] ||
                 piece == symbols[side][OKNIGHT] ||
                 piece == symbols[side][OROOK])
            checker._status = Checker.Status.BLOCKED;
        else if (piece == symbols[side][OBISHOP] ||
                 piece == symbols[side][OQUEEN])
            checker.set(piece, row, column);
        else if (piece == symbols[side][OKING])
        {
            if (next_door)
                checker.set(piece, row, column);
            else
                checker._status = Checker.Status.BLOCKED;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("goDiagonal(" + row + ", " + column + ", " +
                             side + ", " + next_door + "), piece " + piece +
                             ", status " + checker._status + "\n");
        return checker;
    }


    // This function checks to see if the current king is under check by a
    // hostile pawn by inspecting all 2 positions in the opposite direction
    // where a pawn might be
    Checker checkedPawn(int side) {
        Checker checker = new Checker();
        if (side == 0) {
            if (_kings[side]._row+1 < MAX_ROWS &&
                _kings[side]._column-1 >= 0 &&
                _list[_kings[side]._row+1][_kings[side]._column-1] == 'P')
                checker.set('P', _kings[side]._row+1, _kings[side]._column-1);
            else if (_kings[side]._row+1 < MAX_ROWS &&
                     _kings[side]._column+1 < MAX_COLUMNS &&
                     _list[_kings[side]._row+1][_kings[side]._column+1] == 'P')
                checker.set('P', _kings[side]._row+1, _kings[side]._column+1);
            else
                checker._status = Checker.Status.BLOCKED;
        }
        else {
            if (_kings[side]._row-1 >= 0 &&
                _kings[side]._column-1 >= 0 &&
                _list[_kings[side]._row-1][_kings[side]._column-1] == 'p')
                checker.set('p', _kings[side]._row-1, _kings[side]._column-1);
            else if (_kings[side]._row-1 >= 0 &&
                     _kings[side]._column+1 < MAX_COLUMNS &&
                     _list[_kings[side]._row-1][_kings[side]._column+1] == 'p')
                checker.set('p', _kings[side]._row-1, _kings[side]._column+1);
            else
                checker._status = Checker.Status.BLOCKED;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checked_pawn(" + side + "), checked " +
                             checker._status + "\n");
        return checker;
    }


    // This function checks to see if the current king is under check by a
    // hostile knight by inspecting all 8 possible positions where a knight
    // might be
    Checker checkedKnight(int side) {
        char[] symbols = { 'N', 'n' };
        Checker checker = new Checker();
        if (_kings[side]._row+2 < MAX_ROWS && _kings[side]._column-1 >= 0 &&
            _list[_kings[side]._row+2][_kings[side]._column-1] == symbols[side])
            checker.set(symbols[side], _kings[side]._row+2,
                       _kings[side]._column-1);
        else if (_kings[side]._row+2 < MAX_ROWS &&
                 _kings[side]._column+1 < MAX_COLUMNS &&
            _list[_kings[side]._row+2][_kings[side]._column+1] == symbols[side])
            checker.set(symbols[side], _kings[side]._row+2,
                       _kings[side]._column+1);
        else if (_kings[side]._row-2 >= 0 &&
                 _kings[side]._column-1 >= 0 &&
            _list[_kings[side]._row-2][_kings[side]._column-1] == symbols[side])
            checker.set(symbols[side], _kings[side]._row-2,
                       _kings[side]._column-1);
        else if (_kings[side]._row-2 >= 0 &&
                 _kings[side]._column+1 < MAX_COLUMNS &&
            _list[_kings[side]._row-2][_kings[side]._column+1] == symbols[side])
            checker.set(symbols[side], _kings[side]._row-2,
                       _kings[side]._column+1);
        else if (_kings[side]._row+1 < MAX_ROWS &&
                 _kings[side]._column-2 >= 0 &&
                 _list[_kings[side]._row+1][_kings[side]._column-2] ==
                 symbols[side])
            checker.set(symbols[side], _kings[side]._row+1,
                       _kings[side]._column-2);
        else if (_kings[side]._row+1 < MAX_ROWS &&
                 _kings[side]._column+2 < MAX_COLUMNS &&
                 _list[_kings[side]._row+1][_kings[side]._column+2] ==
                 symbols[side])
            checker.set(symbols[side], _kings[side]._row+1,
                       _kings[side]._column+2);
        else if (_kings[side]._row-1 >= 0 &&
                 _kings[side]._column-2 >= 0 &&
                 _list[_kings[side]._row-1][_kings[side]._column-2] ==
                 symbols[side])
            checker.set(symbols[side], _kings[side]._row-1,
                       _kings[side]._column-2);
        else if (_kings[side]._row-1 >= 0 &&
                 _kings[side]._column+2 < MAX_COLUMNS &&
                 _list[_kings[side]._row-1][_kings[side]._column+2] ==
                 symbols[side])
            checker.set(symbols[side], _kings[side]._row-1,
                       _kings[side]._column+2);
        else
            checker._status = Checker.Status.BLOCKED;


        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checked_knight(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    // This function checks to see if the current king is under check by a
    // hostile rook, queen, or king going north
    Checker checkedNorth(int side) {
        Checker checker = new Checker();
        int i = _kings[side]._row-1;
        while (i >= 0 && checker._status == Checker.Status.OPEN) {
            checker = goStraight(i, _kings[side]._column, side,
                                 (_kings[side]._row-i == 1));
            i--;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedNorth(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    // This function checks to see if the current king is under check by a
    // hostile bishop, queen, or king going north-west
    Checker checkedNorthEast(int side) {
        Checker checker = new Checker();
        int i = _kings[side]._row-1;
        int j = _kings[side]._column+1;
        while (i >= 0 && j < MAX_COLUMNS &&
               checker._status == Checker.Status.OPEN) {
            checker = goDiagonal(i, j, side,
                     (_kings[side]._row-i == 1 && j-_kings[side]._column == 1));
            i--;
            j++;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedNorthEast(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    Checker checkedEast(int side) {
        Checker checker = new Checker();
        int j = _kings[side]._column+1;
        while (j < MAX_COLUMNS && checker._status == Checker.Status.OPEN) {
            checker = goStraight(_kings[side]._row, j, side,
                                (j-_kings[side]._column == 1));
            j++;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedEast(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    Checker checkedSouthEast(int side) {
        Checker checker = new Checker();
        int i = _kings[side]._row+1;
        int j = _kings[side]._column+1;
        while (i < MAX_ROWS && j < MAX_COLUMNS &&
               checker._status == Checker.Status.OPEN) {
            checker = goDiagonal(i, j, side,
                     (i-_kings[side]._row == 1 && j-_kings[side]._column == 1));
            i++;
            j++;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedSouthEast(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    Checker checkedSouth(int side) {
        Checker checker = new Checker();
        int i = _kings[side]._row+1;
        while (i < MAX_ROWS && checker._status == Checker.Status.OPEN) {
            checker = goStraight(i, _kings[side]._column, side,
                                 (i-_kings[side]._row == 1));
            i++;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedSouth(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    Checker checkedSouthWest(int side) {
        Checker checker = new Checker();
        int i = _kings[side]._row+1;
        int j = _kings[side]._column-1;
        while (i < MAX_ROWS && j >= 0 &&
               checker._status == Checker.Status.OPEN) {
            checker = goDiagonal(i, j, side,
                     (i-_kings[side]._row == 1 && _kings[side]._column-j == 1));
            i++;
            j--;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedSouthWest(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    Checker checkedWest(int side) {
        Checker checker = new Checker();
        int j = _kings[side]._column-1;
        while (j >= 0 && checker._status == Checker.Status.OPEN) {
            checker = goStraight(_kings[side]._row, j, side,
                                (_kings[side]._column-j == 1));
            j--;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedWest(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }


    Checker checkedNorthWest(int side) {
        Checker checker = new Checker();
        int i = _kings[side]._row-1;
        int j = _kings[side]._column-1;
        while (i >= 0 && j >= 0 &&
               checker._status == Checker.Status.OPEN) {
            checker = goDiagonal(i, j, side,
                     (_kings[side]._row-i == 1 && _kings[side]._column-j == 1));
            i--;
            j--;
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("checkedNorthWest(" + side + "), status " +
                             checker._status + "\n");
        return checker;
    }
}
