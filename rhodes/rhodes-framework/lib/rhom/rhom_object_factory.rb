#
#  rhom_object_factory.rb
#  rhodes
#  Returns an array of RhomObjects
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
require 'rhom'
require 'rho'
require 'rho/rhosupport'

module Rhom
  class RhomObjectFactory
    
    def initialize
	    unless not defined? Rho::RhoConfig::sources
  	    init_objects
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
                self.send("object=".to_sym(), "#{((Time.now.to_f - Time.mktime(2009,"jan",1,0,0,0,0).to_f) * 10**6).to_i}")
                if obj
                  self.send("source_id=".to_sym(), obj['source_id'].to_s)
                  self.send("update_type=".to_sym(), 'create')
                  obj.each do |key,value|
					          val =  self.inst_strip_braces(value)
                    self.send("#{key}=".to_sym(), val)
                  end
                end
            
              end
		  
              class << self
              
                def count
                  ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME,
                                                           'object',
                                                           {"source_id"=>get_source_id},
                                                           {"distinct"=>true}).length
                end
              
                def get_source_id
                  Rho::RhoConfig::sources[self.name.to_s]['source_id'].to_s
                end
                
                # retrieve a single record if object id provided, otherwise return
                # full list corresponding to factory's source id
                def find(*args)
                  list = []
                  hash_list = {}
                  conditions = {}

                  # first find all query objects
                  if args.first == :all
                    conditions = {"source_id"=>get_source_id}
                  elsif args.first.is_a?(String)
                    conditions = {"object"=>strip_braces(args.first.to_s)}
                  end
                  
                  # do we have conditions?
                  # if so, add them to the query
                  condition_hash = {}
                  select_arr = nil
                  if args[1]
                    condition_hash = get_conditions_hash(args[1][:conditions]) if args[1] and args[1][:conditions] and args[1][:conditions].is_a?(Hash)
                    conditions.merge!(condition_hash)
                    
                    select_arr = args[1][:select] if args[1][:select]
                  end

                  # process query, create, and update lists in order
                  ["query", "create", "update"].each do |update_type|
                    conditions.merge!({"update_type"=>update_type})
                    objs = ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME, '*', conditions, {"order by"=>'object'},select_arr)

                    # fetch the rest of the attributes if we're searching by specific attrib value
                    if conditions and conditions.size > 0
                      full_objects = []
                      objs.each do |obj|
                        if not select_arr
                          full_objects += ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME, '*', {'object' => obj['object'].to_s})
                        elsif select_arr and conditions
                          full_objects += ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME, '*', {'object' => obj['object'].to_s}, nil, select_arr)
                        end
                      end
                      objs = full_objects
                    end

                    # build up the object array where each
                    # row in this array is a rhom_object
                    objs.collect! do |obj|
                      object = obj['object']
                      attrib = obj['attrib']
                      value = obj['value']
                      hash_list[object] = get_new_obj(obj) if not hash_list[object]
                      if not method_name_reserved?(attrib) and hash_list[object].send(attrib.to_sym)
                        hash_list[object].remove_var(attrib)
                      end
                      hash_list[object].send("#{attrib}=".to_sym(), value) if not method_name_reserved?(attrib)
                      nil # remove the element from the array
                    end
                    
                  end

                  # convert hash to array
                  list = hash_list.values
                  hash_list = nil
                  
                  # setup order by if provided
                  order = extract_options(args)
                  order_value = order[:order] if order and order[:order]
                  if order_value
                    order_sym = order_value.to_sym
                    list.sort! {|x,y| x.send(order_sym) && y.send(order_sym) ? x.send(order_sym) <=> y.send(order_sym) : 0}
                  end
                  
                  # return a single rhom object if searching for one
                  if args.first == :first or args.first.is_a?(String)
                    return list[0]
                  end
                  list
                end
              
                def find_all(args={})
                  find(args)
                end
  
                def set_notification(url,params)
                  SyncEngine.set_notification(get_source_id.to_i,url,params)
                end
                
                def clear_notification
                  SyncEngine.clear_notification(get_source_id.to_i)
                end
                
                def ask(question)
                  tmp_obj = get_new_obj(:object =>djb_hash("#{question}#{rand.to_s}", 10).to_s)
                  if question
                    # We only support one ask at a time!
                    ::Rhom::RhomDbAdapter::delete_from_table(::Rhom::TABLE_NAME,
                                                              {"source_id"=>get_source_id,
                                                               "update_type"=>'ask'})
                    ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                              {"source_id"=>get_source_id,
                                                               "object"=>tmp_obj.object,
                                                               "attrib"=>'question',
                                                               "value"=>Rho::RhoSupport.url_encode(question),
                                                               "update_type"=>'ask'})
                    SyncEngine::dosync
                  end
                end
                
                # deletes all records matching conditions (optionally nil)
                def delete_all(conditions=nil)
                  if conditions
                    del_conditions = get_conditions_hash(conditions[:conditions])
                    # find all relevant objects, then delete them
                    del_objects = ::Rhom::RhomDbAdapter::select_from_table(::Rhom::TABLE_NAME,
                                                                           'object',
                                                                           del_conditions.merge!({"source_id"=>get_source_id}),
                                                                           {"distinct"=>true})
                    del_objects.each do |obj|                                                       
                      ::Rhom::RhomDbAdapter::delete_from_table(::Rhom::TABLE_NAME, {'object'=>obj['object']})
                    end
                  else
                    ::Rhom::RhomDbAdapter::delete_from_table(::Rhom::TABLE_NAME, {"source_id"=>get_source_id})
                  end
                end
                    
                private
                # returns new model instance with a temp object id
                def get_new_obj(obj)
                  tmp_obj = self.new
                  tmp_obj.send("object=".to_sym(), "{#{obj['object'].to_s}}")
                  tmp_obj
                end
                
                # get hash of conditions in sql form
                def get_conditions_hash(conditions=nil)
                  if conditions
                    condition_hash = {}
                    conditions.each do |key,value|
                      condition_hash.merge!('attrib' => key.to_s, 'value' => value.to_s)
                    end
                    condition_hash
                  else
                    nil
                  end
                end
              end #class methods
	
              # deletes the record from the viewable list as well as
              # adding a delete record to the list of sync operations
              def destroy
                result = nil
                obj = self.inst_strip_braces(self.object)
                update_type=self.get_update_type_by_source('delete')
                if obj
                  # first delete the record from viewable list
                  result = ::Rhom::RhomDbAdapter::delete_from_table(::Rhom::TABLE_NAME,
                                                            {"object"=>obj})
                  if update_type
                    # now add delete operation
                    result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                              {"source_id"=>self.get_inst_source_id,
                                                               "object"=>obj,
                                                               "update_type"=>update_type})
                  end
                end
                result
              end
		
              # saves the current object to the database as a create type
              def save
                result = nil
                # iterate over each instance variable and insert create row to table
				        obj = self.inst_strip_braces(self.object)
				        update_type=self.get_update_type_by_source('create')
                self.instance_variables.each do |method|
                  method = method.to_s.gsub(/@/,"")
                  # Don't save objects with braces to database
                  val = self.inst_strip_braces(self.send(method.to_sym))
                  # add rows excluding object, source_id and update_type
                  unless self.method_name_reserved?(method)
                    fields = {"source_id"=>self.get_inst_source_id,
                              "object"=>obj,
                              "attrib"=>method,
                              "value"=>val,
                              "update_type"=>update_type}
                    fields = method == "image_uri" ? fields.merge!({"attrib_type" => "blob.file"}) : fields
                    result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME, fields)                                     
                  end
                end
                result
              end
          
              # updates the current record in the viewable list and adds
              # a sync operation to update
              def update_attributes(attrs)
                result = nil
                obj = self.inst_strip_braces(self.object)
                update_type=self.get_update_type_by_source('update')
                attrs.each do |attrib,val|
                  attrib = attrib.to_s.gsub(/@/,"")
                  old_val = self.send attrib.to_sym unless self.method_name_reserved?(attrib)
                  
                  # Don't save objects with braces to database
                  new_val = self.inst_strip_braces(val)
                  
                  # if the object's value doesn't match the database record
                  # then we procede with update
                  if old_val != new_val
                    unless self.method_name_reserved?(attrib)
                      ::Rhom::RhomDbAdapter::delete_from_table(::Rhom::TABLE_NAME,
                                                                {"source_id"=>self.get_inst_source_id,
                                                                 "object"=>obj,
                                                                 "attrib"=>attrib,
                                                                 "update_type"=>update_type})
                      # update sync list
                      result = ::Rhom::RhomDbAdapter::insert_into_table(::Rhom::TABLE_NAME,
                                                                {"source_id"=>self.get_inst_source_id,
                                                                 "object"=>obj,
                                                                 "attrib"=>attrib,
                                                                 "value"=>new_val,
                                                                 "update_type"=>update_type})
                    end
                  end
                end
                result
              end
	
              def get_inst_source_id
                Rho::RhoConfig::sources[self.class.name.to_s]['source_id'].to_s
              end
              
              def get_update_type_by_source(update_type)
                source_type = Rho::RhoConfig::sources[self.class.name.to_s]['type']
                if source_type and source_type == "ask" and update_type == 'delete'
                  nil
                elsif source_type and source_type == "ask"
                  "query"
                else
                  update_type
                end
              end
              
              def inst_strip_braces(str=nil)
                str ? str.gsub(/\{/,"").gsub(/\}/,"") : nil
              end
            end)
        end
      end
    end
  end # RhomObjectFactory
end # Rhom