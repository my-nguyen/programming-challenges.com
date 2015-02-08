# ruby sol_1_6_3.rb in_1_6_3 | tr -d '\r' | diff out_1_6_3 -
class Trip
  attr_accessor :expenses

  def initialize
    @expenses = []
  end

  # Calculate the average of all expenses
  def average
    total = @expenses.inject do |sum, expense|
      sum += expense
    end
    return total / @expenses.size
  end

  def exchange
    total = 0.0
    avg = average
    @expenses.each do |expense|
      if expense < avg
        total += avg - expense
      else
        break
      end
    end

    # return a string, formatted Ruby style
    return "%.2f" % total
  end

  # Convert all float expenses into string format, for use by puts
  def to_s
    string = ""
    @expenses.each do |expense|
      string << "#{expense} "
    end
    string << "\n"
  end
end

def input(filename)
  trips = []
  File.open(filename.to_s) do |file|
    while true
      # Read the count of exchanges from file
      count = file.gets.to_i

      # 0 means end of input
      if count == 0
        file.close();
        break
      else
        # read in the students' expense
        trip = Trip.new
        count.times do
          trip.expenses << file.gets.to_f
        end

        # sort all the expenses, to facilitate calculating the average
        # and the exchange amount per student
        trip.expenses.sort!
        trips << trip
      end
    end
  end
  return trips
end

# print output in the format required
def output(trips)
  trips.each do |trip|
    puts("$#{trip.exchange}")
  end
end

if ARGV.empty?
  puts("Usage: Must pass input file as command-line argument")
else
  output(input(ARGV[0]))
end
