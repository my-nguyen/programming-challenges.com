import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_1_6_5 {
    static PrintWriter writer;
    static Random random;


    interface Functor {
        public void execute(int columns, int rows);
    }


    static class FClear implements Functor {
        public void execute(int columns, int rows) {
            writer.write("C\n");
            System.out.print("C\n");
        }
    }


    static class FColor implements Functor {
        public void execute(int columns, int rows) {
            String line = String.format("L %d %d %c\n",
                                        random.nextInt(columns)+1,
                                        random.nextInt(rows)+1,
                                        random.nextInt(26)+'A');
            writer.write(line);
            System.out.print(line);
        }
    }


    static class Range {
        int _low;
        int _high;

        Range(int max) {
            _low = random.nextInt(max-1) + 1;
            _high = random.nextInt(max-_low) + _low+1;
        }
    }


    static class FVertical implements Functor {
        public void execute(int columns, int rows) {
            Range range = new Range(rows);
            String line = String.format("V %d %d %d %c\n",
                                        random.nextInt(columns)+1,
                                        range._low, range._high,
                                        random.nextInt(26)+'A');
            writer.write(line);
            System.out.print(line);
        }
    }


    static class FHorizontal implements Functor {
        public void execute(int columns, int rows) {
            Range range = new Range(columns);
            String line = String.format("H %d %d %d %c\n",
                                        range._low, range._high,
                                        random.nextInt(rows)+1,
                                        random.nextInt(26)+'A');
            writer.write(line);
            System.out.print(line);
        }
    }


    static class FRectangle implements Functor {
        public void execute(int columns, int rows) {
            Range range1 = new Range(columns);
            Range range2 = new Range(rows);
            String line = String.format("K %d %d %d %d %c\n",
                                        range1._low, range2._low,
                                        range1._high, range2._high,
                                        random.nextInt(26)+'A');
            writer.write(line);
            System.out.print(line);
        }
    }


    static class FRegion implements Functor {
        public void execute(int columns, int rows) {
            String line = String.format("F %d %d %c\n",
                                        random.nextInt(columns)+1,
                                        random.nextInt(rows)+1,
                                        random.nextInt(26)+'A');
            writer.write(line);
            System.out.print(line);
        }
    }


    static class FSave implements Functor {
        public void execute(int columns, int rows) {
            String line = String.format("S %s\n", FILE_SAVE);
            writer.write(line);
            System.out.print(line);
        }
    }


    private static final int MAX_COMMANDS = 7;
    static void generate(int num_cases, int columns, int rows) {
        Functor[] functors = null;

        if (functors == null)
        {
            functors = new Functor[MAX_COMMANDS];

            functors[0] = new FClear();
            functors[1] = new FColor();
            functors[2] = new FVertical();
            functors[3] = new FHorizontal();
            functors[4] = new FRectangle();
            functors[5] = new FRegion();
            functors[6] = new FSave();
        }

        for (int i = 0; i < num_cases; i++)
        {
            int index = random.nextInt(MAX_COMMANDS);
            functors[index].execute(columns, rows);
        }
    }


    private static final String FILE_SAVE = "tmp_1_6_5";
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("r", true, "Maximum rows");
        options.addOption("l", true, "Maximum columns");
        options.addOption("d", true, "Maximum commands");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);


        int num_cases = 0;
        int max_rows = 0;
        int max_columns = 0;
        int max_commands = 0;
        String output_file = null;


        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("r"))
            max_rows = Integer.parseInt(cmd.getOptionValue("r"));
        if (cmd.hasOption("l"))
            max_columns = Integer.parseInt(cmd.getOptionValue("l"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        // begin the session
        writer = new PrintWriter(new File(output_file));
        random = new Random();
        int rows = random.nextInt(max_rows)+2;
        int columns = random.nextInt(max_columns)+2;
        String line = String.format("I %d %d\n", columns, rows);
        writer.write(line);
        System.out.print(line);


        // generate a bunch of commands
        generate(num_cases, columns, rows);


        // make at least one save
        line = String.format("S %s\n", FILE_SAVE);
        writer.write(line);
        System.out.print(line);


        // terminate the session
        writer.write("X\n");
        System.out.print("X\n");


        writer.close();
    }
}
