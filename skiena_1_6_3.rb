class Expenses
  def initialize
    @data = []
  end

  def add(expense)
    @data << expense
  end

  def sort
    @data.sort!
  end

  def exchange
    total = 0.0
    avg = average
    @data.each do |expense|
      if expense < avg
        total += avg - expense
      else
        break
      end
    end

    # return a string, formatted Ruby style
    return "%.2f" % total
  end

  def average
    total = @data.inject(:+)
    total / @data.size
  end

  # Convert all float expenses into string format, for use by puts
  def to_s
    string = ""
    @data.each do |expense|
      string << "#{expense} "
    end
    string << "\n"
  end
end

def input
  list = Array.new
  while (true)
    # read in the number of students on this Expenses
    count = gets.to_i

    # end of input
    if (count == 0)
      break
    else
      # read in the expense of each student in this Expenses
      expenses = Expenses.new
      count.times do
        expenses.add(gets.to_f)
      end

      expenses.sort
      list << expenses
    end
  end
  list
end

def output(list)
  list.each do |expenses|
    puts("$#{expenses.exchange}")
  end
end

output(input())
