#
#  rhom_spec.rb
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
require 'spec/spec_helper'

describe "Rhom" do
  
  CLIENT_ID = '67320d31-e42e-4156-af91-5d9bd7175b08'

  it_should_behave_like "rhom initializer"
  
  it "should get client_id" do
    Rhom::Rhom::client_id.should == CLIENT_ID
  end
  
#  it "should perform full reset of database" do
#    Rhom::Rhom::database_full_reset
#    verify_reset
#  end
  
#  it "should perform full database reset and logout" do
#    Rhom::Rhom::database_full_reset_and_logout
#    verify_reset
#  end
  
#  it "should call select_from_table with select array" do
#    sel_arr = ['name','industry']
#    @res = ::Rho::RHO.get_user_db().select_from_table(Account.get_values_table_name(), '*', {'object' => '44e804f2-4933-4e20-271c-48fcecd9450d'}, nil, sel_arr)
#    @res.length.should == 2
#  end
  
  def verify_reset
    ::Rho::RHO.get_user_db().select_from_table('object_values','*').length.should == 0
    res = ::Rho::RHO.get_user_db().select_from_table('client_info','*')
    res.length.should == 1
    res[0]['reset'].should == 1
    res[0]['client_id'].should == CLIENT_ID
    
    ::Rho::RHO.get_user_db().select_from_table('sources','*').each do |source|
      source['token'].should == 0
    end
    Rhom::Rhom::client_id.should == CLIENT_ID
  end
end