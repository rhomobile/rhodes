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

# A colour object representing YIQ (NTSC) colour encoding.
class Color::YIQ
  # Creates a YIQ colour object from fractional values 0 .. 1.
  #
  #   Color::YIQ.new(0.3, 0.2, 0.1)
  def self.from_fraction(y = 0, i = 0, q = 0)
    color = Color::YIQ.new
    color.y = y
    color.i = i
    color.q = q
    color
  end

  # Creates a YIQ colour object from percentages 0 .. 100.
  #
  #   Color::YIQ.new(10, 20, 30)
  def initialize(y = 0, i = 0, q = 0)
    @y = y / 100.0
    @i = i / 100.0
    @q = q / 100.0
  end

  # Compares the other colour to this one. The other colour will be
  # converted to YIQ before comparison, so the comparison between a YIQ
  # colour and a non-YIQ colour will be approximate and based on the other
  # colour's #to_yiq conversion. If there is no #to_yiq conversion, this
  # will raise an exception. This will report that two YIQ values are
  # equivalent if all component colours are within COLOR_TOLERANCE of each
  # other.
  def ==(other)
    other = other.to_yiq
    other.kind_of?(Color::YIQ) and
    ((@y - other.y).abs <= Color::COLOR_TOLERANCE) and
    ((@i - other.i).abs <= Color::COLOR_TOLERANCE) and
    ((@q - other.q).abs <= Color::COLOR_TOLERANCE) 
  end

  def to_yiq
    self
  end

  def brightness
    @y
  end
  def to_grayscale
    Color::GrayScale.new(@y)
  end
  alias to_greyscale to_grayscale

  def y
    @y
  end
  def y=(yy)
    @y = Color.normalize(yy)
  end
  def i
    @i
  end
  def i=(ii)
    @i = Color.normalize(ii)
  end
  def q
    @q
  end
  def q=(qq)
    @q = Color.normalize(qq)
  end

  def inspect
    "YIQ [%.2f%%, %.2f%%, %.2f%%]" % [ @y * 100, @i * 100, @q * 100 ]
  end
end
