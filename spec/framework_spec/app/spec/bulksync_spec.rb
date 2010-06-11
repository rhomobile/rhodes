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
require 'spec/spec_helper'
require 'rho/rho'

describe "BulkSync_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    ::Rhom::Rhom.database_full_reset_and_logout
    
    SyncEngine.set_syncserver('http://localhost:9292/application')
    Rho::RhoConfig.bulksync_state='0'
    
  end
  
  it "should login" do
    SyncEngine.login('admin', "", "/app/Settings/login_callback")
  
    res = ::Rho::RhoSupport::parse_query_parameters C_login_callback
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    SyncEngine.logged_in.should == 1
  end

  it "should sync BulkTest" do
    SyncEngine.logged_in.should == 1
  
    SyncEngine.set_bulk_notification("/app/Settings/bulk_sync_notify", "")
    Product.sync( "/app/Settings/sync_notify")

    res = ::Rho::RhoSupport::parse_query_parameters C_bulk_sync_notify
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end
end
