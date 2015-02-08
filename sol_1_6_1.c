#include <stdio.h>
#include <unistd.h>
#include "defs.h"


int _debug = DEBUG_OFF;


struct Pair
{
    int _first;
    int _second;
};
typedef struct Pair pair_t;


int cycle_length(int n)
{
    int length = 1;
    int tmp = n;

    if (_debug >= DEBUG_COPIOUS)
        printf("%d", n);

    while (n != 1)
    {
        if ((n % 2) == 1) /* odd number */
           n = n * 3 + 1;
        else              /* even number */
           n = n / 2;

        if (_debug >= DEBUG_COPIOUS)
            printf(" %d", n);
        length++;         /* keep track of cycle length */
    }
    if (_debug >= DEBUG_COPIOUS)
        printf("\n");

    if (_debug >= DEBUG_LIGHT)
        printf("cycle length of %d is %d\n", tmp, length);

    return length;
}


int input(char* input_file, pair_t* pairs, int* count)
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
         * of file
         */
        if (_debug >= DEBUG_COPIOUS)
            printf("count: %d\n", *count);
        while (fscanf(file, "%d%d",
                      &pairs[*count]._first, &pairs[*count]._second) != EOF)
        {
            if (_debug >= DEBUG_LIGHT)
               printf("%d %d\n", pairs[*count]._first, pairs[*count]._second);
            (*count)++;
            if (_debug >= DEBUG_COPIOUS)
                printf("count: %d\n", *count);
        }
        fclose(file);
        success = 1;
    }

    return success;
}


void output(pair_t* pairs, int count)
{
    /* process the actual entries in the array */
    int i;
    for (i = 0; i < count; i++)
    {
        /* for each array entry (pair of lower and upper bounds), do this:
         * for each integer from the lower bound to the upper bound, inclusive
         * of both bounds, get the cycle length for that integer, then obtain
         * the maximum value of all such cycle lengths
         */
        int j;
        int max = 0;
        for (j = pairs[i]._first; j <= pairs[i]._second; j++)
        {
            int tmp = cycle_length(j);
            if (tmp > max)
                max = tmp;
        }

        printf("%d %d %d\n", pairs[i]._first, pairs[i]._second, max);
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
        /* each test case is a pair of 2 integers */
        pair_t pairs[MAX_CASES];
        int count = 0;              /* how many actual entries in the array */

        if (input(input_file, pairs, &count) != 0)
            output(pairs, count);
    }
}
