#
#  rhoerror_spec.rb
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
