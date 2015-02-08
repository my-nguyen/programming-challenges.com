/* program that generates a random number of pairs of lower bound and upper
 * bound, each bound is also a random number up to a maximum value, to be used
 * by the programs in 1.6.1.sol.*
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */


void main(int argc, char** argv)
{
    char opt;
    int num_cases = 0;
    int upper_bound = 0;
    char* output_file = NULL;

    while ((opt = getopt(argc, argv, "c:r:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'r':
            upper_bound = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -r upper_bound -o output_file\n",
                   argv[0]);
        }
    }

    if (num_cases == 0 || upper_bound == 0 || output_file == NULL)
        printf("Usage: %s -c num_cases -r upper_bound -o output_file\n",
               argv[0]);
    else
    {
        srand(time(NULL));

        int i;
        FILE* file = fopen(output_file, "w");
        for (i = 0; i < num_cases; i++)
        {
            int lower = rand() % (upper_bound-1) + 1;
            int upper = rand() % (upper_bound-lower) + lower + 1;
            fprintf(file, "%d %d\n", lower, upper);
        }

        fclose(file);
    }
}
