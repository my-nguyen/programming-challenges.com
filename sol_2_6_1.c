#include <stdio.h>
#include <unistd.h> // optarg
#include <stdlib.h> // abs()
#include "defs.h"


int _debug = DEBUG_OFF;


typedef int bool;
#define false 0
#define true  1


struct Input
{
    int* _list;
    int  _size;
};
typedef struct Input Input;


struct InputList
{
    Input _list[MAX_CASES];
    int   _size;
};
typedef struct InputList InputList;


void i_init(Input* input, int size)
{
    input->_list = (int*)malloc(size * sizeof(int));
    input->_size = size;
}


void i_print(Input* input)
{
    printf("%d", input->_size);
    int i;
    for (i = 0; i < input->_size; i++)
        printf(" %d", input->_list[i]);
    printf("\n");
}


void i_free(Input* input)
{
    free(input->_list);
}


void il_free(InputList* input)
{
    int i;
    for (i = 0; i < input->_size; i++)
        i_free(&input->_list[i]);
}


struct Difference
{
    bool _list[1000];
    int  _size;
};
typedef struct Difference Difference;


void d_init(Difference* diff, int size)
{
    int i;
    for (i = 0; i < size; i++)
        diff->_list[i] = false;
    diff->_size = size;
}


void d_print(Difference* diff)
{
    printf("Difference entries:\n");
    int i;
    for (i = 1; i < diff->_size; i++)
        printf("%d ", diff->_list[i]);
    printf("\n");
}


// scan the visited array to see if any number was not represented
bool d_scan(Difference* diff)
{
    int i;
    for (i = 1; i < diff->_size; i++)
        if (diff->_list[i] == false)
        {
            if (_debug >= DEBUG_COPIOUS)
                printf("Entry %d not represented\n", i);
            return false;
        }

    return true;
}


bool is_jolly(Input* input)
{
    // a sequence having only one integer is a jolly jumper
    if (input->_size == 1)
        return true;
    else
    {
        // create an array whose entries (between 1 and n-1) signify whether
        // such a value has been represented in the input sequence
        Difference diff;
        d_init(&diff, input->_size);


        // for each absolute difference, mark it in the visited array
        int i;
        for (i = 0; i < input->_size-1; i++)
        {
            int abs_diff = abs(input->_list[i]-input->_list[i+1]);
            if (_debug >= DEBUG_COPIOUS)
                printf("Absolute difference between %d and %d is %d\n",
                       input->_list[i], input->_list[i+1], abs_diff);

            if (abs_diff <= input->_size)
            {
                diff._list[abs_diff] = true;
                if (_debug >= DEBUG_COPIOUS)
                    printf("Marking entry %d as represented\n", abs_diff);
            }
            else
                if (_debug >= DEBUG_COPIOUS)
                    printf("Entry %d out of bound\n", abs_diff);
        }

        if (_debug >= DEBUG_COPIOUS)
            d_print(&diff);
        return d_scan(&diff);
    }
}


int input(char* input_file, InputList* input)
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
        // read the first number, which is the size of how many more integers
        // to follow
        int size;
        int i;
        for (i = 0; fscanf(file, "%d", &size) != EOF; i++)
        {
            // read the sequence of integers that follow the size
            Input current;
            i_init(&current, size);
            int j;
            for (j = 0; j < size; j++)
                fscanf(file, "%d", &current._list[j]);
            input->_list[i] = current;


            if (_debug >= DEBUG_LIGHT)
            {
                printf("%d", size);
                for (j = 0; j < size; j++)
                    printf(" %d", current._list[j]);
                printf("\n");
            }
        }

        input->_size = i;
        fclose(file);
        success = 1;
    }

    return success;
}


void output(InputList* input)
{
    int i;
    for (i = 0; i < input->_size; i++)
    {
        if (_debug >= DEBUG_LIGHT)
            i_print(&input->_list[i]);

        if (is_jolly(&input->_list[i]))
            printf("Jolly\n");
        else
            printf("Not jolly\n");
    }

    il_free(input);
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
        InputList cases;
        if (input(input_file, &cases) != 0)
            output(&cases);
    }
}
