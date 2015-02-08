/* program that generates input for 1.6.5.sol.*
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */
#include "defs.h"


void c_clear(FILE* file, int columns, int rows)
{
    fprintf(file, "C\n");
}


void c_color(FILE* file, int columns, int rows)
{
    fprintf(file, "L %d %d %c\n",
            rand()%columns+1, rand()%rows+1, rand()%26+'A');
}


void c_range(int max, int* low, int* high)
{
    *low = rand() % (max-1) + 1;
    *high = rand() % (max-*low) + *low+1;
}


void c_vertical(FILE* file, int columns, int rows)
{
    int low, high;
    c_range(rows, &low, &high);
    fprintf(file, "V %d %d %d %c\n", rand()%columns+1, low, high, rand()%26+'A');
}


void c_horizontal(FILE* file, int columns, int rows)
{
    int low, high;
    c_range(columns, &low, &high);
    fprintf(file, "H %d %d %d %c\n", low, high, rand()%rows+1, rand()%26+'A');
}


void c_rectangle(FILE* file, int columns, int rows)
{
    int low_column, low_row, high_column, high_row;
    c_range(columns, &low_column, &high_column);
    c_range(rows, &low_row, &high_row);
    fprintf(file, "K %d %d %d %d %c\n", low_column, low_row,
            high_column, high_row, rand()%26+'A');
}


void c_region(FILE* file, int columns, int rows)
{
    fprintf(file, "F %d %d %c\n",
            rand()%columns+1, rand()%rows+1, rand()%26+'A');
}


#define FILE_SAVE "1.6.5.rand"
void c_save(FILE* file, int columns, int rows)
{
    fprintf(file, "S %s\n", FILE_SAVE);
}


#define MAX_COMMANDS 7
void generate(int num_cases, FILE* file, int columns, int rows)
{
    /* define and initialize an array of pointer to prints */
    static void (*commands[MAX_COMMANDS])(FILE*, int, int);
    static int init = 0;

    if (init == 0)
    {
        commands[0] = &c_clear;
        commands[1] = &c_color;
        commands[2] = &c_vertical;
        commands[3] = &c_horizontal;
        commands[4] = &c_rectangle;
        commands[5] = &c_region;
        commands[6] = &c_save;

        init = 1;
    }

    int i;
    for (i = 0; i < num_cases; i++)
    {
        int index = rand() % MAX_COMMANDS;
        (*commands[index])(file, columns, rows);
    }
}



void main(int argc, char** argv)
{
    char opt;
    int num_cases = 0;
    int max_rows = 0;
    int max_columns = 0;
    char* output_file = NULL;

    while ((opt = getopt(argc, argv, "c:r:l:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'r':
            max_rows = atoi(optarg);
            break;
        case 'l':
            max_columns = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -r max_rows -l max_columns -o output_file\n",
                   argv[0]);
        }
    }

    if (num_cases == 0 || max_rows == 0 || max_columns == 0 ||
        output_file == NULL)
        printf("Usage: %s -c num_cases -r max_rows -l max_columns -o output_file\n",
               argv[0]);
    else
    {
        /* must make the total number of cases equal (MAX_CASES-3 -- to
         * accommodate the 3 commands I, S and X), so they may altogether fit
         * the array of commands declared in the various solutions to this
         * problem
         */
        if (num_cases > MAX_CASES-3)
            num_cases = MAX_CASES-3;


        /* begin the session */
        srand(time(NULL));
        FILE* file = fopen(output_file, "w");
        int rows = rand()%max_rows+1;
        int columns = rand()%max_columns+1;
        fprintf(file, "I %d %d\n", columns, rows);


        /* generate a bunch of commands */
        generate(num_cases, file, columns, rows);


        /* make at least one save */
        fprintf(file, "S %s\n", FILE_SAVE);


        /* terminate the session */
        fprintf(file, "X\n");
        fclose(file);
    }
}
