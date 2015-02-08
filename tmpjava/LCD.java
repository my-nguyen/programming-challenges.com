class LCD {
    private int    _base;
    private String _digits;


    public void base(int base) {
        _base = base;
    }


    public void digits(int number) {
        _digits = Integer.toString(number);
    }


    public String toString() {
        Printer printer = new Printer();

        if (Debug._level >= Debug.COPIOUS) {
            System.out.println("base: " + _base + ", digits: " + _digits +
                               ", digits length: " + _digits.length());
            System.out.println("LCD::toString() _builder: " +
                               printer._builder.hashCode());
        }

        for (int row = 0; row < 2*_base+3; row++) {
            for (int i = 0; i < _digits.length(); i++) {
                String tmp = printer._builder.toString();
                if (i != 0)
                    printer._builder.append(" ");
                for (int column = 0; column < _base+2; column++) {
                    int digit = _digits.charAt(i) - '0';
                    if (Debug._level >= Debug.COPIOUS)
                        System.out.println("Functor[" + digit + "](" + _base +
                                           ", " + row + ", " + column + ")");
                    printer._functors[digit].execute(_base, row, column);
                }

                if (Debug._level >= Debug.COPIOUS) {
                    System.out.println("New Functor _builder: " +
                                       printer._builder.hashCode());
                    System.out.println("BEFORE(" + tmp.length() + ")\n" + tmp);
                    System.out.println("AFTER:(" + printer._builder.length() +
                                       ")\n" + printer._builder);
                }
            }
            printer._builder.append("\n");
        }
        printer._builder.append("\n");

        return printer._builder.toString();
    }
}
