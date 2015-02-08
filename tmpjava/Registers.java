class Registers implements Cloneable {
    public int[] _list = new int[10];


    public void clear() {
        for (int i = 0; i < 10; i++)
            _list[i] = 0;
    }


    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append("REGS ");
        for (int i = 0; i < 10; i++)
            builder.append(_list[i] + " ");
        return builder.toString();
    }


    // must accompany equals(), per Java specification. I just copied from
    // examples of implementation on the net, without understanding why
    public int hashCode() {
        final int multiplier = 31;
        int hash = 7;
        for (int i = 0; i < 10; i++)
            hash = hash * multiplier + _list[i];
        return hash;
    }

    // Compares the array of integers this to the array other. Returns whether
    // this list of registers is equal to that of another list
    // this method is the equivalence of C++ operator==()
    public boolean equals(Object other) {
        if (Debug._level >= Debug.LIGHT)
        {
            System.out.print("\nTHIS " + this);
            System.out.println( "OTHER " + this);
        }

        if (other == null)
            return false;
        if (other == this)
            return true;
        if (other.getClass() != getClass())
            return false;

        Registers that = (Registers)other;
        boolean result = true;
        for (int i = 0; i < 10; i++)
        {
            if (_list[i] != that._list[i])
            {
                result = false;
                break;
            }
        }
        return result;
    }


    // Copies to array of integers pointed by source into the array pointed by
    // destination. destination is returned
    // this method is the equivalence of C++ assignment operator
    public Object clone() {
        Registers reg = new Registers();
        reg._list = (int[])_list.clone();
        if (Debug._level >= Debug.LIGHT)
            System.out.print("CLONE " + reg);
        return reg;
    }


    // Whether within the next 10 times the current value will become zero */
    public static boolean approaching_zero(int previous, int current) {
        int diff = current - previous;
        int tmp = current;
        for (int i = 0; i < 10 && current != 0; i++)
            current += diff;
        if (Debug._level >= Debug.LIGHT)
            System.out.println("previous: " + previous + ", current: " + tmp +
                               ", approaching zero: " + (current == 0));
        return current == 0;
    }
}
