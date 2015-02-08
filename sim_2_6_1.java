import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_2_6_1 {
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("s", true, "Maximum number of entries in a sequence");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);


        int num_cases = 0;
        int max_sequence = 0;
        String output_file = null;
        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("s"))
            max_sequence = Integer.parseInt(cmd.getOptionValue("s"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        PrintWriter writer = new PrintWriter(new File(output_file));
        Random random = new Random();
        for (int k = 0; k < num_cases; k++)
        {
            // print the sequence count
            int num_sequence = random.nextInt(max_sequence) + 1;
            String line = String.format("%d", num_sequence);
            writer.write(line);
            System.out.print(line);


            // print all numbers in the sequence
            int j;
            for (j = 0; j < num_sequence; j++)
            {
                line = String.format(" %d", random.nextInt(num_sequence)+1);
                writer.write(line);
                System.out.print(line);
            }


            line = "\n";
            writer.write(line);
            System.out.print(line);
        }

        writer.close();
    }
}
