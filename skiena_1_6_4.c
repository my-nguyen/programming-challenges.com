// this program currently seg-faults on input skiena_1_6_4-failed-c.in,
// possibly due to incorrect malloc() and free()
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // malloc() and free()

char print0(int size, int row, int column)
{
  if (row == 0 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else if (row == size+1)
    return ' ';
  else
  {
    if (column == 0 || column == size+1)
      return '|';
    else
      return ' ';
  }
}

char print1(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
    return ' ';
  else
  {
    if (column == size+1)
      return '|';
    else
      return ' ';
  }
}

char print2(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == 0)
        return '|';
      else
        return ' ';
    }
  }
}

char print3(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (column == size+1)
      return '|';
    else
      return ' ';
  }
}

char print4(int size, int row, int column)
{
  if (row == 0 || row == 2*size+2)
    return ' ';
  else if (row == size+1)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0 || column == size+1)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char print5(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char print6(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == 0 || column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char print7(int size, int row, int column)
{
  if (row == 0)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else if (row == size+1 || row == 2*size+2)
    return ' ';
  else
  {
    if (column == size+1)
      return '|';
    else
      return ' ';
  }
}

char print8(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (column == 0 || column == size+1)
      return '|';
    else
      return ' ';
  }
}

char print9(int size, int row, int column)
{
  if (row == 0 || row == size+1 || row == 2*size+2)
  {
    if (column == 0 || column == size+1)
      return ' ';
    else
      return '-';
  }
  else
  {
    if (row <= size)
    {
      if (column == 0 || column == size+1)
        return '|';
      else
        return ' ';
    }
    else
    {
      if (column == size+1)
        return '|';
      else
        return ' ';
    }
  }
}

char (*prints[10])(int, int, int);

void static_init()
{
  if (prints[0] == 0)
  {
    prints[0] = &print0;
    prints[1] = &print1;
    prints[2] = &print2;
    prints[3] = &print3;
    prints[4] = &print4;
    prints[5] = &print5;
    prints[6] = &print6;
    prints[7] = &print7;
    prints[8] = &print8;
    prints[9] = &print9;
  }
}

#define MAX_DIGITS 10
struct LCD
{
  // the size based on which to print a digit, with number of columns = size+2,
  // and number of rows = 2*size + 3
  int size;
  // a string of all digits, e.g. "12345", or "67890"
  char digits[MAX_DIGITS];
};
typedef struct LCD LCD;

void lcd_init(LCD* lcd, int base, int number)
{
  lcd->size = base;
  // convert int to a string of digits
  sprintf(lcd->digits, "%d", number);
}

void concatenate(char string[], char letter)
{
  sprintf(string+strlen(string), "%c", letter);
}

// due to the problem requirements, a string of digits (which is a number),
// is printed as follows: at each row, print all the characters that form a
// digit at that row, followed by a blank, then repeat the process for others.
// For example, the string 84 of size 3 would be printed as:
// at row 0: " ---       "
// at row 1: "|   | |   |"
// at row 2: "|   | |   |"
// at row 3: "|   | |   |"
// at row 4: " ---   --- "
// at row 5: "|   |     |"
// at row 6: "|   |     |"
// at row 7: "|   |     |"
// at row 8: " ---       "
// note how this satisfies the requirement of size + 2 columns and 2size + 3 rows
char* lcd_tostring(LCD* lcd)
{
  // each digit occupies size+2 characters, plus a space or a newline at the end
  // so each row occupies the product of the previous calculation and the number
  // of digits
  int column_size = (lcd->size+2+1) * strlen(lcd->digits);
  // the number of rows as per problem requirement
  int row_size = 2*lcd->size + 3;
  // extra 2 characters for blank line and the '\0' character
  char* builder = malloc(column_size * row_size + 2);
// printf("column: %d, row: %d, malloc %d\n", column_size, row_size, (column_size*row_size+2));

  strcpy(builder, "");

  // print row by row
  int row;
  for (row = 0; row < 2*lcd->size+3; row++)
  {
    // print all digits in each row
    int i;
    for (i = 0; i < strlen(lcd->digits); i++)
    {
      // blank column after each digit
      if (i != 0)
        strcat(builder, " ");

      // for each digit
      int column;
      for (column = 0; column < lcd->size+2; column++)
      {
        // convert each digit from char to int
        int index = lcd->digits[i] - '0';
        // using the int digit as an index to an array of prints, append just
        // one character forming the digit based on size and position(row, column)
        concatenate(builder, prints[index](lcd->size, row, column));
      }
    }
    strcat(builder, "\n");
  }
  // one blank line after each string of digits
  strcat(builder, "\n");

  return builder;
}

void input(LCD list[], int* count)
{
  static_init();
  *count = 0;
  while (1)
  {
    // read the 2 integers
    int size, number;
    scanf("%d%d", &size, &number);

    // end of input
    if (size == 0 || number == 0)
      break;
    else
    {
      LCD lcd;
      lcd_init(&lcd, size, number);

      list[*count] = lcd;
      *count += 1;
    }
  }
}

void output(LCD list[], int count)
{
  int i;
  for (i = 0; i < count; i++)
  {
    char* builder = lcd_tostring(list+i);
    printf("%s", builder);
    free(builder);
  }
}

void main()
{
  LCD list[10];
  int count;
  input(list, &count);
  output(list, count);
}
