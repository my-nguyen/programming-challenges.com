import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Random;
import java.util.Arrays;
import org.apache.commons.cli.*;


class sim_1_6_7 {
    private static final int NUM_ROWS = 8;
    private static final int NUM_COLUMNS = 8;

    private static final int MAX_PAWNS = 8;
    private static final int MAX_KNIGHTS = 2;
    private static final int MAX_BISHOPS = 2;
    private static final int MAX_ROOKS = 2;
    private static final int MAX_QUEENS = 1;


    private static char[][] _boards = new char[NUM_ROWS][NUM_COLUMNS];
    private static Random   _random = new Random();

    private static void assign_piece(int num_pieces, char piece) {
        for (int j = 0; j < num_pieces; j++)
        {
            int row = 0;
            int column = 0;
            do
            {
                if (piece == 'p')
                    row = _random.nextInt(NUM_ROWS-1) + 1;
                else if (piece == 'P')
                    row = _random.nextInt(NUM_ROWS-1);
                else
                    row = _random.nextInt(NUM_ROWS);
                column = _random.nextInt(NUM_COLUMNS);
            } while (_boards[row][column] != '.');

            _boards[row][column] = piece;
        }
    }


    public static void main(String[] args) throws ParseException, FileNotFoundException {
        Options options = new Options();
        options.addOption("c", true, "Number of cases");
        options.addOption("o", true, "Output filename");

        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);

        int num_cases = 0;
        String output_file = null;
        if (cmd.hasOption("c"))
            num_cases = Integer.parseInt(cmd.getOptionValue("c"));
        if (cmd.hasOption("o"))
            output_file = cmd.getOptionValue("o");

        PrintWriter writer = new PrintWriter(new File(output_file));
        for (int k = 0; k < num_cases; k++)
        {
            for (int i = 0; i < NUM_ROWS; i++)
                _boards[i] = Arrays.copyOf(("........").toCharArray(),
                                           NUM_COLUMNS);

            char[][] pieces = {
                { 'p', 'n', 'b', 'r', 'q', 'k' },
                { 'P', 'N', 'B', 'R', 'Q', 'K' }
            };

            for (int i = 0; i < 2; i++)
            {
                assign_piece(_random.nextInt(MAX_PAWNS+1), pieces[i][0]);
                assign_piece(_random.nextInt(MAX_KNIGHTS+1), pieces[i][1]);
                assign_piece(_random.nextInt(MAX_BISHOPS+1), pieces[i][2]);
                assign_piece(_random.nextInt(MAX_ROOKS+1), pieces[i][3]);
                assign_piece(_random.nextInt(MAX_QUEENS+1), pieces[i][4]);
                assign_piece(1, pieces[i][5]);
            }

            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLUMNS; j++)
                {
                    writer.write(_boards[i][j]);
                    System.out.print(_boards[i][j]);
                }
                writer.write("\n");
                System.out.print("\n");
            }
            writer.write("\n");
            System.out.print("\n");
        }

        for (int i = 0; i < NUM_ROWS; i++)
        {
            writer.write("........\n");
            System.out.print("........\n");
        }
        writer.close();
    }
}
