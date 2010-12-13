#             ~~~~~~~~~~  
# Script encoding of this file should be neither ASCII-8BIT, US-ASCII nor UTF-8.
# This makes it easier to verify that Strings are converted into correct encodings.

#require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

def binary(str) str.force_encoding(Encoding::ASCII_8BIT) end
def utf8(str) str.force_encoding(Encoding::UTF_8) end

describe "Array#pack with format 'm'" do
  it "encodes string with Base64 encoding" do
    ["ABCDEF"].pack('m').should == "QUJDREVG\n"
  end

  it "converts series of 3-char sequences into four 4-char sequences" do
    ["ABCDEFGHI"].pack('m').size.should == 4+4+4+1
  end

  it "fills chars with non-significant bits with '=' sign" do
    ["A"].pack('m').should == "QQ==\n"
  end

  #it "appends newline at the end of result string" do
  #  ["A"].pack('m')[-1].should == ?\n
  #end

  it "appends newline whenever after consumes 45 bytes by default" do
    ["ABC"*31].pack('m').should == 
      "QUJD"*15 + "\n" +
      "QUJD"*15 + "\n" +
      "QUJD\n"
  end

=begin
  it "appends newline whenever after consumes [number/3]*3 of bytes for the specified number > 2, where [x] is the floor function" do
    s = ["ABC"*3].pack('m3').should == "QUJD\n"*3
    s = ["ABC"*3+"\x01"].pack('m3').should == "QUJD\n"*3 + "AQ==\n"

    s = ["ABC"*3].pack('m4').should == "QUJD\n"*3
    s = ["ABC"*3+"\x01"].pack('m4').should == "QUJD\n"*3 + "AQ==\n"
    s = ["ABC"*3+"\x01"].pack('m5').should == "QUJD\n"*3 + "AQ==\n"
    s = ["ABC"*3+"\x01"].pack('m6').should == "QUJDQUJD\nQUJDAQ==\n"
  end
=end

  it "ignores line length parameter if it is 1 or 2" do
    wrapped_at_45 = 
      "QUJD"*15 + "\n" +
      "QUJD"*15 + "\n" +
      "QUJD\n"
    ["ABC"*31].pack('m1').should == wrapped_at_45
    ["ABC"*31].pack('m2').should == wrapped_at_45
  end

=begin
  # feature changed in 1.9 - [ruby-dev:35904].
  it "does not append newline if line length parameter is 0" do
    ["ABC"*31].pack('m0').should == "QUJD"*31
    ["ABC"*31 + "\x01"].pack('m0').should == "QUJD"*31 + "AQ=="
  end
=end

  it "comsumres only one array item per a format" do
    ["ABC", "DEF"].pack('m').should == "QUJD\n"
    ["ABC", "DEF"].pack('m3').should == "QUJD\n"
    ["ABC", "DEF"].pack('m3m3').should == "QUJD\nREVG\n"
  end

  it "encodes 6-bit char less than 26 with capital letters" do
    [( 0*4).chr].pack('m').should == "AA==\n"
    [( 1*4).chr].pack('m').should == "BA==\n"

    [(25*4).chr].pack('m').should == "ZA==\n"
  end

  it "encodes 6-bit char from 26 to 51 with lowercase letters" do
    [(26*4).chr].pack('m').should == "aA==\n"
    [(27*4).chr].pack('m').should == "bA==\n"

    [(51*4).chr].pack('m').should == "zA==\n"
  end

  it "encodes 6-bit char 62 with '+'" do
    [(62*4).chr].pack('m').should == "+A==\n"
  end

  it "encodes 6-bit char 63 with '/'" do
    [(63*4).chr].pack('m').should == "/A==\n"
  end

  it "returns empty string if source string is empty" do
    [""].pack('m').should == ""
  end

=begin
  it "tries to convert the pack argument to a String using #to_str" do
    obj = mock('to_str')
    obj.should_receive(:to_str).and_return("ABC")
    [obj].pack('m').should == "QUJD\n"
  end
=end

=begin
  it "returns an US-ASCII string" do
    ["abcd"].pack('m').encoding.should == Encoding::US_ASCII
  end
=end
end
