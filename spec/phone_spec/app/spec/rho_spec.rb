#
#  rho_spec.rb
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

require 'date'
require 'time'

describe "System" do

   it "should test app_installed?" do
   	[1..1000].each do |i|
   			System::app_installed?("mythebesttestandroidapplication")
   	end
   end

if System.get_property('platform') != 'WP8' && System.get_property('platform') != 'Blackberry'
    it "should test zip/unzip file" do

		dir_name = Rho::RhoApplication::get_app_path('DataTemp')
		Dir.mkdir(dir_name) unless Dir.exist?(dir_name)

        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'ziptest.txt')
        file_name_zip = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'ziptest.zip')
        
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should ==  false

        File.delete(file_name_zip) if File.exist?(file_name_zip)
        File.exist?(file_name_zip).should ==  false

        write_data  = "this is zip rhodes test"
        f = File.new(file_name, "wb")
        f.write(write_data)
        f.close

        System.zip_file(file_name_zip, file_name)
        File.exist?(file_name_zip).should == true
        
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should ==  false

        System.unzip_file(file_name_zip)
        File.exist?(file_name).should == true
    end    
    
    it "should test zip/unzip folder" do

		dir_name1 = Rho::RhoApplication::get_app_path('DataTemp')
		Dir.mkdir(dir_name1) unless Dir.exist?(dir_name1)
		dir_name = File.join(dir_name1, 'ZipFolder')
		Dir.mkdir(dir_name) unless Dir.exist?(dir_name)

        file_name = File.join(Rho::RhoApplication::get_app_path('DataTemp/ZipFolder'), 'ziptest.txt')
        file_name1 = File.join(Rho::RhoApplication::get_app_path('DataTemp/ZipFolder'), 'ziptest1.txt')
        file_name_zip = File.join(Rho::RhoApplication::get_app_path('DataTemp'), 'zipfolder.zip')
        
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should ==  false
        File.delete(file_name1) if File.exist?(file_name1)
        File.exist?(file_name1).should ==  false

        File.delete(file_name_zip) if File.exist?(file_name_zip)
        File.exist?(file_name_zip).should ==  false

        write_data  = "this is zip rhodes test"
        f = File.new(file_name, "wb")
        f.write(write_data)
        f.close

        write_data  = "this is zip rhodes test2"
        f = File.new(file_name1, "wb")
        f.write(write_data)
        f.close

        files = [] #[dir_name]
        files += Dir.glob(File.join(dir_name, "*") )
        puts "files: #{files}"
        System.zip_files(file_name_zip, dir_name1, files)
        File.exist?(file_name_zip).should == true
        
        File.delete(file_name) if File.exist?(file_name)
        File.exist?(file_name).should ==  false
        File.delete(file_name1) if File.exist?(file_name1)
        File.exist?(file_name1).should ==  false
        Dir.delete(dir_name) if Dir.exist?(dir_name)
        Dir.exist?(dir_name).should ==  false

        System.unzip_file(file_name_zip)
        Dir.exist?(dir_name).should == true
        File.exist?(file_name).should == true
        File.exist?(file_name1).should == true
    end    
    
end

end

describe "RhoConfig" do

  before(:each) do
    Rho::RhoConfig.start_path = '/app'
    Rho::RhoConfig.options_path = '/app/Settings'
  end
  
unless System.get_property('platform') == 'WINDOWS' && System.get_property('is_emulator')
  it "should populate configuration in sources table" do
    sources = ::Rho::RHO.get_user_db().select_from_table('sources','*')
    sources.size.should == 0
    
    Rho::RhoConfig.sources().size.should > 0
  end
end
  
  it "should have start_path" do
    Rho::RhoConfig.start_path.should == '/app'
  end
  
  it "should retrieve start_path" do
    Rho::RhoConfig.start_path.should == '/app'
  end
  
  it "should set start_path" do
    oldpath = Rho::RhoConfig.start_path
    Rho::RhoConfig.start_path = '/foo/bar'
    Rho::RhoConfig.start_path.should == '/foo/bar'
    Rho::RhoConfig.start_path = oldpath
  end
  
  it "should have options_path" do
    Rho::RhoConfig.options_path.should == '/app/Settings'
  end
  
  it "should set options_path" do
    oldpath = Rho::RhoConfig.options_path
    Rho::RhoConfig.options_path = '/ops2'
    Rho::RhoConfig.options_path.should == '/ops2'
    Rho::RhoConfig.options_path = oldpath
  end
  
  it "should read arbitrary options" do
    Rho::RhoConfig.arbitrary_option.should == 'rhodes rocks!'
  end
  
  it "should reload" do
    oldpath = Rho::RhoConfig.start_path
    Rho::RhoConfig.reload
    Rho::RhoConfig.start_path.should == oldpath
  end  
  
if !defined?(RHO_WP7)
  it "should read log" do
  #  log_text = Rho::RhoConfig.read_log
  #  log_text.length.should > 0
  
    log_text = Rho::RhoConfig.read_log(20000)
    log_text.length.should <= 20000
    
    log_text = Rho::RhoConfig.read_log(1000)
    log_text.length.should == 1000
  end  
end
  
end

describe "RhoError" do

  it "should save error code" do
    error = Rho::RhoError.new(Rho::RhoError::ERR_RUNTIME);
    error.code.should == Rho::RhoError::ERR_RUNTIME
  end

  it "should catch the error" do
    error = Rho::RhoError.new(Rho::RhoError::ERR_RUNTIME);
    begin
      raise error
    rescue Rho::RhoError
      $!.code.should == Rho::RhoError::ERR_RUNTIME
    end
  end

end

describe "RhomSource" do

  it "should find first source" do
    Rhom::RhomSource.load_all_sources
    src = Rhom::RhomSource.find(:first)
    src.should_not be_nil
  end

  it "should find all source" do
    srcs = Rhom::RhomSource.find(:all)
    srcs.should_not be_nil
    
    srcs.length.should > 1
  end
  
  it "should find Product_s source" do
    src = Rhom::RhomSource.find("Product_s")
    src.should_not be_nil
    
    #src.get_lastsync_objectcount.should == 0
    src.source_id.should == Product_s.get_source_id.to_i
    src.name.should == "Product_s"
    src.last_updated.year.should > 1960
    #src.last_inserted_size.should == 0
    #src.last_deleted_size.should == 0
    #src.last_sync_duration.should == 0
    #src.last_sync_success.should == false
    #src.distinct_objects.should == 0
    src.backend_refresh_time.year.should > 1960
    src.partition.should == "user"
    src.schema.length.should > 0
    src.schema_version.length.should > 0
    
  end

end

describe "RhoRuby" do

  it "should compute string to_f to test flt_rounds" do
    "100.250".to_f.should == 100.25
  end
  
  it "should compute == on float to test flt_rounds" do
    1.0.should == 1
  end

  it "should create octet string" do
    val = "\1\2\3\0\5\8\6\7\34\39" #octet numbers
    #val[3].should == "\x00"
    #val[8].should == "\x1C"
    #val[9].should == "\x03"
    #val[10].should == '9'
  end
if !defined?(RHO_WP7)
  it "should support encoding" do
    "Utf8 String".force_encoding( Encoding::UTF_8 )
  end
end
  
  it "should create hex string" do
    val = "\x21\x43"
    #val[0].should == "\x21"
    #val[1].should == "\x43"
  end

  it "should base64 binary string" do
    val = ["\1\2\3\0\5\8\6\7\34"]
    str = val.pack('m')
    str.should_not be_nil
    
    res = str.unpack('m')
    res.should_not be_nil
    
    res[0].should == val[0]
  end
    
  it "split should have nil" do
    line = "Account|parent_name|44e804f2-4933-4e20-271c-48fcecd9450d||\n"
    parts = line.chomp.split('|')

    parts.should == ['Account','parent_name','44e804f2-4933-4e20-271c-48fcecd9450d']
    parts[3].should be_nil
    parts[4].should be_nil
  end

  it "should empty fields when limit isn't given" do
    "1,2,,3,4,,".split(',').should == ["1", "2", "", "3", "4"]
  end
  
  it "should call to_a on string" do
    s = "abcdef"
if !defined? RHO_ME    
    #https://www.pivotaltracker.com/story/show/4292906
    a = s.chars.to_a
    a[0].should == "a"
    a[3].should == "d"
end    
    ch = '1'
    s.each_char do |c|
        ch =  c
    end
    
    ch.should == 'f'
  end
    
  it "should call each on array of hashes"  do
    id = 8775
    providers = [{:IsMandatory=>"true", :RequiresSaveOrderToFile=>"true", :HasFarmPackageAccess=>"true", :OfficeID=>"2354", :OfficeName=>"Poway Realty Services", :ProviderID=>"8775", :ProviderFirstName=>"Jodie", :ProviderLastName=>"Fagan", :ProviderName=>"Jodie Fagan"}]

    provider = {}
    providers.each do |e|
	    if e[:ProviderID].to_i == id
		    provider = e
		    break
	    end
    end
    
    provider.should_not be_nil
    provider[:OfficeID].should == "2354"
  end

  it "should calll []= on string with non-empty string"  do

    file = "some file text with [propertyOwnerName] within"

    replace_str = 'a' 

    file["[propertyOwnerName]"] = replace_str
    
    file.should == "some file text with a within"
  end  

  it "should calll []= on string with empty string"  do

    file = "some file text with [propertyOwnerName] within"

    replace_str = ''

    file["[propertyOwnerName]"] = replace_str
    
    file.should == "some file text with  within"
  end  

  it "should test string split" do 
    s = "password="  
    key, value = s.split('=', 2)
    
    key.should == "password"
    value.should == ""
    
    key1, value1 = s.split('=')
    
    key1.should == "password"
    value1.should be_nil
    
  end

  it "should test string split with limit" do 
    s = "splash_screen='delay=2;center'"  
    key, value = s.split('=', 2)
    
    key.should == 'splash_screen'
    value.should == "'delay=2;center'"
  end                              

  it "should test string split without parameters" do 
    res = "Revision: 18805".split[1]
    res.should == "18805"
  end
   
  it "should require with diff case" do
    require "Data/reqTest.rb"
    
    ReqTest.get_name.should == "reqTest"
  end
  
  it "should array count" do
    ary = [1, 2, 4, 2]
    ary.count.should == 4
if !defined?(RHO_WP7)
    ary.count(2).should == 2
    ary.count{|x|x%2==0}.should == 3
end
  end

if !defined?(RHO_WP7)
  it "should array group_by" do
    res = [1,2,3,4,5,6].group_by {|i| i%3}
    res.should == {0=>[3, 6], 1=>[1, 4], 2=>[2, 5]}
  end


  it "should range group_by" do
    res = (1..6).group_by {|i| i%3}
    res.should == {0=>[3, 6], 1=>[1, 4], 2=>[2, 5]}
  end
end  

  it "should split with regex"  do
    str = "http://www.abc.com/abc/servlet/SearchServlet?act=viewDetail&amp;LanguageCountry=en_US&amp;searchLang=en_US&amp;caseLang=en_US&amp;orgPrefix=NCMC&amp;caseNum=1234567&amp;seqNum=1"

    ufld = str.split(/\&amp;/)

    ufld[0].should == "http://www.abc.com/abc/servlet/SearchServlet?act=viewDetail"
    ufld[1].should == "LanguageCountry=en_US"
    ufld[2].should == "searchLang=en_US"
    ufld[3].should == "caseLang=en_US"
    ufld[4].should == "orgPrefix=NCMC"
    ufld[5].should == "caseNum=1234567"
    ufld[6].should == "seqNum=1"
  end

  it "should slice array"  do
      a = [ "a", "b", "c", "d", "e" ]
      (a[2] +  a[0] + a[1]).should == "cab"
      a[6].should be_nil
      a[1, 1].should == [ "b" ]
      a[1, 2].should == [ "b", "c" ]
      a[1..3].should == [ "b", "c", "d" ]
      a[4..7].should == [ "e" ]
      a[6..10].should be_nil
      a[-3, 3].should == [ "c", "d", "e" ]
      # special cases
      a[5].should be_nil
      a[5, 1].should == []
      a[5..10].should == []
  end

  it "should call Float.to_f"  do
    ftest = 0.34
    ftest.is_a?(Float).should == true
    
    fres = ftest.to_f
    fres.is_a?(Float).should == true
    
    fres.should == ftest
    
  end

  it "should split empty string"  do
    block = nil
    if block.nil?
        buffer = []
    else
        buffer = block.split('')
    end
    
    buffer.should_not be_nil
    buffer.should == []
    buffer.length.should == 0
  end

  it "should split empty string1"  do
  
    block = '' if block.nil?
    buffer = block.split('')
    
    buffer.should_not be_nil
    buffer.should == []
    buffer.length.should == 0
  end

  it "should parse xml date"  do

    now_at = "2011-02-05T21:10:11.123T001"

    date = now_at.split("T")
    date[1]["Z"] = "" if date[1]["Z"]
    day = date[0].split("-")
    time = date[1].split(":")
    
    day[0].should == '2011'
    day[1].should == '02'
    day[2].should == '05'
    time[0].should == '21'
    time[1].should == '10'
    time[2].should == "11.123"
    
    unless System.get_property('is_emulator') && System.get_property('platform') == 'APPLE'
      var_time= Time.local(day[0],day[1],day[2],time[0],time[1],time[2])
      res = var_time.strftime("%b %d, %y, %I:%M%p")

      res.should == "Feb 05, 11, 09:10PM"
    end
  
  end
        
end
