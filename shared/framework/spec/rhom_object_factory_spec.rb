#
#  rhom_object_factory_spec.rb
#  rhodes
#
#  Copyright (C) 2008 Lars Burgess. All rights reserved.
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
$:.unshift File.join(File.dirname(__FILE__),'..','lib')

require 'spec/spec_helper'

describe "RhomObjectFactory" do
  
  attr_accessor :rhom
  
  Object::const_set("RHO_SOURCES", {"Account"=>1, "Case"=>2, "Employee"=>3}) unless defined? RHO_SOURCES
  Object::const_set("SYNC_DB_FILE", "../../spec/syncdbtest.sqlite")
  @rhom = Rhom::Rhom.new
  
  def array_print(arr)
    arr.each_with_index do |x,i|
      puts "arr[#{i}] = #{x.inspect}"
    end
  end
  
  it "should set source_id attributes" do
    "1".should == Account.get_source_id
    "2".should == Case.get_source_id
  end
  
  it "should dynamically assign values" do
    account = Account.new
    account.name = 'hello name'
    account.industry = 'hello industry'
    account.object = '3560c0a0-ef58-2f40-68a5-fffffffffffff'
    account.value = 'xyz industries'
    account.name.should == 'hello name'
    account.industry.should == 'hello industry'
    account.object.should == '3560c0a0-ef58-2f40-68a5-fffffffffffff'
    account.value.should == 'xyz industries'
  end
  
  it "should retrieve Case models" do
    results = Case.find(:all)
    array_print(results)
    results.length.should == 7
    "58".should == results[0].case_number
    "hire another engineer".should == results[6].name
  end
  
  it "should retrieve Account models" do
    results = Account.find(:all)
    results.each_with_index do |result,i|
      puts "result[#{i}]: " + result.inspect
    end
    results.length.should == 5
    array_print(results)
    
    "Mobio India".should == results[0].name
    "Technology".should == results[0].industry
    "Aeroprise".should == results[1].name
    "Technology".should == results[1].industry
    "Electronics".should == results[4].industry
    "Mirapath".should == results[4].name
  end
  
  it "should calculate same djb_hash" do
    vars = {"name"=>"foobarthree", "industry"=>"entertainment"}
    account = Account.new(vars)
    puts 'account: ' + account.inspect
    account.object.should == "272128608299468889014"
  end
end
