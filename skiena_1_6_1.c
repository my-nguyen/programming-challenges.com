#include <stdio.h>

struct bound_t
{
  int lower;
  int upper;
};
typedef struct bound_t bound_t;

// this method returns the cycle length of a given number
int cycle_length(int number)
{
  int length = 1;

  while (number != 1)
  {
    if ((number % 2) == 1) // odd number
      number = number * 3 + 1;
    else              // even number
      number /= 2;

    length++;         // keep track of cycle length
  }

  return length;
}

// this method reads input and returns a list of pairs of lower and upper bounds
void input(bound_t* list, int* size)
{
  *size = 0;
  while (scanf("%d", &list[*size].lower) != EOF)
  {
    // read the 2 integers from file into a pair
    scanf("%d", &list[*size].upper);
    *size += 1;
  }
}

// this method returns the maximum of all cycle lengths
void process(bound_t* list, int size)
{
  // for each array entry of lower and upper bounds, get the cycle length for
  // each integer within those bounds, then obtain the maximum value of all
  // such cycle lengths
  int i;
  for (i = 0; i < size; i++)
  {
    int max = 0;
    int j;
    for (j = list[i].lower; j <= list[i].upper; j++)
    {
      int tmp = cycle_length(j);
      if (tmp > max)
        max = tmp;
    }

    printf("%d %d %d\n", list[i].lower, list[i].upper, max);
  }
}

void main()
{
  bound_t list[20];
  int size;
  input(list, &size);
  process(list, size);
}
