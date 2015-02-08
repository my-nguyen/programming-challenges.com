import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_1_6_4 {
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("b", true, "Maximum base");
        options.addOption("d", true, "Maximum digits");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);


        int num_cases = 0;
        int max_base = 0;
        int max_digits = 0;
        String output_file = null;


        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("b"))
            max_base = Integer.parseInt(cmd.getOptionValue("b"));
        if (cmd.hasOption("d"))
            max_digits = Integer.parseInt(cmd.getOptionValue("d"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        PrintWriter writer = new PrintWriter(new File(output_file));
        Random random = new Random();
        for (int k = 0; k < num_cases; k++)
        {
            int base = random.nextInt(max_base) + 1;
            int digits = random.nextInt(max_digits) + 1;
            writer.write(base + " " + digits + "\n");
            System.out.print(base + " " + digits + "\n");
        }
        writer.write("0 0\n");
        System.out.print("0 0\n");


        writer.close();
    }
}
