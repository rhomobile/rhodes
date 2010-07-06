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
#require 'spec/spec_helper'
require 'rhom'
require 'rho/rhoutils'

def clean_db_data
    ::Rho::RHO.get_user_db().start_transaction
    ::Rho::RHO.get_user_db().delete_all_from_table('client_info')
    ::Rho::RHO.get_user_db().delete_all_from_table('object_values')
    ::Rho::RHO.get_user_db().delete_all_from_table('changed_values')
    
    ::Rho::RHO.get_user_db().commit
end

#TODO:
#BB: issue with sql search : https://www.pivotaltracker.com/story/show/3182398 ; after fix uncomment all RHO_ME comments

describe "Rhom::RhomObject" do
 
  before(:all) do
    @save_sync_types = ::Rho::RHO.get_user_db().select_from_table('sources','name, sync_type')
    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'none'})
  end
  
  before(:each) do
    Rho::RhoUtils.load_offline_data(['client_info','object_values'], 'Spec')
  end

  after(:each) do
    clean_db_data
  end

  after(:all) do
      @save_sync_types.each do |src|
        ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>src['sync_type']}, {'name'=>src['name']})
      end
  end
  
  it "should set source_id attributes" do
    Account.get_source_id.should == "23"
    #Case.get_source_id.should == "1"
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
    results = Account.find(:all, :order => 'name', :orderdir => "DESC")
    results.length.should == 2
    results[0].name.should == "Mobio India"
    results[0].industry.should == "Technology"
    results[1].name.should == "Aeroprise"
    results[1].industry.should == "Technology"
  end
  
  it "should respond to find_all" do
    results = Account.find_all(:order => 'name', :orderdir => "DESC")
    results.length.should == 2
    results[0].name.should == "Mobio India"
    results[0].industry.should == "Technology"
    results[1].name.should == "Aeroprise"
    results[1].industry.should == "Technology"
  end
  
  it "should have correct number of attributes" do
    @account = Account.find(:all, :order => 'name', :orderdir => "DESC").first
  
    @account.vars.size.should == 17
  end
  
  it "should get count of objects" do
    Account.count.should == 2
  end

  it "should get count of objects using find" do
    Account.find(:count).should == 2
  end

  it "should get count of objects using find with condition" do
    Account.find(:count, :conditions => {'name'=>'Aeroprise'}).should == 1
  end

  it "should raise RecordNotFound error if nil given as find argument" do
  
    bExc = false
    begin
      Account.find(nil)
    rescue Exception => e
        bExc = e.is_a?(::Rhom::RecordNotFound)
    end  
    Test_equal( bExc, true )
    
  end

  it "should create multiple records offline" do
    vars = {"name"=>"foobarthree", "industry"=>"entertainment"}
    Account.changed?.should == false
    account = Account.new(vars)
    obj = account.object
    account.save
    Account.changed?.should == true
    account.changed?.should == true
    
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

  it "should update attribs while save" do
    acct = Account.find(:first)
    obj_id = acct.object
    acct.name = 'soccer'
    acct.save
    acct2 = Account.find(obj_id)
    acct2.name.should == 'soccer'
  end
  
  it "should create records with no attribs in database" do
    ::Rho::RHO.get_user_db().delete_all_from_table('object_values')
    res = ::Rho::RHO.get_user_db().select_from_table('object_values',"*")
    res.length.should == 0
    vars = {"name"=>"foobarthree", "industry"=>"entertainment"}
    account = Account.new(vars)
    obj = account.object
    account.save
    acct = Account.find(obj)
    acct.name.should == 'foobarthree'
    acct.industry.should == 'entertainment'
  end
  
  it "should create a record" do
    vars = {"name"=>"some new record", "industry"=>"electronics"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id)
    @account2.object.should =="#{@account1.object}"
    @account2.name.should == vars['name']
    @account2.industry.should == vars['industry']
  end

  it "should create a record with comma" do
    vars = {"name"=>"some new record", "industry"=>"elec'tronics"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id)
    @account2.object.should =="#{@account1.object}"
    @account2.name.should == vars['name']
    @account2.industry.should == vars['industry']
  end
  
  it "should create multiple records" do
    vars = {"name"=>"some new record", "industry"=>"electronics"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id)
    @account2.object.should =="#{@account1.object}"
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

  it "should create a record, then update" do
    vars = {"name"=>"some new record", "industry"=>"electronics"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    @account2 = Account.find(new_id)
    @account2.object.should =="#{@account1.object}"
    @account2.name.should == vars['name']
    @account2.industry.should == vars['industry']
    
    update_attributes = {"industry"=>"electronics2"}
    @account2.update_attributes(update_attributes)

    @account3 = Account.find(new_id)    
    @account3.object.should =="#{@account1.object}"
    @account3.name.should == vars['name']
    @account3.industry.should == update_attributes['industry']

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 2
    
    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'update')
    records.length.should == 0
  end
  
  it "should create a record, then update 2" do
    vars = {"name"=>"some new record"}
    @account1 = Account.new(vars)
    new_id = @account1.object
    @account1.save
    
    @account2 = Account.find(new_id)
    @account2.object.should =="#{@account1.object}"
    @account2.name.should == vars['name']
    
    update_attributes = {"industry"=>"electronics2"}
    @account2.industry = update_attributes['industry']
    @account2.save
    
    @account3 = Account.find(new_id)    
    @account3.object.should =="#{@account1.object}"
    @account3.name.should == vars['name']
    @account3.industry.should == update_attributes['industry']

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 2
    
    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'update')
    records.length.should == 0
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
    @account.name.should == "Mobio US"
    @account.industry.should == "Electronics"
    
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
  
  it "should _NOT_ set 'attrib_type' field for a record" do
    new_attributes = {"attrib_type"=>"Partner"}
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
  
  #it "should update an attribute to nil" do
  #  new_attributes = {"name"=>nil}
  #  @account = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
  #  @account.update_attributes(new_attributes)
    
  #  @new_acct = Account.find('44e804f2-4933-4e20-271c-48fcecd9450d')
  
  #  @new_acct.name.should be_nil
  #  @new_acct.industry.should == "Technology"
  #end
  
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
    
    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'update')
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
  
  it "should find with conditions" do
    @accts = Account.find(:all, :conditions => {'industry' => 'Technology'}, :order => 'name', :orderdir => "DESC")
    @accts.length.should == 2
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"
  end
  
  it "should find with multiple conditions" do
    @accts = Account.find(:all, :conditions => {'name' => 'Mobio India', 'industry' => 'Technology'})
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

if !defined? RHO_ME
  it "should find with SQL multiple conditions" do
    @acct = Account.find(:first, :conditions => [ "name = ? AND industry = ?", "'Mobio India'", "'Technology'" ])
    @acct.name.should == "Mobio India"
    @acct.industry.should == "Technology"
  end
end

  it "should find with advanced conditions" do
    query = '%IND%'    
    @accts = Account.find( :all, 
       :conditions => { 
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query, 
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query}, 
        :op => 'OR', :select => ['name','industry'])
  
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should find with group of advanced conditions" do
    query = '%IND%'    
    cond1 = {
       :conditions => { 
            {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query, 
            {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query}, 
       :op => 'OR'
    }
    cond2 = {
        :conditions => { 
            {:name=>'description', :op=>'LIKE'} => 'Hello%'}
    }

    @accts = Account.find( :all, 
       :conditions => [cond1, cond2], 
       :op => 'AND', 
       :select => ['name','industry','description'])
  
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should not find with group of advanced conditions" do
    query = '%IND%'    
    cond1 = {
       :conditions => { 
            {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query, 
            {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query}, 
       :op => 'OR'
    }
    cond2 = {
        :conditions => { 
            {:name=>'description', :op=>'LIKE'} => 'Hellogg%'}
    }

    @accts = Account.find( :all, 
       :conditions => [cond1, cond2], 
       :op => 'AND', 
       :select => ['name','industry'])
  
    @accts.length.should == 0
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

  it "should desc order by column" do
    @accts = Account.find(:all, :order => 'name', :orderdir => 'DESC')
    
    @accts.first.name.should == "Mobio India"
    @accts.first.industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"
  end
  
  it "should return records when order by is nil for some records" do
    @accts = Account.find(:all, :order => 'shipping_address_country')
    @accts.length.should == 2
    
    if ( @accts[1].name == "Aeroprise" )
        @accts[1].name.should == "Aeroprise"
    else
        @accts[0].name.should == "Aeroprise"
    end        
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

=begin  
  it "should support blob file type" do
    @acct = Account.new({'image_uri'=>"/db/images/mynewimage.png"})
    @acct.name = "my new acct"
    @acct.save
    @res = ::Rho::RHO.get_user_db().select_from_table('object_values','*', 'attrib_type' => "blob.file")
    @res.length.should == 1
  end
=end  

  it "should include only selected column" do
    @accts = Account.find(:all, :select => ['name'], :order => 'name', :orderdir => 'DESC' )
    
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should be_nil
    @accts[0].vars.length.should == 3
  end
  
  it "should include only selected columns" do
    @accts = Account.find(:all, :select => ['name','industry'], :order => 'name', :orderdir => 'DESC')
    
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[0].shipping_address_street.should be_nil
    @accts[0].vars.length.should == 4
  end
  
  it "should include selected columns and conditions" do
    @accts = Account.find(:all, :conditions => {'name' => 'Mobio India'}, :select => ['name','industry'])
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[0].shipping_address_street.should be_nil
    @accts[0].vars.length.should == 4
  end
  
    #it "should perform find with select and merged conditions" do
    #@accts = Account.find(:all, :conditions => {'name' => 'Mobio India'}, :select => ['industry'])
    #@accts.length.should == 1
    #@accts[0].name.should == "Mobio India"
    #@accts[0].industry.should == "Technology"
    #@accts[0].shipping_address_street.should be_nil
    #@accts[0].vars.length.should == 3
    #end
  
  it "should support find with conditions => nil" do
    @accts = Account.find(:all, :conditions => {'description' => nil})
    @accts.length.should == 1
    @accts[0].name.should == "Aeroprise"
    @accts[0].industry.should == "Technology"
  end

if !defined? RHO_ME  
  
  it "should support sql conditions arg" do
    @accts = Account.find(:all, :conditions => "name = 'Mobio India'")
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should support complex sql conditions arg" do
    @accts = Account.find(:all, :conditions => "name like 'Mobio%'")
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end
  
  it "should support sql conditions single filter" do
    @accts = Account.find(:all, :conditions => ["name like ?", "'Mob%'"])
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end
  
  it "should support sql conditions with multiple filters" do
    @accts = Account.find(:all, :conditions => ["name like ? and industry like ?", "'Mob%'", "'Tech%'"])
    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end
end  
end

if !defined? RHO_ME  
describe "Rhom#paginate" do

  before(:all) do
    @save_sync_types = ::Rho::RHO.get_user_db().select_from_table('sources','name, sync_type')
    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'none'})
  end
  
  before(:each) do
      Rho::RhoUtils.load_offline_data(['object_values'], 'Spec/pagination')
  end

  after(:each) do
    clean_db_data
  end

  after(:all) do
      @save_sync_types.each do |src|
        ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>src['sync_type']}, {'name'=>src['name']})
      end
  end

    @expected = [
                {:object => '3788304956', :name => 'c2z5izd8w9', :address => '6rd9nv8dml', :industry => 'hxua4d6ttl'},
                #{:object => '8763523348', :name => '39afj8vbj6', :address => 'x7jincp3xj', :industry => 'sge128jo9o'},    
                {:object => '7480317731', :name => '79nqr7ekzr', :address => 'emv1tezmdf', :industry => '1zg7f7q6ib'},
                {:object => '9897778878', :name => 'n5qx54qcye', :address => 'stzc1x7upn', :industry => '9kdinrjlcx'}]

    it "should support paginate with no options" do
      3.times do |x|
        @accts = Account.paginate(:page => x)
        @accts.length.should == 10
        @accts[9].object.should == "#{@expected[x][:object]}"
        @accts[9].name.should == @expected[x][:name]
        @accts[9].address.should == @expected[x][:address]
        @accts[9].industry.should == @expected[x][:industry]
      end
      @accts = Account.paginate(:page => 3)
      @accts.length.should == 0
    end
    
    it "should support paginate with options" do
      @accts = Account.paginate(:page => 0, :per_page => 20)
      @accts.length.should == 20
      @accts[9].object.should == "#{@expected[0][:object]}"
      @accts[9].name.should == @expected[0][:name]
      @accts[9].address.should == @expected[0][:address]
      @accts[9].industry.should == @expected[0][:industry]
      @accts = Account.paginate(:page => 3)
      @accts.length.should == 0
    end
    
    it "should support paginate with options and conditions" do
      @accts = Account.paginate(:page => 0, :per_page => 20, :conditions => {'name' => 'c2z5izd8w9'})
      @accts.length.should == 1
      @accts[0].object.should == "#{@expected[0][:object]}"
      @accts[0].name.should == @expected[0][:name]
      @accts[0].address.should == @expected[0][:address]
      @accts[0].industry.should == @expected[0][:industry]
    end

    it "should support paginate with options, conditions and order" do
      @accts = Account.paginate(:page => 0, :per_page => 1, :conditions => {'name' => 'test'}, :order=> 'name')
      @accts.length.should == 1

      @accts = Account.paginate(:page => 1, :per_page => 1, :conditions => {'name' => 'test'}, :order=> 'name')
      @accts.length.should == 1

      @accts = Account.paginate(:page => 2, :per_page => 1, :conditions => {'name' => 'test'}, :order=> 'name')
      @accts.length.should == 1

      @accts = Account.paginate(:page => 3, :per_page => 1, :conditions => {'name' => 'test'}, :order=> 'name')
      @accts.length.should == 0
    end
    
    it "should support paginate with options and order" do
      @accts = Account.paginate(:per_page => 20, :order=> 'name')
      @accts.length.should == 20

      @accts2 = Account.paginate(:per_page => 20, :order=> 'name', :page => 1)
      @accts2.length.should == 10
      
      @accts3 = Account.paginate(:per_page => 20, :order=> 'name', :page => 2)
      @accts3.length.should == 0
    end
end
end
