import org.apache.commons.cli.*;


class Parser {
    String _filename;

    Parser(String[] args) throws ParseException {
        Options options = new Options();
        options.addOption("i", true, "Input filename");
        options.addOption("d", true, "Debug level");

        CommandLineParser parser = new PosixParser();
        CommandLine cmd = parser.parse(options, args);

        if (cmd.hasOption("i"))
            _filename = cmd.getOptionValue("i");
        if (cmd.hasOption("d"))
            Debug._level = Integer.parseInt(cmd.getOptionValue("d"));
    }
}
