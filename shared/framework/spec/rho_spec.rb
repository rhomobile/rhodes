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

  it "should populate configuration in sources table" do
    sources = Rhom::RhomDbAdapter::select_from_table('sources','*')
    puts 'sources: ' + sources.inspect
    
    sources.size.should == 3
  end
  
  it "should initialize configuration only once" do
    Rhom::RhomDbAdapter::delete_all_from_table('sources')
    @rho.sources_initialized?.should == false
    @rho.init_sources
    @rho.sources_initialized?.should == true
  end
end
