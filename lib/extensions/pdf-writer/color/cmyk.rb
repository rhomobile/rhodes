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

# An CMYK colour object. CMYK (cyan, magenta, yellow, and black) colours are
# based on additive percentages of ink. A CMYK colour of (0.3, 0, 0.8, 0.3)
# would be mixed from 30% cyan, 0% magenta, 80% yellow, and 30% black.
# Primarily used in four-colour printing processes.
class Color::CMYK
  # The format of a DeviceCMYK colour for PDF. In color-tools 2.0 this will
  # be removed from this package and added back as a modification by the
  # PDF::Writer package.
  PDF_FORMAT_STR = "%.3f %.3f %.3f %.3f %s"

  # Compares the other colour to this one. The other colour will be
  # converted to CMYK before comparison, so the comparison between a CMYK
  # colour and a non-CMYK colour will be approximate and based on the other
  # colour's #to_cmyk conversion. If there is no #to_cmyk conversion, this
  # will raise an exception. This will report that two CMYK colours are
  # equivalent if all component values are within COLOR_TOLERANCE of each
  # other.
  def ==(other)
    other = other.to_cmyk
    other.kind_of?(Color::CMYK) and
    ((@c - other.c).abs <= Color::COLOR_TOLERANCE) and
    ((@m - other.m).abs <= Color::COLOR_TOLERANCE) and
    ((@y - other.y).abs <= Color::COLOR_TOLERANCE) and
    ((@k - other.k).abs <= Color::COLOR_TOLERANCE)
  end

  # Creates a CMYK colour object from fractional values 0..1.
  #
  #   Color::CMYK.from_fraction(0.3, 0, 0.8, 0.3)
  def self.from_fraction(c = 0, m = 0, y = 0, k = 0)
    colour = Color::CMYK.new
    colour.c = c
    colour.m = m
    colour.y = y
    colour.k = k
    colour
  end

  # Creates a CMYK colour object from percentages. Internally, the colour is
  # managed as fractional values 0..1.
  #
  #   Color::CMYK.new(30, 0, 80, 30)
  def self.from_percent(c = 0, m = 0, y = 0, k = 0)
    Color::CMYK.new(c, m, y, k)
  end

  # Creates a CMYK colour object from percentages. Internally, the colour is
  # managed as fractional values 0..1.
  #
  #   Color::CMYK.new(30, 0, 80, 30)
  def initialize(c = 0, m = 0, y = 0, k = 0)
    @c = c / 100.0
    @m = m / 100.0
    @y = y / 100.0
    @k = k / 100.0
  end

  # Present the colour as a DeviceCMYK fill colour string for PDF. This will
  # be removed from the default package in color-tools 2.0.
  def pdf_fill
    PDF_FORMAT_STR % [ @c, @m, @y, @k, "k" ]
  end

  # Present the colour as a DeviceCMYK stroke colour string for PDF. This
  # will be removed from the default package in color-tools 2.0.
  def pdf_stroke
    PDF_FORMAT_STR % [ @c, @m, @y, @k, "K" ]
  end

  # Present the colour as an RGB HTML/CSS colour string (e.g., "#aabbcc").
  # Note that this will perform a #to_rgb operation using the default
  # conversion formula.
  def html
    to_rgb.html
  end

  # Present the colour as an RGB HTML/CSS colour string (e.g., "rgb(0%, 50%,
  # 100%)"). Note that this will perform a #to_rgb operation using the
  # default conversion formula.
  def css_rgb
    to_rgb.css_rgb
  end

  # Present the colour as an RGBA (with alpha) HTML/CSS colour string (e.g.,
  # "rgb(0%, 50%, 100%, 1)"). Note that this will perform a #to_rgb
  # operation using the default conversion formula.
  def css_rgba
    to_rgb.css_rgba
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

  # Converts the CMYK colour to RGB. Most colour experts strongly suggest
  # that this is not a good idea (some even suggesting that it's a very bad
  # idea). CMYK represents additive percentages of inks on white paper,
  # whereas RGB represents mixed colour intensities on a black screen.
  #
  # However, the colour conversion can be done, and there are two different
  # methods for the conversion that provide slightly different results.
  # Adobe PDF conversions are done with the first form.
  #
  #     # Adobe PDF Display Formula
  #   r = 1.0 - min(1.0, c + k)
  #   g = 1.0 - min(1.0, m + k)
  #   b = 1.0 - min(1.0, y + k)
  #
  #     # Other
  #   r = 1.0 - (c * (1.0 - k) + k)
  #   g = 1.0 - (m * (1.0 - k) + k)
  #   b = 1.0 - (y * (1.0 - k) + k)
  #
  # If we have a CMYK colour of [33% 66% 83% 25%], the first method will
  # give an approximate RGB colour of (107, 23, 0) or #6b1700. The second
  # method will give an approximate RGB colour of (128, 65, 33) or #804121.
  # Which is correct? Although the colours may seem to be drastically
  # different in the RGB colour space, they are very similar colours,
  # differing mostly in intensity. The first is a darker, slightly redder
  # brown; the second is a lighter brown.
  #
  # Because of this subtlety, both methods are now offered for conversion.
  # The Adobe method is not used by default; to enable it, pass +true+ to
  # #to_rgb.
  #
  # Future versions of Color may offer other conversion mechanisms that
  # offer greater colour fidelity, including recognition of ICC colour
  # profiles.
  def to_rgb(use_adobe_method = false)
    if use_adobe_method
      r = 1.0 - [1.0, @c + @k].min
      g = 1.0 - [1.0, @m + @k].min
      b = 1.0 - [1.0, @y + @k].min
    else
      r = 1.0 - (@c.to_f * (1.0 - @k.to_f) + @k.to_f)
      g = 1.0 - (@m.to_f * (1.0 - @k.to_f) + @k.to_f)
      b = 1.0 - (@y.to_f * (1.0 - @k.to_f) + @k.to_f)
    end
    Color::RGB.from_fraction(r, g, b)
  end

  # Converts the CMYK colour to a single greyscale value. There are
  # undoubtedly multiple methods for this conversion, but only a minor
  # variant of the Adobe conversion method will be used:
  #
  #   g = 1.0 - min(1.0, 0.299 * c + 0.587 * m + 0.114 * y + k)
  #
  # This treats the CMY values similarly to YIQ (NTSC) values and then adds
  # the level of black. This is a variant of the Adobe version because it
  # uses the more precise YIQ (NTSC) conversion values for Y (intensity)
  # rather than the approximates provided by Adobe (0.3, 0.59, and 0.11).
  def to_grayscale
    c = 0.299 * @c.to_f
    m = 0.587 * @m.to_f
    y = 0.114 * @y.to_f
    g = 1.0 - [1.0, c + m + y + @k].min
    Color::GrayScale.from_fraction(g)
  end
  alias to_greyscale to_grayscale

  def to_cmyk
    self
  end

  def inspect
    "CMYK [%.2f%%, %.2f%%, %.2f%%, %.2f%%]" % [ cyan, magenta, yellow, black ]
  end

  # Converts to RGB then YIQ.
  def to_yiq
    to_rgb.to_yiq
  end

  # Converts to RGB then HSL.
  def to_hsl
    to_rgb.to_hsl
  end

  # Returns the cyan (C) component of the CMYK colour as a percentage value.
  def cyan
    @c * 100.0
  end
  # Returns the cyan (C) component of the CMYK colour as a value in the
  # range 0.0 .. 1.0.
  def c
    @c
  end
  # Sets the cyan (C) component of the CMYK colour as a percentage value.
  def cyan=(cc)
    @c = Color.normalize(cc / 100.0)
  end
  # Sets the cyan (C) component of the CMYK colour as a value in the range
  # 0.0 .. 1.0.
  def c=(cc)
    @c = Color.normalize(cc)
  end

  # Returns the magenta (M) component of the CMYK colour as a percentage
  # value.
  def magenta
    @m * 100.0
  end
  # Returns the magenta (M) component of the CMYK colour as a value in the
  # range 0.0 .. 1.0.
  def m
    @m
  end
  # Sets the magenta (M) component of the CMYK colour as a percentage value.
  def magenta=(mm)
    @m = Color.normalize(mm / 100.0)
  end
  # Sets the magenta (M) component of the CMYK colour as a value in the
  # range 0.0 .. 1.0.
  def m=(mm)
    @m = Color.normalize(mm)
  end

  # Returns the yellow (Y) component of the CMYK colour as a percentage
  # value.
  def yellow
    @y * 100.0
  end
  # Returns the yellow (Y) component of the CMYK colour as a value in the
  # range 0.0 .. 1.0.
  def y
    @y
  end
  # Sets the yellow (Y) component of the CMYK colour as a percentage value.
  def yellow=(yy)
    @y = Color.normalize(yy / 100.0)
  end
  # Sets the yellow (Y) component of the CMYK colour as a value in the range
  # 0.0 .. 1.0.
  def y=(kk)
    @y = Color.normalize(kk)
  end

  # Returns the black (K) component of the CMYK colour as a percentage
  # value.
  def black
    @k * 100.0
  end
  # Returns the black (K) component of the CMYK colour as a value in the
  # range 0.0 .. 1.0.
  def k
    @k
  end
  # Sets the black (K) component of the CMYK colour as a percentage value.
  def black=(kk)
    @k = Color.normalize(kk / 100.0)
  end
  # Sets the black (K) component of the CMYK colour as a value in the range
  # 0.0 .. 1.0.
  def k=(kk)
    @k = Color.normalize(kk)
  end
end
