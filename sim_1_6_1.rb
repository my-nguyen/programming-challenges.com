# program that generates a random number of pairs of lower bound and upper
# bound, each bound is also a random number up to a maximum value, to be used
# by the programs in 1.6.1.sol.*
require 'optparse'

def parse(args)
  options = {}
  parser = OptionParser.new do |opts|
    opts.banner = "Usage: sim_1_6_1.rb -c CASES -u UPPER"

    options[:cases] = 1
    opts.on("-c", "--number-of CASES", "Number of Test Cases") do |opt|
      options[:cases] = opt.to_i
    end

    options[:upper] = 10
    opts.on("-u", "--upper-bound UPPER", "Upper Bound") do |opt|
      options[:upper] = opt.to_i
    end
  end
  parser.parse!(args)

  return options
end

def generate(options)
  options[:cases].times do
    lower = rand(1..options[:upper]/2)
    upper = rand(lower..options[:upper])
    puts("#{lower} #{upper}")
  end
end

generate(parse(ARGV))
