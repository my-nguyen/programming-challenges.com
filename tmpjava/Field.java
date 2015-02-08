import java.util.ArrayList;


class Field {
    // _input has 2 more lines and 2 more columns than the actual number of
    // lines and columns read from input file. The extra lines and columns will
    // be initialized to '+'. This serves to facilitate calculating the number
    // of mines in the surrounding cells of any particular cell. So, if the
    // field from input file is:
    //		*...
    //		....
    //		.*..
    //		....
    // Then the internal _input field looks something like this:
    //		++++++
    //		+*...+
    //		+....+
    //		+.*..+
    //		++++++
    ArrayList<char[]> _input;

    // _output contains the mine indicia as well as the calculated value of
    // each of the cell in the field. This doesn't need the special extra 2
    // lines and 2 columns, as opposed to _input
    ArrayList<char[]> _output;


    // create a new string having the desired length, and padd the contents
    // of such string with blank characters "+"
    private char[] blankString(int size) {
        char[] string = new char[size];
        for (int i = 0; i < size; i++)
            string[i] = '+';
        return string;
    }


    // copy data from a list of strings into an internal list of strings
    // while making sure to surround all borders of such internal list with
    // blank characters '+'
    public void input(ArrayList<String> input) {
    for (int i = 0; i < input.size(); i++)
        // create a new ArrayList
        _input = new ArrayList<char[]>();

        // add to the beginning of ArrayList a string containing only
        // blanks ("+")
        _input.add(blankString(input.get(0).length() + 2));

        for (int i = 0; i < input.size(); i++) {
            // create a local string with 2 extra spaces
            char[] string = new char[input.get(i).length() + 2];

            // padd the beginning of local string with blank character "+"
            string[0] = '+';

            // copy from argument string to local string
            for (int j = 0; j < input.get(i).length(); j++)
                string[j+1] = input.get(i).charAt(j);

            // padd the end of local string with blank character "+"
            string[input.get(i).length() + 1] = '+';

            // add local string to ArrayList
            _input.add(string);
        }

        // add to the end of ArrayList a string containing only blanks ("+")
        _input.add(blankString(input.get(0).length() + 2));
    }


    // print only the field contained within the surrounding cells
    // that contain '+'
    public void printInput() {
        if (Debug._level >= Debug.LIGHT) {
            System.out.println(_input.size()-2 + " " +
                              (_input.get(0).length-2));
            for (int i = 1; i < _input.size()-1; i++) {
                for (int j = 1; j < _input.get(i).length-1; j++)
                    System.out.print(_input.get(i)[j]);
                System.out.println();
            }
        }
    }


    // print everything in _input
    public void printRealInput() {
        if (Debug._level >= Debug.COPIOUS) {
            System.out.println(_input.size()-2 + " " + (_input.get(0).length-2));
            for (int i = 0; i < _input.size(); i++) {
                for (int j = 0; j < _input.get(i).length; j++)
                    System.out.print(_input.get(i)[j]);
                System.out.println();
            }
        }
    }


    // printing _output is straightforward, unlike printing _input
    public void printOutput() {
        for (int i = 0; i < _output.size(); i++) {
            for (int j = 0; j < _output.get(i).length; j++)
                System.out.print(_output.get(i)[j]);
            System.out.print("\n");
        }
    }


    // does this cell contain a mine?
    private int isMine(char value) {
        return (value == '*') ? 1 : 0;
    }


    public void sweep() {
        // delay creation of _output field until now
        _output = new ArrayList<char[]>();

        for (int i = 0; i < _input.size()-2; i++)
        {
            // must also add individual strings into ArrayList
            _output.add(new char[_input.get(i).length - 2]);

            for (int j = 0; j < _input.get(i).length-2; j++)
            {
                // if this input cell contains a mine, record the mine in the
                // output cell
                if (_input.get(i+1)[j+1] == '*')
                    _output.get(i)[j] = '*';

                // otherwise, calculate the number of mines in the surrounding 8
                // cells and record that number in the output cell
                else
                {
                    int total = isMine(_input.get(i)[j]) +
                                isMine(_input.get(i)[j+1]) +
                                isMine(_input.get(i)[j+2]) +
                                isMine(_input.get(i+1)[j]) +
                                isMine(_input.get(i+1)[j+2]) +
                                isMine(_input.get(i+2)[j]) +
                                isMine(_input.get(i+2)[j+1]) +
                                isMine(_input.get(i+2)[j+2]);
                    _output.get(i)[j] = (char)((int)'0' + total);
                }
            }
        }
    }
}
