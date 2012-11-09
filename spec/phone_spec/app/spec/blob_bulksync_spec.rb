#
#  blob_bulksync_spec.rb
#  rhodes
#
#  Copyright (C) 2012 Rhomobile, Inc. All rights reserved.
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

require 'rho/rho'

describe "BlobBulkSync_test" do

  before(:all)  do
    SyncEngine.set_threaded_mode(false)
  
    ::Rhom::Rhom.database_full_reset_and_logout
    
	  SyncEngine.set_syncserver('http://store-bulk.rhohub.com/application')
	  #SyncEngine.set_syncserver('http://192.168.0.102:9292/application')
	  
	  Rho::RhoConfig.bulksync_state='1'
  end
	
  after(:all)  do
	Rho::RhoConfig.bulksync_state='1'
	sleep(2)
  end

  
  it "should login" do

    login_name = System.get_property('platform') + System.get_property('device_name')    
    res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.login('login_name', '', "/app/Settings/login_callback")
    res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
    
    SyncEngine.logged_in.should == 1
    
    Rho::RhoConfig.bulksync_state='1'

  end

  def copy_file(src, dst_dir)
if !defined?(RHO_WP7)  
    content = File.binread(src)  
else
    content = File.read(src)  
end    
    File.open(File.join( dst_dir, File.basename(src) ), "wb"){|f| f.write(content) }
  end

  def createBlob(model,dir,filename)
	  SyncEngine.logged_in.should == 1
	  
	  file_name = File.join(Rho::RhoApplication::get_model_path('app',dir),filename)
	  copy_file(file_name, Rho::RhoApplication::get_blob_folder() )
	  file_name = File.join(Rho::RhoApplication::get_blob_folder(), filename)
	  File.exists?(file_name).should == true
	  if !defined?(RHO_WP7)   
		file_size = File.size(file_name)
	  end    
	  file_content = File.read(file_name)
							
	  item = model.new
	  item.name = 'BlobTestItem'
	  item.image_uri = file_name
	  item.save
							
	  File.exists?(file_name).should == true
						
	  res = ::Rho::RhoSupport::parse_query_parameters model.sync( "/app/Settings/sync_notify")
	  res['status'].should == 'ok'
	  res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
						
	  if (File.exists?(file_name))
		File.delete(file_name)
	  end
	  File.exists?(file_name).should == false
							
	  return file_size,file_content

  end
							
  propBagSize = 0
  propBagContent = nil
  fixedSchemaSize = 0
  fixedSchemaContent = nil

  it "should create property bag blob" do
	propBagSize, propBagContent = createBlob(BlobBulkTest,'BlobBulkTest','blob_bulk_test.png')
  end

  it "should create fixed schema blob" do
	fixedSchemaSize, fixedSchemaContent = createBlob(BlobBulkTest_s,'BlobBulkTest_s','blob_bulk_test_s.png')
  end
							
  def checkBlob(model,size,content,old_filename)
	items = model.find(:all, :conditions => { :name => old_filename} )
	items.should_not == nil
	items.length.should == 1
	puts "items = #{items.inspect}"
							
	items[0].image_uri.should_not == old_filename
	new_file_name = File.join(Rho::RhoApplication::get_blob_path(items[0].image_uri))
	File.exists?(new_file_name).should == true
	if !defined?(RHO_WP7)    
	  File.size(new_file_name).should == size
	end    
	content_new = File.read(new_file_name)
	content_new.should == content
  end

  it "should bulk sync blobs" do
	Rho::RhoConfig.bulksync_state='0'
	res = ::Rho::RhoSupport::parse_query_parameters SyncEngine.dosync
							
	res['status'].should == 'complete'
	res['error_code'].to_i.should == ::Rho::RhoError::ERR_NONE
							
	checkBlob(BlobBulkTest,propBagSize,propBagContent,'blob_bulk_test.png')
	checkBlob(BlobBulkTest_s,fixedSchemaSize,fixedSchemaContent,'blob_bulk_test_s.png')
  end
							
  def deleteBlob(model)
	SyncEngine.logged_in.should == 1
							
	model.delete_all
	Rho::RhoConfig.bulksync_state='1'
	model.sync("/app/Settings/SyncNotify")
							
	items = model.find(:all)
	items.should_not == nil
	items.length.should == 0
  end

  it "should create objects after bulk sync" do
    propBagItem = BlobBulkTest.new
    propBagItem.name = "test"
    propBagItem.should_not be_nil

    schemaItem = BlobBulkTest_s.new
    schemaItem.name = "test"
    schemaItem.should_not be_nil
  end

  it "should logout" do
    SyncEngine.logout()
  
    SyncEngine.logged_in.should == 0
  end
end
