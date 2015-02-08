#include <stdio.h>
#include <time.h>
#include <unistd.h> /* optarg */
#include <stdlib.h> /* RAND_MAX */
#include <string.h> /* strcpy() */


#define MAX_CANDIDATES 20
#define MAX_NAME       80
#define MAX_BALLOTS    400


char names[MAX_CANDIDATES][MAX_NAME] =
{
    "Zelda Roberto",
    "Pearle Bouffard",
    "Mahalia Pelkey",
    "Theodore Montijo",
    "Manual Hiebert",
    "Manuela Hoffmeister",
    "Loris Freund",
    "Annabelle Lasher",
    "Angele Mclemore",
    "Charlotte Dimas",
    "Graig Mancuso",
    "Frida Gatto",
    "Tamela Mcglone",
    "Latisha Freese",
    "Farrah Hemingway",
    "Hoa Messina",
    "Gussie Summa",
    "Alisia Vangorder",
    "Robyn Harbison",
    "Janelle Cusson"
};


struct Ballot
{
    int _list[MAX_BALLOTS];
    int _count;
};
typedef struct Ballot ballot_t;


void b_init(ballot_t* ballot, int size)
{
    int i;
    for (i = 0; i < size; i++)
        ballot->_list[i] = i+1;
    ballot->_count = size;
}


void b_shuffle(ballot_t* ballot)
{
    if (ballot->_count > 1)
    {
        int i;
        for (i = 0; i < ballot->_count - 1; i++) 
        {
            int j = i + rand() / (RAND_MAX / (ballot->_count - i) + 1);
            int tmp = ballot->_list[j];
            ballot->_list[j] = ballot->_list[i];
            ballot->_list[i] = tmp;
        }
    }
}


void b_print(ballot_t* ballot, FILE* file)
{
    int i;
    for (i = 0; i < ballot->_count; i++)
        fprintf(file, "%d ", ballot->_list[i]);
    fprintf(file, "\n");
}


void main(int argc, char** argv)
{
    int num_cases = 0;
    int max_candidates = 0;
    int max_ballots = 0;
    char* output_file = NULL;
    char opt;
    while ((opt = getopt(argc, argv, "c:d:b:o:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'd':
            max_candidates = atoi(optarg);
            break;
        case 'b':
            max_ballots = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        default:
            printf("Usage: %s -c num_cases -d max_candidates -b max_ballots -o output_file\n", argv[0]);
        }
    }

    if (num_cases == 0 || output_file == NULL)
        printf("Usage: %s -c num_cases -d max_candidates -b max_ballots -o output_file\n", argv[0]);
    else
    {
        srand(time(NULL));

        FILE* file = fopen(output_file, "w");


        // print the number of cases
        fprintf(file, "%d\n", num_cases);
        int i;
        for (i = 0; i < num_cases; i++)
        {
            // print a blank line
            fprintf(file, "\n");


            // print the number of candidates
            int num_candidates = rand() % max_candidates + 1;
            fprintf(file, "%d\n", num_candidates);


            // print all the candidates' name, each name on one line
            int j;
            for (j = 0; j < num_candidates; j++)
                fprintf(file, "%s\n", names[j]);


            // print the ballots
            ballot_t ballots;
            b_init(&ballots, num_candidates);
            int num_ballots = rand() % max_ballots + 1;
            for (j = 0; j < num_ballots; j++)
            {
                b_shuffle(&ballots);
                b_print(&ballots, file);
            }
        }

        fclose(file);
    }
}
