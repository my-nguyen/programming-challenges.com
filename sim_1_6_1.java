import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_1_6_1 {
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("u", true, "Upper bound");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);


        int num_cases = 0;
        int upper_bound = 0;
        String output_file = null;
        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("u"))
            upper_bound = Integer.parseInt(cmd.getOptionValue("u"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        PrintWriter writer = new PrintWriter(new File(output_file));
        Random random = new Random();
        for (int k = 0; k < num_cases; k++)
        {
            int lower = random.nextInt(upper_bound-1) + 1;
            int upper = random.nextInt(upper_bound-lower) + lower + 1;
            writer.write(lower + " " + upper + "\n");
            System.out.print(lower + " " + upper + "\n");
        }

        writer.close();
    }
}
