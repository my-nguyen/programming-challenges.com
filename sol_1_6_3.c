#include <stdio.h>
#include <unistd.h>
#include "defs.h"


int _debug = DEBUG_OFF;

#define MAX_STUDENTS 1000


struct Trip
{
    float _expenses[1000]; /* expense per student */
    int   _count;          /* number of students */
};
typedef struct Trip trip_t;


int float_cmp(void* one, void* two)
{
    return *((int*)one) - *((int*)two);
}


void t_sort(trip_t* this)
{
    qsort(this->_expenses, this->_count, sizeof(float), float_cmp);
}


void t_print(trip_t* this)
{
    int i;
    for (i = 0; i < this->_count; i++)
        printf("%.2f ", this->_expenses[i]);
    printf("\n");
}


float t_average(trip_t* this)
{
    int i;
    float total = 0.0;
    for (i = 0; i < this->_count; i++)
        total += this->_expenses[i];

    if (_debug >= DEBUG_COPIOUS)
        printf("AVERAGE: %.2f\n", total / this->_count);
    return total / this->_count;
}


float t_exchange(trip_t* this)
{
    float total = 0.0;
    float avg = t_average(this);
    int i;
    for (i = 0; i < this->_count && this->_expenses[i] < avg; i++)
        total += avg - this->_expenses[i];
    return total;
}


int input(char* input_file, trip_t* trips, int* count)
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
        while (1)
        {
            /* read in the number of students on this trip */
            int tmp;
            fscanf(file, "%d", &tmp);

            /* number of students equal zero means end of input */
            if (tmp == 0)
                break;
            else
            {
                /* read in the expense of each student in this trip */
                trips[*count]._count = tmp;
                int i;
                for (i = 0; i < tmp; i++)
                    fscanf(file, "%f", trips[*count]._expenses+i);
                (*count)++;
            }
        }

        if (_debug >= DEBUG_COPIOUS)
            printf("read %d trips\n", *count);

        success = 1;
    }
    return success;
}


void output(trip_t* trips, int count)
{
    /* sort all the expenses, to facilitate calculating the average
     * and the exchange amount per student
     */
    int i;
    for (i = 0; i < count; i++)
    {
        if (_debug >= DEBUG_COPIOUS)
        {
            printf("PRE-SORT: ");
            t_print(trips+i);
        }
        t_sort(trips+i);
        if (_debug >= DEBUG_COPIOUS)
        {
            printf("POST-SORT: ");
            t_print(trips+i);
        }
    }


    /* print output in the format required */
    for (i = 0; i < count; i++)
        printf("$%.2f\n", t_exchange(trips+i));
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
        /* each test case is one trip, each trip having up to 1000 students
         * and a maximum expense of $10,000
         */
        trip_t trips[MAX_CASES];
        int count = 0;    /* number of trips */
        if (input(input_file, trips, &count) != 0)
            output(trips, count);
    }
}
