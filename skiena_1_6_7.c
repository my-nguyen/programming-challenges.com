#include <stdio.h>
#include <string.h>
#include <unistd.h> /* optarg */
#include "defs.h"


int _debug = DEBUG_OFF;


typedef int bool;
#define false 0
#define true 1


struct Location
{
    int _row;
    int _column;
};
typedef struct Location location_t;


#define BLACK 0
#define WHITE 1

struct Board
{
    char       _list[8][9];
    location_t _kings[2];
};
typedef struct Board board_t;


/* Check to see if the current board is empty */
bool b_empty(board_t* board)
{
    int i;
    for (i = 0; i < 8; i++)
        if (strcmp(board->_list[i], "........") != 0)
            return false;
    return true;
}


/* Find the king of this side (BLACK or WHITE) from the current board and
 * record the location of the king
 */
void find_king(board_t* board, int side)
{
    static char kings[] = { 'k', 'K' };
    location_t* king = board->_kings+side;
    int i;
    for (i = 0; i < 8; i++)
    {
        char* search = strchr(board->_list[i], kings[side]);
        if (search != NULL)
        {
            king->_row = i;
            king->_column = search-board->_list[i];
            if (_debug >= DEBUG_COPIOUS)
                printf("Found kings[%d] at (%d, %d)\n",
                       side, king->_row, king->_column);
            break;
        }
    }
}


bool input(char* input_file, board_t* list, int* count)
{
    bool success;
    FILE* file = fopen(input_file, "r");
    if (file == NULL)
    {
        printf("Cannot open file %s\n", input_file);
        success = false;
    }
    else
    {
        *count = 0;
        while (true)
        {
            char line[MAX_LINE];
            int i;
            for (i = 0; i < 8; i++)
            {
                /* read a line from file, discard the trailing newline, then
                 * store the line in the current board
                 */
                fgets(line, MAX_LINE, file);
                line[strlen(line) - 1] = '\0';
                strcpy(list[*count]._list[i], line);
            }

            /* exit loop if last board was empty */
            if (b_empty(list + *count))
                break;

            /* discard the blank line before moving on to the next board */
            else
            {
                fgets(line, 80, file);
                (*count)++;
            }
        }

        fclose(file);
        success = true;

        if (_debug >= DEBUG_COPIOUS)
            printf("READ %d boards\n", *count);
    }

    return success;
}


/* OPEN    means the current square is empty
 * BLOCKED means the current square is occupied by a chess piece, but since the
 *         piece is either a friendly or an unharmful hostile, the current king
 *         is not under attacked, but cannot go further
 * CHECKED means the current square is occupied by a hostile piece that is
 *         putting the current king under attack or in check
 */
typedef int status_t;

#define OPEN    10
#define BLOCKED 11
#define CHECKED 12


/* TPAWN, etc stand for this pawn, etc (on the same side as this king)
 * OPAWN, etc stand for opposing pawn, etc (on the opposing side)
 */
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
    status_t   _status;
    char       _id;
    location_t _location;
};
typedef struct Attacker attacker_t;

void a_set(attacker_t* attacker, char id, int row, int column)
{
    attacker->_status = CHECKED;
    attacker->_id = id;
    attacker->_location._row = row;
    attacker->_location._column = column;
}


/* This function checks to see if the current square is open to the current
 * king, or is occupied by a chess piece that is unharmful to the king, or is
 * a hostile ROOK, QUEEN, or KING which puts the king in check
 */
attacker_t go_straight(board_t* board, int side, int row, int column,
                       bool next_door)
{
    static char symbols[2][11] =
    {
        { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
        { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
    };

    attacker_t attacker;
    char piece = board->_list[row][column];
    if (piece == '.')
        attacker._status = OPEN;
    else if (piece == symbols[side][TPAWN] || piece == symbols[side][TKNIGHT] ||
             piece == symbols[side][TBISHOP] || piece == symbols[side][TROOK] ||
             piece == symbols[side][TQUEEN] || piece == symbols[side][OPAWN] ||
             piece == symbols[side][OKNIGHT] || piece == symbols[side][OBISHOP])
        attacker._status = BLOCKED;
    else if (piece == symbols[side][OROOK] || piece == symbols[side][OQUEEN])
        a_set(&attacker, piece, row, column);
    else if (piece == symbols[side][OKING])
    {
        if (next_door)
            a_set(&attacker, piece, row, column);
        else
            attacker._status = BLOCKED;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("go_straight(%d, %d, %d, %d), piece %c, status %d\n",
               row, column, side, next_door, piece, attacker._status);
    return attacker;
}


/* This function checks to see if the current square is open to the current
 * king, or is occupied by a chess piece that is unharmful to the king, or is
 * a hostile BISHOP, QUEEN, or KING which puts the king in check
 */
attacker_t go_diagonal(board_t* board, int side, int row, int column,
                       bool next_door)
{
    static char symbols[2][11] =
    {
        { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
        { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
    };

    attacker_t attacker;
    char piece = board->_list[row][column];
    if (piece == '.')
        attacker._status = OPEN;
    else if (piece == symbols[side][TPAWN] || piece == symbols[side][TKNIGHT] ||
             piece == symbols[side][TBISHOP] || piece == symbols[side][TROOK] ||
             piece == symbols[side][TQUEEN] || piece == symbols[side][OPAWN] ||
             piece == symbols[side][OKNIGHT] || piece == symbols[side][OROOK])
        attacker._status = BLOCKED;
    else if (piece == symbols[side][OBISHOP] || piece == symbols[side][OQUEEN])
        a_set(&attacker, piece, row, column);
    else if (piece == symbols[side][OKING])
    {
        if (next_door)
            a_set(&attacker, piece, row, column);
        else
            attacker._status = BLOCKED;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("go_diagonal(%d, %d, %d, %d), piece %c, status %d\n",
               row, column, side, next_door, piece, attacker._status);
    return attacker;
}


#define MAX_ROWS    8
#define MAX_COLUMNS 8

/* This function checks to see if the current king is under check by a hostile
 * pawn by inspecting all 2 positions in the opposite direction where a pawn
 * might be
 */
attacker_t checked_pawn(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    if (side == BLACK)
    {
        if (king->_row+1 < MAX_ROWS && king->_column-1 >= 0 &&
            board->_list[king->_row+1][king->_column-1] == 'P')
            a_set(&attacker, 'P', king->_row+1, king->_column-1);
        else if (king->_row+1 < MAX_ROWS &&
                 king->_column+1 < MAX_COLUMNS &&
                 board->_list[king->_row+1][king->_column+1] == 'P')
            a_set(&attacker, 'P', king->_row+1, king->_column+1);
        else
            attacker._status = BLOCKED;
    }
    else
    {
        if (king->_row-1 >= 0 && king->_column-1 >= 0 &&
            board->_list[king->_row-1][king->_column-1] == 'p')
            a_set(&attacker, 'p', king->_row-1, king->_column-1);
        else if (king->_row-1 >= 0 &&
                 king->_column+1 < MAX_COLUMNS &&
                 board->_list[king->_row-1][king->_column+1] == 'p')
            a_set(&attacker, 'p', king->_row-1, king->_column+1);
        else
            attacker._status = BLOCKED;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_pawn(%d), status %d\n", side, attacker._status);
    return attacker;
}


/* This function checks to see if the current king is under check by a hostile
 * knight by inspecting all 8 possible positions where a knight might be
 */
attacker_t checked_knight(board_t* board, int side)
{
    static char symbols[2] = { 'N', 'n' };
    location_t* king = board->_kings+side;
    attacker_t attacker;

    if (king->_row+2 < MAX_ROWS && king->_column-1 >= 0 &&
        board->_list[king->_row+2][king->_column-1] == symbols[side])
        a_set(&attacker, symbols[side], king->_row+2, king->_column-1);
    else if (king->_row+2 < MAX_ROWS && king->_column+1 < MAX_COLUMNS &&
        board->_list[king->_row+2][king->_column+1] == symbols[side])
        a_set(&attacker, symbols[side], king->_row+2, king->_column+1);
    else if (king->_row-2 >= 0 && king->_column-1 >= 0 &&
        board->_list[king->_row-2][king->_column-1] == symbols[side])
        a_set(&attacker, symbols[side], king->_row-2, king->_column-1);
    else if (king->_row-2 >= 0 && king->_column+1 < MAX_COLUMNS &&
        board->_list[king->_row-2][king->_column+1] == symbols[side])
        a_set(&attacker, symbols[side], king->_row-2, king->_column+1);
    else if (king->_row+1 < MAX_ROWS && king->_column-2 >= 0 &&
             board->_list[king->_row+1][king->_column-2] == symbols[side])
        a_set(&attacker, symbols[side], king->_row+1, king->_column-2);
    else if (king->_row+1 < MAX_ROWS && king->_column+2 < MAX_COLUMNS &&
             board->_list[king->_row+1][king->_column+2] == symbols[side])
        a_set(&attacker, symbols[side], king->_row+1, king->_column+2);
    else if (king->_row-1 >= 0 && king->_column-2 >= 0 &&
             board->_list[king->_row-1][king->_column-2] == symbols[side])
        a_set(&attacker, symbols[side], king->_row-1, king->_column-2);
    else if (king->_row-1 >= 0 && king->_column+2 < MAX_COLUMNS &&
             board->_list[king->_row-1][king->_column+2] == symbols[side])
        a_set(&attacker, symbols[side], king->_row-1, king->_column+2);
    else
        attacker._status = BLOCKED;

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_knight(%d), status %d\n", side, attacker._status);
    return attacker;
}


/* This function checks to see if the current king is under check by a hostile
 * rook, queen, or king going north
 */
attacker_t checked_north(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int i;
    for (i = king->_row-1; i >= 0 && attacker._status == OPEN; i--)
        attacker = go_straight(board, side, i, king->_column,
                               (king->_row-i == 1));

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_north(%d): status %d\n", side, attacker._status);
    return attacker;
}


/* This function checks to see if the current king is under check by a hostile
 * bishop, queen, or king going north-west
 */
attacker_t checked_north_east(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int i = king->_row-1;
    int j = king->_column+1;
    while (i >= 0 && j < 8 && attacker._status == OPEN)
    {
        attacker = go_diagonal(board, side, i, j,
                             (king->_row-i == 1 && j-king->_column == 1));
        i--;
        j++;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_north_east(%d): status %d\n", side, attacker._status);
    return attacker;
}


attacker_t checked_east(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int j;
    for (j = king->_column+1; j < 8 && attacker._status == OPEN; j++)
        attacker = go_straight(board, side, king->_row, j,
                             (j-king->_column == 1));

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_east(%d): status %d\n", side, attacker._status);
    return attacker;
}


attacker_t checked_south_east(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int i = king->_row+1;
    int j = king->_column+1;
    while (i < 8 && j < 8 && attacker._status == OPEN)
    {
        attacker = go_diagonal(board, side, i, j,
                             (i-king->_row == 1 && j-king->_column == 1));
        i++;
        j++;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_south_east(%d): status %d\n", side, attacker._status);
    return attacker;
}


attacker_t checked_south(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int i;
    for (i = king->_row+1; i < 8 && attacker._status == OPEN; i++)
        attacker = go_straight(board, side, i, king->_column,
                             (i-king->_row == 1));

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_south(%d): status %d\n", side, attacker._status);
    return attacker;
}


attacker_t checked_south_west(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int i = king->_row+1;
    int j = king->_column-1;
    while (i < 8 && j >= 0 && attacker._status == OPEN)
    {
        attacker = go_diagonal(board, side, i, j,
                             (i-king->_row == 1 && king->_column-j == 1));
        i++;
        j--;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_south_west(%d): status %d\n", side, attacker._status);
    return attacker;
}


attacker_t checked_west(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int j;
    for (j = king->_column-1; j >= 0 && attacker._status == OPEN; j--)
        attacker = go_straight(board, side, king->_row, j,
                             (king->_column-j == 1));

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_west(%d): status %d\n", side, attacker._status);
    return attacker;
}


attacker_t checked_north_west(board_t* board, int side)
{
    location_t* king = board->_kings+side;
    attacker_t attacker;
    attacker._status = OPEN;
    int i = king->_row-1;
    int j = king->_column-1;
    while (i >= 0 && j >= 0 && attacker._status == OPEN)
    {
        attacker = go_diagonal(board, side, i, j,
                             (king->_row-i == 1 && king->_column-j == 1));
        i--;
        j--;
    }

    if (_debug >= DEBUG_COPIOUS)
        printf("checked_north_west(%d): status %d\n", side, attacker._status);
    return attacker;
}


void output(board_t* boards, int count)
{
    if (_debug >= DEBUG_COPIOUS)
        printf("PROCESSING %d boards\n", count);

    char* kings[2] = { "black", "white" };
    int i;
    for (i = 0; i < count; i++)
    {
        /* print out current board */
        if (_debug >= DEBUG_LIGHT)
        {
            int j;
            for (j = 0; j < 8; j++)
                printf("%s\n", boards[i]._list[j]);
        }

        attacker_t attacker;
        int j;
        for (j = 0; j < 2; j++)
        {
            find_king(boards+i, j);

            /* From the current king's location, check:
             * (1) all 2 vertical and 2 horizontal directions for hostile
             *     rooks, queens and kings;
             * (2) all 4 diagonal directions for hostile bishops, queens and
             *     kings;
             * (3) all 8 positions for hostile knights; and
             * (4) all 2 positions for hostile pawns
             */
            attacker = checked_north(boards+i, j);
            if (attacker._status != CHECKED)
            {
                attacker = checked_north_east(boards+i, j);
                if (attacker._status != CHECKED)
                {
                    attacker = checked_east(boards+i, j);
                    if (attacker._status != CHECKED)
                    {
                        attacker = checked_south_east(boards+i, j);
                        if (attacker._status != CHECKED)
                        {
                            attacker = checked_south(boards+i, j);
                            if (attacker._status != CHECKED)
                            {
                                attacker = checked_south_west(boards+i, j);
                                if (attacker._status != CHECKED)
                                {
                                    attacker = checked_west(boards+i, j);
                                    if (attacker._status != CHECKED)
                                    {
                                        attacker = checked_north_west(boards+i, j);
                                        if (attacker._status != CHECKED)
                                        {
                                            attacker = checked_knight(boards+i, j);
                                            if (attacker._status != CHECKED)
                                                attacker = checked_pawn(boards+i, j);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            location_t* king = boards[i]._kings+j;
            if (attacker._status == CHECKED)
            {
                if (_debug >= DEBUG_LIGHT)
                    printf("%c at (%d, %d) is in check by %c at (%d, %d)\n",
                           boards[i]._list[king->_row][king->_column],
                           king->_row, king->_column, attacker._id,
                           attacker._location._row, attacker._location._column);
                printf("Game #%d: %s king is in check.\n", i+1, kings[j]);
                break;
            }
            else
            {
                if (_debug >= DEBUG_LIGHT)
                    printf("%c at (%d, %d) is not in check.\n",
                           boards[i]._list[king->_row][king->_column],
                           king->_row, king->_column);
            }
        }

        if (attacker._status != CHECKED)
            printf("Game #%d: no king is in check.\n", i+1);
        if (_debug >= DEBUG_LIGHT)
            printf("\n");
    }
}


void main(int argc, char** argv)
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
            printf("Usage: %s -i input_file [-d 0|1|2]\n", argv[0]);
        }
    }

    if (input_file == NULL || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        printf("Usage: %s -i input_file [-d 0|1|2]\n", argv[0]);
    else
    {
        board_t list[MAX_CASES];
        int count;
        if (input(input_file, list, &count))
            output(list, count);
    }
}
