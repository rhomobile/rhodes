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
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
require File.dirname(__FILE__) + "/spec_helper"

describe "RhomObjectFactory" do

  it_should_behave_like "rho initializer"
  
  before(:each) do
    FileUtils.cp_r('spec/syncdbtest.sqlite','build/syncdbtest.sqlite')
  end
  
  after(:each) do 
    FileUtils.rm_rf('build/syncdbtest.sqlite')
    @rhom = nil
  end
  
  it "should set source_id attributes" do
    "1".should == Account.get_source_id
    "2".should == Case.get_source_id
    "3".should == Employee.get_source_id
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
    results.length.should == 7
    results[0].case_number.should == "57"
    results[4].name.should == "implement SugarCRM sample app"
  end
  
  it "should retrieve Account models" do
    results = Account.find(:all)
    results.length.should == 5
    "vSpring".should == results[0].name
    "Finance".should == results[0].industry
    "Rhomobile".should == results[1].name
    "Technology".should == results[1].industry
    "Technology".should == results[4].industry
    "Mobio India".should == results[4].name
  end
  
  it "should have correct number of attributes" do
    @account = Account.find(:all).first

    @account.instance_variables.size.should == 36
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
    @account2 = Account.find(new_id)
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
    @account = Account.find("44e804f2-4933-4e20-271c-48fcecd9450d")
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find("44e804f2-4933-4e20-271c-48fcecd9450d")
    
    puts "new acct: #{@new_acct.inspect}"
    @new_acct.name.should == "Mobio US"
    @new_acct.industry.should == "Technology"
  end
  
  it "should fully update a record" do
    new_attributes = {"name"=>"Mobio US", "industry"=>"Electronics"}
    @account = Account.find(:all).first
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find(:all).first
    
    @new_acct.name.should == "Mobio US"
    @new_acct.industry.should == "Electronics"
  end
end
