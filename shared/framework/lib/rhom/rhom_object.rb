#
#  rhom_object.rb
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
module Rhom
  module RhomObject
    # defines a method at runtime for the
    # dynamically created class
    
    
    # we override method_missing here so that instance variables,
    # when retrieved or set, are added to the object
    def method_missing(name, *args)
      unless name == Fixnum
        varname = name.to_s.gsub(/=/,"")
        setting = (name.to_s =~ /=/)
        inst_var = nil

        if setting
          inst_var = instance_variable_set( "@#{varname}", args[0] )  
        else
          inst_var = instance_variable_get( "@#{varname}" )
        end
        
        inst_var
      end
    end
    
    def remove_var(name)
      remove_instance_variable("@#{name}")
    end
  
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
    
    def method_name_reserved?(method)
      method =~ /object|source_id|update_type|type/
    end
  end # RhomObject
end # Rhom