#
#  rho_spec.rb
#  rhodes
#
#  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#require 'spec/spec_helper'

describe "RhoConfig" do

  before(:each) do
    Rho::RhoConfig.config['start_path'] = '/app'
    Rho::RhoConfig.config['options_path'] = '/app/Settings'
  end
  
  it "should populate configuration in sources table" do
    sources = ::Rho::RHO.get_user_db().select_from_table('sources','*')
    sources.size.should > 1
  end
  
  it "should have start_path" do
    Rho::RhoConfig.start_path.should == '/app'
  end
  
  it "should retrieve start_path" do
    Rho::RhoConfig.start_path.should == '/app'
  end
  
  it "should set start_path" do
    oldpath = Rho::RhoConfig.start_path
    Rho::RhoConfig.start_path = '/foo/bar'
    Rho::RhoConfig.start_path.should == '/foo/bar'
    Rho::RhoConfig.start_path = oldpath
  end
  
  it "should have options_path" do
    Rho::RhoConfig.options_path.should == '/app/Settings'
  end
  
  it "should set options_path" do
    oldpath = Rho::RhoConfig.options_path
    Rho::RhoConfig.options_path = '/ops2'
    Rho::RhoConfig.options_path.should == '/ops2'
    Rho::RhoConfig.options_path = oldpath
  end
  
  it "should read arbitrary options" do
    Rho::RhoConfig.arbitrary_option.should == 'rhodes rocks!'
  end
  
  it "should reload" do
    oldpath = Rho::RhoConfig.start_path
    Rho::RhoConfig.reload
    Rho::RhoConfig.start_path.should == oldpath
  end  
end

describe "RhoError" do

  it "should save error code" do
    error = Rho::RhoError.new(Rho::RhoError::ERR_RUNTIME);
    error.code.should == Rho::RhoError::ERR_RUNTIME
  end

  it "should catch the error" do
    error = Rho::RhoError.new(Rho::RhoError::ERR_RUNTIME);
    begin
      raise error
    rescue Rho::RhoError
      $!.code.should == Rho::RhoError::ERR_RUNTIME
    end
  end

end

describe "RhoRuby" do
  
  it "should compute string to_f to test flt_rounds" do
    "100.250".to_f.should == 100.25
  end
  
  it "should compute == on float to test flt_rounds" do
    1.0.should == 1
  end
  
  it "split should have nil" do
    line = "Account|parent_name|44e804f2-4933-4e20-271c-48fcecd9450d||\n"
    parts = line.chomp.split('|')
    
    parts[3].should be_nil
    parts[4].should be_nil
  end
  
  it "should call to_a on string" do
    s = "abcdef"
if !defined? RHO_ME    
    #https://www.pivotaltracker.com/story/show/4292906
    a = s.chars.to_a
    a[0].should == "a"
    a[3].should == "d"
end    
    ch = '1'
    s.each_char do |c|
        ch =  c
    end
    
    ch.should == 'f'
  end
    
  it "should call each on array of hashes"  do
    id = 8775
    providers = [{:IsMandatory=>"true", :RequiresSaveOrderToFile=>"true", :HasFarmPackageAccess=>"true", :OfficeID=>"2354", :OfficeName=>"Poway Realty Services", :ProviderID=>"8775", :ProviderFirstName=>"Jodie", :ProviderLastName=>"Fagan", :ProviderName=>"Jodie Fagan"}]

    provider = {}
    providers.each do |e|
	    if e[:ProviderID].to_i == id
		    provider = e
		    break
	    end
    end
    
    provider.should_not be_nil
    provider[:OfficeID].should == "2354"
  end

  it "should calll []= on string with non-empty string"  do

    file = "some file text with [propertyOwnerName] within"

    replace_str = 'a' 

    file["[propertyOwnerName]"] = replace_str
    
    file.should == "some file text with a within"
  end  

  it "should calll []= on string with empty string"  do

    file = "some file text with [propertyOwnerName] within"

    replace_str = ''

    file["[propertyOwnerName]"] = replace_str
    
    file.should == "some file text with  within"
  end  

  it "should test string split" do 
    s = "password="  
    key, value = s.split('=', 2)
    
    key.should == "password"
    value.should == ""
    
    key1, value1 = s.split('=')
    
    key1.should == "password"
    value1.should be_nil
    
  end
  
  it "should require with diff case" do
    require "Data/reqTest.rb"
    
    ReqTest.get_name.should == "reqTest"
  end
  
end
=begin
describe "String#split with String" do
  it "returns an array of substrings based on splitting on the given string" do
    "mellow yellow".split("ello").should == ["m", "w y", "w"]
  end
  
  it "suppresses trailing empty fields when limit isn't given or 0" do
    "1,2,,3,4,,".split(',').should == ["1", "2", "", "3", "4"]
    "1,2,,3,4,,".split(',', 0).should == ["1", "2", "", "3", "4"]
    "  a  b  c\nd  ".split("  ").should == ["", "a", "b", "c\nd"]
    "hai".split("hai").should == []
    ",".split(",").should == []
    ",".split(",", 0).should == []
  end

  it "returns an array with one entry if limit is 1: the original string" do
    "hai".split("hai", 1).should == ["hai"]
    "x.y.z".split(".", 1).should == ["x.y.z"]
    "hello world ".split(" ", 1).should == ["hello world "]
    "hi!".split("", 1).should == ["hi!"]
  end

  it "returns at most limit fields when limit > 1" do
    "hai".split("hai", 2).should == ["", ""]

    "1,2,,3,4,,".split(',', 2).should == ["1", "2,,3,4,,"]
    "1,2,,3,4,,".split(',', 3).should == ["1", "2", ",3,4,,"]
    "1,2,,3,4,,".split(',', 4).should == ["1", "2", "", "3,4,,"]
    "1,2,,3,4,,".split(',', 5).should == ["1", "2", "", "3", "4,,"]
    "1,2,,3,4,,".split(',', 6).should == ["1", "2", "", "3", "4", ","]

    "x".split('x', 2).should == ["", ""]
    "xx".split('x', 2).should == ["", "x"]
    "xx".split('x', 3).should == ["", "", ""]
    "xxx".split('x', 2).should == ["", "xx"]
    "xxx".split('x', 3).should == ["", "", "x"]
    "xxx".split('x', 4).should == ["", "", "", ""]
  end
  
  it "doesn't suppress or limit fields when limit is negative" do
    "1,2,,3,4,,".split(',', -1).should == ["1", "2", "", "3", "4", "", ""]
    "1,2,,3,4,,".split(',', -5).should == ["1", "2", "", "3", "4", "", ""]
    "  a  b  c\nd  ".split("  ", -1).should == ["", "a", "b", "c\nd", ""]
    ",".split(",", -1).should == ["", ""]
  end
  
  it "defaults to $; when string isn't given or nil" do
    begin
      old_fs = $;
    
      [",", ":", "", "XY", nil].each do |fs|
        $; = fs
        
        ["x,y,z,,,", "1:2:", "aXYbXYcXY", ""].each do |str|
          expected = str.split(fs || " ")
          
          str.split(nil).should == expected
          str.split.should == expected

          str.split(nil, -1).should == str.split(fs || " ", -1)
          str.split(nil, 0).should == str.split(fs || " ", 0)
          str.split(nil, 2).should == str.split(fs || " ", 2)
        end
      end
    ensure
      $; = old_fs
    end    
  end
    
  it "ignores leading and continuous whitespace when string is a single space" do
    " now's  the time  ".split(' ').should == ["now's", "the", "time"]
    " now's  the time  ".split(' ', -1).should == ["now's", "the", "time", ""]

    "\t\n a\t\tb \n\r\r\nc\v\vd\v ".split(' ').should == ["a", "b", "c", "d"]
    "a\x00a b".split(' ').should == ["a\x00a", "b"]
  end
  
  it "splits between characters when its argument is an empty string" do
    "hi!".split("").should == ["h", "i", "!"]
    "hi!".split("", -1).should == ["h", "i", "!", ""]
    "hi!".split("", 2).should == ["h", "i!"]
  end
  
  it "doesn't set $~" do
    $~ = nil
    "x.y.z".split(".")
    $~.should == nil
  end
    
  it "taints the resulting strings if self is tainted" do
    ["", "x.y.z.", "  x  y  "].each do |str|
      ["", ".", " "].each do |pat|
        [-1, 0, 1, 2].each do |limit|
          str.dup.taint.split(pat).each do |x|
            x.tainted?.should == true
          end
          
          str.split(pat.dup.taint).each do |x|
            x.tainted?.should == false
          end
        end
      end
    end    
  end
end

describe "String#split with Regexp" do
  it "divides self on regexp matches" do
    " now's  the time".split(/ /).should == ["", "now's", "", "the", "time"]
    " x\ny ".split(/ /).should == ["", "x\ny"]
    "1, 2.34,56, 7".split(/,\s*/).should == ["1", "2.34", "56", "7"]
    "1x2X3".split(/x/i).should == ["1", "2", "3"]
  end

  it "treats negative limits as no limit" do
    "".split(%r!/+!, -1).should == []
  end
  
  it "suppresses trailing empty fields when limit isn't given or 0" do
    "1,2,,3,4,,".split(/,/).should == ["1", "2", "", "3", "4"]
    "1,2,,3,4,,".split(/,/, 0).should == ["1", "2", "", "3", "4"]
    "  a  b  c\nd  ".split(/\s+/).should == ["", "a", "b", "c", "d"]
    "hai".split(/hai/).should == []
    ",".split(/,/).should == []
    ",".split(/,/, 0).should == []
  end

  it "returns an array with one entry if limit is 1: the original string" do
    "hai".split(/hai/, 1).should == ["hai"]
    "xAyBzC".split(/[A-Z]/, 1).should == ["xAyBzC"]
    "hello world ".split(/\s+/, 1).should == ["hello world "]
    "hi!".split(//, 1).should == ["hi!"]
  end

  it "returns at most limit fields when limit > 1" do
    "hai".split(/hai/, 2).should == ["", ""]

    "1,2,,3,4,,".split(/,/, 2).should == ["1", "2,,3,4,,"]
    "1,2,,3,4,,".split(/,/, 3).should == ["1", "2", ",3,4,,"]
    "1,2,,3,4,,".split(/,/, 4).should == ["1", "2", "", "3,4,,"]
    "1,2,,3,4,,".split(/,/, 5).should == ["1", "2", "", "3", "4,,"]
    "1,2,,3,4,,".split(/,/, 6).should == ["1", "2", "", "3", "4", ","]

    "x".split(/x/, 2).should == ["", ""]
    "xx".split(/x/, 2).should == ["", "x"]
    "xx".split(/x/, 3).should == ["", "", ""]
    "xxx".split(/x/, 2).should == ["", "xx"]
    "xxx".split(/x/, 3).should == ["", "", "x"]
    "xxx".split(/x/, 4).should == ["", "", "", ""]
  end
  
  it "doesn't suppress or limit fields when limit is negative" do
    "1,2,,3,4,,".split(/,/, -1).should == ["1", "2", "", "3", "4", "", ""]
    "1,2,,3,4,,".split(/,/, -5).should == ["1", "2", "", "3", "4", "", ""]
    "  a  b  c\nd  ".split(/\s+/, -1).should == ["", "a", "b", "c", "d", ""]
    ",".split(/,/, -1).should == ["", ""]
  end
  
  it "defaults to $; when regexp isn't given or nil" do
    begin
      old_fs = $;
    
      [/,/, /:/, //, /XY/, /./].each do |fs|
        $; = fs
        
        ["x,y,z,,,", "1:2:", "aXYbXYcXY", ""].each do |str|
          expected = str.split(fs)
          
          str.split(nil).should == expected
          str.split.should == expected

          str.split(nil, -1).should == str.split(fs, -1)
          str.split(nil, 0).should == str.split(fs, 0)
          str.split(nil, 2).should == str.split(fs, 2)
        end
      end
    ensure
      $; = old_fs
    end    
  end
  
  it "splits between characters when regexp matches a zero-length string" do
    "hello".split(//).should == ["h", "e", "l", "l", "o"]
    "hello".split(//, -1).should == ["h", "e", "l", "l", "o", ""]
    "hello".split(//, 2).should == ["h", "ello"]
    
    "hi mom".split(/\s*/).should == ["h", "i", "m", "o", "m"]
  end
  
  it "includes all captures in the result array" do
    "hello".split(/(el)/).should == ["h", "el", "lo"]
    "hi!".split(/()/).should == ["h", "", "i", "", "!"]
    "hi!".split(/()/, -1).should == ["h", "", "i", "", "!", "", ""]
    "hello".split(/((el))()/).should == ["h", "el", "el", "", "lo"]
    "AabB".split(/([a-z])+/).should == ["A", "b", "B"]
  end

  it "does not include non-matching captures in the result array" do
    "hello".split(/(el)|(xx)/).should == ["h", "el", "lo"]
  end
  
  it "doesn't set $~" do
    $~ = nil
    "x:y:z".split(/:/)
    $~.should == nil
  end
  
  it "returns the original string if no matches are found" do
    "foo".split("\n").should == ["foo"]
  end
  
  it "taints the resulting strings if self is tainted" do
    ["", "x:y:z:", "  x  y  "].each do |str|
      [//, /:/, /\s+/].each do |pat|
        [-1, 0, 1, 2].each do |limit|
          str.dup.taint.split(pat).each do |x|
            x.tainted?.should == true
          end
          
          str.split(pat.dup.taint).each do |x|
            x.tainted?.should == false
          end
        end
      end
    end    
  end  
end
=end
