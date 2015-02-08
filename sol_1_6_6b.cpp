#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
using namespace std;


const int DEBUG_OFF     = 0;
const int DEBUG_LIGHT   = 1;
const int DEBUG_COPIOUS = 2;


int _debug = DEBUG_OFF;


struct RAM
{
    // Considerations for implementing the field _locations. Facts: the field
    // represents a RAM address of 1000 instruction words, each word
    // comprising 3 digits. Even if the instruction words given in input don't
    // fill all 1000 address locations, the unused portion must still be
    // available and initialized to "000", because the instruction 9xx will
    // attempt to override a word instruction at address so-and-so with some
    // value that is beyond the number of words from input.
    // (1) a vector of char[] was considered but rejected because a vector
    //     element must be an object type to allow copying and assignment,
    //     which is not the case with an array of char;
    // (2) a vector of string was considered but also rejected because the
    //     field is a fixed-size list, as opposed to the dynamical allocation
    //     nature of a vector
    char _locations[1000][4];
    int  _count;

public:
    void reset();
};


void RAM::reset()
{
    for (int i = 0; i < 1000; i++)
        strcpy(_locations[i], "000");
    _count = 0;
}


bool input(const char* input_file, vector<RAM>& list)
{
    bool success;
    ifstream file(input_file);
    if (file.is_open() == false)
    {
        cout << "Cannot open file " << input_file << endl;
        success = false;
    }
    else
    {
        char line[80];
        file.getline(line, 80);
        int i = atoi(line);
        if (_debug >= DEBUG_LIGHT)
            cout << i << endl;

        while (file.getline(line, 80))
        {
            // blank line means the beginning of a new set (case) of input
            if (*line == '\0')
            {
                RAM record;
                record.reset();
                list.push_back(record);
                if (_debug >= DEBUG_LIGHT)
                    cout << "Case " << i << endl;
            }
            else
            {
                if (_debug >= DEBUG_LIGHT)
                    cout << line << endl;
                // fetch the 3-digit instruction
                strncpy(list.back()._locations[list.back()._count], line, 3);
                list.back()._count++;
            }
        }
        file.close();
        success = true;
    }

    return success;
}


struct Registers
{
public:
    int _list[10];

public:
    void clear();
    void print() const;
    bool operator==(const Registers& other) const;
    Registers& operator=(const Registers& rhs);

    static bool approaching_zero(int previous, int current);
};


void Registers::clear()
{
    for (int i = 0; i < 10; i++)
        _list[i] = 0;
}


void Registers::print() const
{
    cout << "REGS ";
    for (int i = 0; i < 10; i++)
        cout << _list[i] << " ";
}


// Compares the array of integers this to the array other. Returns whether
// this list of registers is equal to that of another list
bool Registers::operator==(const Registers& other) const
{
    if (_debug >= DEBUG_LIGHT)
    {
        cout << endl << "THIS ";
        print();
        cout << "OTHER ";
        other.print();
        cout << endl;
    }

    bool equal = true;
    for (int i = 0; i < 10; i++)
    {
        if (_list[i] != other._list[i])
        {
            equal = false;
            break;
        }
    }
    return equal;
}


// Copies to array of integers pointed by source into the array pointed by
// destination. destination is returned
Registers& Registers::operator=(const Registers& source)
{
    for (int i = 0; i < 10; i++)
        _list[i] = source._list[i];
    return *this;
}


// Whether within the next 10 times the current value will become zero */
bool Registers::approaching_zero(int previous, int current)
{
    int diff = current - previous;
    for (int i = 0; i < 10 && current != 0; i++)
        current += diff;
    return current == 0;
}


void output(vector<RAM>& list)
{
    Registers registers;
    for (int i = 0; i < list.size(); i++)
    {
        registers.clear();
        int location = 0;
        int count = 0;
        bool halt = false;
        while (halt == false)
        {
            if (_debug >= DEBUG_LIGHT)
                cout << "LOC " << location << " INSTR "
                     << list[i]._locations[location] << " ";
            int zero = list[i]._locations[location][0] - '0';
            int one = list[i]._locations[location][1] - '0';
            int two = list[i]._locations[location][2] - '0';
            switch (zero)
            {
            case 0:
                // if register at two does not contain 0, go to the location in
                // register at one
                if (_debug >= DEBUG_COPIOUS)
                    cout << "REG[" << two << "]: "
                         << registers._list[two] << ", ";
                if (registers._list[two] != 0)
                {
                    // snapshot of all 1000 instructions
                    static int snap_instr[1000];
                    // snapshot of all 10 registers
                    static Registers snap_reg;

                    if (_debug >= DEBUG_COPIOUS)
                        cout << "REG[" << one << "]: LOC "
                             << registers._list[one] << ", ";

                    // if the location in register at one is the same as the
                    // current location, then we have an infinite loop. In that
                    // case, go to the next instruction
                    if (location == registers._list[one])
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            cout << "same as current LOC: " << location << endl;
                        location++;
                    }

                    // if the instruction at the location contained in register
                    // at one hasn't changed between the last GOTO instruction
                    // (0xx) and now, then we have another indefinite loop.
                    // In that case, halt the program
                    else if (snap_instr[registers._list[one]] ==
                             atoi(list[i]._locations[registers._list[one]]) &&
                             !Registers::approaching_zero(snap_reg._list[two],
                                                          registers._list[two]))
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            cout << "INSTR hasn't changed: "
                                 << snap_instr[registers._list[one]] << endl;
                        halt = true;
                    }

                    // only go to the location in register at one if everything
                    // is ok
                    else
                    {
                        if (_debug >= DEBUG_COPIOUS)
                            cout << "INSTR: "
                                 << list[i]._locations[registers._list[one]]
                                 << endl;
                        // take a snapshot of the instruction at the location
                        // contained in register at one
                        snap_instr[registers._list[one]] =
                            atoi(list[i]._locations[registers._list[one]]);

                        // take a snapshot of all registers
                        snap_reg = registers;

                        // go to the location in register at one
                        location = registers._list[one];
                    }
                }
                // if the current location is beyond the current set of
                // instructions, halt the program
                else if (location >= list[i]._count &&
                         strcmp(list[i]._locations[location], "000") == 0)
                {
                    if (_debug >= DEBUG_COPIOUS)
                    {
                        printf("beyond %d\n", list[i]._count);
                        registers.print();
                    }
                    halt = true;
                }

                // otherwise, go to the next location
                else
                    location++;
                count++;
                break;
            case 1:
                // 100 means halt
                if (one == 0 && two == 0)
                    halt = true;
                // otherwise, if instruction is 1xx, ignore the instruction
                else
                    ;
                count++;
                location++;
                break;
            case 2:
                // set register at one to two
                registers._list[one] = two;
                count++;
                location++;
                break;
            case 3:
                // add two to register at one
                registers._list[one] = (registers._list[one] + two) % 1000;
                count++;
                location++;
                break;
            case 4:
                // multiply register at one by two
                registers._list[one] = (registers._list[one] * two) % 1000;
                count++;
                location++;
                break;
            case 5:
                // set register at one to the value of register at two
                registers._list[one] = registers._list[two];
                count++;
                location++;
                break;
            case 6:
                // add the value of register at two to register at one
                registers._list[one] = (registers._list[one] +
                                        registers._list[two]) % 1000;
                count++;
                location++;
                break;
            case 7:
                // multiply register at one by the value of register at two
                registers._list[one] = (registers._list[one] *
                                        registers._list[two]) % 1000;
                count++;
                location++;
                break;
            case 8:
                // set register at one to the value in RAM whose address is
                // in register at two
                registers._list[one] =
                    atoi(list[i]._locations[registers._list[two]]);
                count++;
                location++;
                break;
            case 9:
                // set the value in RAM whose address is in register at two to
                // that of register at one
                {
                    char before[4];
                    strcpy(before, list[i]._locations[registers._list[two]]);
                    char after[4];
                    sprintf(after, "%03d", registers._list[one]);
                    strcpy(list[i]._locations[registers._list[two]], after);
                    count++;
                    if (_debug >= DEBUG_LIGHT)
                        cout << "LOC " << registers._list[two]
                             << " INSTR BEF " << before << " AFT "
                             << list[i]._locations[registers._list[two]];
                    location++;
                    break;
                }
            }

            if (_debug >= DEBUG_LIGHT)
            {
                registers.print();
                cout << "EXE " << count << endl;
            }
        }

        // print the number of executed instructions, as required
        cout << count << endl << endl;
    }
}


int main(int argc, char** argv)
{
    char opt;
    char* input_file = NULL;

    while ((opt = getopt(argc, argv, "i:d:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            input_file = optarg;
            break;
        case 'd':
            _debug = atoi(optarg);
            break;
        default:
            cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
        }
    }

    if (input_file == NULL || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0] << " -i input_file [-d 0|1|2]" << endl;
    else
    {
        vector<RAM> list;

        if (input(input_file, list) == true)
            output(list);
    }
}
