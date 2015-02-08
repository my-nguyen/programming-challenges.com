/* program that generates a random number of pairs of cases, each field is a
 * rantangle with a random number of lines and a random number of columns.
 * Within each cell of each such field is a character denoting a bomb "*" or
 * not ".", to be used by the programs in 1.6.2.sol.*
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */



void main(int argc, char** argv)
{
    char opt;
    int num_cases = 0;
    int max_students = 0;
    float max_expense = 0.0;
    char* output_file = NULL;

    while ((opt = getopt(argc, argv, "c:s:e:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 's':
            max_students = atoi(optarg);
            break;
        case 'e':
            max_expense = atof(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -s max_students -e max_expense -o output_file\n",
                   argv[0]);
        }
    }

    if (num_cases == 0 || max_students == 0 ||
        max_expense == 0.0 || output_file == NULL)
        printf("Usage: %s -c num_cases -s max_students -e max_expense -o output_file\n",
               argv[0]);
    else
    {
        srand(time(NULL));

        FILE* file = fopen(output_file, "w");
        int i;
        for (i = 0; i < num_cases; i++)
        {
            int num_students = rand() % max_students + 1;
            fprintf(file, "%d\n", num_students);

            int j;
            for (j = 0; j < num_students; j++)
                fprintf(file, "%.2f\n",
                       (float)rand() / (RAND_MAX / max_expense));
        }
        fprintf(file, "0\n");

        fclose(file);
    }
}
