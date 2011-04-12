#
#  syncengine_spec.rb
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
require 'rho/rho'

def getProduct
    return Product_s if $spec_settings[:schema_model]
    
    Product
end

def getProduct_str
    return 'Product_s' if $spec_settings[:schema_model]
    
    'Product'
end

def getCustomer
    return Customer_s if $spec_settings[:schema_model]
    
    Customer
end

def getCustomer_str
    return 'Customer_s' if $spec_settings[:schema_model]
    
    'Customer'
end

def getTestDB
    ::Rho::RHO.get_db_partitions['user']
end

SYNC_SERVER_URL = 'http://rhodes-store-server.heroku.com/application'
#SYNC_SERVER_URL = 'http://localhost:9292/application'

describe "SyncEngine_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    
    SyncEngine.set_syncserver(SYNC_SERVER_URL)

    Rho::RHO.load_all_sources()

    @save_sync_types = ::Rho::RHO.get_user_db().select_from_table('sources','name, sync_type')
    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'none'})
    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'incremental'}, {'name'=>getProduct_str})
    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'incremental'}, {'name'=>getCustomer_str})
  end
  
  after(:all)  do
      @save_sync_types.each do |src|
        ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>src['sync_type']}, {'name'=>src['name']})
      end
    
  end
  
  after (:each) do
    Rho::RhoConfig.syncserver = SYNC_SERVER_URL
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", "" )        
    ::Rho::RHO.get_user_db().delete_all_from_table('changed_values')
    
  end
if !defined?(RHO_WP7)    
  it "should update syncserver at runtime" do
  
    dbRes = ::Rho::RHO.get_user_db().select_from_table('client_info','token,token_sent')
    dbRes.length.should == 0
  
    saveSrv =  Rho::RhoConfig.syncserver
    
    SyncEngine.set_syncserver('')
    Rho::RhoConfig.syncserver.should == ''
    
    SyncEngine.set_syncserver('http://example.com/sources/')
    Rho::RhoConfig.syncserver.should == 'http://example.com/sources/'
    
    SyncEngine.set_syncserver(saveSrv)
    Rho::RhoConfig.syncserver.should == saveSrv
  end
end
  it "should not sync without login" do
    SyncEngine.logged_in.should == 0
  
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_CLIENTISNOTLOGGEDIN

  end
=begin
  it "should update sources from database" do
    uniq_sources = Rho::RhoConfig::sources.values  
    
    ::Rho::RHO.get_instance().load_server_sources("{\"partition\":\"user\"}")
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    
    #uniq_sources.should == Rho::RhoConfig::sources.values  
  end
=end  

  it "should login" do
    
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    SyncEngine.logged_in.should == 1
  end

  it "should sync Product" do
    SyncEngine.logged_in.should == 1
    dbRes = ::Rho::RHO.get_user_db().select_from_table('client_info','token,token_sent, client_id')
    puts "dbRes : #{dbRes}"
    dbRes.length.should == 1
    dbRes[0]['token_sent'].should == 0
    dbRes[0]['token'].should be_nil
    dbRes[0]['client_id'].should be_nil
  
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
  end

  it "should sync Product by name" do
    SyncEngine.logged_in.should == 1

    SyncEngine.set_notification(getProduct.get_source_id.to_i(), "/app/Settings/sync_notify", "")

    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync_source( getProduct_str )
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

  it "should search Product" do
    #TODO: add search to sync adapter
    SyncEngine.logged_in.should == 1

    _search_id = Time.now.to_i.to_s

    search_res = getProduct.search(
      :from => 'search',
      :search_params => { :filterData => "Test", :search_id => _search_id },
      :offset => 0,
      :max_results => 1000,
      :progress_step => 10,
      :callback => '/app/Contact/search_callback',
      :callback_param => "")
  
    res = ::Rho::RhoSupport::parse_query_parameters search_res
    res['status'].should == 'complete'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

  it "should sync all" do
    SyncEngine.logged_in.should == 1
  
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync

    res['status'].should == 'complete'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

  it "should create new Product" do
    SyncEngine.logged_in.should == 1
  
    item = getProduct.create({:name => 'Test'})
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    item2 = getProduct.find(item.object)
    item2.vars.should == item.vars

    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item3 = getProduct.find(item.object)
    item3.should be_nil
  end

  it "should create new Product with Customers" do
    SyncEngine.logged_in.should == 1
  
    cust1 = getCustomer.create( {:first => "CustTest1"})
    cust2 = getCustomer.create( {:first => "CustTest2"})
    
    @product_test_name = Rho::RhoConfig.generate_id().to_s
    item = getProduct.create({:name => @product_test_name, :quantity => cust1.object, :sku => cust2.object})
    item2 = getProduct.find(item.object)
    item2.vars.should == item.vars
    
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync
    res['status'].should == 'complete'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    prod = getProduct.find(:first, :conditions => {:name => @product_test_name})
    prod.should_not == nil
    prod.object.should_not == item.object
    prod.quantity.should_not == cust1.object
    prod.sku.should_not == cust2.object

    cust11 = getCustomer.find(prod.quantity);
    cust11.should_not == nil    
    cust11.first.should == cust1.first
    
    cust22 = getCustomer.find(prod.sku);
    cust22.should_not == nil    
    cust22.first.should == cust2.first
    
  end
if !defined?(RHO_WP7)
  it "should process error sync" do

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*')
    records.length.should == 0
    
    cust1 = getCustomer.create( {:first => "CustTest1"})

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*')
    records.length.should_not == 0

    Rho::RhoConfig.syncserver = 'http://rhodes-store-server.heroku2.com/application'
    res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync
    res['status'].should == 'error'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NETWORK

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*')
    records.length.should_not == 0

    Rho::RhoConfig.syncserver = SYNC_SERVER_URL
    res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*')
    records.length.should == 0
    
  end
end  
  it "should modify Product" do
    SyncEngine.logged_in.should == 1
  
    item = getProduct.find(:first, :conditions => {:name => 'Test'})
    item.should_not == nil
    saved_obj = item.object
    
    new_sku = item.sku ? item.sku : ""
    new_sku += "_TEST"
    item.sku = new_sku
    item.save
    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item2 = getProduct.find(saved_obj)
    item2.sku.should == new_sku
    
  end

  it "should delete all Test Product" do
    SyncEngine.logged_in.should == 1
  
    items = getProduct.find(:all, :conditions => {:name => 'Test'})
    items.should_not == nil
    items.each do |item|
        item.destroy
    end    

    items = getProduct.find(:all, :conditions => {:name => @product_test_name})
    items.should_not == nil
    items.each do |item|
        item.destroy
    end    

    items = getCustomer.find(:all, :conditions => {:first => "CustTest1"})
    items.should_not == nil
    items.each do |item|
        item.destroy
    end    

    items = getCustomer.find(:all, :conditions => {:first => "CustTest2"})
    items.should_not == nil
    items.each do |item|
        item.destroy
    end    

    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync
    res['status'].should == 'complete'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item2 = getProduct.find(:first, :conditions => {:name => 'Test'})
    item2.should == nil
    item2 = getProduct.find(:first, :conditions => {:name => @product_test_name})
    item2.should == nil

    item2 = getCustomer.find(:first, :conditions => {:first => 'CustTest1'})
    item2.should == nil
    item2 = getCustomer.find(:first, :conditions => {:first => 'CustTest2'})
    item2.should == nil
    
  end

  it "should client register" do
    if System.get_property('device_name') == 'Win32' || System.get_property('platform') == 'Blackberry'
        dbRes = ::Rho::RHO.get_user_db().select_from_table('client_info','token,token_sent, client_id')
        dbRes.length.should == 1
        dbRes[0]['token_sent'].should == 1
        
        dbRes[0]['token'].should_not be_nil
        dbRes[0]['token'].length().should > 0

        dbRes[0]['client_id'].should_not be_nil        
        dbRes[0]['client_id'].length().should > 0
    end
  end

  it "should reset data after login with different user" do
    SyncEngine.logged_in.should == 1    

    item = getProduct.create({:name => 'Test1'})
    item2 = getProduct.find(item.object)
    item2.vars.should == item.vars

    SyncEngine.logout
    SyncEngine.logged_in.should == 0
    
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login('lars1', 'larspass', "/app/Settings/login_callback")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    SyncEngine.logged_in.should == 1
    
    item2 = getProduct.find(item.object)    
    item2.should be_nil
  end

  it "should process create-error : delete" do    
 
    item = getProduct.create({:name => 'Test', :brand => "Rho"})
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    item.update_attributes({:price => "123"})

    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"" + item.object + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + item.object + "-error\":{\"message\":\"error create\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    
    res['server_errors'].should_not be_nil
    res['server_errors']['create-error'].should_not be_nil
    res['server_errors']['create-error'][item.object].should_not be_nil    
    res['server_errors']['create-error'][item.object]['message'].should == "error create"
    res['server_errors']['create-error'][item.object]['attributes']['name'].should == "wrongname"

    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2

    item.update_attributes({:price => "456"})
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records2.length.should == 1
    records2[0]['attrib'].should == 'price'        
    records2[0]['sent'].should == 0

    getProduct.on_sync_create_error( res['server_errors']['create-error'].keys(), :delete)
    
    records3 = getTestDB().select_from_table('changed_values','*', 'object' => item.object)
    records3.length.should == 0
    records4 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records4.length.should == 0
    
    item = getProduct.find(item.object)
    item.should be_nil
  end
  
  it "should process create-error : recreate" do    
 
    item = getProduct.create({:name => 'Test', :brand => "Rho"})
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    item.update_attributes({:price => "123"})

    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"" + item.object + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + item.object + "-error\":{\"message\":\"error create\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    
    res['server_errors'].should_not be_nil
    res['server_errors']['create-error'].should_not be_nil
    res['server_errors']['create-error'][item.object].should_not be_nil    
    res['server_errors']['create-error'][item.object]['message'].should == "error create"
    res['server_errors']['create-error'][item.object]['attributes']['name'].should == "wrongname"

    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2

    item.update_attributes({:price => "456"})
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records2.length.should == 1
    records2[0]['attrib'].should == 'price'        
    records2[0]['sent'].should == 0

    getProduct.on_sync_create_error( res['server_errors']['create-error'].keys(), :recreate)
    
    records3 = getTestDB().select_from_table('changed_values','*', 'object' => item.object)
    records3.length.should == 1
    records3[0]['attrib'].should == 'object'        
    
    records4 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records4.length.should == 0
    
    item = getProduct.find(item.object)
    item.should_not be_nil
    
  end

  it "should process create-error : recreate deleted item" do    
 
    item = getProduct.create({:name => 'Test', :brand => "Rho"})
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    item.update_attributes({:price => "123"})

    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'
    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"" + item.object + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + item.object + "-error\":{\"message\":\"error create\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    
    res['server_errors'].should_not be_nil
    res['server_errors']['create-error'].should_not be_nil
    res['server_errors']['create-error'][item.object].should_not be_nil    
    res['server_errors']['create-error'][item.object]['message'].should == "error create"
    res['server_errors']['create-error'][item.object]['attributes']['name'].should == "wrongname"

    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2

    item.destroy
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'delete')
    records2.length.should == ($spec_settings[:schema_model] ? 7 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 0

    #getProduct.on_sync_create_error( res['server_errors']['create-error'].keys(), :recreate)
    SyncEngine.on_sync_create_error( getProduct_str(), res['server_errors']['create-error'], :recreate)
    
    records3 = getTestDB().select_from_table('changed_values','*', 'object' => item.object)
    records3.length.should == 0
    
    records4 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records4.length.should == 0
    
    item = getProduct.find(item.object)
    item.should be_nil
    
  end
    
  it "should process update-error" do    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"update-error\":{\"broken_object_id\":{\"name\":\"wrongname\",\"an_attribute\":\"error update\"},\"broken_object_id-error\":{\"message\":\"error update\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    puts "res : #{res}"
    
    res['server_errors'].should_not be_nil
    res['server_errors']['update-error'].should_not be_nil
    res['server_errors']['update-error']['broken_object_id'].should_not be_nil    
    res['server_errors']['update-error']['broken_object_id']['message'].should == "error update"
    res['server_errors']['update-error']['broken_object_id']['attributes']['name'].should == "wrongname"    

    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records.length.should == 0
    
    getProduct.on_sync_update_error( res['server_errors']['update-error'], :retry)
    
    records1 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records1.length.should == 2
    if ( records1[0]["attrib"] == "name" )
        records1[0]["object"].should == "broken_object_id"
        records1[0]["attrib"].should == "name"
        records1[0]["value"].should == "wrongname"
        records1[0]["update_type"].should == "update"
    else
        records1[1]["object"].should == "broken_object_id"
        records1[1]["attrib"].should == "name"
        records1[1]["value"].should == "wrongname"
        records1[1]["update_type"].should == "update"
    end

    if ( records1[1]["attrib"] == "an_attribute" )    
        records1[1]["object"].should == "broken_object_id"
        records1[1]["attrib"].should == "an_attribute"
        records1[1]["value"].should == "error update"
        records1[1]["update_type"].should == "update"
    else
        records1[0]["object"].should == "broken_object_id"
        records1[0]["attrib"].should == "an_attribute"
        records1[0]["value"].should == "error update"
        records1[0]["update_type"].should == "update"
    end
        
  end

  it "should process delete-error" do    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"delete-error\":{\"broken_object_id\":{\"name\":\"wrongname\",\"an_attribute\":\"error delete\"},\"broken_object_id-error\":{\"message\":\"Error delete record\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    puts "res : #{res}"
    
    res['server_errors'].should_not be_nil
    res['server_errors']['delete-error'].should_not be_nil
    res['server_errors']['delete-error']['broken_object_id'].should_not be_nil    
    res['server_errors']['delete-error']['broken_object_id']['message'].should == "Error delete record"
    res['server_errors']['delete-error']['broken_object_id']['attributes']['name'].should == "wrongname"
    
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'delete')
    records.length.should == 0
    
    getProduct.on_sync_delete_error( res['server_errors']['delete-error'], :retry)
    
    records1 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'delete')
    records1.length.should == 2
    if ( records1[0]["attrib"] == "name" )
        records1[0]["object"].should == "broken_object_id"
        records1[0]["attrib"].should == "name"
        records1[0]["value"].should == "wrongname"
        records1[0]["update_type"].should == "delete"
    else
        records1[1]["object"].should == "broken_object_id"
        records1[1]["attrib"].should == "name"
        records1[1]["value"].should == "wrongname"
        records1[1]["update_type"].should == "delete"
    end

    if ( records1[1]["attrib"] == "an_attribute" )
        records1[1]["object"].should == "broken_object_id"
        records1[1]["attrib"].should == "an_attribute"
        records1[1]["value"].should == "error delete"
        records1[1]["update_type"].should == "delete"
    else
        records1[0]["object"].should == "broken_object_id"
        records1[0]["attrib"].should == "an_attribute"
        records1[0]["value"].should == "error delete"
        records1[0]["update_type"].should == "delete"
    end
        
  end

  it "should process source-error" do    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"source-error\":{\"query-error\":{\"message\":\"Error during query\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    puts "res : #{res}"
    
    res['server_errors'].should_not be_nil
    res['server_errors']['query-error'].should_not be_nil
    res['server_errors']['query-error']['message'].should == "Error during query"
  end

  it "should process search-error" do    
    err_resp = "[[{\"version\":3},{\"source\":\"" + getProduct_str() + "\"},{\"search-error\":{\"search-error\":{\"message\":\"Error during search\"}}}]]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    _search_id = Time.now.to_i.to_s
    search_res = getProduct.search(
      :from => 'search',
      :search_params => { :filterData => "Test", :search_id => _search_id },
      :offset => 0,
      :max_results => 1000,
      :progress_step => 10,
      :callback => '/app/Contact/search_callback',
      :callback_param => "")
    res = ::Rho::RhoSupport::parse_query_parameters(search_res)      
    puts "res : #{res}"
    
    res['server_errors'].should_not be_nil
    res['server_errors']['search-error'].should_not be_nil
    res['server_errors']['search-error']['message'].should == "Error during search"
    
  end

  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end

end
