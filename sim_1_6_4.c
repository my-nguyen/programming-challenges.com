/* program that generates input for 1.6.4.sol.*
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */



void main(int argc, char** argv)
{
    char opt;
    int num_cases = 0;
    int max_base = 0;
    int max_digits = 0;
    char* output_file = NULL;

    while ((opt = getopt(argc, argv, "c:b:d:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'b':
            max_base = atoi(optarg);
            break;
        case 'd':
            max_digits = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -b max_base -d max_digits -o output_file\n",
                   argv[0]);
        }
    }

    if (num_cases == 0 || max_base == 0 ||
        max_digits == 0.0 || output_file == NULL)
        printf("Usage: %s -c num_cases -b max_base -d max_digits -o output_file\n",
               argv[0]);
    else
    {
        srand(time(NULL));

        FILE* file = fopen(output_file, "w");
        int i;
        for (i = 0; i < num_cases; i++)
            fprintf(file, "%d %d\n", rand()%max_base+1, rand()%max_digits+1);
        fprintf(file, "0 0\n");

        fclose(file);
    }
}
