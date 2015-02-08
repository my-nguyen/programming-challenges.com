#include <stdio.h>


main()
{
    char line[] = "L 4 2 7 C";
    int x, y1, y2;
    char c;

    printf("command: %c\n", line[0]);
    sscanf(line+2, "%d %d %d %c", &x, &y1, &y2, &c);
    printf("x: %d, y1: %d, y2: %d, c: %c\n", x, y1, y2, c);
}
