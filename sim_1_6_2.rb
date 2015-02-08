# program that generates a random number of pairs of cases, each field is a
# rantangle with a random number of lines and a random number of columns.
# Within each cell of each such field is a character denoting a bomb "*" or
# not ".", to be used by the programs in 1.6.2.sol.*
require 'optparse'

def parse(args)
  options = {}
  parser = OptionParser.new do |opts|
    options[:cases] = 1
    opts.on("-c", "--number-of CASES", "Number of Test Cases") do |opt|
      options[:cases] = opt.to_i
    end

    options[:rows] = 10
    opts.on("-r", "--rows ROWS", "Number of Rows") do |opt|
      options[:rows] = opt.to_i
    end

    options[:columns] = 10
    opts.on("-l", "--columns COLUMNS", "Number of Columns") do |opt|
      options[:columns] = opt.to_i
    end

    # empty-to-mine ratio
    options[:ratio] = 5
    opts.on("-a", "--ratio RATIO", "Ratio of Empty-to-Mine Cell") do |opt|
      options[:ratio] = opt.to_i
    end
  end
  parser.parse!(args)

  return options
end

def generate(options)
  options[:cases].times do
    rows = rand(1..options[:rows])
    columns = rand(1..options[:columns])
    puts("#{rows} #{columns}")
    rows.times do
      columns.times do
        print((rand(100) % options[:ratio] == 0) ? "*" : ".")
      end
      puts
    end
  end
  puts("0 0")
end

generate(parse(ARGV))
