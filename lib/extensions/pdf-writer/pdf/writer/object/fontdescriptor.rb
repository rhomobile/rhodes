#--
# PDF::Writer for Ruby.
#   http://rubyforge.org/projects/ruby-pdf/
#   Copyright 2003 - 2005 Austin Ziegler.
#
#   Licensed under a MIT-style licence. See LICENCE in the main distribution
#   for full licensing information.
#
# $Id: fontdescriptor.rb 198 2008-03-03 15:26:31Z jh $
#++
  # A font descriptor, needed for including additional fonts. +options+ is a
  # Hash with one of the following keys: Ascent, CapHeight, Descent, Flags,
  # ItalicAngle, StemV, AvgWidth, Leading, MaxWidth, MissingWidth, StemH,
  # XHeight, CharSet, FontFile, FontFile2, FontFile3, FontBBox, or FontName.
class PDF::Writer::Object::FontDescriptor < PDF::Writer::Object
  def initialize(parent, options = nil)
    super(parent)

    @options = options
  end

  attr_accessor :options

  def to_s
    res = "\n#{@oid} 0 obj\n<< /Type /FontDescriptor\n"
    @options.each do |k, v|
      res << "/#{k} #{v}\n" if %w{Ascent CapHeight Descent Flags ItalicAngle StemV AvgWidth Leading MaxWidth MissingWidth StemH XHeight CharSet}.include?(k)
      res << "/#{k} #{v} 0 R\n" if %w{FontFile FontFile2 FontFile3}.include?(k)
      res << "/#{k} [#{v.join(' ')}]\n" if k == "FontBBox"
      res << "/#{k} /#{v}\n" if k == "FontName"
    end
    res << ">>\nendobj"
  end
end
