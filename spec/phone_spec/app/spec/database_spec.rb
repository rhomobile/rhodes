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
		SyncEngine.set_threaded_mode(false)
		
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
	  
		
	
	it "should export database Database spec" do		
		file_name = File.join(Rho::RhoApplication::get_model_path('app','BlobBulkTest_s'), 'blob_bulk_test_s.png')
		copy_file(file_name, Rho::RhoApplication::get_blob_folder() )
		file_name = File.join(Rho::RhoApplication::get_blob_folder(), 'blob_bulk_test_s.png')
		File.exists?(file_name).should == true
		if !defined?(RHO_WP7)   
			file_size = File.size(file_name)
		end    
		file_content = File.read(file_name)
		
		item = BlobBulkTest_s.create( { 'name'=>'BlobTestItem','image_uri'=>'blob_bulk_test_s.png' })
		
		File.exists?(file_name).should == true

		exportPath = ::Rhom::Rhom.database_export('user')
		exportPath.should_not be_nil		
		File.exists?(exportPath).should == true
		File.size(exportPath).should_not == 0

	end
end