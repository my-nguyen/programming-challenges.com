import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;
import org.apache.commons.cli.*;


class sol_2_6_1 {
    private static boolean is_jolly(Input input) {
        // a sequence having only one integer is a jolly jumper
        if (input._list.size() == 1)
            return true;
        else {
            // create an array whose entries (between 1 and n-1) signify
            // whether such a value has been represented in the input sequence
            Difference diff = new Difference(input._list.size());


            // for each absolute difference, mark it in the visited array
            for (int i = 0; i < input._list.size()-1; i++) {
               int abs_diff = Math.abs(input._list.get(i)-input._list.get(i+1));
                if (Debug._level >= Debug.COPIOUS)
                    System.out.print("Absolute difference between "
                                     + input._list.get(i) + " and "
                                     + input._list.get(i+1) + " is " + abs_diff
                                     + "\n");

                if (abs_diff <= input._list.size()) {
                    diff._list.set(abs_diff, true);
                    if (Debug._level >= Debug.COPIOUS)
                        System.out.print("Marking entry " + abs_diff
                                         + " as represented\n");
                }
                else
                    if (Debug._level >= Debug.COPIOUS)
                        System.out.print("Entry " + abs_diff
                                         + " out of bound\n");
            }

            if (Debug._level >= Debug.COPIOUS)
                diff.print();
            return diff.scan();
        }
    }


    private static void input(String filename, List<Input> list) throws FileNotFoundException {
        // read the number of test cases
        Scanner scanner = new Scanner(new File(filename));
        for (int i = 0; scanner.hasNextInt(); i++) {
            // read the first number, which is the size of how many more
            // integers to follow
            int size = scanner.nextInt();


            // read the sequence of integers that follow the size
            Input current = new Input();
            for (int j = 0; j < size; j++)
                current._list.add(scanner.nextInt());
            list.add(current);


            if (Debug._level >= Debug.LIGHT) {
                for (int j = 0; j < size; j++)
                    System.out.print(" " + current._list.get(j));
                System.out.print("\n");
            }
        }

        scanner.close();
    }


    private static void output(List<Input> list) {
        for (int i = 0; i < list.size(); i++) {
            if (Debug._level >= Debug.LIGHT)
                list.get(i).print();

            if (is_jolly(list.get(i)))
                System.out.print("Jolly\n");
            else
                System.out.print("Not jolly\n");
        }
    }


    public static void main(String[] args) throws FileNotFoundException, ParseException {
        Parser parser = new Parser(args);
        List<Input> list = new ArrayList<Input>();
        input(parser._filename, list);
        output(list);
    }
}


class Input {
    List<Integer> _list = new ArrayList<Integer>();


    void print() {
        System.out.print(_list.size());
        for (int i = 0; i < _list.size(); i++)
            System.out.print(" " + _list.get(i));
        System.out.print("\n");
    }
}


class Difference {
    List<Boolean> _list = new ArrayList<Boolean>();


    Difference(int size) {
        for (int i = 0; i < size; i++)
            _list.add(false);
    }


    void print() {
        System.out.print("Difference entries:\n");
        for (int i = 1; i < _list.size(); i++)
            System.out.print(_list.get(i) + " ");
        System.out.print("\n");
    }


    // scan the visited array to see if any number was not represented
    boolean scan() {
        for (int i = 1; i < _list.size(); i++)
            if (_list.get(i) == false) {
                if (Debug._level >= Debug.COPIOUS)
                    System.out.print("Entry " + i + " not represented\n");
                return false;
            }

        return true;
    }
}
