// compile: g++ -std=c++11 skiena_1_6_6.cpp (to enable std::stoi())
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>  // stringstream
#include <iomanip>  // setfill(), setw()
using namespace std;

class Instructions
{
public:
  // "data" is a field of 1000 instruction words, each containing 3 digits.
  // Even when the instruction words given in input don't fill all 1000 address
  // locations, the unused portion must still be available and initialized to
  // "000", because the instruction 9xx will attempt to override a instruction
  // instruction at address so-and-so with some value that is beyond the number
  // of words from input. consideration for selecting a type for "data":
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
  map<int, string> data;

public:
  Instructions()
  {
  }

  operator string()
  {
    string builder;
    for (map<int, string>::iterator it = data.begin(); it != data.end(); it++)
      builder.append(to_string(it->first)).append(": ").append(it->second).append("\n");
    return builder;
  }

  string get(int index)
  {
    return data.find(index) == data.end() ? "000" : data.find(index)->second;
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
      // pure C++: use of stringstream, setfill() and setw() to replace sprintf()
      stringstream stream;
      stream << setfill('0') << setw(3) << data[i];
      builder.append(stream.str()).append(" ");
    }
    return builder;
  }
};

vector<Instructions> input()
{
  // read number of cases
  string line;
  getline(cin, line);
  int case_count = stoi(line);

  // skip the blank line following the number of cases
  getline(cin, line);

  vector<Instructions> list;
  for (int i = 0; i < case_count; i++)
  {
    Instructions instructions;
    int index = 0;

    // read until end of input
    while (getline(cin, line))
    {
      // empty line means the start of a new set of instructions
      if (line.length() == 0)
        break;
      else
      {
        // fetch the 3-digit instruction
        instructions.data[index] = line;
        index++;
      }
    }
    // retain the current set of instructions
    list.push_back(instructions);
  }
  return list;
}

#define MAX_LOOPS 10
void output(vector<Instructions> list)
{
  for (int i = 0; i < list.size(); i++)
  {
    // cout << "Instructions:\n" << it->operator string() << endl;
    // snapshot of instructions
    int goto_register_2 = 999;
    int goto_count = 0;

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
      string instruction = list[i].get(location);
      // cout << "location: " << location << ", instruction: " << instruction.c_str() << ", registers: " << registers.operator string() << endl;
      // the digits at location 0, 1, and 2 inside the current instruction instruction
      int zero = instruction[0] - '0';
      int one = instruction[1] - '0';
      int two = instruction[2] - '0';
      switch (zero)
      {
      // special and troublesome case
      case 0:
        // cout << "\tgoto::instruction: " << instruction << ", register[" << one << "]: " << registers.data[one] << ", register[" << two << "]: " << registers.data[two] << endl;
        // if register at 2 does not contain 0
        if (registers.data[two] != 0)
        {
          // if the location in register at 1 is the same as the current
          // location, then we have an infinite loop. In that case, just go to
          // the next instruction
          if (location == registers.data[one])
          {
            // System.out.println("\tlocation: " + location + ", instruction: " + instruction + ", register[" + one + "]: " + registers.data[one]);
            location++;
          }
          // if the content of register at 2 hasn't changed between the last
          // GOTO instruction (0xx) and now, then we have another infinite
          // loop. In that case, halt the program
          else if (goto_register_2 == registers.data[two])
          {
            // cout << "\thalt::current location: " << location << ", next location: " << registers.data[one] << ", snapshot == register[" << two << "]: " << registers.data[two] << endl;
            halt = true;
          }
          else if (goto_count >= MAX_LOOPS)
          {
            // cout << "\thalt::current location: " << location << ", next location: " << registers.data[one] << ", goto_count: " << goto_count << endl;
            halt = true;
            execution_count -= goto_count;
          }
          // only go to the location in register at 1 if everything is ok
          else
          {
            // cout << "\tsnapshot::current location: " << location << ", next location: " << registers.data[one] << ", saving register[" << two << "]: " << registers.data[two] << endl;
            goto_register_2 = registers.data[two];
            goto_count++;
            // go to the location in register at 1
            location = registers.data[one];
          }
        }
        // if the next location is beyond the current set of instructions,
        // halt the program
        else if (list[i].data.find(location+1) == list[i].data.end())
        {
          // cout << "\thalt::next location (" << (location+1) << ") not in RAM" << endl;
          halt = true;
        }
        // otherwise, go to the next location
        else
          location++;
        break;
      case 1:
        // 100 means halt
        if (one == 0 && two == 0)
        {
          // cout << "\texit(100)" << endl;
          halt = true;
        }
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
        registers.data[one] = stoi(list[i].get(registers.data[two]));
        location++;
        break;
      case 9:
        {
          // set the value in RAM whose address is in register at 2 to that of
          // register at 1. So first, go to the address location in RAM
          // contained in register at 2, then change the instruction at that
          // location to the content of register at 1
          // pure C++: use of stringstream, setfill() and setw() to replace sprintf()
          stringstream stream;
          stream << setfill('0') << setw(3) << registers.data[one];
          list[i].data[registers.data[two]] = stream.str();
          // cout << "\tRAM-write::instruction: " << instruction << ", register[" << two << "](address): " << registers.data[two] << ", register[" << one << "](value): " << registers.data[one] << endl;
          // sprintf(it->instructions[registers.data[two]], "%03d", registers.data[one]);
          location++;
          break;
        }
      }
      execution_count++;
    }

    // print the number of executed instructions, as required
    cout << execution_count << endl << endl;
  }
}

int main()
{
  output(input());
}
