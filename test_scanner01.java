import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


class tscanner01 {
    public static void main(String[] args) throws FileNotFoundException {
        Scanner scanner = new Scanner(new File("1.6.5.in"));
        while (scanner.hasNext()) {
            String next = scanner.next();
            System.out.println(next);
        }
    }
}
