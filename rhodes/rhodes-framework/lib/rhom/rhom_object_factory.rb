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
	    unless not defined? Rho::RhoConfig.sources
  	    init_objects
  	  end
    end
  
    # Initialize new object with dynamic attributes
    def init_objects
      Rho::RhoConfig.sources.each do |classname,source|
        unless Object.const_defined?(classname.intern)
          Object.const_set(classname.intern, 
            Class.new do
              include ::Rhom::RhomObject
              extend ::Rhom::RhomObject
              
              # This holds the attributes for an instance of
              # the rhom object
              attr_accessor :vars
          
              def initialize(obj=nil)
                @vars = {}
                self.vars['object'] = "#{((Time.now.to_f - Time.mktime(2009,"jan",1,0,0,0,0).to_f) * 10**6).to_i}"
                if obj
                  obj.each do |key,value|
                    self.vars[key] = value
                  end
                end
              end
              
              def method_missing(name, *args)
                unless name == Fixnum
                  varname = name.to_s.gsub(/\=/,"")
                  setting = (name.to_s =~ /=/)
                  if setting
                    @vars[varname] = args[0]  
                  else
                    @vars[varname]
                  end
                end
              end
		  
              class << self
              
                def count
                  ::Rhom::RhomDbAdapter.select_from_table('object_values','object', {"source_id"=>get_source_id}, {"distinct"=>true}).length
                end
              
                def get_source_id
                  Rho::RhoConfig.sources[self.name.to_s]['source_id'].to_s
                end
                
                # retrieve a single record if object id provided, otherwise return
                # full list corresponding to factory's source id
                def find(*args)
                  puts "Inside find: args - #{args.inspect}"
                  ret_list = []
                  conditions = {}
                  where_cond = nil
                  # first find all query objects
                  if args.first == :all
                    where_cond = {"source_id"=>get_source_id}
                  elsif args.first.is_a?(String)
                    where_cond = {"object"=>strip_braces(args.first.to_s)}
                  end

                  # do we have conditions?
                  # if so, add them to the query
                  condition_hash = nil
                  select_arr = nil
                  condition_str = nil
                  if args[1]
                    if args[1][:conditions]
                      condition_hash = args[1][:conditions] if args[1][:conditions].is_a?(Hash)
                      # conditions are a string
                      condition_str = args[1][:conditions] if args[1][:conditions].is_a?(String)
                      # conditions are an array
                      if args[1][:conditions].is_a? (Array)
                        condition_str = args[1][:conditions][0].split(/\?/).each_with_index { |param,i| 
                          param << args[1][:conditions][i+1].to_s
                        }.join(' ').to_s 
                      end
                    end
                    select_arr = args[1][:select] if args[1][:select]
                  end
                  
				          start = Time.new
                  # return horizontal resultset from database
                  # for example, an object that has attributes name,industry:
                  # |               object                 |       name         |  industry   |
                  # ---------------------------------------------------------------------------
                  # | 3560c0a0-ef58-2f40-68a5-48f39f63741b |A.G. Parr PLC 37862 |Entertainment|
                  attribs = get_attribs
                  if attribs and attribs.length > 0
                    sql = ""
                    sql << "SELECT * FROM (\n" if condition_hash or condition_str
                    sql << "SELECT object,\n"
                    attribs.reject! {|attrib| select_arr.index(attrib).nil?} if select_arr
                    start = Time.new
                    attribs.each do |attrib|
                      unless attrib.nil? or attrib.length == 0 or method_name_reserved?(attrib)
                        sql << "MAX(CASE WHEN attrib = '#{attrib}' AND update_type IN (#{::Rhom::UPDATE_TYPES.join(',')}) THEN value ELSE NULL END) AS \"#{attrib}\",\n"
                      end
                    end 
                    sql.chomp!
                    sql.chop!
                    sql << " FROM object_values ov where update_type not in ('delete','update')\n"
                    sql << "AND " + ::Rhom::RhomDbAdapter.where_str(where_cond) + "\n" if where_cond and where_cond.length > 0
                    sql << "group by object\n"
                    sql << "order by \"#{args[1][:order]}\"" if args[1] and args[1][:order]
                    sql << ") WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                    sql << ") WHERE " + condition_str if condition_str
                  
                    list = ::Rhom::RhomDbAdapter.execute_sql(sql)
                    puts "Database query took #{Time.new - start} sec, #{list.length} rows"
                    start = Time.new
                    list.each do |rowhash|
                      # always return object field with surrounding '{}'
                      rowhash['object'] = "{#{rowhash['object']}}"
                      new_obj = self.new
                      new_obj.vars.merge!(rowhash)
                      ret_list << new_obj
                    end
                  end
                  puts "Processing rhom objects took #{Time.new - start} sec, #{ret_list.length} objects"
                  args.first == :first || args.first.is_a?(String) ? ret_list[0] : ret_list
                end
              
                def find_all(args=nil)
                  find(:all, args)
                end
  
                def set_notification(url,params)
                  SyncEngine.set_notification(get_source_id.to_i,url,params)
                end
                
                def clear_notification
                  SyncEngine.clear_notification(get_source_id.to_i)
                end
                
                def ask(question)
                  tmp_obj = self.new(:object =>djb_hash("#{question}#{rand.to_s}", 10).to_s)
                  if question
                    # We only support one ask at a time!
                    ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"source_id"=>get_source_id, "update_type"=>'ask'})
                    ::Rhom::RhomDbAdapter.insert_into_table('object_values', {"source_id"=>get_source_id, "object"=>tmp_obj.object, "attrib"=>'question', "value"=>Rho::RhoSupport.url_encode(question), "update_type"=>'ask'})
                    SyncEngine.dosync
                  end
                end
                
                # deletes all records matching conditions (optionally nil)
                def delete_all(conditions=nil)
                  if conditions
                    del_conditions = get_conditions_hash(conditions[:conditions])
                    # find all relevant objects, then delete them
                    del_objects = ::Rhom::RhomDbAdapter.select_from_table('object_values', 'object', del_conditions.merge!({"source_id"=>get_source_id}), {"distinct"=>true})
                    del_objects.each do |obj|                                                       
                      ::Rhom::RhomDbAdapter.delete_from_table('object_values', {'object'=>obj['object']})
                    end
                  else
                    ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"source_id"=>get_source_id})
                  end
                end
                    
                private
                
                # returns attributes for the source
                def get_attribs
                  attribs = ::Rhom::RhomDbAdapter.select_from_table('object_values','attrib', {"source_id"=>get_source_id}, {"distinct"=>true})
                  attribs.collect! do |attrib|
                    attrib['attrib']
                  end
                  attribs
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
                  result = ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"object"=>obj})
                  if update_type
                    # now add delete operation
                    result = ::Rhom::RhomDbAdapter.insert_into_table('object_values', {"source_id"=>self.get_inst_source_id, "object"=>obj, "update_type"=>update_type})
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
                begin
                    ::Rhom::RhomDbAdapter.start_transaction
                
                    self.vars.each do |key,value|
                      val = self.inst_strip_braces(value)
                      # add rows excluding object, source_id and update_type
                      unless self.method_name_reserved?(key)
                        fields = {"source_id"=>self.get_inst_source_id,
                                  "object"=>obj,
                                  "attrib"=>key,
                                  "value"=>val,
                                  "update_type"=>update_type}
                        fields = key == "image_uri" ? fields.merge!({"attrib_type" => "blob.file"}) : fields
                        result = ::Rhom::RhomDbAdapter.insert_into_table('object_values', fields)                                     
                      end
                    end
                    
                    ::Rhom::RhomDbAdapter.commit

                rescue Exception => e
                    ::Rhom::RhomDbAdapter.rollback    
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
                      # only one update at a time
                      ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"source_id"=>self.get_inst_source_id, "object"=>obj, "attrib"=>attrib, "update_type"=>update_type})
                      # add to syncengine queue
                      ::Rhom::RhomDbAdapter.insert_into_table('object_values', {"source_id"=>self.get_inst_source_id, "object"=>obj, "attrib"=>attrib, "value"=>new_val, "update_type"=>update_type})
                      # update viewable ('query') list
                      ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"source_id"=>self.get_inst_source_id, "object"=>obj, "attrib"=>attrib, "update_type"=>'query'})
                      result = ::Rhom::RhomDbAdapter.insert_into_table('object_values', {"source_id"=>self.get_inst_source_id, "object"=>obj, "attrib"=>attrib, "value"=>new_val, "update_type"=>'query'})
                    end
                  end
                end
                result
              end
	
              def get_inst_source_id
                Rho::RhoConfig.sources[self.class.name.to_s]['source_id'].to_s
              end
              
              def get_update_type_by_source(update_type)
                source_type = Rho::RhoConfig.sources[self.class.name.to_s]['type']
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