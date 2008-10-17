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
  attr_accessor :obj_list, :classname, :attrib_count, :source_id
  
  def initialize(new_classname, source_id)
    @classname = new_classname
    puts 'classname: ' + @classname
    @source_id = source_id
    init_object 
  end
  
  def init_attrib_count(list)
    @attrib_count = list[0]['count']
  end
  
  # Initialize new object with dynamic attributes
  def init_object
    
    unless Object::const_defined?(@classname.intern)
      Object::const_set(@classname.intern, 
        Class::new do
          include RhomObject
          extend RhomObject
        
          class << self
            def find(object)
              query = nil
              if object.to_s == :all.to_s
                query = "select * from #{TABLE_NAME} where source_id=#{@source_id}"
              else
                query = "select * from #{TABLE_NAME} where object=#{object}"
              end
              Rhom::execute_sql query
            end
    
            def find(condition)
              Rhom::execute_sql "select * from #{TABLE_NAME} where #{condition}"
            end
    
            def find_by_sql(sql)
              Rhom::execute_sql sql
            end
          end #class methods
        
          def save
            #TODO: Implement save
            Rhom::execute_sql ""
          end
        
          def update(attributes)
            #TODO: Inspect attributes and generate sql
          end
        end)
    end
  end
  
  # returns an array of objects based on an existing array
  def get_list(objs)
    puts ''
    puts ''
    new_list = nil
    if objs
      new_list = []
      new_obj = nil
      # initialize previous object id to the first in the array
      previous_object = objs[0]['object']
      first = true
      objs.each_with_index do |obj, i| 
        puts 'beginning loop...'
        
        puts 'current object : ' + obj['object']
        puts 'previous object: ' + previous_object
        puts 'first ' + first.to_s
        if obj['object'] == previous_object
          new_obj = assign_attribute(new_obj, obj, false)
          previous_object = obj['object']
          first = false
          
          if i == (objs.length - 1)
            # reached the end of the current object's attributes, need to
            # add it (this only happens if there is only one unique object
            new_list << new_obj
            previous_object = obj['object']
          end
        elsif first
          new_obj = assign_attribute(new_obj, obj, true)
          previous_object = obj['object']
          first = false
        else
          # if we have more than one unique objects with n attributes,
          # we will reach a new object id and add the previous 
          # object to the list
          new_list << new_obj
          previous_object = obj['object']
          first = true
        end
      end 
    end
    new_list
  end
  
  def assign_attribute(new_obj, obj, create=false)
    # since we are flattening the list, only
    # create one new object for the rowset of attributes
    if create || new_obj.nil?
      new_obj = Object::const_get(@classname).new
    end
    
    attrib = obj['attrib']
    val = obj['value']
    puts "assigning attribute, value: #{attrib}, #{val}"
    new_obj.send attrib.to_sym, val.to_s
    new_obj
  end
end