#
#  blobsync_spec.rb
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
#require 'fileutils'

describe "BlobSync_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    ::Rhom::Rhom.database_full_reset_and_logout
    
    SyncEngine.set_syncserver('http://rhodes-samples-server.heroku.com/application')
    
  end
  
  it "should login" do
    
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login('lars', 'b', "/app/Settings/login_callback")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    SyncEngine.logged_in.should == 1
  end

  it "should sync BlobTest" do
    SyncEngine.logged_in.should == 1
  
    res = ::Rho::RhoSupport::parse_query_parameters BlobTest.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
  end

  it "should delete all Test Blobs" do
    SyncEngine.logged_in.should == 1
  
    items = BlobTest.find(:all ) #, :conditions => {:name => 'BlobTestItem'})
    items.should_not == nil
    
    items.each do |item|
        file_name = File.join(Rho::RhoApplication::get_blob_path(item.image_uri))
        File.exists?(file_name).should == true
        item.destroy
        File.exists?(file_name).should == false
    end    

    BlobTest.sync( "/app/Settings/sync_notify")
    sleep(2) #wait till sync server update data
    
    res = ::Rho::RhoSupport::parse_query_parameters BlobTest.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item2 = BlobTest.find(:first ) #, :conditions => {:name => 'BlobTestItem'})
    item2.should == nil
  end

  def copy_file(src, dst_dir)
    content = File.binread(src)  
    File.open(File.join( dst_dir, File.basename(src) ), "wb"){|f| f.write(content) }
  end
  
  it "should create new BlobTest" do
    SyncEngine.logged_in.should == 1

    file_name = File.join(Rho::RhoApplication::get_model_path('app','BlobTest'), 'test.png')
    copy_file(file_name, Rho::RhoApplication::get_blob_folder() )
    file_name = File.join(Rho::RhoApplication::get_blob_folder(), 'test.png')
    
    file_size = File.size(file_name)
    file_content = File.read(file_name)
    
    item = BlobTest.new
    #item.name = 'BlobTestItem'
    item.image_uri = file_name
    item.save
    BlobTest.sync( "/app/Settings/sync_notify")
    sleep(2) #wait till sync server update data

    res = ::Rho::RhoSupport::parse_query_parameters BlobTest.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE

    items = BlobTest.find(:all ) #, :conditions => {:name => 'BlobTestItem'})
    items.should_not == nil
    items.length.should == 1
    
    items[0].image_uri.should_not == file_name
    new_file_name = File.join(Rho::RhoApplication::get_blob_path(items[0].image_uri))
    File.size(new_file_name).should == file_size
    content_new = File.read(new_file_name)
    content_new.should == file_content
    
  end

  it "should modify BlobTest" do
    SyncEngine.logged_in.should == 1
  
    item = BlobTest.find(:first)
    item.should_not == nil
    saved_obj = item.object

    file_name = File.join(Rho::RhoApplication::get_model_path('app','BlobTest'), 'test2.png')
    copy_file(file_name, Rho::RhoApplication::get_blob_folder() )
    file_name = File.join(Rho::RhoApplication::get_blob_folder(), 'test2.png')
    
#    file_size = File.size(file_name)
#    file_content = File.read(file_name)
    
    old_file = item.image_uri
    item.image_uri = file_name
    item.save
    File.exists?(old_file).should == false
    
    BlobTest.sync( "/app/Settings/sync_notify")
    sleep(2) #wait till sync server update data

    res = ::Rho::RhoSupport::parse_query_parameters BlobTest.sync( "/app/Settings/sync_notify")
    res['status'].should == 'ok'
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    item2 = BlobTest.find(saved_obj)
    
    item2.image_uri.should == file_name
#    new_file_name = File.join(Rho::RhoApplication::get_blob_path(item2.image_uri))
#    File.size(new_file_name).should == file_size
#    content_new = File.read(new_file_name)
#    content_new.should == file_content
    
  end

  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end
end
