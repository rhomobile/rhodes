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
  it_should_behave_like "rho db initializer"
  
  it "should set source_id attributes" do
    Account.get_source_id.should == "1"
    Case.get_source_id.should == "2"
    Employee.get_source_id.should == "3"
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
    results[0].name.should == "vSpring"
    results[0].industry.should == "Finance"
    results[1].name.should == "Rhomobile"
    results[1].industry.should == "Technology"
    results[4].industry.should == "Technology"
    results[4].name.should == "Mobio India"
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

  it "should retrieve and modify one record" do
    @acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @acct.name.should == "Mobio US"
    @acct.industry.should == "Technology"
    
    @acct.name = "Rhomobile US"
    
    @acct.name.should == "Rhomobile US"
  end
  
  it "should return an empty value for a non-existent attribute" do
    @acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @acct.foobar.should be_nil
  end
end
