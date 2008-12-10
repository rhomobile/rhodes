#
#  settings_spec.rb
#  rhodes
#
#  Copyright (C) 2008 Lars Burgess. All rights reserved.
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
$:.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))
require 'rho/settings_controller'
require File.dirname(__FILE__) + "/spec_helper"

describe "Rho" do
  
  it_should_behave_like "rho initializer"
  
  before(:all) do
    @controller = Rho::SettingsController.new
  end
  
  it "should get all sources for index" do 
    @controller.get_all_sources.size.should == 4
  end
  
  it "should retrieve source for update" do
    @controller.get_source('{1}')['source_url'].should == 'http://rhosync.rhohub.com/sources/1'
  end
  
  it "should retrieve source without braces" do
    @controller.get_source('1')['source_url'].should == 'http://rhosync.rhohub.com/sources/1'
  end
  
  it "should update source" do
    @controller.update_source({'source_url'=>'http://acme.com/sources/30'},'1')
    @controller.get_source('1')['source_url'].should == 'http://acme.com/sources/30'
  end
end
