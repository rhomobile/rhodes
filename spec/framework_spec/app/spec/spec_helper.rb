require 'rbconfig'

class Encoding
  ASCII_8BIT = Encoding.find("ASCII-8BIT")
  US_ASCII = Encoding.find("US-ASCII")
  UTF_8 = Encoding.find("UTF-8")

  #UTF_32BE = Encoding.find("UTF-32BE")
end

TOLERANCE = 0.00003 unless Object.const_defined?(:TOLERANCE)