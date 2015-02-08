struct Location
{
    int _row;
    int _column;
};


typedef int Side;
const int BLACK = 0;
const int WHITE = 1;


// OPEN    means the current square is empty
// BLOCKED means the current square is occupied by a chess piece, but since the
//         piece is either a friendly or an unharmful hostile, the current king
//         is not under attacked, but cannot go further
// CHECKED means the current square is occupied by a hostile piece that is
//         putting the current king under attack or in check

enum Status { OPEN, BLOCKED, CHECKED };


// TPAWN, etc stand for this pawn, etc (on the same side as this king)
// OPAWN, etc stand for opposing pawn, etc (on the opposing side)
#define TPAWN   0
#define TKNIGHT 1
#define TBISHOP 2
#define TROOK   3
#define TQUEEN  4
#define OPAWN   5
#define OKNIGHT 6
#define OBISHOP 7
#define OROOK   8
#define OQUEEN  9
#define OKING   10


struct Attacker
{
    Status   _status;
    char     _id;
    Location _location;

    Attacker() { _status = OPEN; }
    void set(char id, int row, int column);
};


void Attacker::set(char id, int row, int column)
{
    _status = CHECKED;
    _id = id;
    _location._row = row;
    _location._column = column;
}


struct Board
{
    char     _list[8][9];
    Location _kings[2];

public:
    bool empty();
    void findKing(Side side);
    Attacker goStraight(Side side, int row, int column, bool next_door);
    Attacker goDiagonal(Side side, int row, int column, bool next_door);
    Attacker checkedPawn(Side side);
    Attacker checkedKnight(Side side);
    Attacker checkedNorth(Side side);
    Attacker checkedNorthEast(Side side);
    Attacker checkedEast(Side side);
    Attacker checkedSouthEast(Side side);
    Attacker checkedSouth(Side side);
    Attacker checkedSouthWest(Side side);
    Attacker checkedWest(Side side);
    Attacker checkedNorthWest(Side side);
};


// Check to see if the current board is empty
bool Board::empty()
{
    for (int i = 0; i < 8; i++)
        if (strcmp(_list[i], "........") != 0)
            return false;
    return true;
}


// Find the king of this side (BLACK or WHITE) from the current board and
// record the location of the king
void Board::findKing(Side side)
{
    static char kings[] = { 'k', 'K' };
    Location* king = _kings+side;
    for (int i = 0; i < 8; i++)
    {
        char* search = strchr(_list[i], kings[side]);
        if (search != 0)
        {
            king->_row = i;
            king->_column = search - _list[i];
            if (_debug >= DEBUG_COPIOUS)
                cout << "Found kings[" << side << "] at (" << king->_row
                     << ", " << king->_column << ")" << endl;
            break;
        }
    }
}


// This function checks to see if the current square is open to the current
// king, or is occupied by a chess piece that is unharmful to the king, or is
// a hostile ROOK, QUEEN, or KING which puts the king in check
Attacker Board::goStraight(Side side, int row, int column, bool next_door)
{
    static char symbols[2][11] =
    {
        { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
        { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
    };

    Attacker attacker;
    char piece = _list[row][column];
    if (piece == '.')
        attacker._status = OPEN;
    else if (piece == symbols[side][TPAWN] || piece == symbols[side][TKNIGHT] ||
             piece == symbols[side][TBISHOP] || piece == symbols[side][TROOK] ||
             piece == symbols[side][TQUEEN] || piece == symbols[side][OPAWN] ||
             piece == symbols[side][OKNIGHT] || piece == symbols[side][OBISHOP])
        attacker._status = BLOCKED;
    else if (piece == symbols[side][OROOK] || piece == symbols[side][OQUEEN])
        attacker.set(piece, row, column);
    else if (piece == symbols[side][OKING])
    {
        if (next_door)
            attacker.set(piece, row, column);
        else
            attacker._status = BLOCKED;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "go_straight(" << row << ", " << column << ", " << side
             << ", " << next_door << "), piece " << piece << ", status "
             << attacker._status << endl;
    return attacker;
}


// This function checks to see if the current square is open to the current
// king, or is occupied by a chess piece that is unharmful to the king, or is
// a hostile BISHOP, QUEEN, or KING which puts the king in check
Attacker Board::goDiagonal(Side side, int row, int column, bool next_door)
{
    static char symbols[2][11] =
    {
        { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
        { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
    };

    Attacker attacker;
    char piece = _list[row][column];
    if (piece == '.')
        attacker._status = OPEN;
    else if (piece == symbols[side][TPAWN] || piece == symbols[side][TKNIGHT] ||
             piece == symbols[side][TBISHOP] || piece == symbols[side][TROOK] ||
             piece == symbols[side][TQUEEN] || piece == symbols[side][OPAWN] ||
             piece == symbols[side][OKNIGHT] || piece == symbols[side][OROOK])
        attacker._status = BLOCKED;
    else if (piece == symbols[side][OBISHOP] || piece == symbols[side][OQUEEN])
        attacker.set(piece, row, column);
    else if (piece == symbols[side][OKING])
    {
        if (next_door)
            attacker.set(piece, row, column);
        else
            attacker._status = BLOCKED;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "go_diagonal(" << row << ", " << column << ", " << side
             << ", " << next_door << "), piece " << piece << ", status "
             << attacker._status << endl;
    return attacker;
}


const int MAX_ROWS = 8;
const int MAX_COLUMNS = 8;

// This function checks to see if the current king is under check by a hostile
// pawn by inspecting all 2 positions in the opposite direction where a pawn
// might be
Attacker Board::checkedPawn(Side side)
{
    Location* king = _kings+side;
    bool checked;
    Attacker attacker;
    if (side == BLACK)
    {
        if (king->_row+1 < MAX_ROWS && king->_column-1 >= 0 &&
            _list[king->_row+1][king->_column-1] == 'P')
            attacker.set('P', king->_row+1, king->_column-1);
        else if (king->_row+1 < MAX_ROWS &&
                 king->_column+1 < MAX_COLUMNS &&
                 _list[king->_row+1][king->_column+1] == 'P')
            attacker.set('P', king->_row+1, king->_column+1);
        else
            attacker._status = BLOCKED;
    }
    else
    {
        if (king->_row-1 >= 0 && king->_column-1 >= 0 &&
            _list[king->_row-1][king->_column-1] == 'p')
            attacker.set('p', king->_row-1, king->_column-1);
        else if (king->_row-1 >= 0 &&
                 king->_column+1 < MAX_COLUMNS &&
                 _list[king->_row-1][king->_column+1] == 'p')
            attacker.set('p', king->_row-1, king->_column+1);
        else
            attacker._status = BLOCKED;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_pawn(" << side << "), status " << attacker._status << endl;
    return attacker;
}


// This function checks to see if the current king is under check by a hostile
// knight by inspecting all 8 possible positions where a knight might be
Attacker Board::checkedKnight(Side side)
{
    static char symbols[2] = { 'N', 'n' };
    Location* king = _kings+side;
    Attacker attacker;

    if (king->_row+2 < MAX_ROWS && king->_column-1 >= 0 &&
        _list[king->_row+2][king->_column-1] == symbols[side])
        attacker.set(symbols[side], king->_row+2, king->_column-1);
    else if (king->_row+2 < MAX_ROWS && king->_column+1 < MAX_COLUMNS &&
        _list[king->_row+2][king->_column+1] == symbols[side])
        attacker.set(symbols[side], king->_row+2, king->_column+1);
    else if (king->_row-2 >= 0 && king->_column-1 >= 0 &&
        _list[king->_row-2][king->_column-1] == symbols[side])
        attacker.set(symbols[side], king->_row-2, king->_column-1);
    else if (king->_row-2 >= 0 && king->_column+1 < MAX_COLUMNS &&
        _list[king->_row-2][king->_column+1] == symbols[side])
        attacker.set(symbols[side], king->_row-2, king->_column+1);
    else if (king->_row+1 < MAX_ROWS && king->_column-2 >= 0 &&
             _list[king->_row+1][king->_column-2] == symbols[side])
        attacker.set(symbols[side], king->_row+1, king->_column-2);
    else if (king->_row+1 < MAX_ROWS && king->_column+2 < MAX_COLUMNS &&
             _list[king->_row+1][king->_column+2] == symbols[side])
        attacker.set(symbols[side], king->_row+1, king->_column+2);
    else if (king->_row-1 >= 0 && king->_column-2 >= 0 &&
             _list[king->_row-1][king->_column-2] == symbols[side])
        attacker.set(symbols[side], king->_row-1, king->_column-2);
    else if (king->_row-1 >= 0 && king->_column+2 < MAX_COLUMNS &&
             _list[king->_row-1][king->_column+2] == symbols[side])
        attacker.set(symbols[side], king->_row-1, king->_column+2);
    else
        attacker._status = BLOCKED;

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_knight(" << side << "), status " << attacker._status << endl;
    return attacker;
}


// This function checks to see if the current king is under check by a hostile
// rook, queen, or king going north
Attacker Board::checkedNorth(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    for (int i = king->_row-1; i >= 0 && attacker._status == OPEN; i--)
        attacker = goStraight(side, i, king->_column, (king->_row-i == 1));

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_north(" << side << "), status " << attacker._status << endl;
    return attacker;
}


// This function checks to see if the current king is under check by a hostile
// bishop, queen, or king going north-west
Attacker Board::checkedNorthEast(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int i = king->_row-1;
    int j = king->_column+1;
    while (i >= 0 && j < 8 && attacker._status == OPEN)
    {
        attacker = goDiagonal(side, i, j,
                            (king->_row-i == 1 && j-king->_column == 1));
        i--;
        j++;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_north_east(" << side << "), status " << attacker._status << endl;
    return attacker;
}


Attacker Board::checkedEast(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int j;
    for (j = king->_column+1; j < 8 && attacker._status == OPEN; j++)
        attacker = goStraight(side, king->_row, j, (j-king->_column == 1));

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_east(" << side << "), status " << attacker._status << endl;
    return attacker;
}


Attacker Board::checkedSouthEast(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int i = king->_row+1;
    int j = king->_column+1;
    while (i < 8 && j < 8 && attacker._status == OPEN)
    {
        attacker = goDiagonal(side, i, j,
                            (i-king->_row == 1 && j-king->_column == 1));
        i++;
        j++;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_south_east(" << side << "), status " << attacker._status << endl;
    return attacker;
}


Attacker Board::checkedSouth(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int i;
    for (i = king->_row+1; i < 8 && attacker._status == OPEN; i++)
        attacker = goStraight(side, i, king->_column, (i-king->_row == 1));

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_south(" << side << "), status " << attacker._status << endl;
    return attacker;
}


Attacker Board::checkedSouthWest(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int i = king->_row+1;
    int j = king->_column-1;
    while (i < 8 && j >= 0 && attacker._status == OPEN)
    {
        attacker = goDiagonal(side, i, j,
                            (i-king->_row == 1 && king->_column-j == 1));
        i++;
        j--;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_south_east(" << side << "), status " << attacker._status << endl;
    return attacker;
}


Attacker Board::checkedWest(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int j;
    for (j = king->_column-1; j >= 0 && attacker._status == OPEN; j--)
        attacker = goStraight(side, king->_row, j, (king->_column-j == 1));

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_west(" << side << "), status " << attacker._status << endl;
    return attacker;
}


Attacker Board::checkedNorthWest(Side side)
{
    Location* king = _kings+side;
    Attacker attacker;
    int i = king->_row-1;
    int j = king->_column-1;
    while (i >= 0 && j >= 0 && attacker._status == OPEN)
    {
        attacker = goDiagonal(side, i, j,
                            (king->_row-i == 1 && king->_column-j == 1));
        i--;
        j--;
    }

    if (_debug >= DEBUG_COPIOUS)
        cout << "checked_north_west(" << side << "), status " << attacker._status << endl;
    return attacker;
}


bool input(char* input_file, vector<Board>& list)
{
    bool success;
    ifstream file(input_file);
    if (file.is_open() == false)
    {
        cout << "Cannot open file " << input_file << endl;
        success = false;
    }
    else
    {
        while (true)
        {
            Board tmp;
            char line[80];
            for (int i = 0; i < 8; i++)
            {
                // read a line from file, discard the trailing newline, then
                // store the line in the current board
                file.getline(line, 80);
                strcpy(tmp._list[i], line);
            }

            // exit loop if last board was empty
            if (tmp.empty())
                break;

            // discard the blank line before moving on to the next board
            else
            {
                list.push_back(tmp);
                file.getline(line, 80);
            }
        }

        file.close();
        success = true;

        if (_debug >= DEBUG_COPIOUS)
            cout << "READ " << list.size() << " boards" << endl;
    }

    return success;
}


void output(vector<Board>& boards)
{
    if (_debug >= DEBUG_COPIOUS)
        cout << "PROCESSING " << boards.size() << " boards" << endl;

    const char* kings[2] = { "black", "white" };
    int i;
    for (i = 0; i < boards.size(); i++)
    {
        // print out current board
        if (_debug >= DEBUG_LIGHT)
        {
            int j;
            for (j = 0; j < 8; j++)
                cout << boards[i]._list[j] << endl;
        }

        Attacker attacker;
        for (Side j = BLACK; j <= WHITE; j++)
        {
            boards[i].findKing(j);

            // From the current king's location, check:
            // (1) all 2 vertical and 2 horizontal directions for hostile
            //     rooks, queens and kings;
            // (2) all 4 diagonal directions for hostile bishops, queens and
            //     kings;
            // (3) all 8 positions for hostile knights; and
            // (4) all 2 positions for hostile pawns
            attacker = boards[i].checkedNorth(j);
            if (attacker._status != CHECKED)
            {
                attacker = boards[i].checkedNorthEast(j);
                if (attacker._status != CHECKED)
                {
                    attacker = boards[i].checkedEast(j);
                    if (attacker._status != CHECKED)
                    {
                        attacker = boards[i].checkedSouthEast(j);
                        if (attacker._status != CHECKED)
                        {
                            attacker = boards[i].checkedSouth(j);
                            if (attacker._status != CHECKED)
                            {
                                attacker = boards[i].checkedSouthWest(j);
                                if (attacker._status != CHECKED)
                                {
                                    attacker = boards[i].checkedWest(j);
                                    if (attacker._status != CHECKED)
                                    {
                                        attacker = boards[i].checkedNorthWest(j);
                                        if (attacker._status != CHECKED)
                                        {
                                            attacker = boards[i].checkedKnight(j);
                                            if (attacker._status != CHECKED)
                                                attacker = boards[i].checkedPawn(j);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Location* king = boards[i]._kings+j;
            if (attacker._status == CHECKED)
            {
                if (_debug >= DEBUG_LIGHT)
                    cout << boards[i]._list[king->_row][king->_column]
                         << " at (" << king->_row << ", " << king->_column
                         << ") is in check by " << attacker._id << " at ("
                         << attacker._location._row << ", "
                         << attacker._location._column << ")" << endl;
                cout << "Game #" << i+1 << ": " << kings[j]
                     << " king is in check." << endl;
                break;
            }
            else
            {
                if (_debug >= DEBUG_LIGHT)
                    cout << boards[i]._list[king->_row][king->_column]
                         << " at (" << king->_row << ", " << king->_column
                         << ") is not in check." << endl;
            }
        }

        if (attacker._status != CHECKED)
            cout << "Game #" << (i+1) << ": no king is in check." << endl;

        if (_debug >= DEBUG_LIGHT)
            cout << endl;
    }
}


int main(int argc, char** argv)
{
    char opt;
    char* input_file = NULL;

    while ((opt = getopt(argc, argv, "i:d:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            input_file = optarg;
            break;
        case 'd':
            _debug = atoi(optarg);
            break;
        default:
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]"
                 << endl;
        }
    }

    if (input_file == NULL || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
    else
    {
        vector<Board> list;
        if (input(input_file, list))
            output(list);
    }
}
