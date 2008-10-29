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
$:.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))
require 'spec/spec_helper'

describe "RhomObjectFactory" do
  
  attr_accessor :rhom
  
  before(:all) do
    Object::const_set("RHO_SOURCES", {"Account"=>1, "Case"=>2, "Employee"=>3}) unless defined? RHO_SOURCES
    Object::const_set("SYNC_DB_FILE", "../../build/syncdbtest.sqlite") unless defined? SYNC_DB_FILE
    FileUtils.mkdir_p(File.join(File.dirname(__FILE__),'../build'))
  end
  
  before(:each) do
    FileUtils.cp_r(File.join(File.dirname(__FILE__), './syncdbtest.sqlite'), 
                   File.join(File.dirname(__FILE__),'../build/syncdbtest.sqlite'))
    @rhom = Rhom::Rhom.new
  end
  
  after(:each) do 
    FileUtils.rm_rf(File.join(File.dirname(__FILE__), '../build/syncdbtest.sqlite'))
    @rhom = nil
  end
  
  after(:all) do
    FileUtils.rm_rf(File.join(File.dirname(__FILE__),'../build'))
  end
  
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
    results.length.should == 5
    "60".should == results[0].case_number
    "hire another engineer".should == results[4].name
  end
  
  it "should retrieve Account models" do
    results = Account.find(:all)
    results.each_with_index do |result,i|
      puts "result[#{i}]: " + result.inspect
    end
    results.length.should == 5
    #array_print(results)
    
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
    account.object.should == "272128608299468889014"
  end
  
  it "should create a record" do
    vars = {"name"=>"some new record", "industry"=>"electronices"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id).first
    @account2.object.should =="{#{@account1.object}}"
    @account2.name.should == vars['name']
    @account2.industry.should == vars['industry']
  end
  
  it "should destroy a record" do
    count = Account.find(:all).size
    @account = Account.find(:all)[0]
    destroy_id = @account.object
    @account.destroy
    @account_nil = Account.find(destroy_id)
    
    @account_nil.size.should == 0
    new_count = Account.find(:all).size
    
    (count - 1).should == new_count
  end
  
  it "should partially update a record" do
    new_attributes = {"name"=>"Mobio US"}
    @account = Account.find(:all).first
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find(:all).first
    
    "Mobio US".should == @new_acct.name
    "Technology".should == @new_acct.industry
  end
  
  it "should fully update a record" do
    new_attributes = {"name"=>"Mobio US", "industry"=>"Electronics"}
    @account = Account.find(:all).first
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find(:all).first
    
    "Mobio US".should == @new_acct.name
    "Electronics".should == @new_acct.industry
  end
end
