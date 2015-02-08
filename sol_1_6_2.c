#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "defs.h"


int _debug = DEBUG_OFF;


struct Field
{
    /* these 2 fields are self-explanatory */
    int  _lines;
    int  _columns;

    /* _input has 2 more lines and 2 more columns than the actual number of
     * lines and columns read from input file. The extra lines and columns will
     * be initialized to '+'. This serves to facilitate calculating the number
     * of mines in the surrounding cells of any particular cell. For example,
     * if the field from input file is:
     *		*...
     *		....
     *		.*..
     *		....
     * Then the internal _input field looks something like this:
     *		++++++
     *		+*...+
     *		+....+
     *		+.*..+
     *		+....+
     *		++++++
     */
    char** _input;

    /* _output contains the mine indicia as well as the calculated value of
     * each of the cell in the field. This doesn't need the special extra 2
     * lines and 2 columns, as opposed to _input
     */
    char** _output;
};
typedef struct Field field_t;


void f_lines(field_t* field, int lines)
{
    field->_lines = lines;
    field->_input = (char**)malloc((lines+2) * sizeof(char*));
    field->_output = (char**)malloc(lines * sizeof(char*));
}


void f_print_real_input(field_t* field)
{
    if (_debug >= DEBUG_COPIOUS)
    {
        /* print everything in _input */
        printf("%d %d\n", field->_lines, field->_columns);
        int i;
        for (i = 0; i < field->_lines+2; i++)
        {
            int j;
            for (j = 0; j < field->_columns; j++)
                printf("%c", field->_output[i][j]);
            printf("\n");
        }
    }
}


void f_columns(field_t* field, int columns)
{
    field->_columns = columns;
    if (field->_lines != 0)
    {
        int i;
        for (i = 0; i < field->_lines+2; i++)
        {
            /* allocate memory for each column and initialize the column to
             * the characters '+'
             */
            field->_input[i] = (char*)malloc(columns+3);
            int j;
            for (j = 0; j < columns+3; j++)
                field->_input[i][j] = '+';
            field->_input[i][columns+3] = '\0';
        }

        /* just allocate memory for each column; no initialization done */
        for (i = 0; i < field->_lines; i++)
            field->_output[i] = (char*)malloc(columns+1);
    }
}


void f_input(field_t* field, char input[][MAX_LINE])
{
    /* copy data from a 2-dimensional array of characters into an internal
     * 2-dimensional array while making sure to surround all borders of such
     * internal array with character '+'
     */
    int i;
    for (i = 0; i < field->_lines; i++)
        strncpy(field->_input[i+1]+1, input[i], field->_columns);
}


void f_print_input(field_t* field)
{
    if (_debug >= DEBUG_LIGHT)
    {
        /* make sure to print the field contained within the surrounding cells
         * that contain '+'
         */
        printf("%d %d\n", field->_lines, field->_columns);
        int i;
        for (i = 1; i < field->_lines+1; i++)
        {
            int j;
            for (j = 1; j < field->_columns+1; j++)
                printf("%c", field->_input[i][j]);
            printf("\n");
        }
    }
}


void fprint_output(field_t* field)
{
    /* printing _output is straightforward, unlike printing _input */
    int i;
    for (i = 0; i < field->_lines; i++)
    {
        int j;
        for (j = 0; j < field->_columns; j++)
            printf("%c", field->_output[i][j]);
        printf("\n");
    }
}


int is_mine(char value)
{
    /* does this cell contain a mine? */
    return (value == '*') ? 1 : 0;
}


void f_sweep(field_t* field)
{
    int i;
    for (i = 0; i < field->_lines; i++)
    {
        int j;
        for (j = 0; j < field->_columns; j++)
        {
            /* if this input cell contains a mine, record the mine in the
             * output cell
             */
            if (field->_input[i+1][j+1] == '*')
                field->_output[i][j] = '*';

            /* otherwise, calculate the number of mines in the surrounding 8
             * cells and record that number in the output cell
             */
            else
            {
                int total = is_mine(field->_input[i][j]) +
                            is_mine(field->_input[i][j+1]) +
                            is_mine(field->_input[i][j+2]) +
                            is_mine(field->_input[i+1][j]) +
                            is_mine(field->_input[i+1][j+2]) +
                            is_mine(field->_input[i+2][j]) +
                            is_mine(field->_input[i+2][j+1]) +
                            is_mine(field->_input[i+2][j+2]);
                field->_output[i][j] = '0' + total;
            }
        }
        field->_output[i][field->_columns] = '\0';
    }
}


void f_destructor(field_t* field)
{
    int i;
    for (i = 0; i < field->_lines+2; i++)
        free(field->_input[i]);
    free(field->_input);

    for (i = 0; i < field->_lines; i++)
        free(field->_output[i]);
    free(field->_output);
}


int input(char* input_file, field_t* fields, int* count)
{
    int success;
    FILE* file = fopen(input_file, "r");
    if (file == NULL)
    {
        printf("Cannot open file %s\n", input_file);
        success = 0;
    }
    else
    {
        /* read input from a file. The first line of each field contains
         * 2 integers which stand for the number of lines and columns of
         * the field. Each of the next n lines contains exactly
         * m characters, representing the field. Safe squares are denoted
         * by "." and mine squares by "*". The first field line where
         * n = m = 0 represents the end of input and should not be
         * processed.
         */
        while (1)
        {
            int tmplines, tmpcolumns;
            char tmp[MAX_LINE];

            /* make sure to read BOTH the 2 integers AND the EOL
             * character using either:
             * (1) fgets() + sscanf("%d%d"); or
             * (2) fscanf(file, "%d%d\n", &tmplines, &tmpcolumns);
             */
            fgets(tmp, MAX_LINE, file);
            sscanf(tmp, "%d%d", &tmplines, &tmpcolumns);

            /* lines = 0 and columns = 0 mean the end of input */
            if (tmplines == 0 || tmpcolumns == 0)
                break;
            else
            {
                f_lines(fields+*count, tmplines);
                f_columns(fields+*count, tmpcolumns);

                /* read the next few lines (determined by tmplines),
                 * each line containing several characters (determined
                 * by tmpcolumns), where each character is either
                 * a mine ('*') or a safe square ('.')
                 */
                int i;
                char input[80][MAX_LINE];
                for (i = 0; i < tmplines; i++)
                {
                    fgets(input[i], MAX_LINE, file);
                    input[i][strlen(input[i])-1] = '\0';
                }
                f_input(fields+*count, input);
                (*count)++;
            }
        }
        fclose(file);


        /* debug prints */
        int i;
        for (i = 0; i < *count; i++)
            f_print_input(fields+i);
        for (i = 0; i < *count; i++)
            f_print_real_input(fields+i);

        success = 1;
    }
    return success;
}


void output(field_t* fields, int count)
{
    /* for each cell in the field, sweep the surrounding cells for mines and
     * store the number of mines in the cell
     */
    int i;
    for (i = 0; i < count; i++)
        f_sweep(fields+i);


    /* print output in the format required */
    for (i = 0; i < count; i++)
    {
        if (i != 0)
            printf("\n");
        printf("Field #%d:\n", i+1);
        fprint_output(fields+i);
    }

    /* clean up memory allocation */
    for (i = 0; i < count; i++)
        f_destructor(fields+i);
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
        /* each test case is one mine field, each with a number of rows
         * and columns */
        field_t fields[MAX_CASES];
        int count = 0;
        if (input(input_file, fields, &count) != 0)
            output(fields, count);
    }
}
