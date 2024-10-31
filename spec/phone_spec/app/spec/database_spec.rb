#
#  database_spec.rb
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
#require 'spec/spec_helper'
require 'rho/rho'
#require 'fileutils'

describe "Database_test" do
	
	before(:all)  do
		#		SyncEngine.set_threaded_mode(false)
		
		::Rhom::Rhom.database_full_reset_and_logout
		
	end
	
	after(:all)  do
		
	end
	
def copy_file(src, dst_dir)
	if !defined?(RHO_WP7)  
		content = File.binread(src)  
	else
		content = File.read(src)  
	end    
    File.open(File.join( dst_dir, File.basename(src) ), "wb"){|f| f.write(content) }
end

if  System.get_property('platform') != 'WP8'  
	it "should export database Database spec" do		
		file_name = File.join(Rho::RhoApplication::get_model_path('app','BlobBulkTest_s'), 'blob_bulk_test_s.png')
		copy_file(file_name, Rho::RhoApplication::get_blob_folder() )
		file_name = File.join(Rho::RhoApplication::get_blob_folder(), 'blob_bulk_test_s.png')
		File.exist?(file_name).should == true
		puts "blob path = #{file_name}"		
		item = BlobBulkTest_s.create( { 'name'=>'BlobTestItem','image_uri'=>'blob_bulk_test_s.png' })
		items = BlobBulkTest_s.find(:all)
		items.size.should == 1

		file_name = File.join(Rho::RhoApplication::get_model_path('app','BlobBulkTest'), 'blob_bulk_test.png')
		copy_file(file_name, Rho::RhoApplication::get_blob_folder() )
		file_name = File.join(Rho::RhoApplication::get_blob_folder(), 'blob_bulk_test.png')
		File.exist?(file_name).should == true
		puts "blob path = #{file_name}"		
		item = BlobBulkTest.create( { 'name'=>'BlobTestItem','image_uri'=>'blob_bulk_test.png' })
		items = BlobBulkTest.find(:all)
		items.size.should == 1		

		exportPath = ::Rhom::Rhom.database_export('user')
		exportPath.should_not be_nil		
		File.exist?(exportPath).should == true
		File.size(exportPath).should_not == 0

		Rhom::Rhom.database_full_reset

		items = BlobBulkTest_s.find(:all)
		puts "BlobBulkTest_s = #{items}"
		items.size.should == 0

		items = BlobBulkTest.find(:all)
		puts "BlobBulkTest = #{items}"
		items.size.should == 0

		::Rhom::Rhom.database_import('user',exportPath).should == true

		BlobBulkTest_s.find(:all).size.should == 1
		BlobBulkTest.find(:all).size.should == 1

		File.delete(exportPath)
		File.exist?(exportPath).should == false
	end

	it "should reset database Database spec" do
		::Rhom::Rhom.database_full_reset_and_logout
	end

	it "should import database Database spec" do
		::Rhom::Rhom.database_import('user',File.join(Rho::RhoApplication::get_base_app_path(),'app/Data/valid_import_db.zip')).should == true
		
		items = BlobBulkTest_s.find(:all)
		items.should_not be_nil
		items.size.should == 1
		item = items[0]
		item.name.should == 'BlobTestItem'
		item.image_uri.should == 'blob_bulk_test_s.png'
		puts "item = #{item.inspect}"
		File.exist?(File.join(Rho::RhoApplication::get_blob_folder(),item.image_uri)).should == true
	end

	it "should import invalid data and rollback" do
	end
end

end