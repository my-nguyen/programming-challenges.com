#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */


void main(int argc, char** argv)
{
    int num_cases = 0;
    int max_sequence = 0;
    char* output_file = NULL;
    char opt;
    while ((opt = getopt(argc, argv, "c:s:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 's':
            max_sequence = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -s max_sequence -o output_file\n", argv[0]);
        }
    }

    if (num_cases == 0 || output_file == NULL)
        printf("Usage: %s -c num_cases -s max_sequence -o output_file\n", argv[0]);
    else
    {
        srand(time(NULL));
        FILE* file = fopen(output_file, "w");


        int i;
        for (i = 0; i < num_cases; i++)
        {
            // print the sequence count
            int num_sequence = rand() % max_sequence + 1;
            fprintf(file, "%d", num_sequence);
            printf("%d", num_sequence);


            // print all the numbers in the sequence
            // for testing purpuse, here I choose not to include negative
            // numbers as well as positive ones that are beyond the sequence
            // count
            int j;
            for (j = 0; j < num_sequence; j++)
            {
                int number = rand() % num_sequence + 1;
                fprintf(file, " %d", number);
                printf(" %d", number);
            }
            fprintf(file, "\n");
            printf("\n");
        }

        fclose(file);
    }
}
