import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import org.apache.commons.cli.*;


class sim_1_6_8 {
    static Random rand = new Random();

    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("d", true, "Maximum candidates");
        options.addOption("b", true, "Maximum ballots");
        options.addOption("o", true, "Output filename");


        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);
        int num_cases = 0;
        int max_candidates = 0;
        int max_ballots = 0;
        String output_file = null;
        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("d"))
            max_candidates = Integer.parseInt(cmd.getOptionValue("d"));
        if (cmd.hasOption("b"))
            max_ballots = Integer.parseInt(cmd.getOptionValue("b"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");


        // print the number of cases
        PrintWriter writer = new PrintWriter(new File(output_file));
        String tmp = String.format("%d\n", num_cases);
        writer.write(tmp);
        System.out.print(tmp);


        String[] names =
        {
            "Zelda Roberto",
            "Pearle Bouffard",
            "Mahalia Pelkey",
            "Theodore Montijo",
            "Manual Hiebert",
            "Manuela Hoffmeister",
            "Loris Freund",
            "Annabelle Lasher",
            "Angele Mclemore",
            "Charlotte Dimas",
            "Graig Mancuso",
            "Frida Gatto",
            "Tamela Mcglone",
            "Latisha Freese",
            "Farrah Hemingway",
            "Hoa Messina",
            "Gussie Summa",
            "Alisia Vangorder",
            "Robyn Harbison",
            "Janelle Cusson"
        };

        for (int i = 0; i < num_cases; i++)
        {
            // print a blank line
            tmp = String.format("\n");
            writer.write(tmp);
            System.out.print(tmp);


            // print the number of candidates
            int num_candidates = rand.nextInt(max_candidates)+1;
            tmp = String.format("%d\n", num_candidates);
            writer.write(tmp);
            System.out.print(tmp);


            // print all the candidates' names, each on a line
            for (int j = 0; j < num_candidates; j++) {
                tmp = String.format("%s\n", names[j]);
                writer.write(tmp);
                System.out.print(tmp);
            }


            // print the ballots, each on a line
            BallotList ballots = new BallotList(num_candidates);
            int num_ballots = rand.nextInt(max_ballots) + 1;
            for (int j = 0; j < num_ballots; j++) {
                ballots.shuffle();
                ballots.print(writer);
            }
        }

        writer.close();
    }


    static class BallotList {
        final int MAX_BALLOTS = 400;
        List<Integer> _list;


        BallotList(int size) {
            _list = new ArrayList<Integer>();
            for (int i = 0; i < size; i++)
                _list.add(i+1);
        }


        void shuffle() {
/*
            // old way to shuffle
            if (_list.size() > 1) {
                for (int i = 0; i < _list.size()-1; i++) {
                    int j = rand.nextInt(_list.size());
                    int tmp = _list.get(j);
                    _list.set(j, _list.get(i));
                    _list.set(i, tmp);
                }
            }
*/
            // new way to shuffle
            Collections.shuffle(_list);
        }


        void print(PrintWriter writer) {
            for (int i = 0; i < _list.size(); i++) {
                String tmp = String.format("%d ", _list.get(i));
                writer.write(tmp);
                System.out.print(tmp);
            }
            writer.write("\n");
            System.out.print("\n");
        }
    }
}
