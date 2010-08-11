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

describe "SyncEngine_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    ::Rhom::Rhom.database_full_reset_and_logout
    
    SyncEngine.set_syncserver('http://rhodes-store-server.heroku.com/application')
    
  end
  
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

  it "should not sync without login" do
    SyncEngine.logged_in.should == 0
  
    res = ::Rho::RhoSupport::parse_query_parameters Product.sync( "/app/Settings/sync_notify")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_CLIENTISNOTLOGGEDIN

  end
  
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
  
    res = ::Rho::RhoSupport::parse_query_parameters Product.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    #check that clientregister did called
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

  it "should sync Product by name" do
    SyncEngine.logged_in.should == 1

    SyncEngine.set_notification(Product.get_source_id.to_i(), "/app/Settings/sync_notify", "")

    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync_source( "Product" )
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

  it "should search Product" do
    #TODO: add search to sync adapter
    SyncEngine.logged_in.should == 1

    _search_id = Time.now.to_i.to_s

    search_res = Product.search(
      :from => 'search',
      :search_params => { :filterData => "Test", :search_id => _search_id },
      :offset => 0,
      :max_results => 1000,
      :progress_step => 10,
      :callback => '/app/Contact/search_callback',
      :callback_param => "")
  
    res = ::Rho::RhoSupport::parse_query_parameters search_res
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

=begin  
  it "should sync all" do
    SyncEngine.logged_in.should == 1
  
    Product.set_notification("/app/Settings/sync_notify", "fixed sync_notify for Product")
    SyncEngine.dosync

    res = ::Rho::RhoSupport::parse_query_parameters C_sync_notify
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end
=end

  it "should create new Product" do
    SyncEngine.logged_in.should == 1
  
    item = Product.create({:name => 'Test'})
    item2 = Product.find(item.object)
    item2.vars.should == item.vars
    
    res = ::Rho::RhoSupport::parse_query_parameters Product.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end
  
  it "should modify Product" do
    SyncEngine.logged_in.should == 1
  
    item = Product.find(:first, :conditions => {:name => 'Test'})
    item.should_not == nil
    saved_obj = item.object
    
    new_sku = item.sku ? item.sku : ""
    new_sku += "_TEST"
    item.sku = new_sku
    item.save
    
    res = ::Rho::RhoSupport::parse_query_parameters Product.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item2 = Product.find(saved_obj)
    item2.sku.should == new_sku
    
  end

  it "should delete all Test Product" do
    SyncEngine.logged_in.should == 1
  
    items = Product.find(:all, :conditions => {:name => 'Test'})
    items.should_not == nil
    
    items.each do |item|
        item.destroy
    end    

    res = ::Rho::RhoSupport::parse_query_parameters Product.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item2 = Product.find(:first, :conditions => {:name => 'Test'})
    item2.should == nil
  end

  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end

end
