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
          
              @@g_base_temp_id = nil
              def generate_id
                @@g_base_temp_id = ((Time.now.to_f - Time.mktime(2009,"jan",1,0,0,0,0).to_f) * 10**6).to_i unless @@g_base_temp_id
                
                @@g_base_temp_id = @@g_base_temp_id + 1
                @@g_base_temp_id
              end
              
              def initialize(obj=nil)
                @vars = {}
                self.rhom_init(@vars)
                self.vars[:object] = "#{generate_id()}"
                self.vars[:source_id] = self.get_inst_source_id.to_i
                
                if obj
                  obj.each do |key,value|
                    self.vars[key.to_sym()] = value if key && key.length > 0
                  end
                end
              end
              
              def method_missing(name, *args)
                unless name == Fixnum
                  varname = name.to_s.gsub(/\=/,"")
                  setting = (name.to_s =~ /=/)
                  if setting
                    @vars[varname.to_sym()] = args[0]  
                  else
                    @vars[varname.to_sym()]
                  end
                end
              end
		  
              class << self
              
                def count
                  #SyncEngine.lock_sync_mutex
                  res = ::Rhom::RhomDbAdapter.select_from_table('object_values','object', {"source_id"=>get_source_id}, {"distinct"=>true}).length
                  #SyncEngine.unlock_sync_mutex
                  res
                end
              
                def get_source_id
                  Rho::RhoConfig.sources[self.name.to_s]['source_id'].to_s
                end

                def makeCondWhere(key,value,srcid_value)
                    sql = ""
                    val_op = '='
                    val_func = ''
                    attrib_name = ''
                    if key.is_a?(Hash)
                        val_op = key[:op] if key[:op] 
                        val_func = key[:func] if key[:func] 
                        attrib_name = key[:name] if key[:name] 
                    else
                        attrib_name = key
                    end
                    
                    sql << "attrib=" + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(attrib_name)
                    sql << " AND source_id=" + srcid_value 
                    sql << " AND " + (val_func.length > 0 ? val_func + "(value)" : "value") + ' ' + val_op + ' ' + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(value) 
                    
                    sql
                end
                
                def find_bycondhash(args, &block)                
                    puts 'find_bycondhash start'
                    
                    condition_hash = {}
                    select_arr = nil
                    limit = nil
                    offset = 0
                    order_dir=""
                    ret_list = []
                    op = 'AND'
                    nSrcID = get_source_id.to_i
                    
                    if args[1]
                        condition_hash = args[1][:conditions] if args[1][:conditions]
                        
                        if args[1][:per_page] #and args[1][:offset]
                          limit = args[1][:per_page].to_i
                          offset = args[1][:offset] ? args[1][:offset].to_i : 0
                        end
                        select_arr = args[1][:select] if args[1][:select]
                        order_dir = args[1][:orderdir].upcase() if args[1][:orderdir]
                        order_attr = args[1][:order]
                        
                        op = args[1][:op].upcase if args[1][:op]
                    end
                    
                    if args.first == :first
                        limit = 1                    
                        offset = 0 unless offset
                    end
                    
                    if select_arr
                      attribs = select_arr
                      if order_attr 
                        order_attr_arr = []
                        order_attr_arr.push(order_attr)
                        attribs = attribs | order_attr_arr
                      end  
                    else
                      attribs = SyncEngine.get_src_attrs(nSrcID)
                    end
                    
                    nulls_cond = {}
                    if op == 'AND'
                        condition_hash.each do |key,value|
                            if !value
                                nulls_cond[key] = value
                                condition_hash.delete(key)
                            end
                        end
                    end
                    
                    srcid_value = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(get_source_id)
                    strLimit = nil
                    if !order_attr 
                        strLimit = " LIMIT " + limit.to_s + " OFFSET " + offset.to_s if limit && offset && condition_hash.length <= 1 && nulls_cond.length == 0
                    end
                    
                    ::Rhom::RhomDbAdapter.start_transaction
                    begin
                        puts "non-null select start"
                        listObjs = []
                        if op == 'OR' && condition_hash.length > 1
                            mapObjs = {}
                            bStop = false 
                            condition_hash.each do |key,value|
                                sql = ""
                                sql << "SELECT object,attrib,value FROM object_values WHERE \n"
                                sql << makeCondWhere(key,value,srcid_value)
                                
                                resObjs = ::Rhom::RhomDbAdapter.execute_sql(sql)
                                resObjs.each do |rec|
                                    next if mapObjs[ rec['object'] ] 
                                    
                                    mapObjs[ rec['object'] ] = 1
                                    listObjs << rec
                                    
                                    bStop = !order_attr && limit && offset && nulls_cond.length == 0 && listObjs.length >= offset + limit
                                    break if bStop
                                end
                                
                                break if bStop
                            end
                            
                        else
                            sql = ""
                            if condition_hash.length > 0
                                condition_hash.each do |key,value|
                                    sql << "\nINTERSECT\n" if sql.length > 0 
                                    sql << "SELECT object FROM object_values WHERE \n"
                                    sql << makeCondWhere(key,value,srcid_value)
                                    sql << strLimit if strLimit
                                end
                            else
                                sql << "SELECT distinct object FROM object_values WHERE \n"
                                sql << "source_id=" + srcid_value 
                                sql << strLimit if strLimit
                            end
                                                
                            listObjs = ::Rhom::RhomDbAdapter.execute_sql(sql)
                        end
                        puts "non-null select end : #{listObjs.length}"
                        
                        nIndex = -1
                        nCount = 0;
                        listObjs.each do |obj|
                            nIndex += 1
                            next if !order_attr && offset && nIndex < offset && !strLimit
                            
                            bSkip = false
                            #obj_value = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(obj['object'])
                            nulls_cond.each do |key,value|
                                sql = ""
                                sql << "SELECT value FROM object_values WHERE \n"
                                sql << "object=?" # + obj_value
                                sql << " AND attrib=?" # + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(key)
                                sql << " AND source_id=?" # + srcid_value

                                attrVal = ::Rhom::RhomDbAdapter.execute_sql(sql, 
                                    obj['object'], key, get_source_id)
                                #puts 'attrVal: ' + attrVal.inspect  if attrVal
                                if attrVal && attrVal.length>0 && attrVal[0]['value']
                                    bSkip = true
                                    break
                                end    
                            end
                            
                            next if bSkip
                            
                            nCount += 1
                            next if args.first == :count
                            
                            sql = ""                        
                            nonExistAttrs = attribs.dup
                            nonExistAttrs.delete(obj['attrib']) if obj['attrib']
                            
                            values = []
                            nonExistAttrs.each do |attr|
                                sql << "\nUNION ALL\n" if sql.length > 0
                                sql << "SELECT attrib,value FROM object_values WHERE \n"
                                sql << "object=?" #+ obj_value
                                sql << " AND attrib=?" #+ ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(attr)
                                sql << " AND source_id=?" #+ srcid_value 
                                
                                values << obj['object']
                                values << attr
                                values << get_source_id
                            end
                            
                            listAttrs = sql.length > 0 ? ::Rhom::RhomDbAdapter.execute_sql(sql,values) : []
                            
                            new_obj = self.new
                            # always return object field with surrounding '{}'
                            new_obj.vars.merge!({:object=>"{#{obj['object']}}"})
                            #new_obj.vars.merge!({:source_id=>nSrcID})
                            
                            if obj['attrib']
                                new_obj.vars.merge!( {obj['attrib'].to_sym()=>obj['value'] })
                            end
                            
                            listAttrs.each do |attrValHash|
                              attrName = attrValHash['attrib']
                              attrVal = attrValHash['value']
                              new_obj.vars.merge!( { attrName.to_sym()=>attrVal } )
                              
                              nonExistAttrs.delete(attrName)
                            end
                            
                            nonExistAttrs.each do |attrName|
                              new_obj.vars.merge!( { attrName.to_sym()=>nil } )
                            end
                            
                            ret_list << new_obj
                            break if !order_attr && limit && ret_list.length >= limit
                      end
                  ensure
                    ::Rhom::RhomDbAdapter.commit
                  end
                  
                  if order_attr
                    ret_list.sort! { |x,y| 
                       vx = x.vars[order_attr.to_sym()]
                       vy = y.vars[order_attr.to_sym()]
                       res = vx && vy ? (block_given? ? yield(vx,vy): vx <=> vy) : 0
                       res *= -1 if order_dir && order_dir == 'DESC'
                       res
                    }
                  end
                  
                  if order_attr && limit
                    ret_list = ret_list.slice(offset,limit)
                  end

                  puts "find_bycondhash end: #{ret_list.length} objects"
                  
                  return nCount if args.first == :count
                  
                  if args.first == :first
                    return ret_list.length > 0 ? ret_list[0] : nil
                  end 

                  ret_list
                end
                
                # retrieve a single record if object id provided, otherwise return
                # full list corresponding to factory's source id
                def find(*args, &block)
                  raise ::Rhom::RecordNotFound if args[0].nil? or args.length == 0
                  puts "Inside find: args - #{args.inspect}"
                  ret_list = []
                  conditions = {}
                  where_cond = nil
                  limit = nil
                  offset = nil
                  
                  # first find all query objects
                  if args.first == :all || args.first == :first || args.first == :count
                  
                    #!args[1] || !args[1][:conditions] || 
                    if args[1] && args[1][:conditions] && args[1][:conditions].is_a?(Hash)
                        return find_bycondhash(args,&block)
                    end
                  
                    where_cond = {"source_id"=>get_source_id}
                  elsif args.first.is_a?(String)
                    where_cond = {"object"=>strip_braces(args.first.to_s),"source_id"=>get_source_id}
                    limit = 1                    
                    offset = 0
                  end

                  if args.first == :count && !args[1]
                      return count()
                  end
                  
                  # do we have conditions?
                  # if so, add them to the query
                  condition_hash = nil
                  select_arr = nil
                  condition_str = nil
                  order_dir=""
                  nSrcID = get_source_id.to_i
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
                    if args[1][:per_page] #and args[1][:offset]
                      limit = args[1][:per_page].to_i
                      offset = args[1][:offset] ? args[1][:offset].to_i : 0
                    end
                    
                    select_arr = args[1][:select] if args[1][:select]
                    order_dir = args[1][:orderdir].upcase() if args[1][:orderdir]
                    order_attr = args[1][:order]
                  end
                  
                  if args.first == :first
                    limit = 1                    
                    offset = 0 unless offset
                  end
                  
                  strLimit = nil
                  if !(block_given? && order_attr)
                      strLimit = " LIMIT " + limit.to_s + " OFFSET " + offset.to_s if limit && offset
                  end
                  
                  # return horizontal resultset from database
                  # for example, an object that has attributes name,industry:
                  # |               object                 |       name         |  industry   |
                  # ---------------------------------------------------------------------------
                  # | 3560c0a0-ef58-2f40-68a5-48f39f63741b |A.G. Parr PLC 37862 |Entertainment|
                  if select_arr
                    attribs = select_arr
                    attribs = attribs | condition_hash.keys if condition_hash
                  else
                    attribs = SyncEngine.get_src_attrs(nSrcID)
                  end
                    
                  if attribs and attribs.length > 0
                    sql = ""
                    sql << "SELECT * FROM (\n" if condition_hash or condition_str
                    sql << "SELECT object, \n"
                    #attribs.reject! {|attrib| select_arr.index(attrib).nil?} if select_arr
                    attribs.each do |attrib|
                      unless attrib.nil? or attrib.length == 0 or ::Rhom::RhomObject.method_name_reserved?(attrib)
                        sql << "MAX(CASE WHEN attrib = '#{attrib}' THEN value ELSE NULL END) AS \"#{attrib}\",\n"
                      end
                    end 
                    sql.chomp!
                    sql.chop!
                    sql << " FROM object_values ov \n"
                    sql << "where " + ::Rhom::RhomDbAdapter.where_str(where_cond) + "\n" if where_cond and where_cond.length > 0
                    sql << "group by object\n"
                    sql << "order by \"#{order_attr}\" " + order_dir if !block_given? && order_attr
                    sql << ") WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                    sql << ") WHERE " + condition_str if condition_str
                    sql << strLimit if strLimit

                    puts "Database query start"
                    list = ::Rhom::RhomDbAdapter.execute_sql(sql)
                    puts "Database query end"
                    
                    if args.first != :count
                        list.each do |rowhash|
                          # always return object field with surrounding '{}'
                          rowhash[:object] = "{#{rowhash['object']}}"
                          #rowhash[:source_id] = nSrcID
                          new_obj = self.new
                          #new_obj.vars.merge!(rowhash)
                          
                          rowhash.each do |attrName, attrVal|
                            new_obj.vars.merge!( { attrName.to_sym()=>attrVal } )
                          end
                            
                          ret_list << new_obj
                        end
                        
                        puts "Processing rhom objects end, #{ret_list.length} objects"
                    end
                                        
                  end
                 
                  if block_given? && order_attr
                    ret_list.sort! { |x,y| 
                       vx = x.vars[order_attr.to_sym()]
                       vy = y.vars[order_attr.to_sym()]
                       res = vx && vy ? (block_given? ? yield(vx,vy): vx <=> vy) : 0
                       res *= -1 if order_dir && order_dir == 'DESC'
                       res
                    }
                  end
                  
                  if order_attr && limit
                    ret_list = ret_list.slice(offset,limit)
                  end
                  
                  return list.length if args.first == :count
                  if args.first == :first || args.first.is_a?(String) 
                    return ret_list.length > 0 ? ret_list[0] : nil
                  end 
                  
                  ret_list
                end
              
                def search(args)
                  searchParams = ""
                  if args[:search_params]
                    args[:search_params].each do |key,value|
                      searchParams += '&' + "conditions[#{Rho::RhoSupport.url_encode(key)}]" + '=' + Rho::RhoSupport.url_encode(value)
                    end  
                  end
                    
                  searchParams += '&offset=' + Rho::RhoSupport.url_encode(args[:offset]) if args[:offset]
                  searchParams += '&max_results=' + Rho::RhoSupport.url_encode(args[:max_results]) if args[:max_results]
                    
                  set_notification(args[:callback], args[:callback_param]) if args[:callback]
                  SyncEngine.dosearch_source(get_source_id.to_i(), args[:from] ? args[:from] : 'search',
                    searchParams, args[:sync_changes] ? args[:sync_changes] : false, args[:progress_step] ? args[:progress_step] : -1 )
                end
                
                # Alias for find
                def find_all(args=nil)
                  find(:all, args)
                end
                
                # Returns a set of rhom objects, limiting the set to length :per_page
                # If no :per_page is specified, the default size is 10
                def paginate(args=nil)
                  # Default to 10 items per page
                  args[:page] ||= 0
                  args[:per_page] ||= 10
                  args[:offset] = args[:page] * args[:per_page]
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
                      begin
                          ::Rhom::RhomDbAdapter.start_transaction
                          
                          # We only support one ask at a time!
                          result = ::Rhom::RhomDbAdapter.select_from_table('changed_values', 'object', {"source_id"=>get_source_id, "update_type"=>'ask'}) 
                          if result && result.length > 0 
                            ::Rhom::RhomDbAdapter.delete_from_table('changed_values', {"source_id"=>get_source_id, "update_type"=>'ask', "sent"=>0 })
                            ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"object"=>result[0]['object'], "attrib"=>'question', "source_id"=>get_source_id} )
                          end
                          
                          ::Rhom::RhomDbAdapter.insert_into_table('changed_values', {"source_id"=>get_source_id, "object"=>tmp_obj.object, "attrib"=>'question', "value"=>Rho::RhoSupport.url_encode(question), "update_type"=>'ask'} )
                          ::Rhom::RhomDbAdapter.insert_into_table('object_values', {"source_id"=>get_source_id, "object"=>tmp_obj.object, "attrib"=>'question', "value"=>Rho::RhoSupport.url_encode(question)} )
                          
                          ::Rhom::RhomDbAdapter.commit
                          
                      rescue Exception => e
                          puts 'Ask Exception: ' + e.inspect
                          ::Rhom::RhomDbAdapter.rollback    
                      end    
                          
                    SyncEngine.dosync
                  end
                end
                
                # deletes all records matching conditions (optionally nil)
                def delete_all(conditions=nil)
                
                  begin
                      ::Rhom::RhomDbAdapter.start_transaction
                      if conditions
                        del_conditions = get_conditions_hash(conditions[:conditions])
                      
                        # find all relevant objects, then delete them
                        del_objects = ::Rhom::RhomDbAdapter.select_from_table('object_values', 'object', del_conditions.merge!({"source_id"=>get_source_id}), {"distinct"=>true})
                        del_objects.each do |obj|                                                       
                          ::Rhom::RhomDbAdapter.delete_from_table('object_values', {'object'=>obj['object']})
                          ::Rhom::RhomDbAdapter.delete_from_table('changed_values', {'object'=>obj['object']})
                        end
                      else
                        ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"source_id"=>get_source_id})
                        ::Rhom::RhomDbAdapter.delete_from_table('changed_values', {"source_id"=>get_source_id})
                        #TODO: add delete all update_type
                      end
                      ::Rhom::RhomDbAdapter.commit
 
                  rescue Exception => e
                      puts 'delete_all Exception: ' + e.inspect
                      ::Rhom::RhomDbAdapter.rollback    
                  end    
                      
                end
                    
                private
                
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
	          
	          # if app server does not support oo in inserts. 
	          # app client should check this method before update or delete
	          # overwise all modifications of unconfirmed created item will be lost
	          def can_modify
                result = ::Rhom::RhomDbAdapter.execute_sql("SELECT object FROM changed_values WHERE sent>1 LIMIT 1 OFFSET 0")
                return !(result && result.length > 0) 
	          end
	            
              # deletes the record from the viewable list as well as
              # adding a delete record to the list of sync operations
              def destroy
                obj = self.inst_strip_braces(self.object)
                update_type=self.get_update_type_by_source('delete')
                
                if obj
                    begin
                      ::Rhom::RhomDbAdapter.start_transaction
                
                      # first delete the record from viewable list
                      result = ::Rhom::RhomDbAdapter.delete_from_table('object_values', {"object"=>obj})
                      
                      resUpdateType = ::Rhom::RhomDbAdapter.select_from_table('changed_values', 'update_type', {"object"=>obj, "update_type"=>'create', "sent"=>0}) 
                      if resUpdateType && resUpdateType.length > 0 
                        update_type = nil                              
                      end
                      
                      ::Rhom::RhomDbAdapter.delete_from_table('changed_values', {"object"=>obj, "sent"=>0})
                      
                      if update_type
                        # now add delete operation
                        result = ::Rhom::RhomDbAdapter.insert_into_table('changed_values', {"source_id"=>self.get_inst_source_id, "object"=>obj, "update_type"=>update_type})
                      end
                      
                      ::Rhom::RhomDbAdapter.commit
 
                    rescue Exception => e
                      puts 'destroy Exception: ' + e.inspect
                      ::Rhom::RhomDbAdapter.rollback    
                    end    
                      
                end
                true
              end
		
              # saves the current object to the database as a create type
              def save
                # iterate over each instance variable and insert create row to table
				obj = self.inst_strip_braces(self.object)
				nSrcID = self.get_inst_source_id
                begin
                
                    ::Rhom::RhomDbAdapter.start_transaction
                    
                    result = ::Rhom::RhomDbAdapter.execute_sql("SELECT object FROM object_values WHERE object=? AND source_id=? LIMIT 1 OFFSET 0",obj,nSrcID)
                    bUpdate = result && result.length > 0 
    				update_type = bUpdate ? self.get_update_type_by_source('update') : self.get_update_type_by_source('create')
                    self.vars.each do |key_a,value|
                        key = key_a.to_s
                        next if ::Rhom::RhomObject.method_name_reserved?(key)

                        val = self.inst_strip_braces(value.to_s)
                        
                        # add rows excluding object, source_id and update_type
                        fields = {"source_id"=>nSrcID,
                                  "object"=>obj,
                                  "attrib"=>key,
                                  "value"=>val,
                                  "update_type"=>update_type}
                        fields = key == "image_uri" ? fields.merge!({"attrib_type" => "blob.file"}) : fields

                        resValue = ::Rhom::RhomDbAdapter.select_from_table('object_values', 'value,id', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID}) 
                        if resValue && resValue.length > 0 
                            oldValue = resValue[0]['value']
                            
                            isModified = oldValue != val
                            if isModified && val && oldValue.nil? && val.to_s().length == 0
                              isModified = false
                            end  
                            if isModified && oldValue && val.nil? && oldValue.to_s().length == 0
                              isModified = false
                            end
                            
                            if isModified
                            
                                resUpdateType = ::Rhom::RhomDbAdapter.select_from_table('changed_values', 'update_type', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID, 'sent'=>0}) 
                                if resUpdateType && resUpdateType.length > 0 
                                    fields['update_type'] = resUpdateType[0]['update_type'] 
                                    ::Rhom::RhomDbAdapter.delete_from_table('changed_values', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID, "sent"=>0})
                                end

                                fields['main_id'] = resValue[0]['id'] 
                                ::Rhom::RhomDbAdapter.insert_into_table('changed_values', fields)
                                result = ::Rhom::RhomDbAdapter.update_into_table('object_values', {"value"=>val}, {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID})
                            end    
                        else
                            tmp_id = generate_id()
                            fields['main_id'] = tmp_id
                            ::Rhom::RhomDbAdapter.insert_into_table('changed_values', fields)
                            fields.delete("update_type")
                            fields.delete("main_id")
                            fields['id'] = tmp_id
                            result = ::Rhom::RhomDbAdapter.insert_into_table('object_values', fields)                                     
                        end
                        
                    end
                    ::Rhom::RhomDbAdapter.commit

                rescue Exception => e
                    puts 'save Exception: ' + e.inspect
                    ::Rhom::RhomDbAdapter.rollback    
                end    
                
                true
              end
          
              # updates the current record in the viewable list and adds
              # a sync operation to update
              def update_attributes(attrs)
                obj = self.inst_strip_braces(self.object)
                update_type=self.get_update_type_by_source('update')
                nSrcID = self.get_inst_source_id
                begin
                    ::Rhom::RhomDbAdapter.start_transaction
                    attrs.each do |attrib,val|
                      attrib = attrib.to_s.gsub(/@/,"")
                      next if ::Rhom::RhomObject.method_name_reserved?(attrib)
                      
                      old_val = self.send attrib.to_sym unless ::Rhom::RhomObject.method_name_reserved?(attrib)
                      
                      # Don't save objects with braces to database
                      new_val = self.inst_strip_braces(val.to_s)
                      
                      isModified = old_val != new_val
                      if isModified && new_val && old_val.nil? && new_val.to_s().length == 0
                        isModified = false
                      end  
                      if isModified && old_val && new_val.nil? && old_val.to_s().length == 0
                        isModified = false
                      end
                      
                      # if the object's value doesn't match the database record
                      # then we procede with update
                      if isModified
                          # only one update at a time
                          resUpdateType = ::Rhom::RhomDbAdapter.select_from_table('changed_values', 'update_type', {"object"=>obj, "source_id"=>nSrcID, 'sent'=>0}) 
                          if resUpdateType && resUpdateType.length > 0 
                              update_type = resUpdateType[0]['update_type'] 
                              ::Rhom::RhomDbAdapter.delete_from_table('changed_values', {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID, "sent"=>0})
                          end
                          
                          # add to syncengine queue
                          
                          result = ::Rhom::RhomDbAdapter.select_from_table('object_values', 'id', {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID}) 
                          if result && result.length > 0 
                            ::Rhom::RhomDbAdapter.update_into_table('object_values', {"value"=>new_val}, {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID})
                            ::Rhom::RhomDbAdapter.insert_into_table('changed_values', {"main_id"=>result[0]['id'], "source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, "value"=>new_val, "update_type"=>update_type})
                          else
                            tmp_id = generate_id()
                            result = ::Rhom::RhomDbAdapter.insert_into_table('object_values', {"id"=>tmp_id,"source_id"=>self.get_inst_source_id, "object"=>obj, "attrib"=>attrib, "value"=>new_val})                          
                            ::Rhom::RhomDbAdapter.insert_into_table('changed_values', {"main_id"=>tmp_id, "source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, "value"=>new_val, "update_type"=>update_type})
                          end    
                          
                          # update in-memory object
                          self.vars[attrib.to_sym()] = new_val
                      end
                    end
                    
                    ::Rhom::RhomDbAdapter.commit

                rescue Exception => e
                    puts 'update_attributes Exception: ' + e.inspect
                    ::Rhom::RhomDbAdapter.rollback    
                end    
                    
                true
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