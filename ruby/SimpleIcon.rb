class SimpleIcon

  DELIM = ';;'

  def initialize(file)
    @name = 'EMPTY'
    @file_version = 1
    @width = 8
    @height = 8
    @data = []

    load_from_file file
  end

  def load_from_file(file)
    raise 'Need file to read from' if file == nil
    raise 'File "%s" does not exist' % file unless File.exists? file

    fp = nil
    begin
      fp = open(file, 'r')
    rescue Exception => ex
      raise 'Failed to load file: %s' % ex
    end

    file_content = fp.readlines.join.strip!
    parse_header file_content
  end

  def parse_header(file_content)
    raise 'Cannot parse empty file' if file_content == nil

    tmp = file_content.split DELIM
    raise 'File content has illegal format (expect 4 fields)' unless tmp.size == 4

    @name, @file_version, size, file_data = tmp
    @width, @height = size.split('x').map { |x| x.to_i }

    # Parse data
    case @file_version.to_i
      when 1
        @data = parse_data(file_data)
      when 2
        @data = parse_data_v2(file_data)
      else
        raise 'Cannot parse file with unknown version %d' % @file_version
    end
  end

  def parse_data(file_data)
    offset = 0
    data = Array.new(@height) { Array.new @width }

    (0..@height - 1).each do |row|
      (0..@width - 1).each do |col|
        data[row][col] = file_data[offset].to_i
        offset += 1
      end
    end

    data
  end

  def parse_data_v2(file_data)
    char_pos = 0
    offset = 0
    data = Array.new(@height) { Array.new @width }

    while offset <= @width
      (0..@height - 1).each do |row|
        (offset..offset + 7).each do |col|
          next if col >= @width
          data[row][col] = file_data[char_pos].to_i
          char_pos += 1
        end
      end

      offset += 8
    end

    data
  end

  def display
    puts '%s (%dx%d)\nVersion: %d' % [@name, @width, @height, @file_version]

    (0..@height - 1).each do |row|
      (0..@width - 1).each do |col|
        putc @data[row][col] == 1 ? 'x' : ' '
      end
      puts ''
    end
  end

end
