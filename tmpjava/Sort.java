class Sort {
    public int _low;
    public int _high;

    public Sort(int one, int two) {
        if (one > two)
        {
            _low = two;
            _high = one;
        }
        else
        {
            _low = one;
            _high = two;
        }
    }
}
