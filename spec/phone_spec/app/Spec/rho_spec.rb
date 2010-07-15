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

describe "RhoConfig" do

  before(:each) do
    Rho::RhoConfig.config['start_path'] = '/app'
    Rho::RhoConfig.config['options_path'] = '/app/Settings'
  end
  
  it "should populate configuration in sources table" do
    sources = ::Rho::RHO.get_user_db().select_from_table('sources','*')
    sources.size.should > 1
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

describe "RhoRuby" do
  
  it "should compute string to_f to test flt_rounds" do
    "100.250".to_f.should == 100.25
  end
  
  it "should compute == on float to test flt_rounds" do
    1.0.should == 1
  end
  
  it "split should have nil" do
    line = "Account|parent_name|44e804f2-4933-4e20-271c-48fcecd9450d||\n"
    parts = line.chomp.split('|')
    
    parts[3].should be_nil
    parts[4].should be_nil
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
  
end
