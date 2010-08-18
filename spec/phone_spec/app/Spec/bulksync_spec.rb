#
#  bulksync_spec.rb
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

describe "BulkSync_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    ::Rhom::Rhom.database_full_reset_and_logout
    
    SyncEngine.set_syncserver('http://184.73.159.63/application')
    Rho::RhoConfig.bulksync_state='0'

    @save_sync_types = ::Rho::RHO.get_user_db().select_from_table('sources','name, sync_type')

    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'none'})
    ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>'incremental'}, {'name'=>'Product'})
  end

  after(:all)  do
    @save_sync_types.each do |src|
        ::Rho::RHO.get_user_db().update_into_table('sources',{'sync_type'=>src['sync_type']}, {'name'=>src['name']})
    end
    Rho::RhoConfig.bulksync_state='1'
  end

  it "should login" do
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login("", "", "/app/Settings/login_callback")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    SyncEngine.logged_in.should == 1
  end

  it "should bulk sync" do
    SyncEngine.logged_in.should == 1
  
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync
    res['status'].should == 'complete'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    Rho::RhoConfig.bulksync_state.should == '1'
    
    items = Product.find(:all)
    items.should_not be_nil
    items.length().should_not == 0
  end

  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end
  

end
