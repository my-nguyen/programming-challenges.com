import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_1_6_6 {
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("i", true, "Maximum instructions");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);
        int num_cases = 0;
        int max_instructions = 0;
        String output_file = null;


        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("i"))
            max_instructions = Integer.parseInt(cmd.getOptionValue("i"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        PrintWriter writer = new PrintWriter(new File(output_file));
        Random random = new Random();


        // a single positive integer on a line by itself indicating the number
        // of cases
        writer.write(num_cases + "\n");
        System.out.print(num_cases + "\n");


        // Each input case consists of up to 1,000 three-digit unsigned
        // integers, representing the contents of consecutive RAM locations
        // starting at 0
        max_instructions = max_instructions % 1001 + 1;
        for (int k = 0; k < num_cases; k++)
        {
            // a blank line before each set of instructions
            writer.write("\n");
            System.out.print("\n");


            int instructions = random.nextInt(max_instructions) + 1;
            for (int j = 0; j < instructions; j++)
            {
                String line = String.format("%03d\n", random.nextInt(1000));
                writer.write(line);
                System.out.print(line);
            }


            // HALT instruction
            writer.write("100\n");
            System.out.print("100\n");
        }

        writer.close();
    }
}
