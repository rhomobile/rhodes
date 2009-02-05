#
#  rho_spec.rb
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
require File.dirname(__FILE__) + "/spec_helper"

describe "Rho" do

  it_should_behave_like "rho initializer"
  it_should_behave_like "rho db initializer"

  it "should populate configuration in sources table" do
    sources = Rhom::RhomDbAdapter::select_from_table('sources','*')
    sources.size.should > 1
  end

  it "should initialize configuration only once" do
    Rhom::RhomDbAdapter::delete_all_from_table('sources')
    @rho.source_initialized?(1).should == false
    @rho.init_sources
    @rho.source_initialized?(1).should == true
  end

  it "should have start_path" do
    Rho::RhoConfig.start_path.should == '/'
  end

  it "should set start_path" do
    Rho::RhoConfig.start_path = '/foo/bar'
    Rho::RhoConfig.start_path.should == '/foo/bar'
  end

  it "should retrieve start_path" do
    Rho::RhoConfig.start_path = '/'
    @rho.get_start_path.should == '/'
  end

  it "should return from get_app" do
    pending "fix relative paths for testing of get_app"
  end

  it "should serve request" do
    pending "need to mock request"
  end
end
