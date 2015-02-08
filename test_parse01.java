import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


class tparse01 {
    public static void main(String[] args) throws FileNotFoundException {
        Scanner scanner = new Scanner(new File("1.6.5.in"));
        while (scanner.hasNext()) {
            String line = scanner.nextLine();
            System.out.println(line);
            String[] tokens = line.split(" ");
            for (int i = 0; i < tokens.length; i++)
                System.out.println(tokens[i] + " ");
        }
    }
}
