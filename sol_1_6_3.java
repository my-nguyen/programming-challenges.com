import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;


class sol_1_6_3 {
   private static void input(ArrayList<Trip> list) throws FileNotFoundException {
      Scanner scanner = new Scanner(System.in);
      while (true) {
         // read in the number of students on this trip
         int count = scanner.nextInt();

         // number of students equal zero means end of input
         if (count == 0)
            break;
         else
         {
            // read in the expense of each student in this trip
            Trip trip = new Trip();
            for (int i = 0; i < count; i++)
            {
               float expense = scanner.nextFloat();
               trip._expenses.add(expense);
            }

            list.add(trip);
         }
      }
      scanner.close();
   }


   // sort all the expenses, to facilitate calculating the average and the
   // exchange amount per student
   private static void output(ArrayList<Trip> list) {
      for (int i = 0; i < list.size(); i++) {
         list.get(i).sort();
      }


      // print output in the format required
      for (int i = 0; i < list.size(); i++)
         System.out.printf("$%.2f\n", list.get(i).exchange());
   }


   public static void main(String[] args) throws FileNotFoundException {
      ArrayList<Trip> list = new ArrayList<Trip>();
      input(list);
      output(list);
   }
}


class Trip {
   public ArrayList<Float> _expenses = new ArrayList<Float>();


   public void sort() {
      Collections.sort(_expenses);
   }


   public String toString() {
      StringBuilder builder = new StringBuilder();
      for (int i = 0; i < _expenses.size(); i++) {
         builder.append(_expenses.get(i));
         builder.append(" ");
      }
      builder.append("\n");

      return builder.toString();
   }


   private float average() {
      float total = 0.0f;
      for (int i = 0; i < _expenses.size(); i++)
         total += _expenses.get(i);

      return total / _expenses.size();
   }


   public float exchange() {
      float total = 0.0f;
      float avg = average();
      for (int i = 0; i < _expenses.size() && _expenses.get(i) < avg; i++)
         total += avg - _expenses.get(i);
      return total;
   }
}
