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

$spec_callback_block = nil
def sync_callback_spec
    $spec_callback_block.call() if $spec_callback_block    
end

SYNC_CALLBACK_NAME = 'sync_callback_spec'

SYNC_SERVER_URL = "http://rhoconnect-spec-exact_platform.heroku.com/application"
#SYNC_SERVER_URL = "http://rhodes-store-spec-server.heroku.com/application"
#SYNC_SERVER_URL = 'http://localhost:9292/application'

def syncserver_url
  platform = case System.get_property('platform')
                    when /android/i    then 'android'
                    when /blackberry/i then 'bb'
                    when /apple/i      then 'iphone'
                    when /symbian/i    then 'symbian'
                    when /wp7/i        then 'wp'
                    when /windows/i    then 'wm'
  end
  platform = 'win32' if System.get_property('platform') == 'WINDOWS_DESKTOP'

  exact_url = SYNC_SERVER_URL.gsub(/exact_platform/, platform)
  puts "going to reset server: #{exact_url}"
  exact_url
end

describe "SyncEngine_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    # init client
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    
    SyncEngine.set_syncserver(syncserver_url)

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
    Rho::RhoConfig.syncserver = syncserver_url
	  
	SyncEngine.set_syncserver(syncserver_url)
	SyncEngine.set_ssl_verify_peer(true)
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", "" )        
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "sync_push_callback", "" )        
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "sync_push_error", "0" )        
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "set_sync_push_body", "" )        
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "sync_push_body", "" )        
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "full_update", 'false' );
    
    ::Rho::RHO.get_user_db().delete_all_from_table('changed_values')

    Rho::RhoConfig.bulksync_state='1'    
    Rho::RhoConfig.sources[getProduct_str]['full_update'] = false
  end

  it "should not connect to self signed SSL with enabled peer check" do
	  SyncEngine.set_syncserver('https://www.pcwebshop.co.uk/application')
	  SyncEngine.set_ssl_verify_peer(true)
	  res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")
	  code = res['error_code'].to_i
	  have_error = (code == ::Rho::RhoError::ERR_NETWORK) || (code == ::Rho::RhoError::ERR_REMOTESERVER )
	  have_error.should == true
	  #res['error_code'].to_i.should == ::Rho::RhoError::ERR_NETWORK
  end
	
  it "should connect and get error from non-rhoconnect SSL server with disabled peer check" do
	  SyncEngine.set_syncserver('https://www.pcwebshop.co.uk/application')
	  SyncEngine.set_ssl_verify_peer(false)
	  res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")
	  res['error_code'].to_i.should == ::Rho::RhoError::ERR_REMOTESERVER
  end

if !defined?(RHO_WP7)
  it "should database_full_reset_ex raise an exception" do  
    exc = false	
    begin
       Rhom::Rhom.database_full_reset_ex( :models => [getProduct_str], :reset_client_info => true )    
    rescue => e
        exc = true
    end
    
    exc.should be_true
  end
end  

  it "should database_full_reset_ex support different parameters" do
    Rhom::Rhom.database_full_reset_ex()
    Rhom::Rhom.database_full_reset_ex( :reset_client_info => true )
    Rhom::Rhom.database_full_reset_ex( :reset_local_models => true )
    Rhom::Rhom.database_full_reset_ex( :reset_local_models => true, :reset_client_info => false )
  end

  it "should database_full_reset_ex support models" do  
  
    getProduct.create({:name=> 'prod1'})
    getCustomer.create({:city => 'SPB'})

    res = getProduct().find(:all)
    res.length.should > 0
    
    res = getCustomer().find(:count)
    res.should > 0    
    
    Rhom::Rhom.database_full_reset_ex( :models => [getProduct_str(), getCustomer_str] )
    Rho::RhoConfig.reset_models.should == "&sources[][name]=#{getProduct_str}&sources[][name]=#{getCustomer_str}"
    
    res = getProduct().find(:all)
    res.length.should == 0
    
    res = getCustomer().find(:count)
    res.should == 0    
  end

  it "should database_full_reset_ex support one model" do  
  
    getProduct.create({:name=> 'prod1'})
    getCustomer.create({:city => 'SPB'})

    res = getProduct().find(:all)
    res.length.should > 0
    
    res = getCustomer().find(:count)
    res.should > 0    
  
    Rhom::Rhom.database_full_reset_ex( :models => [getProduct_str] )
    Rho::RhoConfig.reset_models.should == "&sources[][name]=#{getProduct_str}"
    
    res = getProduct().find(:all)
    res.length.should == 0
    
    res = getCustomer().find(:count)
    res.should > 0    
  end

 
  it "should update syncserver at runtime" do
  
    dbRes = ::Rho::RHO.get_user_db().select_from_table('client_info','token,token_sent')
    dbRes.length.should == 0
  
    saveSrv =  Rho::RhoConfig.syncserver
    
	  # temporary commented until fix bug with sync thread stop hang
	  #    SyncEngine.set_syncserver('')
	  #Rho::RhoConfig.syncserver.should == ''
    
    SyncEngine.set_syncserver('http://example.com/sources/')
    Rho::RhoConfig.syncserver.should == 'http://example.com/sources/'
    
    SyncEngine.set_syncserver(saveSrv)
    Rho::RhoConfig.syncserver.should == saveSrv
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
    if ( dbRes[0]['client_id'] )
        dbRes[0]['client_id'].should == ""
    end
    
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
  
  it "should handle update updated object while sync error" do
    SyncEngine.logged_in.should == 1

    item = getProduct.find(:first)
    item.should_not be_nil
    records = getTestDB().select_from_table('changed_values','*')
    records.length.should == 0

    @product_test_name1 = Rho::RhoConfig.generate_id().to_s
    item.update_attributes( {:name => @product_test_name1, :brand => @product_test_name1 } )
    records1 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records1.length.should == 2

    records2 = nil
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_error", "#{Rho::RhoError::ERR_NETWORK}" )
    $spec_callback_block = Proc.new {
        item.update_attributes( {:name => 'Test3'} )
        
        records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    }
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_callback", SYNC_CALLBACK_NAME );
    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'error'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NETWORK
    
    records = getTestDB().select_from_table('changed_values','*', { 'update_type' => 'update', 'sent'=> 0} )
    records.length.should == 2

    records = getTestDB().select_from_table('changed_values','*', {'update_type' => 'update', 'attrib'=>'name'} )
    records.length.should == 1
    records[0]['attrib'].should == 'name'
    records[0]['value'].should == 'Test3'

    records2.length.should == 3
        
  end

  it "should handle update updated object while sync" do
    SyncEngine.logged_in.should == 1

    item = getProduct.find(:first)
    item.should_not be_nil
    getTestDB().select_from_table('changed_values','*').length.should == 0

    @product_test_name2 = Rho::RhoConfig.generate_id().to_s
    item.update_attributes( {:name => @product_test_name2 } )
    records1 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records1.length.should == 1

    records2 = nil
    $spec_callback_block = Proc.new {
        item.update_attributes( {:name => 'Test4'} )
        
        records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    }
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_callback", SYNC_CALLBACK_NAME );
    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    records = getTestDB().select_from_table('changed_values','*', { 'update_type' => 'update', 'sent'=> 0} )
    records.length.should == 1
    records[0]['attrib'].should == 'name'
    records[0]['value'].should == 'Test4'

    records2.length.should == 2

    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    item1 = getProduct.find(item.object)
    item1.name.should == 'Test4'
    getTestDB().select_from_table('changed_values','*').length.should == 0
        
  end

  it "should handle update updated full_update object while sync" do
    SyncEngine.logged_in.should == 1
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "full_update", 'true' );

    item = getProduct.find(:first)
    item.should_not be_nil
    getTestDB().select_from_table('changed_values','*').length.should == 0

    @product_test_name3 = Rho::RhoConfig.generate_id().to_s
    item.update_attributes( {:name => @product_test_name3 } )
    records1 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records1.length.should == 1

    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "set_sync_push_body", 'true' );
    records2 = nil
    records3 = nil
    $spec_callback_block = Proc.new {
        records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    
        item.update_attributes( {:name => 'Test4'} )
        
        records3 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'update', 'sent'=>0} )
    }
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_callback", SYNC_CALLBACK_NAME );
    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    pushBody = SyncEngine.get_source_property( getProduct().get_source_id.to_i(), "sync_push_body");
    pushBody.index("quantity").should_not be_nil
    
    records = getTestDB().select_from_table('changed_values','*', { 'update_type' => 'update', 'sent'=> 0} )
    records.length.should == 1
    records[0]['attrib'].should == 'name'
    records[0]['value'].should == 'Test4'

    records2.length.should > 1
    records3.length.should == 1

    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    item1 = getProduct.find(item.object)
    item1.name.should == 'Test4'
    getTestDB().select_from_table('changed_values','*').length.should == 0
        
  end

  it "should handle deleted object while error sync" do
    SyncEngine.logged_in.should == 1

    item = getProduct.find(:first)
    item.should_not be_nil
    records = getTestDB().select_from_table('changed_values','*')
    records.length.should == 0

    item.destroy
    getTestDB().select_from_table('changed_values','*', 'update_type' => 'delete' ).length.should > 0
    getTestDB().select_from_table('changed_values','*', {'update_type' => 'delete', 'sent'=>1} ).length.should == 0
    
    records2 = nil
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_error", "#{Rho::RhoError::ERR_NETWORK}" )
    $spec_callback_block = Proc.new {
        records2 = getTestDB().select_from_table('changed_values','*', { 'update_type' => 'delete', 'sent'=>1} )
    }
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_callback", SYNC_CALLBACK_NAME );
    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'error'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NETWORK
    
    getTestDB().select_from_table('changed_values','*', 'update_type' => 'delete' ).length.should > 0
    getTestDB().select_from_table('changed_values','*', {'update_type' => 'delete', 'sent'=>1} ).length.should == 0

    records2.length.should > 0
        
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

  it "should process error sync" do

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*')
    records.length.should == 0
    
    cust1 = getCustomer.create( {:first => "CustTest1", :last => "Last1"})

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1

    Rho::RhoConfig.syncserver = 'http://rhodes-store-server.heroku2.com/application'
    res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync
    res['status'].should == 'error'
    res['error_code'].to_i.should >  ::Rho::RhoError::ERR_NONE
    res['error_code'].to_i.should <  ::Rho::RhoError::ERR_RUNTIME

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'

    cust1.update_attributes( {:first => "CustTest2"} )
    records2 = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == records.length

    items2 = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'attrib' => 'first')
    items2.length.should == 0
    
    records3 = ::Rho::RHO.get_user_db().select_from_table('changed_values','*', 'update_type' => 'update')
    records3.length.should == 0

    Rho::RhoConfig.syncserver = syncserver_url
    res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    records = ::Rho::RHO.get_user_db().select_from_table('changed_values','*')
    records.length.should == 0
    
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

    items = getProduct.find(:all, :conditions => {:name => @product_test_name1})
    items.should_not == nil
    items.each do |item|
        item.destroy
    end    

    items = getProduct.find(:all, :conditions => {:name => @product_test_name2})
    items.should_not == nil
    items.each do |item|
        item.destroy
    end    

    items = getProduct.find(:all, :conditions => {:name => @product_test_name3})
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
    if System.get_property('platform') == 'WINDOWS_DESKTOP' || System.get_property('platform') == 'Blackberry'
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
    records2.length.should == ($spec_settings[:schema_model] ? 3 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2

    item.update_attributes({:price => "456"})
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 3 : 3)
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
    records2.length.should == ($spec_settings[:schema_model] ? 3 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2

    item.update_attributes({:price => "456"})
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 3 : 3)
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
    records2.length.should == ($spec_settings[:schema_model] ? 3 : 3)
    records2[0]['attrib'].should_not == 'object'        
    records2[0]['sent'].should == 2

    item.destroy
    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 3 : 3)
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
    
  it "should process retry update-error" do    
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

  it "should process retry update-error full_update model" do    
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

    Rho::RhoConfig.sources[getProduct_str]['full_update'] = true
    getProduct.on_sync_update_error( res['server_errors']['update-error'], :retry)
    
    records1 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records1.length.should == 2
    records1[0]["attrib"].should_not == 'object'

  end

  it "should rollback update-error" do
    prod = getProduct.find(:first)
    prod.should_not be_nil
    prod_name = prod.name
    prod_name.should_not be_nil
    obj_id  = prod.object
    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"update-rollback\": {\"#{obj_id}\": {\"name\": \"OLD_NAME\",\"brand\": \"Planifi\xc3\xa9\"}},\"update-error\":{\"#{obj_id}\":{\"name\":\"wrongname\",\"brand\":\"wrongbrand\",\"an_attribute\":\"error update\"},\"#{obj_id}-error\":{\"message\":\"error update\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    #puts "res : #{res}"
    
    res['server_errors'].should_not be_nil
    res['server_errors']['update-error'].should_not be_nil
    res['server_errors']['update-error'][obj_id].should_not be_nil    
    res['server_errors']['update-error'][obj_id]['message'].should == "error update"
    res['server_errors']['update-error'][obj_id]['attributes']['name'].should == "wrongname"
    res['server_errors']['update-error'][obj_id]['attributes']['brand'].should == "wrongbrand"

    res['server_errors']['update-rollback'].should_not be_nil
    res['server_errors']['update-rollback'][obj_id].should_not be_nil    
    res['server_errors']['update-rollback'][obj_id]['attributes']['name'].should == "OLD_NAME"
    res['server_errors']['update-rollback'][obj_id]['attributes']['brand'].should == "Planifi\u00e9"

    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records.length.should == 0
    
    prod = getProduct.find(obj_id)
    prod.should_not be_nil
    prod.name.should == prod_name
    
    getProduct.on_sync_update_error( res['server_errors']['update-error'], :rollback, res['server_errors']['update-rollback'] )

    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    records.length.should == 0

    prod = getProduct.find(obj_id)
    prod.should_not be_nil
    prod.name.should == "OLD_NAME"
    prod.brand.should == "Planifi\xc3\xa9"

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

  it "should NOT push pending created objects" do
    item = getProduct.create({:name => 'Test', :brand => "Rho"})
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'

    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"" + item.object + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + item.object + "-error\":{\"message\":\"error create\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")

    item.update_attributes({:price => "123"})

    records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records2.length.should == ($spec_settings[:schema_model] ? 2 : 2)

    records2 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'create', "sent"=>0})
    records2.length.should == 0

    records3 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'update', "sent"=>0})
    records3.length.should == 1

    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")

    records3 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'update', "sent"=>1})
    records3.length.should == 1
    
  end

  it "should push when pending created objects" do
    item = getProduct.create({:name => 'Test', :brand => "Rho"})
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'

    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"" + item.object + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + item.object + "-error\":{\"message\":\"error create\"}}}]"
    
    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")

    item2 = getProduct.create({:name => 'Test2', :brand => "Rho2"})
    records2 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'create', "sent"=>0} )
    records2.length.should == 1
    records2[0]['attrib'].should == 'object'

    SyncEngine.set_source_property(getProduct().get_source_id.to_i(), "rho_server_response", "" )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    records2 = getTestDB().select_from_table('changed_values','*')
    records2.length.should == 0

    item3 = getProduct.find(item2.object)
    item3.should be_nil
    
  end

  it "should NOT push when children pending created objects" do
    cust1 = getCustomer.create( {:first => "CustTest1"})
    cust2 = getCustomer.create( {:first => "CustTest2"})
    
    @product_test_name = Rho::RhoConfig.generate_id().to_s
    item = getProduct.create({:name => @product_test_name, :quantity => cust1.object, :sku => cust2.object})
    item2 = getProduct.find(item.object)
    item2.vars.should == item.vars
    
    err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"" + cust1.object + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + cust1.object + "-error\":{\"message\":\"error create\"}}}]"
    SyncEngine.set_source_property(getCustomer().get_source_id.to_i(), "rho_server_response", err_resp )    
    res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync( "/app/Settings/sync_notify")

    test = getTestDB().select_from_table('changed_values','*')
    puts "test: #{test}"

    records2 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'create', 
        'source_id'=>getCustomer().get_source_id.to_i(), 'sent'=>2})
    records2.length.should > 0
    records2 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'create', 
        'source_id'=>getProduct().get_source_id.to_i(), 'sent'=>0})
    records2.length.should > 0

    SyncEngine.set_source_property(getCustomer().get_source_id.to_i(), "rho_server_response", "" )    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    test = getTestDB().select_from_table('changed_values','*')
    puts "test: #{test}"
    records2 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'create', 
        'source_id'=>getCustomer().get_source_id.to_i(), 'sent'=>2})
    records2.length.should > 0
    records2 = getTestDB().select_from_table('changed_values','*', {'update_type' => 'create', 
        'source_id'=>getProduct().get_source_id.to_i(), 'sent'=>1})
    records2.length.should > 0

    item2 = getProduct.find(item.object)
    item2.vars.should_not be_nil
  end
 
=begin
	it "should not sync non-exist properties from freezed model with similar names" do
		SyncEngine.logged_in.should == 1
		
		res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync( "/app/Settings/sync_notify")
		res['status'].should == 'ok'
		res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
		
		puts "customers = #{getCustomer().find(:all).inspect}"
		
		cust = getCustomer().find(:first, :conditions => {:first => 'CustTest2'} )
		cust.should_not be_nil
		
		Rhom::Rhom.database_full_reset
		Rho::RhoConfig.bulksync_state='1'
		
		cust1 = getCustomer().find(:first)
		cust1.should be_nil
		
		saved_src = Rho::RhoConfig.sources[getCustomer_str()]
		begin
			if !$spec_settings[:schema_model]
				Rho::RhoConfig.sources[getCustomer_str()]['freezed'] = true
				else
				Rho::RhoConfig.sources[getCustomer_str()]['schema']['property'].delete('first')
				Rho::RhoConfig.sources[getCustomer_str()]['schema']['property'].add('first1')
				
			end
			::Rho::RHO.init_sync_source_properties(Rho::RhoConfig::sources.values)
			
			res2 = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync( "/app/Settings/sync_notify")
			res2['status'].should == 'ok'
			res2['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
			
			cust_all = getCustomer().find(:all)
			cust_all.should_not be_nil
			cust_all.size.should > 0
			
			cust2 = getCustomer().find(:first, :conditions => {:first => 'CustTest2'} )
			cust2.should be_nil
			
			cust3 = getCustomer().find(:first1, :conditions => {:first => 'CustTest2'} )
			cust3.should be_nil
			ensure
			Rho::RhoConfig.sources[getCustomer_str()] = saved_src
			::Rho::RHO.init_sync_source_properties(Rho::RhoConfig::sources.values)
		end
	end
=end
		
  it "should handle update created object while sync error" do
    SyncEngine.logged_in.should == 1

    item = getProduct.create({:name => 'Test', :brand => 'Canon' })
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'

    item2 = getProduct.create({:name => 'TestDel', :brand => 'Apple' })

    records2 = nil
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_error", "#{Rho::RhoError::ERR_NETWORK}" )
    $spec_callback_block = Proc.new {
        item.update_attributes( {:name => 'Test2'} )
        item2.destroy()
        
        records2 = getTestDB().select_from_table('changed_values','*', 'update_type' => 'update')
    }
    SyncEngine.set_source_property( getProduct().get_source_id.to_i(), "sync_push_callback", SYNC_CALLBACK_NAME );
    
    res = ::Rho::RhoSupport::parse_query_parameters getProduct.sync( "/app/Settings/sync_notify")
    res['status'].should == 'error'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NETWORK
    
    records = getTestDB().select_from_table('changed_values','*', 'update_type' => 'create')
    records.length.should == 1
    records[0]['attrib'].should == 'object'

    records2.length.should == 1
    records2[0]['attrib'].should == 'name'
    records2[0]['value'].should == 'Test2'
        
  end
	
  it "should skip unmodified models in sync" do
	  SyncEngine.logged_in.should == 1
	  
	  Rhom::Rhom.database_full_reset
	  Rho::RhoConfig.bulksync_state='1'    
	  
	  getProduct.create({:name => 'SkipLocalChanges'})
	  getCustomer.create({:first => 'SkipLocalChanges'})
	  
	  Rhom::Rhom.have_local_changes.should == true
	  
	  res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync	  
	  res['status'].should == 'complete'
	  res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
	  	  
	  Rhom::Rhom.have_local_changes.should == false
	  
	  Rhom::Rhom.database_full_reset
	  Rho::RhoConfig.bulksync_state='1' 
	  
	  getProduct.create({:name => 'SkipLocalChanges2'})
	  
	  Rhom::Rhom.have_local_changes.should == true
	  
	  res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync(false,'',true)
	  res['status'].should == 'complete'
	  res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
	  	  
	  Rhom::Rhom.have_local_changes.should == false
	  
	  items = getProduct.find(:all)
	  items.length.should_not == 0
	  
	  items = getCustomer.find(:all)
	  items.length.should == 0
	  
	  Rhom::Rhom.database_full_reset
	  Rho::RhoConfig.bulksync_state='1' 
	  
	  res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync
	  res['status'].should == 'complete'
	  res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
	  	  
	  items = getProduct.find(:all, :conditions => { :name=>'SkipLocalChanges' })
	  puts "items = #{items.inspect}"
	  items.length.should_not == 0
	  items.each do |item|
		  item.destroy
	  end
	  
	  items = getProduct.find(:all, :conditions => { :name=>'SkipLocalChanges2' })
	  puts "items = #{items.inspect}"	  
	  items.length.should_not == 0
	  items.each do |item|
		  item.destroy
	  end
	  
	  items = getCustomer.find(:all, :conditions => { :first=>'SkipLocalChanges' })
	  puts "items = #{items.inspect}"	  
	  items.length.should_not == 0
	  items.each do |item|
		  item.destroy
	  end
	  
	  res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync
	  res['status'].should == 'complete'
	  res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end
	
	it "should not sync non-exist properties from freezed model" do
		SyncEngine.logged_in.should == 1
		
		res = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync( "/app/Settings/sync_notify")
		res['status'].should == 'ok'
		res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
		
		cust = getCustomer().find(:first, :conditions => {:first => 'CustTest2'} )
		cust.should_not be_nil
		
		Rhom::Rhom.database_full_reset
		Rho::RhoConfig.bulksync_state='1'
		
		cust1 = getCustomer().find(:first)
		cust1.should be_nil
		
		saved_src = Rho::RhoConfig.sources[getCustomer_str()]
		begin
			if !$spec_settings[:schema_model]
				Rho::RhoConfig.sources[getCustomer_str()]['freezed'] = true
				else
				Rho::RhoConfig.sources[getCustomer_str()]['schema']['property'].delete('first')
			end
			::Rho::RHO.init_sync_source_properties(Rho::RhoConfig::sources.values)
			
			res2 = ::Rho::RhoSupport::parse_query_parameters getCustomer.sync( "/app/Settings/sync_notify")
			res2['status'].should == 'ok'
			res2['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
			
			cust_all = getCustomer().find(:all)
			cust_all.should_not be_nil
			cust_all.size.should > 0
			
			cust2 = getCustomer().find(:first, :conditions => {:first => 'CustTest2'} )
			cust2.should be_nil
			ensure
			Rho::RhoConfig.sources[getCustomer_str()] = saved_src
			::Rho::RHO.init_sync_source_properties(Rho::RhoConfig::sources.values)
		end
	end
  
  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end
end
