class Printer {
    public Functor[] _functors;
    public StringBuilder _builder = new StringBuilder();


    public Printer() {
        if (_functors == null)
        {
            _functors = new Functor[10];

            _functors[0] = new Functor0();
            _functors[1] = new Functor1();
            _functors[2] = new Functor2();
            _functors[3] = new Functor3();
            _functors[4] = new Functor4();
            _functors[5] = new Functor5();
            _functors[6] = new Functor6();
            _functors[7] = new Functor7();
            _functors[8] = new Functor8();
            _functors[9] = new Functor9();
        }

        if (Debug._level >= Debug.COPIOUS)
            System.out.println("Printer() _builder: " + _builder.hashCode());
    }


    public interface Functor {
        public void execute(int base, int row, int column);
    }


    class Functor0 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else if (row == base+1)
                _builder.append(" ");
            else
            {
                if (column == 0 || column == base+1)
                    _builder.append("|");
                else
                    _builder.append(" ");
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor1 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
                _builder.append(" ");
            else
            {
                if (column == base+1)
                    _builder.append("|");
                else
                    _builder.append(" ");
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor2 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (row <= base)
                {
                    if (column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
                else
                {
                    if (column == 0)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor3 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (column == base+1)
                    _builder.append("|");
                else
                    _builder.append(" ");
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor4 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == 2*base+2)
                _builder.append(" ");
            else if (row == base+1)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (row <= base)
                {
                    if (column == 0 || column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
                else
                {
                    if (column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor5 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (row <= base)
                {
                    if (column == 0)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
                else
                {
                    if (column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor6 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (row <= base)
                {
                    if (column == 0)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
                else
                {
                    if (column == 0 || column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor7 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else if (row == base+1 || row == 2*base+2)
                _builder.append(" ");
            else
            {
                if (column == base+1)
                    _builder.append("|");
                else
                    _builder.append(" ");
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor8 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (column == 0 || column == base+1)
                    _builder.append("|");
                else
                    _builder.append(" ");
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }


    class Functor9 implements Functor {
        public void execute(int base, int row, int column) {
            String tmp = _builder.toString();
            if (row == 0 || row == base+1 || row == 2*base+2)
            {
                if (column == 0 || column == base+1)
                    _builder.append(" ");
                else
                    _builder.append("-");
            }
            else
            {
                if (row <= base)
                {
                    if (column == 0 || column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
                else
                {
                    if (column == base+1)
                        _builder.append("|");
                    else
                        _builder.append(" ");
                }
            }
            if (Debug._level >= Debug.COPIOUS) {
                System.out.println("BEFORE:\n" + tmp);
                System.out.println("AFTER:\n" + _builder.toString());
            }
        }
    }
}
