#
#  rhom_object_factory.rb
#  rhodes
#  Returns an array of RhomObjects
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
require 'rhom_object'

class RhomObjectFactory
  attr_accessor :obj_list, :classname, :attrib_count
  
  def initialize(new_classname)
    @classname = new_classname
    puts 'classname: ' + @classname
    init_object 
  end
  
  def init_attrib_count(list)
    puts 'count: ' + list[0]['count']
    @attrib_count = list[0]['count'].to_i
  end
  
  # Initialize new object with dynamic attributes(
  def init_object
    Object::const_set(@classname.intern, 
      Class::new do
        include RhomObject
        extend RhomObject
      end)
  end
  
  # returns an array of objects based on an existing array
  def get_list(objs)
    new_list = nil
    if objs
      new_list = []
      objs.each do |obj|
        p 'object: ' + obj.inspect
        new_obj = Object::const_get(@classname).new
        
        obj.each_pair do |key, value|
          #puts 'key, value: ' + key.to_s + ', ' + value.to_s
          new_obj.send key.to_sym, value.to_s unless key == 'id'
        end
        new_list << new_obj
      end
      
    end
    new_list
  end
end