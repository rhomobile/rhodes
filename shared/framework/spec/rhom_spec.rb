#
#  rhom_spec.rb
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

describe "Rhom" do

  it_should_behave_like "rho initializer"
  it_should_behave_like "rho db initializer"
  
  it "should get client_id" do
    Rhom::Rhom::client_id.should == '67320d31-e42e-4156-af91-5d9bd7175b08'
  end
  
  it "should perform full reset of database" do
    Rhom::Rhom::database_full_reset
    Rhom::RhomDbAdapter::select_from_table('object_values','*').length.should == 0
    Rhom::RhomDbAdapter::select_from_table('client_info','*').length.should == 0
    Rhom::Rhom::client_id.should be_nil
  end
  
  it "should perform full database reset and logout" do
    Rhom::Rhom::database_full_reset_and_logout
    Rhom::RhomDbAdapter::select_from_table('object_values','*').length.should == 0
    Rhom::RhomDbAdapter::select_from_table('client_info','*').length.should == 0
    Rhom::Rhom::client_id.should be_nil
  end
end