/* program that generates input for 1.6.5.sol.*
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */



void main(int argc, char** argv)
{
    char opt;
    int num_cases = 0;
    int max_instructions = 0;
    char* output_file = NULL;

    while ((opt = getopt(argc, argv, "c:i:f:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'i':
            max_instructions = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -i max_instructions -o output_file\n",
                   argv[0]);
        }
    }

    if (num_cases == 0 || max_instructions == 0 || output_file == NULL)
        printf("Usage: %s -c num_cases -i max_instructions -o output_file\n",
               argv[0]);
    else
    {
        srand(time(NULL));

        FILE* file = fopen(output_file, "w");

        /* a single positive integer on a line by itself indicating the number
         * of cases
         */
        fprintf(file, "%d\n", num_cases);
        int i;

        /* Each input case consists of up to 1,000 three-digit unsigned
         * integers, representing the contents of consecutive RAM locations
         * starting at 0
         */
        max_instructions = max_instructions % 1001 + 1;
        for (i = 0; i < num_cases; i++)
        {
            /* a blank line before each set of instructions */
            fprintf(file, "\n");
            int instructions = rand() % max_instructions + 1;
            int j;
            for (j = 0; j < instructions; j++)
                fprintf(file, "%03d\n", rand()%1000);

            /* HALT instruction */
            fprintf(file, "100\n");
        }

        fclose(file);
    }
}
