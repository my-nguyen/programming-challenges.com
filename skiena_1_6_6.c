#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* atoi() */
#include <unistd.h> /* optarg */
#include "defs.h"


int _debug = DEBUG_OFF;


struct RAM
{
    char _locations[1000][4];
    int  _count;
};
typedef struct RAM ram_t;


void c_reset(ram_t* set)
{
    int i;
    for (i = 0; i < 1000; i++)
        strcpy(set->_locations[i], "000");
    set->_count = 0;
}


int input(char* input_file, ram_t* list, int* count)
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
        char line[80];
        fgets(line, 80, file);
        *count = atoi(line);
        if (_debug >= DEBUG_LIGHT)
            printf("%d\n", *count);

        int i = -1;
        while (fgets(line, 80, file) != NULL)
        {
            /* get rid of the trailing newline */
            line[strlen(line) - 1] = '\0';

            /* blank line means the beginning of a new set (case) of input */
            if (*line == '\0')
            {
                i++;
                c_reset(list+i);
                if (_debug >= DEBUG_LIGHT)
                    printf("Case %d\n", i);
            }
            else
            {
                if (_debug >= DEBUG_LIGHT)
                    puts(line);
                /* fetch the 3-digit instruction */
                strncpy(list[i]._locations[list[i]._count], line, 3);
                list[i]._count++;
            }
        }
        fclose(file);
        success = 1;
    }

    return success;
}


void r_clear(int* registers)
{
    int i;
    for (i = 0; i < 10; i++)
        registers[i] = 0;
}


void r_print(int* registers)
{
    printf("REGS ");
    int i;
    for (i = 0; i < 10; i++)
        printf("%d ", registers[i]);
}


/* Compares the array of integers this to the array other. Returns an integral
 * value indicating the relationship between the two arrays:
 * A zero value indicates that both arrays are equal.
 * A value greater than zero indicates that the first integer that does not
 * match has a greater value in this than in other; and a value less than zero
 * indicates the opposite
 */
int r_cmp(int* this, int* other)
{
    if (_debug >= DEBUG_LIGHT)
    {
        printf("\nTHIS ");
        r_print(this);
        printf("OTHER ");
        r_print(other);
        printf("\n");
    }

    int cmp = 0;
    int i;
    for (i = 0; i < 10; i++)
    {
        cmp = this[i] - other[i];
        if (cmp != 0)
            break;
    }
    return cmp;
}


/* Copies to array of integers pointed by source into the array pointed by
 * destination. destination is returned
 */
int* r_cpy(int* destination, int* source)
{
    int i;
    for (i = 0; i < 10; i++)
        destination[i] = source[i];
    return destination;
}


/* Whether within the next 10 times the current value will become zero */
int approaching_zero(int previous, int current)
{
    int diff = current - previous;
    int i;
    for (i = 0; i < 10 && current != 0; i++)
        current += diff;
    return current == 0;
}


int find(int* list, int size, int key)
{
    int i;
    for (i = 0; i < size; i++)
        if (list[i] == key)
            return 1;
    return 0;
}

#define VSTD_MAX 3


void output(ram_t* list, int count)
{
    int registers[10];
    int i;
    for (i = 0; i < count; i++)
    {
        r_clear(registers);
        int location = 0;
        int halt = 0;
        int count = 0;
        while (halt == 0)
        {
            if (_debug >= DEBUG_LIGHT)
                printf("LOC %d INSTR %s ", location,
                       list[i]._locations[location]);
            int zero = list[i]._locations[location][0] - '0';
            int one = list[i]._locations[location][1] - '0';
            int two = list[i]._locations[location][2] - '0';
            switch (zero)
            {
            case 0:
                /* if register at two does not contain 0, go to the location in
                 * register at one
                 */
                if (_debug >= DEBUG_COPIOUS)
                    printf("REG[%d]: %d, ", two, registers[two]);
                if (registers[two] != 0)
                {
                    /* snapshot of all 1000 instructions */
                    static int snap_instr[1000];
                    /* snapshot of all 10 registers */
                    static int snap_reg[10];
                    /* record of locations visited by a 0ds command */
                    static int visited[VSTD_MAX];
                    static int idx = 0;

                    if (_debug >= DEBUG_COPIOUS)
                        printf("REG[%d]: LOC %d, ", one, registers[one]);

                    /* if the location in register at one is the same as the
                     * current location, then we have an infinite loop. In that
                     * case, go to the next instruction
                     */
                    if (location == registers[one])
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            printf("same as current LOC: %d\n", location);
                        location++;
                    }

                    /* if the instruction at the location contained in register
                     * at one hasn't changed between the last GOTO instruction
                     * (0xx) and now, then we have another indefinite loop.
                     * In that case, halt the program
                     */
                    else if (snap_instr[registers[one]] ==
                                     atoi(list[i]._locations[registers[one]]) &&
                             !approaching_zero(snap_reg[two], registers[two]))
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            printf("INSTR hasn't changed: %d\n",
                                   snap_instr[registers[one]]);
                        halt = 1;
                    }

                    /* if the location contained in register at one has been
                     * recently visited, then we have another infinite loop
                    else if (find(visited, VSTD_MAX, registers[one]))
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            printf("LOC %d has been visited\n", registers[one]);
                        halt = 1;
                    }
                     */

                    /* only go to the location in register at one if everything
                     * is ok
                     */
                    else
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            printf("INSTR: %s\n",
                                   list[i]._locations[registers[one]]);
                        /* take a snapshot of the instruction at the location
                         * contained in register at one
                         */
                        snap_instr[registers[one]] =
                            atoi(list[i]._locations[registers[one]]);

                        /* take a snapshot of all registers */
                        r_cpy(snap_reg, registers);

                        visited[idx] = location;
                        idx = (idx+1) % VSTD_MAX;

                        /* go to the location in register at one */
                        location = registers[one];
                    }
                }

                /* if the current location is beyond the current set of
                 * instructions, halt the program
                 */
                else if (location >= list[i]._count &&
                         strcmp(list[i]._locations[location], "000") == 0)
                {
                    if (_debug >= DEBUG_COPIOUS)
                    {
                        printf("beyond %d\n", list[i]._count);
                        r_print(registers);
                    }
                    halt = 1;
                }

                /* otherwise, go to the next location */
                else
                    location++;
                count++;
                break;
            case 1:
                /* 100 means halt */
                if (one == 0 && two == 0)
                    halt = 1;
                /* otherwise, if instruction is 1xx, ignore the instruction */
                else
                    ;
                count++;
                location++;
                break;
            case 2:
                /* set register at one to two */
                registers[one] = two;
                count++;
                location++;
                break;
            case 3:
                /* add two to register at one */
                registers[one] = (registers[one] + two) % 1000;
                count++;
                location++;
                break;
            case 4:
                /* multiply register at one by two */
                registers[one] = (registers[one] * two) % 1000;
                count++;
                location++;
                break;
            case 5:
                /* set register at one to the value of register at two */
                registers[one] = registers[two];
                count++;
                location++;
                break;
            case 6:
                /* add the value of register at two to register at one */
                registers[one] = (registers[one] + registers[two]) % 1000;
                count++;
                location++;
                break;
            case 7:
                /* multiply register at one by the value of register at two */
                registers[one] = (registers[one] * registers[two]) % 1000;
                count++;
                location++;
                break;
            case 8:
                /* set register at one to the value in RAM whose address is
                 * in register at two
                 */
                registers[one] = atoi(list[i]._locations[registers[two]]);
                count++;
                location++;
                break;
            case 9:
                /* set the value in RAM whose address is in register at two to
                 * that of register at one
                 */
                {
                    char tmp[4];
                    strcpy(tmp, list[i]._locations[registers[two]]);
                    sprintf(list[i]._locations[registers[two]],
                            "%03d", registers[one]);
                    count++;
                    if (_debug >= DEBUG_LIGHT)
                        printf("LOC %d INSTR BEF %s AFT %s ",
                               registers[two], tmp,
                               list[i]._locations[registers[two]]);
                    location++;
                    break;
                }
            }

            if (_debug >= DEBUG_LIGHT)
            {
                r_print(registers);
                printf("COUNT %d\n", count);
            }
        }

        /* print the number of executed instructions, as required */
        printf("%d\n\n", count);
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
        ram_t list[MAX_CASES];
        int count;
        if (input(input_file, list, &count) != 0)
            output(list, count);
    }
}
