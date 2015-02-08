import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;


class sol_1_6_4 {
   private static void input(ArrayList<LCD> list) throws FileNotFoundException {
      Scanner scanner = new Scanner(System.in);
      while (true) {
         // read the 2 integers from file into one array element until end
         // of input
         int base = scanner.nextInt();
         int digits = scanner.nextInt();

         // both integers being zero means the end of input
         if (base == 0 || digits == 0)
            break;
         else
         {
            LCD tmp = new LCD();
            tmp.base(base);
            tmp.digits(digits);

            list.add(tmp);
         }
      }
      scanner.close();
   }


   private static void output(ArrayList<LCD> list) {
      // process the actual entries in the array
      for (int i = 0; i < list.size(); i++)
         System.out.print(list.get(i));
   }


   public static void main(String[] args) throws FileNotFoundException {
      ArrayList<LCD> list = new ArrayList<LCD>();
      input(list);
      output(list);
   }
}


class LCD {
   private int   _base;
   private String _digits;


   public void base(int base) {
      _base = base;
   }


   public void digits(int number) {
      _digits = Integer.toString(number);
   }


   public String toString() {
      Printer printer = new Printer();

      for (int row = 0; row < 2*_base+3; row++) {
         for (int i = 0; i < _digits.length(); i++) {
            String tmp = printer._builder.toString();
            if (i != 0)
               printer._builder.append(" ");
            for (int column = 0; column < _base+2; column++) {
               int digit = _digits.charAt(i) - '0';
               printer._functors[digit].execute(_base, row, column);
            }
         }
         printer._builder.append("\n");
      }
      printer._builder.append("\n");

      return printer._builder.toString();
   }
}


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
      }
   }
}
