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

    options[:size] = 10
    opts.on("-s", "--size SIZE", "Display Size") do |opt|
      options[:size] = opt.to_i
    end

    options[:number] = 1234567890
    opts.on("-n", "--number NUMBER", "Number to Display") do |opt|
      options[:number] = opt.to_i
    end
  end
  parser.parse!(args)

  return options
end

def generate(options)
  options[:cases].times do
    puts("#{rand(1..options[:size])} #{rand(1..options[:number])}")
  end
  puts("0 0")
end

generate(parse(ARGV))
