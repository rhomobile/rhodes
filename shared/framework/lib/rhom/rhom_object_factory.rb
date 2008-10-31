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
require 'rhom'
require 'rhom/rhom_object'
require 'rho'

module Rhom
  class RhomObjectFactory
  
    def initialize
	  unless not defined? Rho::RhoConfig::sources
	    init_source_attribs
	    init_objects
		puts "inside RhomObjectFactory.initialize, sources are #{Rho::RhoConfig::sources.inspect}"
	  end
    end
	
	def init_source_attribs
	  # merge source attributes into config hash
	  # TODO: This shouldn't reference 'source[1]' directly
	  Rho::RhoConfig::sources.each do |source|
	    puts "inside merge loop: #{source.inspect}"
		src_attribs = Rhom::execute_sql "select distinct attrib from #{TABLE_NAME} \
										 where source_id=#{source[1]['source_id'].to_s}"
		# update our source with the proper attributes
		source[1].merge!({"attribs"=>src_attribs})
	  end
	end
  
    # Initialize new object with dynamic attributes
    def init_objects
      Rho::RhoConfig::sources.each do |classname,source|
        unless Object::const_defined?(classname.intern)
          Object::const_set(classname.intern, 
            Class::new do
              include RhomObject
              extend RhomObject
          
              def initialize(obj=nil)
                if obj
                  # create a temp id for the create type
                  # TODO: This is duplicative of get_new_obj
                  temp_objid = djb_hash(obj.values.to_s, 10).to_s
                  self.send 'object'.to_sym, "#{temp_objid}"
                  self.send 'source_id'.to_sym, obj['source_id'].to_s
                  self.send 'update_type'.to_sym, 'create'
                  obj.each do |key,value|
					          val =  self.inst_strip_braces(value)
                    self.send key, val
                  end
                end
            
              end
		  
              class << self
              
                def get_source_id
                  Rho::RhoConfig::sources[self.name.to_s]['source_id'].to_s
                end
                # retrieve a single record if object id provided, otherwise return
                # full list corresponding to factory's source id
                def find(*args)
                  list = []
                  if args.first == :all
                    query = "select * from #{TABLE_NAME} where \
                             source_id=#{get_source_id} \
                             and update_type='query' order by object"
                  else
                    obj = strip_braces(args.first.to_s)
                    query = "select * from #{TABLE_NAME} where object='#{obj}'"
                  end
                  result = Rhom::execute_sql(query)
                  list = get_list(result)
                  if list.length == 1
                    return list[0]
                  end
                  list
                end
              
                def find_by(*args)
                  # TODO: implement
                end
    
                def find_by_sql(sql)
                  result = Rhom::execute_sql(sql)
                  get_list(result)
                end
            
                # returns an array of objects based on an existing array
                def get_list(objs)
                  new_list = []
                  if objs and defined? Rho::RhoConfig::sources[self.name.to_s]
                    attrib_length = Rho::RhoConfig::sources[self.name.to_s]['attribs'].length
                    list_length = 0
                    list_length = (objs.length / attrib_length) unless attrib_length == 0
                    new_obj = nil
                    # iterate over the array and determine object
                    # structure based on attribute/value pairs
                    list_length.times do |i|
                      new_obj = get_new_obj(objs[i*attrib_length])
                      attrib_length.times do |j|
                        # setup index and assign accessors
                        idx = i*attrib_length+j
                        begin
                          # only update attributes if they belong
                          # to the current object
                          if objs[idx]['object'] == strip_braces((new_obj.send 'object'.to_sym))
                            attrib = objs[idx]['attrib'].to_s
                            value = objs[idx]['value'].to_s
                            new_obj.send attrib.to_sym, value
                          end
                        rescue
                          puts "failed to reference objs[#{idx}]..."
                        end
                      end
                      new_list << new_obj
                    end
                  else
                    # source attributes are not initialized, 
                    # try again
                    RhomObjectFactory::init_sources
                  end
                  new_list
                end
  
                # returns new model instance with a temp object id
                def get_new_obj(obj, type='query')
                  tmp_obj = self.new
                  tmp_obj.send 'object'.to_sym, "{#{obj['object'].to_s}}"
                  tmp_obj.send 'source_id'.to_sym, get_source_id
                  tmp_obj.send 'update_type'.to_sym, type
                  tmp_obj
                end
              end #class methods
	
              # deletes the record from the viewable list as well as
              # adding a delete record to the list of sync operations
              def destroy
                result = nil
                obj = self.inst_strip_braces(self.object)
                if obj
                  # first delete the record from viewable list
                  query = "delete from #{TABLE_NAME} where object='#{obj}'"
                  result = Rhom::execute_sql(query)
                  # now add delete operation
                  query = "insert into #{TABLE_NAME} (source_id, object, update_type) \
                           values (#{self.get_inst_source_id}, '#{obj}', 'delete')"
                  result = Rhom::execute_sql(query)
                end
                result
              end
		
              # saves the current object to the database as a create type
              def save
                result = nil
                # iterate over each instance variable and insert create row to table
				        obj = self.inst_strip_braces(self.object)
                self.instance_variables.each do |method|
                  method = method.to_s.gsub(/@/,"")
                  # Don't save objects with braces to database
                  val = self.send(method.to_sym)
                  # add rows excluding object, source_id and update_type
                  unless self.method_name_reserved?(method) or val.nil?
                    query = "insert into #{TABLE_NAME} (source_id, object, attrib, value, update_type) values \
                             (#{self.get_inst_source_id}, \
                             '#{obj}', \
                             '#{method}', \
                             '#{val}', \
                             'create')"
                    result = Rhom::execute_sql(query)
                  end
                end
                # Create a temporary query record to display in the list
                Rho::RhoConfig::sources[self.class.name.to_s]['attribs'].each do |attrib|
                  query = "insert into #{TABLE_NAME} (source_id, object, attrib, value, update_type) values \
                           (#{self.get_inst_source_id}, \
                           '#{obj}', \
                           '#{attrib['attrib']}', \
                           '#{self.send(attrib['attrib'].to_sym)}', \
                           'query')"
                  result = Rhom::execute_sql(query)
                end
                result
              end
          
              # updates the current record in the viewable list and adds
              # a sync operation to update
              def update_attributes(attrs)
                result = nil
                obj = self.strip_braces(self.object)
                self.instance_variables.each do |method|
                  method = method.to_s.gsub(/@/,"")
                  val = self.send method.to_sym
                  # Don't save objects with braces to database
                  new_val = attrs[method]
                  # if the object's value doesn't match the database record
                  # then we procede with update
                  if new_val and val != new_val
                    unless self.method_name_reserved?(method) or new_val.length == 0
                      # update viewable list
                      query = "update #{TABLE_NAME} set value='#{new_val}' where object='#{obj}' \
			                         and attrib='#{method}'"
                      result = Rhom::execute_sql(query)
                      # update sync list
                      query = "insert into #{TABLE_NAME} (source_id, object, attrib, value, update_type) values \
                               (#{self.get_inst_source_id}, \
                               '#{obj}', \
                               '#{method}', \
                               '#{new_val}', \
                               'update')"
                      result = Rhom::execute_sql(query)
                    end
                  end
                end
                result
              end
	
              def get_inst_source_id
                Rho::RhoConfig::sources[self.class.name.to_s]['source_id'].to_s
              end
              
              def inst_strip_braces(str=nil)
                str ? str.gsub(/\{/,"").gsub(/\}/,"") : nil
              end
            
              def method_name_reserved?(method)
                method =~ /object|source_id|update_type/
              end
            end)
        end
      end
    end
  end # RhomObjectFactory
end # Rhom