#--
# PDF::Writer for Ruby.
#   http://rubyforge.org/projects/ruby-pdf/
#   Copyright 2003 - 2005 Austin Ziegler.
#
#   Licensed under a MIT-style licence. See LICENCE in the main distribution
#   for full licensing information.
#
# $Id: action.rb 188 2007-12-11 07:14:58Z sandal $
#++
  # An action object, used to link to URLS initially
class PDF::Writer::Object::Action < PDF::Writer::Object
  def initialize(parent, label, type = "URI")
    super(parent)

    @type   = type
    @label  = label
    raise TypeError if @label.nil?
  end

  attr_accessor :type
  attr_accessor :label

  def to_s
    res = "\n#{@oid} 0 obj\n<< /Type /Action"
    if @type == :ilink
      res << "\n/S /GoTo\n/D #{@parent.destinations[@label].oid} 0 R"
    elsif @type == 'URI'
      res << "\n/S /URI\n/URI ("
      res << PDF::Writer.escape(@label)
      res << ")\n"
    end
    res << ">>\nendobj"
  end
end
