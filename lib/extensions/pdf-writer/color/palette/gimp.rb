#--
# Color
# Colour management with Ruby
# http://rubyforge.org/projects/color
#   Version 1.4.1
#
# Licensed under a MIT-style licence. See Licence.txt in the main
# distribution for full licensing information.
#
# Copyright (c) 2005 - 2010 Austin Ziegler and Matt Lyon
#++

require 'color/palette'

# A class that can read a GIMP (GNU Image Manipulation Program) palette file
# and provide a Hash-like interface to the contents. GIMP colour palettes
# are RGB values only.
#
# Because two or more entries in a GIMP palette may have the same name, all
# named entries are returned as an array.
#
#   pal = Color::Palette::Gimp.from_file(my_gimp_palette)
#   pal[0]          => Color::RGB<...>
#   pal["white"]    => [ Color::RGB<...> ]
#   pal["unknown"]  => [ Color::RGB<...>, Color::RGB<...>, ... ]
#
# GIMP Palettes are always indexable by insertion order (an integer key).
class Color::Palette::Gimp
  include Enumerable

  class << self
    # Create a GIMP palette object from the named file.
    def from_file(filename)
      File.open(filename, "rb") { |io| Color::Palette::Gimp.from_io(io) }
    end

    # Create a GIMP palette object from the provided IO.
    def from_io(io)
      Color::Palette::Gimp.new(io.read)
    end
  end

  # Create a new GIMP palette from the palette file as a string.
  def initialize(palette)
    @colors   = []
    @names    = {}
    @valid    = false
    @name     = "(unnamed)"

    palette.split($/).each do |line|
      line.chomp!
      line.gsub!(/\s*#.*\Z/, '')

      next if line.empty?

      if line =~ /\AGIMP Palette\Z/
        @valid = true
        next
      end

      info = /(\w+):\s(.*$)/.match(line)
      if info
        @name = info.captures[1] if info.captures[0] =~ /name/i
        next
      end

      line.gsub!(/^\s+/, '')
      data = line.split(/\s+/, 4)
      name = data.pop.strip
      data.map! { |el| el.to_i }

      color = Color::RGB.new(*data)

      @colors << color
      @names[name] ||= []
      @names[name]  << color
    end
  end

  # Provides the colour or colours at the provided selectors.
  def values_at(*selectors)
    @colors.values_at(*selectors)
  end

  # If a Numeric +key+ is provided, the single colour value at that position
  # will be returned. If a String +key+ is provided, the colour set (an
  # array) for that colour name will be returned.
  def [](key)
    if key.kind_of?(Numeric)
      @colors[key]
    else
      @names[key]
    end
  end

  # Loops through each colour.
  def each
    @colors.each { |el| yield el }
  end

  # Loops through each named colour set.
  def each_name #:yields color_name, color_set:#
    @names.each { |color_name, color_set| yield color_name, color_set }
  end

  # Returns true if this is believed to be a valid GIMP palette.
  def valid?
    @valid
  end

  def size
    @colors.size
  end

  attr_reader :name
end
