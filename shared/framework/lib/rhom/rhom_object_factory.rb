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
require 'rho'

module Rhom
  class RhomObjectFactory
    
    def initialize
	    unless not defined? Rho::RhoConfig::sources
  	    ::Rhom::RhomObjectFactory::init_source_attribs
  	    init_objects
  	  end
    end
	
	  class << self
	    def init_source_attribs
    	  # merge source attributes into config hash
    	  # TODO: This shouldn't reference 'source[1]' directly
    	  Rho::RhoConfig::sources.each do |source|
    	    src_attribs = ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME,
    	                                                         'attrib',
    	                                                         {"source_id"=>source[1]['source_id'].to_s},
    	                                                         {"distinct"=>true})
    	    # update our source with the proper attributes
    		  source[1].merge!({"attribs"=>src_attribs})
    	  end
    	end
  	end
  
    # Initialize new object with dynamic attributes
    def init_objects
      Rho::RhoConfig::sources.each do |classname,source|
        unless Object::const_defined?(classname.intern)
          Object::const_set(classname.intern, 
            Class::new do
              include ::Rhom::RhomObject
              extend ::Rhom::RhomObject
          
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
                    result = ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME,
                                                              '*',
                                                              {"source_id"=>get_source_id,"update_type"=>'query'},
                                                              {"order by"=>'object'})
                  else
                    obj = strip_braces(args.first.to_s)
                    result = ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME,
                                                              '*',
                                                              {"object"=>obj,"update_type"=>'query'})
                  end
                  list = get_list(result)
                  if list.length == 1 and args.first != :all
                    return list[0]
                  end
                  list
                end
              
                def find_by(*args)
                  # TODO: implement
                end

                # returns an array of objects based on an existing array
                def get_list(objs)
                  new_list = []
                  attrib_length = 0
                  source = Rho::RhoConfig::sources[self.name.to_s]
                  if source
                    attrib_length = source['attribs'].length
                  end
                  if objs and source and attrib_length > 0
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
                    # source attributes are not initialized, try again
                    ::Rhom::RhomObjectFactory::init_source_attribs
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
                  result = ::Rhom::RhomDbAdapter::delete_from_table(::Rhom::TABLE_NAME,
                                                            {"object"=>obj})
                  # now add delete operation
                  result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                            {"source_id"=>self.get_inst_source_id,
                                                             "object"=>obj,
                                                             "update_type"=>'delete'})
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
                  val = self.inst_strip_braces(self.send(method.to_sym))
                  # add rows excluding object, source_id and update_type
                  unless self.method_name_reserved?(method) or val.nil?
                    result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                              {"source_id"=>self.get_inst_source_id,
                                                               "object"=>obj,
                                                               "attrib"=>method,
                                                               "value"=>val,
                                                               "update_type"=>'create'})
                  end
                end
                # Create a temporary query record to display in the list
                Rho::RhoConfig::sources[self.class.name.to_s]['attribs'].each do |attrib|
                  result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                            {"source_id"=>self.get_inst_source_id,
                                                             "object"=>obj,
                                                             "attrib"=>attrib['attrib'],
                                                             "value"=>self.send(attrib['attrib'].to_sym),
                                                             "update_type"=>'query'})
                end
                result
              end
          
              # updates the current record in the viewable list and adds
              # a sync operation to update
              def update_attributes(attrs)
                result = nil
                obj = self.inst_strip_braces(self.object)
                self.instance_variables.each do |method|
                  method = method.to_s.gsub(/@/,"")
                  val = self.send method.to_sym
                  # Don't save objects with braces to database
                  new_val = self.inst_strip_braces(attrs[method])
                  # if the object's value doesn't match the database record
                  # then we procede with update
                  if new_val and val != new_val
                    unless self.method_name_reserved?(method) or new_val.length == 0
                      # update sync list
                      result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                                {"source_id"=>self.get_inst_source_id,
                                                                 "object"=>obj,
                                                                 "attrib"=>method,
                                                                 "value"=>new_val,
                                                                 "update_type"=>'update'})
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