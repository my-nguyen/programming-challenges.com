import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_1_6_2 {
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        int RATIO_SAFE_TO_MINE = 4;


        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("r", true, "Maximum rows");
        options.addOption("l", true, "Maximum columns");
        options.addOption("a", true, "Ratio of safe to mined squares");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);


        int num_cases = 0;
        int max_rows = 0;
        int max_columns = 0;
        int ratio = RATIO_SAFE_TO_MINE;
        String output_file = null;


        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("r"))
            max_rows = Integer.parseInt(cmd.getOptionValue("r"));
        if (cmd.hasOption("l"))
            max_columns = Integer.parseInt(cmd.getOptionValue("l"));
        if (cmd.hasOption("a"))
            ratio = Integer.parseInt(cmd.getOptionValue("a"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        PrintWriter writer = new PrintWriter(new File(output_file));
        Random random = new Random();
        for (int k = 0; k < num_cases; k++)
        {
            int num_rows = random.nextInt(max_rows) + 1;
            int num_columns = random.nextInt(max_columns) + 1;
            writer.write(num_rows + " " + num_columns + "\n");
            System.out.print(num_rows + " " + num_columns + "\n");

            for (int i = 0; i < num_rows; i++)
            {
                for (int j = 0; j < num_columns; j++)
                {
                    char cell = (random.nextInt(ratio) == 0) ? '*' : '.';
                    writer.write(cell);
                    System.out.print(cell);
                }

                writer.write("\n");
                System.out.print("\n");
            }
        }
        writer.write("0 0\n");
        System.out.print("0 0\n");


        writer.close();
    }
}
