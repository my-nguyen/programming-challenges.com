/* program that generates a random number of pairs of cases, each field is a
 * rantangle with a random number of lines and a random number of columns.
 * Within each cell of each such field is a character denoting a bomb "*" or
 * not ".", to be used by the programs in 1.6.2.sol.*
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>


#define RATIO_SAFE_TO_MINE 4

void main(int argc, char** argv)
{
    char opt;
    int num_cases = 0;
    int max_rows = 0;
    int max_columns = 0;
    char* output_file = NULL;
    int ratio = RATIO_SAFE_TO_MINE;

    while ((opt = getopt(argc, argv, "c:r:l:o:a:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'r':
            max_rows = atoi(optarg);
            break;
        case 'l':
            max_columns = atoi(optarg);
            break;
        case 'a':
            ratio = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -r max_rows -l max_columns -o output_file [-a ratio]\n",
                   argv[0]);
        }
    }

    if (num_cases == 0 || max_rows == 0 ||
        max_columns == 0 || output_file == NULL || ratio <= 0)
        printf("Usage: %s -c num_cases -r max_rows -l max_columns -o output_file [-a ratio]\n",
               argv[0]);
    else
    {
        srand(time(NULL));

        FILE* file = fopen(output_file, "w");
        int i;
        for (i = 0; i < num_cases; i++)
        {
            int num_rows = rand() % max_rows + 1;
            int num_columns = rand() % max_columns + 1;
            fprintf(file, "%d %d\n", num_rows, num_columns);

            int j;
            for (j = 0; j < num_rows; j++)
            {
                int k;
                for (k = 0; k < num_columns; k++)
                    fprintf(file, "%c", ((rand() % ratio == 0) ? '*' : '.'));
                fprintf(file, "\n");
            }
        }
        fprintf(file, "0 0\n");

        fclose(file);
    }
}
