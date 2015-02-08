=begin
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


def input()
  File.open(filename.to_s) do |file|
    cases = file.gets.to_i
    # list is an array of records aka test cases
    list = Array.new
    # record is an array of strings, each string containing 3 digits
    record = Array.new
    while (line = file.gets) != nil
      if line.chomp == ""
        if !list.empty?
          list << record
        end
        record = []
      else
        record << line
      end
    end
    list << record
    file.close
  end
  return list
end

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
=end

def output(list)
  registers = []
  location = 0
  list.each do |record|
    zero = record[location][0].to_i
    one = record[location][1].to_i
    two = record[location][2].to_i
    case zero
    # if register at two does not contain 0, go to the location in register at one
    when 0
      if (registers[two] != 0)
        location = registers[one]
      end
    when 1
      # 100 means halt
      if (one == 0 && two == 0)
        halt = true
      end
    when 2
      # set register at one to two
      registers[one] = two
      location += 1
    when 3
      # add two to register at one
      registers[one] = (registers[one] + two) % 1000
      location += 1
    when 4
      # multiply register at one by two
      registers[one] = (registers[one] * two) % 1000
      location += 1
    when 5
      # set register at one to the value of register at two
      registers[one] = registers[two]
      location += 1
    when 6
      # add the value of register at two to register at one
      registers[one] = (registers[one] + registers[two]) % 1000
      location += 1
    when 7
      # multiply register at one by the value of register at two
      registers[one] = (registers[one] * registers[two]) % 1000
      location += 1
    when 8
      # set register at one to the value in RAM whose address is in register at two
      registers[one] = list[i][registers._list[two]].to_i
      location += 1
    when 9
      # set the value in RAM whose address is in register at two to that of register at one
                    char before[4];
                    strcpy(before, list[i]._locations[registers._list[two]]);
                    char after[4];
                    sprintf(after, "%03d", registers._list[one]);
                    strcpy(list[i]._locations[registers._list[two]], after);
                    location += 1

=begin
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
=end
