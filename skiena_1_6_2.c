#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct field_t
{
  // data has 2 more lines and 2 more columns than the actual number of
  // lines and columns read from input file. The extra lines and columns will
  // be initialized to '+'. This serves to facilitate calculating the number
  // of mines in the surrounding cells of any particular cell. So, if the
  // input_t from input file is:
  //		*...
  //		....
  //		.*..
  //		....
  // Then the internal data input_t looks something like this:
  //		++++++
  //		+*...+
  //		+....+
  //		+.*..+
  //		++++++
  char** data;
  int row_count;
  int column_count;
};
typedef struct field_t field_t;

// this method creates a new string having the desired length, and padd the
// contents of such string with blank characters "+"
char* blanks(int size)
{
  char* line = malloc(size);
  memset(line, '+', size-1);
  line[size-1] = '\0';
  return line;
}

// this constructor initializes internal data from a list of strings
#define MAX_ROW 100
#define MAX_COLUMN 100
void fi_init_input(field_t* field, int row, int column, char list[][MAX_COLUMN])
{
// ("inside fi_init_input() field: %p\n", field);
  field->row_count = row + 2;
  field->column_count = column + 2;
  field->data = malloc(field->row_count * sizeof(char*));
  // add a blank string at the beginning
  field->data[0] = blanks(column + 3);

  // store the date while padding a blank at both the beginning and the end
  int i;
  for (i = 0; i < row; i++)
  {
    field->data[i+1] = malloc(column + 3);
    field->data[i+1][0] = '+';
    strcpy(field->data[i+1]+1, list[i]);
    strcat(field->data[i+1], "+");
  }

  // add a blank string to the end
  field->data[row+1] = blanks(column + 3);
}

// this method builds the internal data field, by recording the mines where
// they are present in data, and also the number of mines in the surrounding
// 8 cells, for each cell
void fi_init_output(field_t* output, field_t* input)
{
  // remember: input has 2 extra lines
  output->row_count = input->row_count - 2;
  output->data = malloc(output->row_count * sizeof(char*));

  int i;
  for (i = 1; i < input->row_count-1; i++)
  {
    char* builder = malloc(input->column_count-1);
    builder[0] = '\0';
    // each line in data has 2 extra columns (at beginning and at end)
    int j;
    for (j = 1; j < input->column_count-1; j++)
    {
      // if this input cell contains a mine, record the mine in the output cell
      if (input->data[i][j] == '*')
        strcat(builder, "*");
      // otherwise, calculate the number of mines in the surrounding 8 cells
      // and record that number in the output cell
      else
        sprintf(builder+strlen(builder), "%c", ('0' + fi_mine_total(input, i, j)));
    }
    // retain the string
    output->data[i-1] = builder;
  }
}

void fi_destructor(field_t* field)
{
  int i;
  for (i = 0; i < field->row_count; i++)
    free(field->data[i]);
  free(field->data);
}

// this method prints the original field as read from STDIN
char* fi_tostring_original(field_t* field, char* string)
{
  string[0] = '\0';
  int i;
  for (i = 1; i < field->row_count-1; i++)
  {
    char tmp[MAX_COLUMN];
    strncpy(tmp, field->data[i]+1, field->column_count-2);
    tmp[field->column_count-2] = '\n';
    tmp[field->column_count-1] = '\0';
    strcat(string, tmp);
  }
  return string;
}

// this method prints everything in data
char* fi_tostring(field_t* field, char* string)
{
  string[0] = '\0';
  int i;
  for (i = 0; i < field->row_count; i++)
    sprintf(string+strlen(string), "%s\n", field->data[i]);
  return string;
}

// this method returns whether this cell contain a mine
int mined(char value)
{
  return (value == '*') ? 1 : 0;
}

// this method returns the total number of mines in the surrounding 8 cells
int fi_mine_total(field_t* field, int i, int j)
{
  return mined(field->data[i-1][j-1]) +
         mined(field->data[i-1][j]) +
         mined(field->data[i-1][j+1]) +
         mined(field->data[i][j-1]) +
         mined(field->data[i][j+1]) +
         mined(field->data[i+1][j-1]) +
         mined(field->data[i+1][j]) +
         mined(field->data[i+1][j+1]);
}

// this method reads data from STDIN. The first line of each input_t contains 2
// integers which stand for the number of lines and columns of the input_t. Each
// of the next n lines contains exactly m characters, representing the input_t.
// Safe squares are denoted by "." and mine squares by "*". The first input_t
// line where n = m = 0 represents the end of input and should not be processed.
void input(field_t list[], int* size)
{
  *size = 0;
  while (1)
  {
    // read 2 integers
    int line_count, column_count;
    scanf("%d%d", &line_count, &column_count);
    // throw away the EOL, to prepare for the next line of text
    char tmp[20];
    fgets(tmp, 20, stdin);

    // end of input
    if (line_count == 0 || column_count == 0)
      break;
    else
    {
      char lines[MAX_ROW][MAX_COLUMN];
      // read the next line_count lines
      int i;
      for (i = 0; i < line_count; i++)
      {
        // allocate 1 extra character to accommodate the \n character from fgets()
        fgets(lines[i], column_count+2, stdin);
        // throw away the \n character
        lines[i][column_count] = '\0';
      }

      // create and retain a field of lines
      fi_init_input(list + *size, line_count, column_count, lines);
      *size += 1;
    }
  }
}

// this method sweeps thru a list of fields; for each field it prints out the
// mine cells as well as cells which contain the total number of mines in
// the surrounding 8 cells
void output(field_t list[], int size)
{
  // use traditional for loop instead of the advanced for loop because index
  // is needed in printing
  int i;
  for (i = 0; i < size; i++)
  {
    // for each cell in the input_t, sweep the surrounding cells for mines and
    // store the number of mines in the cell
    field_t out;
    fi_init_output(&out, list+i);

    // print output in the format required
    printf("\n");
    printf("Field #%d:\n", i+1);
    char string[MAX_ROW*MAX_COLUMN];
    printf("%s", fi_tostring(&out, string));

    fi_destructor(list+i);
    fi_destructor(&out);
  }
}

// assume for now maximum of 100 test fields
#define MAX_FIELD 100
void main()
{
  field_t list[MAX_FIELD];
  int size;
  input(list, &size);
  output(list, size);
}
