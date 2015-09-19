#include <stdio.h>

#define MAX_STUDENTS 1000
struct expenses_t
{
  float data[MAX_STUDENTS];
  int count;
};
typedef struct expenses_t expenses_t;

void ex_add(expenses_t* expenses, float expense)
{
  expenses->data[expenses->count] = expense;
  expenses->count += 1;
}

int float_cmp(void* one, void* two)
{
  return *((int*)one) - *((int*)two);
}

void ex_sort(expenses_t* expenses)
{
  qsort(expenses->data, expenses->count, sizeof(float), float_cmp);
}

float ex_average(expenses_t* expenses)
{
  float total = 0.0f;
  int i;
  for (i = 0; i < expenses->count; i++)
    total += expenses->data[i];

  return total / expenses->count;
}

float ex_exchange(expenses_t* expenses)
{
  float total = 0.0f;
  float avg = ex_average(expenses);
  int i;
  for (i = 0; i < expenses->count; i++)
    if (expenses->data[i] >= avg)
      break;
    else
      total += avg - expenses->data[i];
  return total;
}

void ex_print(expenses_t* expenses)
{
  int i;
  for (i = 0; i < expenses->count; i++)
      printf("%.2f ", expenses->data[i]);
  printf("\n");
}

void input(expenses_t list[], int* count)
{
  *count = 0;
  while (1)
  {
    // read in the number of students on this Expenses
    int student_count;
    scanf("%d", &student_count);

    // end of input
    if (student_count == 0)
      break;
    else
    {
      // read in the expense of each student in this Expenses
      int i;
      for (i = 0; i < student_count; i++)
      {
        float expense;
        scanf("%f", &expense);
        ex_add(list+*count, expense);
      }
      *count += 1;
    }
  }
}

void output(expenses_t list[], int count)
{
  int i;
  for (i = 0; i < count; i++)
  {
    // sort all the expenses, to facilitate calculating the average and the
    // exchange amount per student
    ex_sort(list + i);
    // print output in the format required
    printf("$%.2f\n", ex_exchange(list+i));
  }
}

void main()
{
  expenses_t list[100];
  int count;
  input(list, &count);
  output(list, count);
}
