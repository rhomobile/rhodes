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

# An RGB colour object.
class Color::RGB
  # The format of a DeviceRGB colour for PDF. In color-tools 2.0 this will
  # be removed from this package and added back as a modification by the
  # PDF::Writer package.
  PDF_FORMAT_STR  = "%.3f %.3f %.3f %s"

  class << self
    # Creates an RGB colour object from percentages 0..100.
    #
    #   Color::RGB.from_percentage(10, 20 30)
    def from_percentage(r = 0, g = 0, b = 0)
      from_fraction(r / 100.0, g / 100.0, b / 100.0)
    end

    # Creates an RGB colour object from fractional values 0..1.
    #
    #   Color::RGB.from_fraction(.3, .2, .1)
    def from_fraction(r = 0.0, g = 0.0, b = 0.0)
      colour = Color::RGB.new
      colour.r = r
      colour.g = g
      colour.b = b
      colour
    end

    # Creates an RGB colour object from an HTML colour descriptor (e.g.,
    # <tt>"fed"</tt> or <tt>"#cabbed;"</tt>.
    #
    #   Color::RGB.from_html("fed")
    #   Color::RGB.from_html("#fed")
    #   Color::RGB.from_html("#cabbed")
    #   Color::RGB.from_html("cabbed")
    def from_html(html_colour)
      html_colour = html_colour.gsub(%r{[#;]}, '')
      case html_colour.size 
      when 3
        colours = html_colour.scan(%r{[0-9A-Fa-f]}).map { |el| (el * 2).to_i(16) }
      when 6
        colours = html_colour.scan(%r<[0-9A-Fa-f]{2}>).map { |el| el.to_i(16) }
      else
        raise ArgumentError
      end

      Color::RGB.new(*colours)
    end
  end

  # Compares the other colour to this one. The other colour will be
  # converted to RGB before comparison, so the comparison between a RGB
  # colour and a non-RGB colour will be approximate and based on the other
  # colour's default #to_rgb conversion. If there is no #to_rgb conversion,
  # this will raise an exception. This will report that two RGB colours are
  # equivalent if all component values are within COLOR_TOLERANCE of each
  # other.
  def ==(other)
    other = other.to_rgb
    other.kind_of?(Color::RGB) and
    ((@r - other.r).abs <= Color::COLOR_TOLERANCE) and
    ((@g - other.g).abs <= Color::COLOR_TOLERANCE) and
    ((@b - other.b).abs <= Color::COLOR_TOLERANCE)
  end

  # Creates an RGB colour object from the standard range 0..255.
  #
  #   Color::RGB.new(32, 64, 128)
  #   Color::RGB.new(0x20, 0x40, 0x80)
  def initialize(r = 0, g = 0, b = 0)
    @r = r / 255.0
    @g = g / 255.0
    @b = b / 255.0
  end

  # Present the colour as a DeviceRGB fill colour string for PDF. This will
  # be removed from the default package in color-tools 2.0.
  def pdf_fill
    PDF_FORMAT_STR % [ @r, @g, @b, "rg" ]
  end

  # Present the colour as a DeviceRGB stroke colour string for PDF. This
  # will be removed from the default package in color-tools 2.0.
  def pdf_stroke
    PDF_FORMAT_STR % [ @r, @g, @b, "RG" ]
  end

  # Present the colour as an HTML/CSS colour string.
  def html
    r = (@r * 255).round
    r = 255 if r > 255

    g = (@g * 255).round
    g = 255 if g > 255

    b = (@b * 255).round
    b = 255 if b > 255

    "#%02x%02x%02x" % [ r, g, b ]
  end

  # Present the colour as an RGB HTML/CSS colour string (e.g., "rgb(0%, 50%,
  # 100%)"). Note that this will perform a #to_rgb operation using the
  # default conversion formula.
  def css_rgb
    "rgb(%3.2f%%, %3.2f%%, %3.2f%%)" % [ red_p, green_p, blue_p ]
  end

  # Present the colour as an RGBA (with alpha) HTML/CSS colour string (e.g.,
  # "rgb(0%, 50%, 100%, 1)"). Note that this will perform a #to_rgb
  # operation using the default conversion formula.
  def css_rgba
    "rgba(%3.2f%%, %3.2f%%, %3.2f%%, %3.2f)" % [ red_p, green_p, blue_p, 1 ]
  end

  # Present the colour as an HSL HTML/CSS colour string (e.g., "hsl(180,
  # 25%, 35%)"). Note that this will perform a #to_hsl operation using the
  # default conversion formula.
  def css_hsl
    to_hsl.css_hsl
  end

  # Present the colour as an HSLA (with alpha) HTML/CSS colour string (e.g.,
  # "hsla(180, 25%, 35%, 1)"). Note that this will perform a #to_hsl
  # operation using the default conversion formula.
  def css_hsla
    to_hsl.css_hsla
  end

  # Converts the RGB colour to CMYK. Most colour experts strongly suggest
  # that this is not a good idea (some even suggesting that it's a very bad
  # idea). CMYK represents additive percentages of inks on white paper,
  # whereas RGB represents mixed colour intensities on a black screen.
  #
  # However, the colour conversion can be done. The basic method is
  # multi-step:
  #
  # 1. Convert the R, G, and B components to C, M, and Y components.
  #     c = 1.0 - r
  #     m = 1.0 - g
  #     y = 1.0 - b
  # 2. Compute the minimum amount of black (K) required to smooth the colour
  #    in inks.
  #     k = min(c, m, y)
  # 3. Perform undercolour removal on the C, M, and Y components of the
  #    colours because less of each colour is needed for each bit of black.
  #    Also, regenerate the black (K) based on the undercolour removal so
  #    that the colour is more accurately represented in ink.
  #     c = min(1.0, max(0.0, c - UCR(k)))
  #     m = min(1.0, max(0.0, m - UCR(k)))
  #     y = min(1.0, max(0.0, y - UCR(k)))
  #     k = min(1.0, max(0.0, BG(k)))
  #
  # The undercolour removal function and the black generation functions
  # return a value based on the brightness of the RGB colour.
  def to_cmyk
    c = 1.0 - @r.to_f
    m = 1.0 - @g.to_f
    y = 1.0 - @b.to_f

    k = [c, m, y].min
    k = k - (k * brightness)

    c = [1.0, [0.0, c - k].max].min
    m = [1.0, [0.0, m - k].max].min
    y = [1.0, [0.0, y - k].max].min
    k = [1.0, [0.0, k].max].min

    Color::CMYK.from_fraction(c, m, y, k)
  end

  def to_rgb(ignored = nil)
    self
  end

  # Returns the YIQ (NTSC) colour encoding of the RGB value.
  def to_yiq
    y = (@r * 0.299) + (@g *  0.587) + (@b *  0.114)
    i = (@r * 0.596) + (@g * -0.275) + (@b * -0.321)
    q = (@r * 0.212) + (@g * -0.523) + (@b *  0.311)
    Color::YIQ.from_fraction(y, i, q)
  end

  # Returns the HSL colour encoding of the RGB value. The conversions here
  # are based on forumlas from http://www.easyrgb.com/math.php and
  # elsewhere.
  def to_hsl
    min   = [ @r, @g, @b ].min
    max   = [ @r, @g, @b ].max
    delta = (max - min).to_f

    lum   = (max + min) / 2.0

    if Color.near_zero?(delta) # close to 0.0, so it's a grey
      hue = 0
      sat = 0
    else
      if Color.near_zero_or_less?(lum - 0.5)
        sat = delta / (max + min).to_f
      else
        sat = delta / (2 - max - min).to_f
      end

      # This is based on the conversion algorithm from
      # http://en.wikipedia.org/wiki/HSV_color_space#Conversion_from_RGB_to_HSL_or_HSV
      # Contributed by Adam Johnson
      sixth = 1 / 6.0
      if @r == max # Color.near_zero_or_less?(@r - max)
        hue = (sixth * ((@g - @b) / delta))
        hue += 1.0 if @g < @b
      elsif @g == max # Color.near_zero_or_less(@g - max)
        hue = (sixth * ((@b - @r) / delta)) + (1.0 / 3.0)
      elsif @b == max # Color.near_zero_or_less?(@b - max)
        hue = (sixth * ((@r - @g) / delta)) + (2.0 / 3.0)
      end

      hue += 1 if hue < 0
      hue -= 1 if hue > 1
    end
    Color::HSL.from_fraction(hue, sat, lum)
  end

  # Mix the RGB hue with White so that the RGB hue is the specified
  # percentage of the resulting colour. Strictly speaking, this isn't a
  # darken_by operation.
  def lighten_by(percent)
    mix_with(White, percent)
  end

  # Mix the RGB hue with Black so that the RGB hue is the specified
  # percentage of the resulting colour. Strictly speaking, this isn't a
  # darken_by operation.
  def darken_by(percent)
    mix_with(Black, percent)
  end

  # Mix the mask colour (which must be an RGB object) with the current
  # colour at the stated opacity percentage (0..100).
  def mix_with(mask, opacity)
    opacity /= 100.0
    rgb = self.dup
    
    rgb.r = (@r * opacity) + (mask.r * (1 - opacity))
    rgb.g = (@g * opacity) + (mask.g * (1 - opacity))
    rgb.b = (@b * opacity) + (mask.b * (1 - opacity))

    rgb
  end

  # Returns the brightness value for a colour, a number between 0..1. Based
  # on the Y value of YIQ encoding, representing luminosity, or perceived
  # brightness.
  #
  # This may be modified in a future version of color-tools to use the
  # luminosity value of HSL.
  def brightness
    to_yiq.y
  end
  # Convert to grayscale.
  def to_grayscale
    Color::GrayScale.from_fraction(to_hsl.l)
  end
  alias to_greyscale to_grayscale

  # Returns a new colour with the brightness adjusted by the specified
  # percentage. Negative percentages will darken the colour; positive
  # percentages will brighten the colour.
  #
  #   Color::RGB::DarkBlue.adjust_brightness(10)
  #   Color::RGB::DarkBlue.adjust_brightness(-10)
  def adjust_brightness(percent)
    percent /= 100.0
    percent += 1.0
    percent  = [ percent, 2.0 ].min
    percent  = [ 0.0, percent ].max

    hsl      = to_hsl
    hsl.l   *= percent
    hsl.to_rgb
  end

  # Returns a new colour with the saturation adjusted by the specified
  # percentage. Negative percentages will reduce the saturation; positive
  # percentages will increase the saturation.
  #
  #   Color::RGB::DarkBlue.adjust_saturation(10)
  #   Color::RGB::DarkBlue.adjust_saturation(-10)
  def adjust_saturation(percent)
    percent  /= 100.0
    percent  += 1.0
    percent  = [ percent, 2.0 ].min
    percent  = [ 0.0, percent ].max

    hsl      = to_hsl
    hsl.s   *= percent
    hsl.to_rgb
  end

  # Returns a new colour with the hue adjusted by the specified percentage.
  # Negative percentages will reduce the hue; positive percentages will
  # increase the hue.
  #
  #   Color::RGB::DarkBlue.adjust_hue(10)
  #   Color::RGB::DarkBlue.adjust_hue(-10)
  def adjust_hue(percent)
    percent  /= 100.0
    percent  += 1.0
    percent  = [ percent, 2.0 ].min
    percent  = [ 0.0, percent ].max

    hsl      = to_hsl
    hsl.h   *= percent
    hsl.to_rgb
  end

  # Returns the red component of the colour in the normal 0 .. 255 range.
  def red
    @r * 255.0
  end
  # Returns the red component of the colour as a percentage.
  def red_p
    @r * 100.0
  end
  # Returns the red component of the colour as a fraction in the range 0.0
  # .. 1.0.
  def r
    @r
  end
  # Sets the red component of the colour in the normal 0 .. 255 range.
  def red=(rr)
    @r = Color.normalize(rr / 255.0)
  end
  # Sets the red component of the colour as a percentage.
  def red_p=(rr)
    @r = Color.normalize(rr / 100.0)
  end
  # Sets the red component of the colour as a fraction in the range 0.0 ..
  # 1.0.
  def r=(rr)
    @r = Color.normalize(rr)
  end

  # Returns the green component of the colour in the normal 0 .. 255 range.
  def green
    @g * 255.0
  end
  # Returns the green component of the colour as a percentage.
  def green_p
    @g * 100.0
  end
  # Returns the green component of the colour as a fraction in the range 0.0
  # .. 1.0.
  def g
    @g
  end
  # Sets the green component of the colour in the normal 0 .. 255 range.
  def green=(gg)
    @g = Color.normalize(gg / 255.0)
  end
  # Sets the green component of the colour as a percentage.
  def green_p=(gg)
    @g = Color.normalize(gg / 100.0)
  end
  # Sets the green component of the colour as a fraction in the range 0.0 ..
  # 1.0.
  def g=(gg)
    @g = Color.normalize(gg)
  end

  # Returns the blue component of the colour in the normal 0 .. 255 range.
  def blue
    @b * 255.0
  end
  # Returns the blue component of the colour as a percentage.
  def blue_p
    @b * 100.0
  end
  # Returns the blue component of the colour as a fraction in the range 0.0
  # .. 1.0.
  def b
    @b
  end
  # Sets the blue component of the colour in the normal 0 .. 255 range.
  def blue=(bb)
    @b = Color.normalize(bb / 255.0)
  end
  # Sets the blue component of the colour as a percentage.
  def blue_p=(bb)
    @b = Color.normalize(bb / 100.0)
  end
  # Sets the blue component of the colour as a fraction in the range 0.0 ..
  # 1.0.
  def b=(bb)
    @b = Color.normalize(bb)
  end

  # Adds another colour to the current colour. The other colour will be
  # converted to RGB before addition. This conversion depends upon a #to_rgb
  # method on the other colour.
  #
  # The addition is done using the RGB Accessor methods to ensure a valid
  # colour in the result.
  def +(other)
    other = other.to_rgb
    rgb = self.dup

    rgb.r += other.r
    rgb.g += other.g
    rgb.b += other.b

    rgb
  end

  # Subtracts another colour to the current colour. The other colour will be
  # converted to RGB before subtraction. This conversion depends upon a
  # #to_rgb method on the other colour.
  #
  # The subtraction is done using the RGB Accessor methods to ensure a valid
  # colour in the result.
  def -(other) 
    other = other.to_rgb 
    rgb = self.dup

    rgb.r -= other.r
    rgb.g -= other.g
    rgb.b -= other.b

    rgb
  end

  # Retrieve the maxmum RGB value from the current colour as a GrayScale
  # colour
  def max_rgb_as_grayscale
      Color::GrayScale.from_fraction([@r, @g, @b].max)
  end
  alias max_rgb_as_greyscale max_rgb_as_grayscale

  def inspect
    "RGB [#{html}]"
  end
end

require 'color/rgb-colors'
