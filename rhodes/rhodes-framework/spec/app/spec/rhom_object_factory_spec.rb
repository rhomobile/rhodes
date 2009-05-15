#
#  rhom_object_factory_spec.rb
#  rhodes
#
#  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
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
require 'spec/spec_helper'

describe "Rhom::RhomObjectFactory" do
  
  it_should_behave_like "rhom initializer"
  
  before do
    SyncEngine.stub!(:dosync).and_return(true)
  end
  
  it "should set source_id attributes" do
    Account.get_source_id.should == "1"
    Case.get_source_id.should == "2"
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
    results.length.should == 1
    results[0].case_number.should == "58"
  end
  
  it "should retrieve Account models" do
    results = Account.find(:all)
    results.length.should == 2
    results[0].name.should == "Mobio India"
    results[0].industry.should == "Technology"
    results[1].name.should == "Aeroprise"
    results[1].industry.should == "Technology"
  end
  
  it "should have correct number of attributes" do
    @account = Account.find(:all).first
  
    @account.instance_variables.size.should == 36
  end
  
  it "should get count of objects" do
    Account.count.should == 2
  end
  
  # it "should raise RecordNotFound error if nil given as find argument" do
  #   lambda {
  #     get 'index'
  #   }.should_raise(Rhom::RecordNotFound)
  # end
  
  it "should create multiple records offline" do
    vars = {"name"=>"foobarthree", "industry"=>"entertainment"}
    account = Account.new(vars)
    obj = account.object
    account.save
    acct = Account.find(obj)
    acct.name.should == 'foobarthree'
    acct.industry.should == 'entertainment'
    
    account = Account.new
    obj = account.object
    account.name = 'foobarfour'
    account.industry = 'solar'
    account.save
    
    acct = Account.find(obj)
    acct.name.should == 'foobarfour'
    acct.industry.should == 'solar'
  end
  
  it "should create a record" do
    vars = {"name"=>"some new record", "industry"=>"electronics"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id)
    @account2.object.should =="{#{@account1.object}}"
    @account2.name.should == vars['name']
    @account2.industry.should == vars['industry']
  end
  
  it "should create multiple records" do
    vars = {"name"=>"some new record", "industry"=>"electronics"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id)
    @account2.object.should =="{#{@account1.object}}"
    @account2.name.should == vars['name']
    @account2.industry.should == vars['industry']
  end
  
  it "should create multiple records with unique ids" do
    ids = []
    10.times do |i|
      vars = {"name"=>"some new record#{rand.to_s}", "industry"=>"electronics#{rand.to_s}"}
      @acct = Account.new(vars)
      ids << @acct.object
      @acct.save
      @acct = Account.find(ids[i])
      @acct.name.should == vars['name']
      @acct.industry.should == vars['industry']
    end
    ids.uniq.length.should == 10
  end
  
  it "should destroy a record" do
    count = Account.find(:all).size
    @account = Account.find(:all)[0]
    destroy_id = @account.object
    @account.destroy
    @account_nil = Account.find(destroy_id)
    @account_nil.should be_nil
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
  
  it "should set <something>_type_<something> or <something>_object_<something> field for a record" do
    new_attributes = {"account_type"=>"Partner", 
                      "type_acct"=>"Customer", 
                      "object_acct"=>"new object",
                      "acct_object"=>"same object"}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes)
  
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
  
    @new_acct.name.should == "Mobio India"
    @new_acct.account_type.should == "Partner"
    @new_acct.type_acct.should == "Customer"
    @new_acct.object_acct.should == "new object"
    @new_acct.acct_object.should == "same object"
  end
  
  it "should _NOT_ set 'type' field for a record" do
    new_attributes = {"type"=>"Partner"}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes)
  
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
  
    @new_acct.name.should == "Mobio India"
    @new_acct.instance_variables.each do |var|
      var.to_s.gsub(/@/,'').match('\btype\b').should be_nil
    end
  end
  
  it "should update an attribute that was previously nil" do
    new_attributes = {"new_name"=>"Mobio Europe"}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @new_acct.new_name.should == "Mobio Europe"
    @new_acct.name.should == "Mobio India"
    @new_acct.industry.should == "Technology"
  end
  
  it "should update an attribute to nil" do
    new_attributes = {"name"=>nil}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
  
    @new_acct.name.should be_nil
    @new_acct.industry.should == "Technology"
  end
  
  it "should update an attribute to empty string" do
    new_attributes = {"name"=>""}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes)
    
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
  
    @new_acct.name.should == ""
    @new_acct.industry.should == "Technology"
  end
  
  it "should store only last updated value for attrib" do
    new_attributes1 = {"new_name"=>"Mobio Europe"}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes1)
    
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @new_acct.new_name.should == "Mobio Europe"
    @new_acct.name.should == "Mobio India"
    @new_acct.industry.should == "Technology"
    
    new_attributes2 = {"new_name"=>"Mobio Asia"}
    @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    @account.update_attributes(new_attributes2)
    
    @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @new_acct.new_name.should == "Mobio Asia"
    @new_acct.name.should == "Mobio India"
    @new_acct.industry.should == "Technology"
    
    records = Rhom::RhomDbAdapter::select_from_table('object_values','*', 'update_type' => 'update')
    records.length.should == 1
  end
  
  it "should retrieve and modify one record" do
    @acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @acct.name.should == "Mobio India"
    @acct.industry.should == "Technology"
    
    @acct.name = "Rhomobile US"
    
    @acct.name.should == "Rhomobile US"
  end
  
  it "should return an empty value for a non-existent attribute" do
    @acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
    
    @acct.foobar.should be_nil
  end
  
  it "should respond to ask" do
    question = 'Rhodes'
    Account.ask(question)
    res = Rhom::RhomDbAdapter::select_from_table('object_values','*', 'update_type' => 'ask')
    res.length.should == 1
    
    res[0]['attrib'].should == 'question'
    res[0]['value'].should == question
  end
  
  it "should respond to ask with last question only" do
    question = 'Rhodes'
    Account.ask(question)
    res = Rhom::RhomDbAdapter::select_from_table('object_values','*', 'update_type' => 'ask')
    res.length.should == 1
    
    res[0]['attrib'].should == 'question'
    res[0]['value'].should == question
    
    question = 'Ruby on Rails'
    question_encoded = 'Ruby%20on%20Rails'
    Account.ask(question)
    res = Rhom::RhomDbAdapter::select_from_table('object_values','*', 'update_type' => 'ask')
    res.length.should == 1
    
    res[0]['attrib'].should == 'question'
    res[0]['value'].should == question_encoded
  end
  
  it "should encode ask params" do
    question = 'where am i?'
    question_encoded = 'where%20am%20i%3F'
    Account.ask(question)
    @res = Rhom::RhomDbAdapter::select_from_table('object_values','*', 'update_type' => 'ask')
    @res.length.should == 1
    
    @res[0]['attrib'].should == 'question'
    @res[0]['value'].should == question_encoded
  end
  
  it "should store all ask db operations as query" do
    question = 'where am i?'
    question_encoded = 'where%20am%20i%3F'
    Question.ask(question) 
   
    @question = Question.find(:first)
    @question.update_attributes({"question"=>"i am here"})
    
    @res = Rhom::RhomDbAdapter::select_from_table('object_values','*', {'update_type' => 'query', 'source_id' => 400})
    @res.length.should == 1
    @res[0]['attrib'].should == 'question'
    @res[0]['value'].should == 'i am here'
    
    ['create','update','delete'].each do |u_type|
      @res = Rhom::RhomDbAdapter::select_from_table('object_values','*', {'update_type' =>u_type, 'source_id' => 400})
      @res.length.should == 0
    end
  end
  
  it "should delete ask records without delete sync operation" do
    question = 'where am i?'
    question_encoded = 'where%20am%20i%3F'
    Question.ask(question) 
   
    @question = Question.find(:first)
    @question.destroy
    
    ['query','create','update','delete'].each do |u_type|
      @res = Rhom::RhomDbAdapter::select_from_table('object_values','*', {'update_type' =>u_type, 'source_id' => 400})
      @res.length.should == 0
    end
  end
  
  it "should find with conditions" do
    @accts = Account.find(:all, :conditions => {'industry' => 'Technology'})
    @accts.length.should == 2
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"
  end
  
  it "should find first with conditions" do
    @mobio_ind_acct = Account.find(:first, :conditions => {'name' => 'Mobio India'})
    @mobio_ind_acct.name.should == "Mobio India"
    @mobio_ind_acct.industry.should == "Technology"
  end
  
  it "should order by column" do
    @accts = Account.find(:all, :order => 'name')
    
    @accts.first.name.should == "Aeroprise"
    @accts.first.industry.should == "Technology"
    @accts[1].name.should == "Mobio India"
    @accts[1].industry.should == "Technology"
  end
  
  it "should return records when order by is nil for some records" do
    @accts = Account.find(:all, :order => 'shipping_address_country')
    @accts.length.should == 2
    @accts[1].name.should == "Aeroprise"
  end
  
  it "should delete_all" do
    Account.delete_all
    
    Account.find(:all).length.should == 0
  end
  
  it "should delete_all with conditions" do
    Account.delete_all(:conditions => {'name' => 'Mobio India'})
    
    @accts = Account.find(:all, :conditions => {'name' => 'Mobio India'})
    @accts.length.should == 0
  end
  
  it "should delete_all with conditions across objects" do
    Account.delete_all(:conditions => {'industry' => 'Technology'})
    
    @accts = Account.find(:all, :conditions => {'industry' => 'Technology'})
    @accts.length.should == 0
    
    @accts = Account.find(:all)
    
    @accts.length.should == 0
  end
  
  it "should support blob file type" do
    @acct = Account.new({'image_uri'=>"/db/images/mynewimage.png"})
    @acct.name = "my new acct"
    @acct.save
    @res = Rhom::RhomDbAdapter::select_from_table('object_values','*', 'attrib_type' => "blob.file")
    @res.length.should == 1
  end
  
  it "should include only selected column" do
    @accts = Account.find(:all, :select => ['name'])
    
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should be_nil
    @accts[0].instance_variables.length.should == 2
  end
  
  it "should include only selected columns" do
    @accts = Account.find(:all, :select => ['name','industry'])
    
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[0].shipping_address_street.should be_nil
    @accts[0].instance_variables.length.should == 3
  end
  
  it "should include selected columns and conditions" do
    @accts = Account.find(:all, :conditions => {'name' => 'Mobio India'}, :select => ['name','industry'])
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[0].shipping_address_street.should be_nil
    @accts[0].instance_variables.length.should == 3
  end
end
