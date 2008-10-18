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
  
  def initialize(new_classname, source_id)
    @classname = new_classname
    Object::const_set("SourceId", source_id) unless defined? SourceId
    puts 'source_id constant: ' + SourceId
    puts 'classname: ' + @classname
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
            def find(*args)
              puts 'inside find...'
              query = nil
              if args.first == :all
                query = "select * from #{TABLE_NAME} where source_id=#{SourceId}"
              else 
                query = "select * from #{TABLE_NAME} where object='#{args.first.to_s}'"
              end
              puts 'query is: ' + query
              result = Rhom::execute_sql(query)
              puts 'RESULT IS: ' + result.inspect
              get_list(result)
            end
    
            def find_by_sql(sql)
              result = Rhom::execute_sql(sql)
              get_list(result)
            end
            
            # returns an array of objects based on an existing array
            def get_list(objs)
              puts 'inside get_list...'
              puts ''
              puts ''
              new_list = nil
              if objs
                new_list = []
                new_obj = nil
                # initialize previous object id to the first in the array
                objs.each_with_index do |obj, i| 
                  if i == 0
                    new_obj = get_new_obj(obj)
                    new_obj.send obj['attrib'].to_sym, obj['value'].to_s
          
                  elsif obj['object'] != objs[i-1]['object']
                    puts 'adding object to list: ' + new_obj.inspect
                    new_list << new_obj
                    new_obj = get_new_obj(obj)
                    new_obj.send obj['attrib'].to_sym, obj['value'].to_s
          
                  elsif obj['object'] == objs[i-1]['object']
                    new_obj.send obj['attrib'].to_sym, obj['value'].to_s
                    if i == objs.length - 1
                      puts 'adding object to list: ' + new_obj.inspect
                      new_list << new_obj
                    end
          
                  elsif i == objs.length - 1
                    puts 'adding object to list: ' + new_obj.inspect
                    new_list << new_obj
                  end
                end 
              end
              new_list
            end
  
            def get_new_obj(obj)
              
              tmp_obj = self.new
              tmp_obj.send 'object'.to_sym, obj['object'].to_s
              tmp_obj.send 'source_id'.to_sym, obj['source_id'].to_s
              tmp_obj
            end
            
          end #class methods
		  
		  def create(obj)
			#TODO: Generate query based on attribute list
		  end
        
          def update(attributes)
            #TODO: Inspect attributes and generate sql
          end
		  
		  def destroy
			query = "insert into #{TABLE_NAME} (object, update_type) values ('#{self.object}', 'delete')"
		  end
		  
		  def save
            #TODO: Implement save
            get_list(Rhom::execute_sql(""))
          end

        end)
    end
  end
end