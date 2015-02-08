class RAM {
    // Considerations for implementing the field _locations. Facts: the field
    // represents a _locations address of 1000 instruction words, each word
    // comprising 3 digits. Even if the instruction words given in input don't
    // fill all 1000 address locations, the unused portion must still be
    // available and initialized to "000", because the instruction 9xx will
    // attempt to override a word instruction at address so-and-so with some
    // value that is beyond the number of words from input.
    // (1) a vector of char[] was rejected because a vector element must be
    //     an object type to allow copying and assignment, whereas a char[] 
    //     is not an object
    // (2) a vector of string was rejected because the field is a fixed-size
    //     list, as opposed to the dynamical allocation nature of a vector
    // (3) an array of char[] was rejected because of the inconvenience in
    //     converting to and from char[] and String
    // (4) an array of string[] was chosen because of the convenience afforded
    //     by the String class in assigning, format printing, etc.

    public String[] _locations = new String[1000];
    public int  _count;


    public void reset() {
        for (int i = 0; i < 1000; i++)
            _locations[i] = "000";
        _count = 0;
    }
}
