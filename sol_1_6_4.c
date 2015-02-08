#include <stdio.h>
#include <string.h>
#include <unistd.h> /* optarg */
#include "defs.h"


int _debug = DEBUG_OFF;


struct lcd
{
    int  _base;
    char _digits[10];
};
typedef struct lcd lcd_t;


void print_zero(int base, int row, int column)
{
    if (row == 0 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else if (row == base+1)
        printf(" ");
    else
    {
        if (column == 0 || column == base+1)
            printf("|");
        else
            printf(" ");
    }
}


void print_one(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
        printf(" ");
    else
    {
        if (column == base+1)
            printf("|");
        else
            printf(" ");
    }
}


void print_two(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (row <= base)
        {
            if (column == base+1)
                printf("|");
            else
                printf(" ");
        }
        else
        {
            if (column == 0)
                printf("|");
            else
                printf(" ");
        }
    }
}


void print_three(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (column == base+1)
            printf("|");
        else
            printf(" ");
    }
}


void print_four(int base, int row, int column)
{
    if (row == 0 || row == 2*base+2)
        printf(" ");
    else if (row == base+1)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (row <= base)
        {
            if (column == 0 || column == base+1)
                printf("|");
            else
                printf(" ");
        }
        else
        {
            if (column == base+1)
                printf("|");
            else
                printf(" ");
        }
    }
}


void print_five(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (row <= base)
        {
            if (column == 0)
                printf("|");
            else
                printf(" ");
        }
        else
        {
            if (column == base+1)
                printf("|");
            else
                printf(" ");
        }
    }
}


void print_six(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (row <= base)
        {
            if (column == 0)
                printf("|");
            else
                printf(" ");
        }
        else
        {
            if (column == 0 || column == base+1)
                printf("|");
            else
                printf(" ");
        }
    }
}


void print_seven(int base, int row, int column)
{
    if (row == 0)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else if (row == base+1 || row == 2*base+2)
        printf(" ");
    else
    {
        if (column == base+1)
            printf("|");
        else
            printf(" ");
    }
}


void print_eight(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (column == 0 || column == base+1)
            printf("|");
        else
            printf(" ");
    }
}


void print_nine(int base, int row, int column)
{
    if (row == 0 || row == base+1 || row == 2*base+2)
    {
        if (column == 0 || column == base+1)
            printf(" ");
        else
            printf("-");
    }
    else
    {
        if (row <= base)
        {
            if (column == 0 || column == base+1)
                printf("|");
            else
                printf(" ");
        }
        else
        {
            if (column == base+1)
                printf("|");
            else
                printf(" ");
        }
    }
}


void print(int base, char* digits)
{
    /* define and initialize an array of pointer to print functions */
    static void (*prints[10])(int, int, int);
    static int init = 0;

    if (init == 0)
    {
        prints[0] = &print_zero;
        prints[1] = &print_one;
        prints[2] = &print_two;
        prints[3] = &print_three;
        prints[4] = &print_four;
        prints[5] = &print_five;
        prints[6] = &print_six;
        prints[7] = &print_seven;
        prints[8] = &print_eight;
        prints[9] = &print_nine;

        init = 1;
    }


    if (_debug >= DEBUG_COPIOUS)
        printf("base: %d, digits: %s, digits length: %d\n",
               base, digits, strlen(digits));

    int row;
    for (row = 0; row < 2*base+3; row++)
    {
        int i;
        for (i = 0; i < strlen(digits); i++)
        {
            if (i != 0)
                printf(" ");
            int column;
            for (column = 0; column < base+2; column++)
            {
                int digit = digits[i] - '0';
                (*prints[digit])(base, row, column);
            }
        }
        printf("\n");
    }
}

 
int input(char* input_file, lcd_t* lcds, int* count)
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
        /* read the 2 integers from file into one array element until end
         * of input
         */
        while (1)
        {
            /* read 2 integers */
            int base, digits;
            fscanf(file, "%d %d", &base, &digits);
            if (_debug >= DEBUG_COPIOUS)
               printf("%d %d\n", base, digits);

            /* both integers being zero means the end of input */
            if (base == 0 || digits == 0)
                break;
            else
            {
                lcds[*count]._base = base;
                sprintf(lcds[*count]._digits, "%d", digits);
                (*count)++;
            }
        }
        fclose(file);
        success = 1;
    }

    return success;
}


void output(lcd_t* list, int count)
{
    /* process the actual entries in the array */
    int i;
    for (i = 0; i < count; i++)
    {
        print(list[i]._base, list[i]._digits);
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
        /* each test case is one line of input, with each line having two
         * numbers representing the base and the digits to be displayed
         */
        lcd_t list[MAX_CASES];
        int count = 0;  /* how many actual entries in the array */
        if (input(input_file, list, &count) != 0)
            output(list, count);
    }
}
