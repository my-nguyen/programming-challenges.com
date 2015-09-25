#include <iostream>
#include <string>
#include <vector>
#include <cstdio>   // sprintf()
#include <cstring>  // strcpy(), strcmp()
#include <cstdlib>  // atoi()
using namespace std;

#define MAX_WORDS 1000
class RAM
{
public:
  // "words" is a field of 1000 instruction words, each containing 3 digits.
  // Even when the instruction words given in input don't fill all 1000 address
  // locations, the unused portion must still be available and initialized to
  // "000", because the instruction 9xx will attempt to override a word
  // instruction at address so-and-so with some value that is beyond the number
  // of words from input. consideration for selecting a type for "words":
  // (1) a vector of char[] was rejected because a vector element must be
  //   an object type to allow copying and assignment, whereas a char[]
  //   is not an object
  // (2) a vector of String was rejected because the field is a fixed-size
  //   list of 1000 elements, as opposed to the dynamical allocation nature of a
  //   vector
  // (3) an array of char[] was rejected because of the inconvenience in
  //   converting to and from char[] and String
  // (4) an array of String was chosen because of the convenience in assigning,
  //   format printing, etc.
  char words[MAX_WORDS][4];

  // count of all instruction words read from input
  int count;

public:
  RAM()
  {
    for (int i = 0; i < MAX_WORDS; i++)
      strcpy(words[i], "000");
    count = 0;
  }

  operator string()
  {
    string builder;
    for (int i = 0; i < count; i++)
      builder.append(words[i]).append("\n");
    return builder;
  }
};

#define MAX_REGISTERS 10
class Registers
{
public:
  int data[MAX_REGISTERS];

public:
  Registers()
  {
    for (int i = 0; i < MAX_REGISTERS; i++)
      data[i] = 0;
  }

  operator string()
  {
    string builder;
    for (int i = 0; i < MAX_REGISTERS; i++)
    {
      char tmp[10];
      sprintf(tmp, "%d ", data[i]);
      builder.append(tmp);
    }
    return builder;
  }

  // this method makes a copy of itself and returns the destination
  Registers& operator=(Registers& rhs)
  {
    for (int i = 0; i < MAX_REGISTERS; i++)
      data[i] = rhs.data[i];
    return *this;
  }

  // this method tells whether within the next 10 read's the current value will
  // become zero. this is a hack (10 was chosen at random) to prevent an
  // infinite loop.
  static bool approaching_zero(int previous, int current)
  {
    int diff = current - previous;
    int tmp = current;
    for (int i = 0; i < 10 && current != 0; i++)
      current += diff;
    return current == 0;
  }
};

vector<RAM> input()
{
  // number of cases
  char line[80];
  cin.getline(line, 80);
  int case_count = atoi(line);

  // skip the blank line following the number of cases
  cin.getline(line, 80);

  vector<RAM> list;
  for (int i = 0; i < case_count; i++)
  {
    RAM ram;

    // read until end of input
    while (cin.getline(line, 80))
    {
      // empty line means the start of a new set of instructions
      if (strlen(line) == 0)
        break;
      else
      {
        // fetch the 3-digit instruction
        strcpy(ram.words[ram.count], line);
        ram.count++;
      }
    }
    // retain the current set of instructions
    list.push_back(ram);
  }
  return list;
}

void output(vector<RAM>& list)
{
  for (vector<RAM>::iterator it = list.begin(); it != list.end(); it++)
  {
    // snapshot of all 1000 instructions and 10 registers
    char snap_words[1000][4];
    Registers snap_registers;
    bool snap_taken = false;

    Registers registers;
    // location of the current instruction word
    int location = 0;
    // the number of executed instructions
    int execution_count = 0;
    // because of the switch statement inside the while loop, a break won't
    // cause an exit from the while loop; a boolean flag is used instead.
    bool halt = false;
    while (halt == false)
    {
      // current instruction word based on location
      char* word = it->words[location];
      // the digits at location 0, 1, and 2 inside the current instruction word
      int zero = word[0] - '0';
      int one = word[1] - '0';
      int two = word[2] - '0';
      switch (zero)
      {
      // special and troublesome case
      case 0:
        // if register at 2 does not contain 0
        if (registers.data[two] != 0)
        {
          // if the location in register at 1 is the same as the current
          // location, then we have an infinite loop. In that case, just go to
          // the next instruction
          if (location == registers.data[one])
          {
            location++;
          }
          // if the instruction at the location contained in register at 1
          // hasn't changed between the last GOTO instruction (0xx) and now,
          // then we have another indefinite loop. In that case, halt the program
          else if (snap_taken &&
            !strcmp(snap_words[registers.data[one]], it->words[registers.data[one]]) &&
            !Registers::approaching_zero(snap_registers.data[two], registers.data[two]))
          {
            halt = true;
          }
          // only go to the location in register at 1 if everything is ok
          else
          {
            // take a snapshot of the instruction at the location contained in
            // register at 1
            strcpy(snap_words[registers.data[one]], it->words[registers.data[one]]);
            // take a snapshot of all registers
            snap_registers = registers;
            snap_taken = true;

            // go to the location in register at 1
            location = registers.data[one];
          }
        }
        // if the current location is beyond the current set of instructions,
        // halt the program
        else if (location >= it->count && !strcmp(it->words[location], "000"))
          halt = true;
        // otherwise, go to the next location
        else
          location++;
        break;
      case 1:
        // 100 means halt
        if (one == 0 && two == 0)
          halt = true;
        // all other 1xx instructions are invalid, so ignore them and move on
        // to the next instruction
        else
          location++;
        break;
      case 2:
        // set register at 1 to the value of 2
        registers.data[one] = two;
        location++;
        break;
      case 3:
        // add the value of 2 to register at 1
        registers.data[one] = (registers.data[one] + two) % 1000;
        location++;
        break;
      case 4:
        // multiply register at 1 by the value of 2
        registers.data[one] = (registers.data[one] * two) % 1000;
        location++;
        break;
      case 5:
        // set register at 1 to the value of register 2
        registers.data[one] = registers.data[two];
        location++;
        break;
      case 6:
        // add the value of register at 2 to register at 1
        registers.data[one] = (registers.data[one] + registers.data[two]) % 1000;
        location++;
        break;
      case 7:
        // multiply register at 1 by the value of register at 2
        registers.data[one] = (registers.data[one] * registers.data[two]) % 1000;
        location++;
        break;
      case 8:
        // set register at 1 to the value in RAM whose address is in register
        // at 2. So: first, fetch the content of register at 2, which is an
        // address. then, go to the address location in RAM and fetch the
        // instruction there and store it in register at 1.
        registers.data[one] = atoi(it->words[registers.data[two]]);
        location++;
        break;
      case 9:
        // set the value in RAM whose address is in register at 2 to that of
        // register at 1. So first, go to the address location in RAM
        // contained in register at 2, then change the word at that location
        // to the content of register at 1
        sprintf(it->words[registers.data[two]], "%03d", registers.data[one]);
        location++;
        break;
      }
      execution_count++;
    }

    // print the number of executed instructions, as required
    cout << execution_count << endl << endl;
  }
}

int main()
{
  vector<RAM> list = input();
  output(list);
}
