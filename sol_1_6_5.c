#include <stdio.h>
#include <string.h>
#include <unistd.h> /* optarg */
#include "defs.h"


int _debug = DEBUG_OFF;


struct Image
{
    int  _rows;
    int  _columns;
    char _pixels[80][80];
};
typedef struct Image image_t;


void i_print(image_t* image)
{
    int x;
    for (x = 0; x < image->_rows; x++)
        printf("%s\n", image->_pixels[x]);
}


// #define C_CLEAR '.'
#define C_CLEAR 'O'
/* Create a new M x N image with all pixels initially colored white (.) */
void i_create(image_t* image, int columns, int rows)
{
    image->_columns = columns;
    image->_rows = rows;
    int i;
    for (i = 0; i < rows; i++)
    {
        int j;
        for (j = 0; j < columns; j++)
            image->_pixels[i][j] = C_CLEAR;
        image->_pixels[i][columns] = '\0';
    }

    if (_debug >= DEBUG_LIGHT)
    {
        printf("I %d %d\n", columns, rows);
        i_print(image);
    }
}


/* Clear the table by setting all pixels white (.). The size remains unchanged.
 */
void i_clear(image_t* image)
{
    int i;
    for (i = 0; i < image->_rows; i++)
    {
        int j;
        for (j = 0; j < image->_columns; j++)
            image->_pixels[i][j] = C_CLEAR;
    }

    if (_debug >= DEBUG_LIGHT)
    {
        printf("C\n");
        i_print(image);
    }
}


/* Return the color of the pixel at (column, row) */
char i_pixel(image_t* image, int column, int row)
{
    if (_debug >= DEBUG_COPIOUS)
        printf("pixel at (%d, %d): '%c'\n",
               column, row, image->_pixels[row-1][column-1]);
    return image->_pixels[row-1][column-1];
}


/* Colors the pixel (X, Y ) in color (C) */
void i_color(image_t* image, int column, int row, char color)
{
    image->_pixels[row-1][column-1] = color;

    if (_debug >= DEBUG_LIGHT)
    {
        printf("L %d %d %c\n", column, row, color);
        i_print(image);
    }
}


void i_sort(int* one, int* two)
{
    if (*one > *two)
    {
        int tmp = *one;
        *one = *two;
        *two = tmp;
    }
}


/* Draw a vertical segment of color (C) in column X, between the rows Y1 and Y2
 * inclusive.
 */
void i_vertical(image_t* image, int column, int row1, int row2, char color)
{
    /* sort row1 and row2, for the benefit of the following for loop */
    i_sort(&row1, &row2);
    int i;
    for (i = row1-1; i < row2; i++)
        image->_pixels[i][column-1] = color;

    if (_debug >= DEBUG_LIGHT)
    {
        printf("V %d %d %d %c\n", column, row1, row2, color);
        i_print(image);
    }
}


/* Draw a horizontal segment of color (C) in the row Y, between the columns X1
 * and X2 inclusive.
 */
void i_horizontal(image_t* image, int column1, int column2, int row, char color)
{
    /* sort column1 and column2, for the benefit of the following for loop */
    i_sort(&column1, &column2);
    int i;
    for (i = column1-1; i < column2; i++)
        image->_pixels[row-1][i] = color;

    if (_debug >= DEBUG_LIGHT)
    {
        printf("H %d %d %d %c\n", column1, column2, row, color);
        i_print(image);
    }
}


/* Draw a filled rectangle of color C, where (X1, Y1) is the upper-left and
 * (X2, Y2) the lower right corner.
 */
void i_rectangle(image_t* image, int column1, int row1,
                 int column2, int row2, char color)
{
    /* sort row1, row2, column1 and column2 for the benefit of the for loops */
    i_sort(&row1, &row2);
    i_sort(&column1, &column2);
    int i;
    for (i = row1-1; i < row2; i++)
    {
        int j;
        for (j = column1-1; j < column2; j++)
            image->_pixels[i][j] = color;
    }

    if (_debug >= DEBUG_LIGHT)
    {
        printf("K %d %d %d %d %c\n", column1, row1, column2, row2, color);
        i_print(image);
    }
}


/* Fill the region R with the color C, where R is defined as follows. Pixel
 * (X, Y) belongs to R. Any other pixel which is the same color as pixel (X, Y)
 * and shares a common side with any pixel in R also belongs to this region.
 */
void i_fill(image_t* image, int column, int row, char old_color, char new_color)
{
    if (row < 1 || row > image->_rows ||
        column < 1 || column > image->_columns ||
        image->_pixels[row-1][column-1] != old_color)
        ;
    else
    {
        image->_pixels[row-1][column-1] = new_color;
        i_fill(image, column-1, row, old_color, new_color);
        i_fill(image, column+1, row, old_color, new_color);
        i_fill(image, column, row-1, old_color, new_color);
        i_fill(image, column, row+1, old_color, new_color);
    }
}


/* Write the file name in MSDOS 8.3 format followed by the contents of the
 * current image
 */
void i_save(image_t* image, char* filename)
{
    FILE* file = fopen(filename, "w");
    int i;
    for (i = 0; i < image->_rows; i++)
    {
        image->_pixels[i][image->_columns] = '\0';
        fprintf(file, "%s\n", image->_pixels[i]);
    }
    fclose(file);

    printf("%s\n", filename);
    i_print(image);
}


struct Command
{
    char _command;
    int  _x1;
    int  _x2;
    int  _y1;
    int  _y2;
    char _color;
    char _filename[30];
};
typedef struct Command command_t;


void c_copy(command_t* dest, command_t* source)
{
    dest->_command = source->_command;
    dest->_x1 = source->_x1;
    dest->_x2 = source->_x2;
    dest->_y1 = source->_y1;
    dest->_y2 = source->_y2;
    dest->_color = source->_color;
    strcpy(dest->_filename, source->_filename);
}


void c_execute(command_t* command)
{
    static image_t image;

    if (_debug >= DEBUG_COPIOUS)
        printf("Command %c\n", command->_command);

    switch (command->_command)
    {
    case 'I':
        i_create(&image, command->_x1, command->_y1);
        break;
    case 'C':
        i_clear(&image);
        break;
    case 'L':
        i_color(&image, command->_x1, command->_y1, command->_color);
        break;
    case 'V':
        i_vertical(&image, command->_x1, command->_y1,
                   command->_y2, command->_color);
        break;
    case 'H':
        i_horizontal(&image, command->_x1, command->_x2,
                     command->_y1, command->_color);
        break;
    case 'K':
        i_rectangle(&image, command->_x1, command->_y1,
                    command->_x2, command->_y2, command->_color);
        break;
    case 'F':
        i_fill(&image, command->_x1, command->_y1,
               i_pixel(&image, command->_x1, command->_y1), command->_color);

        /* debug print is done outside i_fill(), unlike other i_...() functions,
         * because i_fill() is recursive
         */
        if (_debug >= DEBUG_LIGHT)
        {
            printf("F %d %d %c\n", command->_x1, command->_y1, command->_color);
            i_print(&image);
        }
        break;
    case 'S':
        i_save(&image, command->_filename);
        break;
    case 'X':
        break;
    }
}


int input(char* input_file, command_t commands[], int* count)
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
        /* read the commmands and their corresponding parameters until
         * end of input
         */
        while (1)
        {
            char line[MAX_LINE];
            fgets(line, MAX_LINE, file);
            line[strlen(line)-1] = '\0';
            if (_debug >= DEBUG_LIGHT)
                printf("%s\n", line);

            command_t tmp;
            tmp._command = line[0];
            switch(tmp._command)
            {
            case 'I':
                sscanf(line+2, "%d %d", &tmp._x1, &tmp._y1);
                break;
            case 'C':
                break;
            case 'L':
                sscanf(line+2, "%d %d %c", &tmp._x1, &tmp._y1, &tmp._color);
                break;
            case 'V':
                sscanf(line+2, "%d %d %d %c",
                       &tmp._x1, &tmp._y1, &tmp._y2, &tmp._color);
                break;
            case 'H':
                sscanf(line+2, "%d %d %d %c",
                       &tmp._x1, &tmp._x2, &tmp._y1, &tmp._color);
                break;
            case 'K':
                sscanf(line+2, "%d %d %d %d %c",
                       &tmp._x1, &tmp._y1, &tmp._x2, &tmp._y2, &tmp._color);
                break;
            case 'F':
                sscanf(line+2, "%d %d %c", &tmp._x1, &tmp._y1, &tmp._color);
                break;
            case 'S':
                sscanf(line+2, "%s", tmp._filename);
                break;
            case 'X':
                break;
            }


            if (tmp._command == 'X')
                break;
            else
            {
                c_copy(commands+*count, &tmp);
                (*count)++;
            }
        }
        fclose(file);

        success = 1;
    }

    return success;
}


void output(command_t* commands, int count)
{
    /* process the actual entries in the array */
    int i;
    for (i = 0; i < count; i++)
        c_execute(commands+i);
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
        /* each test case is just one command */
        command_t list[MAX_CASES];
        int count = 0;  /* how many actual entries in the array */
        if (input(input_file, list, &count) != 0)
            output(list, count);
    }
}
