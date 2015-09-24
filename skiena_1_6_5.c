#include <stdio.h>
#include <string.h>
#include <stdlib.h> // malloc(), free()

// #define WHITE '.'
#define WHITE 'O'
struct Image
{
  char** data;
  int row_count;
  int column_count;
};
typedef struct Image Image;

// this constructor creates a new column_count x row_count image with all
// pixels initially colored white 'O'
void im_initialize(Image* image, int column_count, int row_count)
{
  image->data = malloc(row_count * sizeof(char*));
  int i;
  for (i = 0; i < row_count; i++)
  {
    image->data[i] = malloc(column_count + 1);
    memset(image->data[i], WHITE, column_count);
    image->data[i][column_count] = '\0';
  }

  image->column_count = column_count;
  image->row_count = row_count;
}

void im_destructor(Image* image)
{
  int i;
  for (i = 0; i < image->row_count; i++)
    free(image->data[i]);
  free(image->data);
}

// this method clears the image by setting all pixels white '0'. The size
// remains unchanged
void im_clear(Image* image)
{
  int i;
  for (i = 0; i < image->row_count; i++)
  {
    int j;
    for (j = 0; j < image->column_count; j++)
      image->data[i][j] = WHITE;
  }
}

void sort(int* one, int* two)
{
  if (*one > *two)
  {
    int tmp = *one;
    *one = *two;
    *two = tmp;
  }
}

// this method draws a vertical segment of color in column, between row1 and
// row2 inclusive.
void im_vertical(Image* image, int column, int row1, int row2, char color)
{
  // sort row1 and row2, for the benefit of the following for loop
  sort(&row1, &row2);
  int i;
  for (i = row1-1; i < row2; i++)
    image->data[i][column-1] = color;
}

// this method draws a horizontal segment of color in row , between column1
// and column2 inclusive.
void im_horizontal(Image* image, int column1, int column2, int row, char color)
{
  // sort column1 and column2, for the benefit of the following for loop
  sort(&column1, &column2);
  int i;
  for (i = column1-1; i < column2; i++)
    image->data[row-1][i] = color;
}

// this method draws a filled rectangle with color, where (column1, row1) is
// the upper-left and (column2, row2) the lower right corner.
void im_draw(Image* image, int column1, int row1, int column2, int row2, char color)
{
  // sort row1, row2, column1 and column2 for the benefit of the for loops
  sort(&row1, &row2);
  sort(&column1, &column2);
  int i;
  for (i = row1-1; i < row2; i++)
  {
    int j;
    for (j = column1-1; j < column2; j++)
      image->data[i][j] = color;
  }
}

// internal recursive method, used by the other fill()
void im_fill_internal(Image* image, int column, int row, char old_color, char new_color)
{
  if (row >= 1 && row <= image->row_count &&
    column >= 1 && column <= image->column_count &&
    image->data[row-1][column-1] == old_color)
  {
    image->data[row-1][column-1] = new_color;

    im_fill_internal(image, column-1, row, old_color, new_color);
    im_fill_internal(image, column+1, row, old_color, new_color);
    im_fill_internal(image, column, row-1, old_color, new_color);
    im_fill_internal(image, column, row+1, old_color, new_color);
  }
}

// this method retunrs the color of the pixel at (column, row)
char im_get(Image* image, int column, int row)
{
  return image->data[row-1][column-1];
}

// this method colors the pixel at (column, row) in color
void im_set(Image* image, int column, int row, char color)
{
  image->data[row-1][column-1] = color;
}

// this method fills a region with color, where region is defined as follows.
// Pixel at (column, row) belongs to region. Any other pixel which is the same
// color as pixel at (column, row) and shares a common side with any pixel in
// region also belongs to this region.
void im_fill(Image* image, int column, int row, char color)
{
  char old_color = im_get(image, column, row);
  im_fill_internal(image, column, row, old_color, color);
}

void concatenate(char string[], char letter)
{
  sprintf(string+strlen(string), "%c", letter);
}

char* im_tostring(Image* image, char builder[])
{
  strcpy(builder, "");
  int i;
  for (i = 0; i < image->row_count; i++)
  {
    strcat(builder, image->data[i]);
    strcat(builder, "\n");
  }
  return builder;
}

// this method writes the file name in MSDOS 8.3 format followed by the
// contents of the current image
void im_save(Image* image, char* filename)
{
  FILE* file = fopen(filename, "w");
  int i;
  for (i = 0; i < image->row_count; i++)
  {
    image->data[i][image->column_count] = '\0';
    fprintf(file, "%s\n", image->data[i]);
  }
  fclose(file);

  printf("%s\n", filename);
  char builder[1000];
  printf("%s", im_tostring(image, builder));
}

#define MAX_DATA 100
// base class; all subclasses will have the specific functions
struct Command
{
  char line[MAX_DATA];
  // simulate C++ pure virtual function. At some point, the field execute
  // will be assigned to the appropriate function.
  void (*execute)(struct Command* command);
};
typedef struct Command Command;

void cm_virtual(Command* command, void (*function)(Command*))
{
  command->execute = function;
}

// There's only one image, created by the I command. all commands such as
// C, L, V, H, K, F, S operate on this single image.
static Image image;

void cm_constructor(Command* command, char* line)
{
  strcpy(command->line, line);
}

void cm_initialize(Command* command)
{
  int column_count, row_count;
  sscanf(command->line, "%d %d", &column_count, &row_count);
  im_initialize(&image, column_count, row_count);
}

void cm_color(Command* command)
{
  int column, row;
  char color;
  sscanf(command->line, "%d %d %c", &column, &row, &color);
  im_set(&image, column, row, color);
}

void cm_clear(Command* command)
{
  im_clear(&image);
}

void cm_vertical(Command* command)
{
  int column, row1, row2;
  char color;
  sscanf(command->line, "%d %d %d %c", &column, &row1, &row2, &color);
  im_vertical(&image, column, row1, row2, color);
}

void cm_horizontal(Command* command)
{
  int column1, column2, row;
  char color;
  sscanf(command->line, "%d %d %d %c", &column1, &column2, &row, &color);
  im_horizontal(&image, column1, column2, row, color);
}

void cm_draw(Command* command)
{
  int column1, column2, row1, row2;
  char color;
  sscanf(command->line, "%d %d %d %d %c", &column1, &column2, &row1, &row2, &color);
  im_draw(&image, column1, column2, row1, row2, color);
}

void cm_fill(Command* command)
{
  int column, row;
  char color;
  sscanf(command->line, "%d %d %c", &column, &row, &color);
  im_fill(&image, column, row, color);
}

void cm_save(Command* command)
{
  char filename[MAX_DATA];
  sscanf(command->line, "%s", filename);
  im_save(&image, filename);
}

void input(Command list[], int* count)
{
  // vector<Command*> instead of vector<Command> because Command is an abstract
  // base class which can't be instantiated.
  *count = 0;
  while (1)
  {
    // read and parse all tokens in each line
    char line[MAX_DATA];
    fgets(line, MAX_DATA, stdin);
    line[strlen(line)-1] = '\0';
    // decipher the character code
    char letter = line[0];
    // terminate program
    if (letter == 'X')
      break;
    else
    {
      // only process valid commands
      if (strchr("ICLVHKFS", letter) != NULL)
      {
        Command command;
        cm_constructor(&command, line+2);
        switch(letter)
        {
        case 'I':
          cm_virtual(&command, cm_initialize);
          break;
        case 'C':
          cm_virtual(&command, cm_clear);
          break;
        case 'L':
          cm_virtual(&command, cm_color);
          break;
        case 'V':
          cm_virtual(&command, cm_vertical);
          break;
        case 'H':
          cm_virtual(&command, cm_horizontal);
          break;
        case 'K':
          cm_virtual(&command, cm_draw);
          break;
        case 'F':
          cm_virtual(&command, cm_fill);
          break;
        case 'S':
          cm_virtual(&command, cm_save);
          break;
        }

        list[*count] = command;
        *count += 1;
      }
    }
  }
}

void output(Command* list, int count)
{
  int i;
  for (i = 0; i < count; i++)
    list[i].execute(list+i);
}

void main()
{
  Command list[MAX_DATA];
  int count;
  input(list, &count);
  output(list, count);
  im_destructor(&image);
}
