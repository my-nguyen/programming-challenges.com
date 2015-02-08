#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */
#include <string.h> /* strcpy() */



#define NUM_ROWS 8
#define NUM_COLUMNS 8

#define MAX_PAWNS 8
#define MAX_KNIGHTS 2
#define MAX_BISHOPS 2
#define MAX_ROOKS 2
#define MAX_QUEENS 1


void assign_piece(char boards[][NUM_COLUMNS+1], int num_pieces, char piece)
{
    int j;
    for (j = 0; j < num_pieces; j++)
    {
        int row;
        int column;
        do
        {
            if (piece == 'p')
                row = rand() % (NUM_ROWS-1) + 1;
            else if (piece == 'P')
                row = rand() % (NUM_ROWS-1);
            else
                row = rand() % NUM_ROWS;
            column = rand() % NUM_COLUMNS;
        }
        while (boards[row][column] != '.');

        boards[row][column] = piece;
    }
}


void main(int argc, char** argv)
{
    int num_cases = 0;
    char* output_file = NULL;
    char opt;
    while ((opt = getopt(argc, argv, "c:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -b num_cases -o output_file\n", argv[0]);
        }
    }

    if (num_cases == 0 || output_file == NULL)
        printf("Usage: %s -b num_cases -o output_file\n", argv[0]);
    else
    {
        srand(time(NULL));

        FILE* file = fopen(output_file, "w");
        int j;
        for (j = 0; j < num_cases; j++)
        {
            char boards[NUM_ROWS][NUM_COLUMNS+1];
            int i;
            for (i = 0; i < NUM_ROWS; i++)
                strcpy(boards[i], "........");

            char pieces[2][6] =
            {
                { 'p', 'n', 'b', 'r', 'q', 'k' },
                { 'P', 'N', 'B', 'R', 'Q', 'K' }
            };

            for (i = 0; i < 2; i++)
            {
                assign_piece(boards, (rand() % (MAX_PAWNS+1)), pieces[i][0]);
                assign_piece(boards, (rand() % (MAX_KNIGHTS+1)), pieces[i][1]);
                assign_piece(boards, (rand() % (MAX_BISHOPS+1)), pieces[i][2]);
                assign_piece(boards, (rand() % (MAX_ROOKS+1)), pieces[i][3]);
                assign_piece(boards, (rand() % (MAX_QUEENS+1)), pieces[i][4]);
                assign_piece(boards, 1, pieces[i][5]);
            }

            for (i = 0; i < NUM_ROWS; i++)
                fprintf(file, "%s\n", boards[i]);
            fprintf(file, "\n");
        }

        int i;
        for (i = 0; i < NUM_ROWS; i++)
            fprintf(file, "........\n");
        fclose(file);
    }
}
