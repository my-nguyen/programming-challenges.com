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

    options[:students] = 10
    opts.on("-s", "--students STUDENTS", "Number of Students") do |opt|
      options[:students] = opt.to_i
    end

    options[:expenses] = 30.0
    opts.on("-x", "--expenses EXPENSES", "Maximum Expenses per Student") do |opt|
      options[:expenses] = opt.to_f
    end
  end
  parser.parse!(args)

  return options
end

def generate(options)
  options[:cases].times do
    students = rand(1..options[:students])
    puts("#{students}")

    students.times do
      puts("%.2f" % (rand * options[:expenses]))
    end
  end
  puts("0")
end

generate(parse(ARGV))
