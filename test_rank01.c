#define NCARDS 52 /* number of cards */
#define NSUITS 4 /* number of suits */


char values[] = "23456789TJQKA";
char suits[] = "cdhs";


int rank_card(char value, char suit)
{
    int i,j; /* counters */
    for (i = 0; i < NCARDS/NSUITS; i++)
        if (values[i] == value)
            for (j = 0; j < NSUITS; j++)
                if (suits[j] == suit)
                    return(i*NSUITS + j);
    printf("Warning: bad input value=%d, suit=%d\n",value,suit);
}


char suit(int card)
{
    return suits[card % NSUITS];
}


char value(int card)
{
    return values[card / NSUITS];
}
