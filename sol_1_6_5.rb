class Image
  def initialize
    @pixels = []
  end

  # Create a new M x N image with all pixels initially colored white (.)
  def create(columns, rows)
    padding = "".rjust(columns, 'O')
    @pixels = []
    rows.times do
      @pixels << padding
    end
  end

  # Clear the table by setting all pixels white (.). The size remains unchanged.
  def clear
    padding = "".rjust(@pixels[0].size, 'O')
    @pixels.size do
      @pixels << padding
    end
  end

  # Return the color of the pixel at (column, row)
  def at(column, row)
    return @pixels[row-1][column-1]
  end

  # Colors the pixel (X, Y) in couleur
  def color(column, row, couleur)
    puts("color(#{column}, #{row}, #{couleur})")
    puts("BEFORE\n#{to_s}")
    #puts("@pixels[row-1]: #{@pixels[row-1]}")
    #@pixels[row-1] = couleur
    @pixels[row-1][column-1] = couleur
    puts("AFTER\n#{to_s}")
  end

  # Draw a vertical segment of color (C) in column X, between the rows Y1 and Y2
  # inclusive.
  def vertical(column, row1, row2, color)
    # sort row1 and row2, for the benefit of the following for loop
    array = [row1, row2].sort
    row1, row2 = array[0], array[1]
    (row1-1..row2-1).each do |i|
      @pixels[i][column-1] = color
    end
  end

  # Draw a horizontal segment of color (C) in the row Y, between the columns X1
  # and X2 inclusive.
  def horizontal(column1, column2, row, color)
    # sort column1 and column2, for the benefit of the following for loop
    array = [column1, column2].sort
    column1, column2 = array[0], array[1]
    (column1-1..column2-1).each do |i|
      @pixels[row-1][i] = color
    end
  end

  # Draw a filled rectangle of color C, where (X1, Y1) is the upper-left and
  # (X2, Y2) the lower right corner.
  def rectangle(column1, row1, column2, row2, color)
    # sort row1, row2, column1 and column2 for the benefit of the for loops
    array = [row1, row2].sort
    row1, row2 = array[0], array[1]
    array = [column1, column2].sort
    column1, column2 = array[0], array[1]
    (row1-1..row2-1).each do |i|
      (column1-1..column2-1).each do |j|
        @pixels[i][j] = color;
      end
    end
  end

  # Fill the region R with the color C, where R is defined as follows. Pixel
  # (X, Y) belongs to R. Any other pixel which is the same color as pixel (X, Y)
  # and shares a common side with any pixel in R also belongs to this region.
  def fill(column, row, old, new)
    if (row > 0 && row < @pixels.size() &&
        column > 0 && column < @pixels[0].size() &&
        @pixels[row-1][column-1] == old)
      @pixels[row-1][column-1] = new
      fill(column-1, row, old, new)
      fill(column+1, row, old, new)
      fill(column, row-1, old, new)
      fill(column, row+1, old, new)
    end
  end

  # Write the file name in MSDOS 8.3 format followed by the contents of the
  # current image
  def save(filename)
    file = File.open(filename, "w")
    @pixels.each do |pixel|
      file.puts(pixel)
    end
    file.close
  end

  # for debugging
  def to_s
    string = ""
    @pixels.each do |pixel|
      string << pixel << "\n"
    end
    return string
  end
end

class Command
  attr_accessor :command, :x1, :y1, :x2, :y2, :color, :filename
  @@image = Image.new

  def initialize(command)
    @command = command
    @x1 = @x2 = @y1 = @y2 = 0
    @color = 'O'
    @filename = ""
  end

  def execute
    puts("pre-execute()\n#{to_s}")
    case @command
    when 'I'
      @@image.create(@x1, @y1)
    when 'C'
      @@image.clear
    when 'L'
      @@image.color(@x1, @y1, @color)
    when 'V'
      @@image.vertical(@x1, @y1, @y2, @color)
    when 'H'
      @@image.horizontal(@x1, @x2, @y1, @color)
    when 'K'
      @@image.rectangle(@x1, @y1, @x2, @y2, @color)
    when 'F'
      @@image.fill(@x1, @y1, @@image.at(@x1, @y1), @color)
      puts(@@image)
    when 'S'
      @@image.save(@filename)
    when 'X'
    end # switch statement

    puts("post-execute()\n#{@@image}")
  end # method

  # for debugging purpose
  def to_s
    string = "#{@command} #{x1} #{y1} #{x2} #{y2} #{@color} #{@filename}"
  end
end

def input(filename)
  list = []
  File.open(filename.to_s) do |file|
    while true
      line = file.gets
      command = Command.new(line[0])
      args = line[1..-1].split(" ").collect { |arg| arg }
      case command.command
      when 'I'
        command.x1, command.y1 = args[0].to_i, args[1].to_i
      when 'C'
      when 'L'
        command.x1, command.y1, command.color = args[0].to_i, args[1].to_i, args[2]
      when 'V'
        command.x1, command.y1, command.y2, command.color = args[0].to_i, args[1].to_i, args[2].to_i, args[3]
      when 'H'
        command.x1, command.x2, command.y1, command.color = args[0].to_i, args[1].to_i, args[2].to_i, args[3]
      when 'K'
        command.x1, command.y1, command.x2, command.y2, command.color = args[0].to_i, args[1].to_i, args[2].to_i, args[3].to_i, args[4]
      when 'F'
        command.x1, command.y1, command.color = args[0].to_i, args[1].to_i, args[2]
      when 'S'
        command.filename = args[0]
      when 'X'
        break # this break out of the while loop
      end # switch statement

      if command.command != 'X'
        puts command
        list << command
      end
    end # while loop

    file.close
  end # FILE.open do

  return list
end

def output(list)
  list.each do |record|
    record.execute
  end
end

if ARGV.empty?
  puts("Usage: Must pass input file as command-line argument")
else
  output(input(ARGV[0]))
end
