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

# A colour object representing shades of grey. Used primarily in PDF
# document creation.
class Color::GrayScale
  # The format of a DeviceGrey colour for PDF. In color-tools 2.0 this will
  # be removed from this package and added back as a modification by the
  # PDF::Writer package.
  PDF_FORMAT_STR  = "%.3f %s"

  # Creates a greyscale colour object from fractional values 0..1.
  #
  #   Color::GreyScale.from_fraction(0.5)
  def self.from_fraction(g = 0)
    color = Color::GrayScale.new
    color.g = g
    color
  end

  # Creates a greyscale colour object from percentages 0..100.
  #
  #   Color::GrayScale.from_percent(50)
  def self.from_percent(g = 0)
    Color::GrayScale.new(g)
  end

  # Creates a greyscale colour object from percentages 0..100.
  #
  #   Color::GrayScale.new(50)
  def initialize(g = 0)
    @g = g / 100.0
  end

  # Compares the other colour to this one. The other colour will be
  # converted to GreyScale before comparison, so the comparison between a
  # GreyScale colour and a non-GreyScale colour will be approximate and
  # based on the other colour's #to_greyscale conversion. If there is no
  # #to_greyscale conversion, this will raise an exception. This will report
  # that two GreyScale values are equivalent if they are within
  # COLOR_TOLERANCE of each other.
  def ==(other)
    other = other.to_grayscale
    other.kind_of?(Color::GrayScale) and
    ((@g - other.g).abs <= Color::COLOR_TOLERANCE)
  end

  # Present the colour as a DeviceGrey fill colour string for PDF. This will
  # be removed from the default package in color-tools 2.0.
  def pdf_fill
    PDF_FORMAT_STR % [ @g, "g" ]
  end

  # Present the colour as a DeviceGrey stroke colour string for PDF. This
  # will be removed from the default package in color-tools 2.0.
  def pdf_stroke
    PDF_FORMAT_STR % [ @g, "G" ]
  end

  def to_255
    [(@g * 255).round, 255].min
  end
  private :to_255

  # Present the colour as an HTML/CSS colour string.
  def html
    gs = "%02x" % to_255
    "##{gs * 3}"
  end

  # Present the colour as an RGB HTML/CSS colour string (e.g., "rgb(0%, 50%,
  # 100%)").
  def css_rgb
    "rgb(%3.2f%%, %3.2f%%, %3.2f%%)" % [ gray, gray, gray ]
  end

  # Present the colour as an RGBA (with alpha) HTML/CSS colour string (e.g.,
  # "rgb(0%, 50%, 100%, 1)").
  def css_rgba
    "rgba(%3.2f%%, %3.2f%%, %3.2f%%, %1.2f)" % [ gray, gray, gray, 1 ]
  end

  # Present the colour as an HSL HTML/CSS colour string (e.g., "hsl(180,
  # 25%, 35%)"). Note that this will perform a #to_hsl operation.
  def css_hsl
    to_hsl.css_hsl
  end

  # Present the colour as an HSLA (with alpha) HTML/CSS colour string (e.g.,
  # "hsla(180, 25%, 35%, 1)"). Note that this will perform a #to_hsl
  # operation.
  def css_hsla
    to_hsl.css_hsla
  end

  # Convert the greyscale colour to CMYK.
  def to_cmyk
    k = 1.0 - @g.to_f
    Color::CMYK.from_fraction(0, 0, 0, k)
  end

  # Convert the greyscale colour to RGB.
  def to_rgb(ignored = true)
    Color::RGB.from_fraction(g, g, g)
  end

  # Reflexive conversion.
  def to_grayscale
    self
  end
  alias to_greyscale to_grayscale

  # Lightens the greyscale colour by the stated percent.
  def lighten_by(percent)
    g = [@g + (@g * (percent / 100.0)), 1.0].min
    Color::GrayScale.from_fraction(g)
  end

  # Darken the greyscale colour by the stated percent.
  def darken_by(percent)
    g = [@g - (@g * (percent / 100.0)), 0.0].max
    Color::GrayScale.from_fraction(g)
  end

  # Returns the YIQ (NTSC) colour encoding of the greyscale value. This is
  # an approximation, as the values for I and Q are calculated by treating
  # the greyscale value as an RGB value. The Y (intensity or brightness)
  # value is the same as the greyscale value.
  def to_yiq
    y = @g
    i = (@g * 0.596) + (@g * -0.275) + (@g * -0.321)
    q = (@g * 0.212) + (@g * -0.523) + (@g *  0.311)
    Color::YIQ.from_fraction(y, i, q)
  end

  # Returns the HSL colour encoding of the greyscale value.
  def to_hsl
    Color::HSL.from_fraction(0, 0, @g)
  end

  # Returns the brightness value for this greyscale value; this is the
  # greyscale value itself.
  def brightness
    @g
  end

  # Returns the grayscale value as a percentage of white (100% gray is
  # white).
  def gray
    @g * 100.0
  end
  alias grey gray
  # Returns the grayscale value as a fractional value of white in the range
  # 0.0 .. 1.0.
  def g
    @g
  end
  # Sets the grayscale value as a percentage of white.
  def gray=(gg)
    @g = Color.normalize(gg / 100.0)
  end
  alias grey= gray= ;
  # Returns the grayscale value as a fractional value of white in the range
  # 0.0 .. 1.0.
  def g=(gg)
    @g = Color.normalize(gg)
  end

  # Adds another colour to the current colour. The other colour will be
  # converted to grayscale before addition. This conversion depends upon a
  # #to_grayscale method on the other colour.
  #
  # The addition is done using the grayscale accessor methods to ensure a
  # valid colour in the result.
  def +(other)
    other = other.to_grayscale
    ng = self.dup
    ng.g += other.g
    ng
  end

  # Subtracts another colour to the current colour. The other colour will be
  # converted to grayscale before subtraction. This conversion depends upon
  # a #to_grayscale method on the other colour.
  #
  # The subtraction is done using the grayscale accessor methods to ensure a
  # valid colour in the result.
  def -(other)
    other = other.to_grayscale 
    ng = self.dup
    ng.g -= other.g
    ng
  end

  def inspect
    "Gray [%.2f%%]" % [ gray ]
  end
end

module Color
  # A synonym for Color::GrayScale.
  GreyScale = GrayScale
end
