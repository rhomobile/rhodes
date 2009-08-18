#
#  rhom_object.rb
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
module Rhom
  module RhomObject    
    def strip_braces(str=nil)
      str ? str.gsub(/\{/,"").gsub(/\}/,"") : nil
    end
  
    # use djb hash function to generate temp object id
    def djb_hash(str, len)
      hash = 5381
      for i in (0..len) 
        hash = ((hash << 5) + hash) + str[i].to_i
      end
      return hash
    end
    
    def extract_options(arr=[])
      arr.last.is_a?(Hash) ? arr.pop : {}
    end
    
    @@reserved_names = {"object" => "1",
                      "source_id" => "1",
                      "update_type" => "1",
                      "attrib_type" => "1",
                      "type" => "1",
                      "set_notification" => "1",
                      "clear_notification" => "1" }
    

    def RhomObject.method_name_reserved?(method)
      @@reserved_names.has_key?(method)
    end
  end # RhomObject
end # Rhom