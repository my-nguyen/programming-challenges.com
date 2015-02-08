import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import org.apache.commons.cli.*;


class sim_1_6_3 {
    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("s", true, "Maximum students");
        options.addOption("e", true, "Maximum expense");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);


        int num_cases = 0;
        int max_students = 0;
        float max_expense = 0.0f;
        String output_file = null;


        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("s"))
            max_students = Integer.parseInt(cmd.getOptionValue("s"));
        if (cmd.hasOption("e"))
            max_expense = Float.parseFloat(cmd.getOptionValue("e"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        PrintWriter writer = new PrintWriter(new File(output_file));
        Random random = new Random();
        for (int k = 0; k < num_cases; k++)
        {
            int num_students = random.nextInt(max_students) + 1;
            writer.write(num_students + "\n");
            System.out.print(num_students + "\n");


            for (int j = 0; j < num_students; j++)
            {
                String tmp = String.format("%.2f\n",
                                           random.nextFloat() * max_expense);
                writer.write(tmp);
                System.out.print(tmp);
            }
        }
        writer.write("0\n");
        System.out.print("0\n");

        writer.close();
    }
}
