#include <stdio.h>


#define BLOCKED 10
#define OPEN    11
#define CHECKED 12

int function(char piece, int diff, int side)
{
    static char vert_horiz[2][11] =
    {
        { 'p', 'n', 'b', 'r', 'q', 'P', 'N', 'B', 'R', 'Q', 'K' },
        { 'P', 'N', 'B', 'R', 'Q', 'p', 'n', 'b', 'r', 'q', 'k' },
    };

    int status;
    switch (piece)
    {
        case '.':
            status = OPEN;
            break;
        case vert_horiz[side][0]:
        case 'n':
        case 'b':
        case 'r':
        case 'q':
            status = BLOCKED;
            break;
        case 'P':
        case 'N':
        case 'B':
            status = BLOCKED;
            break;
        case 'R':
            status = CHECKED;
            break;
        case 'Q':
            status = CHECKED;
            break;
        case 'K':
            if (diff == 1)
                status = CHECKED;
            else
                status = BLOCKED;
            break;
    }
}

void main()
{
}
