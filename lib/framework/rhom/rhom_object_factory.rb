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
#require 'rhom'
#require 'rho'
#require 'rho/rhosupport'

module Rhom
  
  class RhomObjectFactory
    
    def initialize
	  #unless not defined? Rho::RhoConfig.sources
  	  #  init_objects
  	  #end
    end
  
    # Initialize new object with dynamic attributes
    def self.init_object(classname)
      #Rho::RhoConfig.sources.each do |classname,source|
        unless Object.const_defined?(classname.intern)
          Object.const_set(classname.intern, 
            Class.new do
              include ::Rhom::RhomObject
              extend ::Rhom::RhomObject
              
              # This holds the attributes for an instance of
              # the rhom object
              attr_accessor :vars
          
              def generate_id
                Rho::RhoConfig.generate_id()
              end
              
              def initialize(obj=nil)
                @vars = {}
                self.rhom_init(@vars)
                self.vars[:object] = "#{generate_id()}" unless obj && obj[:object]
                self.vars[:source_id] = self.get_inst_source_id.to_i
                
                if obj
                  obj.each do |key,value|
                    self.vars[key.to_sym()] = value if key && key.length > 0
                  end
                end
              end

              def object
                @vars[:object]                
              end
              
              def source_id
                @vars[:source_id]                
              end

              def to_s
                  @vars.to_s if @vars
              end
              
              def to_json(*args) 
                @vars.to_json(*args) if @vars
              end
              
              def method_missing(name, *args)
                unless name == Fixnum
                  if name[name.length()-1] == '='
                    @vars[name.to_s.chop.to_sym()] = args[0]  
                  else
                    @vars[name]
                  end
                end
              end

              @@metadata = {}
              class << self

                def clean_cached_metadata
                    src_name = get_source_name()
                    @@metadata[src_name] = nil                    
                end
                
                def metadata
                  src_name = get_source_name()
                  return @@metadata[src_name] if @@metadata[src_name]
                  db = ::Rho::RHO.get_src_db(src_name)
                  result = db.select_from_table('sources', 'metadata', {"name"=>src_name} )
                  if result && result.length > 0 && result[0]['metadata']
                    @@metadata[src_name] = Rho::JSON.parse(result[0]['metadata'])
                  end
                  
                  @@metadata[src_name]
                end

                def count
                  db = ::Rho::RHO.get_src_db(get_source_name)
                  
                  if is_schema_source()
                    res = db.select_from_table(get_schema_table_name(),'object').length
                  else
                    res = db.select_from_table('object_values','object', {"source_id"=>get_source_id}, {"distinct"=>true}).length
                  end
                    
                  res
                end

                def backend_refresh_time
                  db = ::Rho::RHO.get_src_db(get_source_name)
                  result = db.select_from_table('sources', 'backend_refresh_time', {"source_id"=>get_source_id} )
                  nTime = 0
                  if result && result.length > 0 && result[0]['backend_refresh_time']
                    nTime = result[0]['backend_refresh_time'].to_i
                  end
                    
                  Time.at(nTime)
                end

                def get_source_name
                  self.name.to_s
                end

                def is_sync_source
                    Rho::RhoConfig.sources[get_source_name]['sync_type'] != 'none'
                end
              
                def is_schema_source
                    !Rho::RhoConfig.sources[get_source_name]['schema'].nil?
                end
                def get_schema_table_name
                    get_source_name()
                end
                def get_values_table_name
                    is_schema_source() ? get_schema_table_name() : 'object_values'
                end
                
                def get_source_id
                  Rho::RhoConfig.sources[get_source_name]['source_id'].to_s
                end

                def convertConditionToStr(cond, op, condition_hash)
                  if cond.is_a?(String)
                    return cond
                  end

                  if cond.is_a?(Array)
                  
                    if ( op )
                        condition_str = ""
                    
                        cond.each do |item|
                            condition_str += ' ' + op + ' ' if condition_str.length() > 0
                            condition_str += '(' + convertConditionToStr(item[:conditions], item[:op], condition_hash) + ')'
                        end
                    else
                        condition_str = cond[0].split(/\?/).each_with_index { |param,i| 
                          param << cond[i+1].to_s
                        }.join(' ').to_s 
                    end
                    
                    puts "condition_str : #{condition_str}"
                    return condition_str
                  end

                  bSimpleHash = true
                  condition_str = ""          
                  cond.each do |key, value|
                    if key.is_a?(Hash)
                        bSimpleHash = false

						val_op = "="
                        val_op = key[:op] if key[:op] 
                        val_func = key[:func] if key[:func] 
                        attrib_name = key[:name] if key[:name] 
                        op = "AND" unless op
						
                        condition_str += ' ' + op + ' ' if condition_str.length() > 0
                        if ( val_func && val_func.length() > 0 )
                            condition_str += val_func + "(\"" + attrib_name + "\")"
                        else
                            condition_str += "\"" + attrib_name + "\""
                        end
                        condition_str += ' '
                        
                        if val_op == 'IN' or val_op == 'in'
                            svalue = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(value)
                            condition_str += val_op + ' ( ' + svalue[1,svalue.length()-2] + ' )'
                        else
                            condition_str += val_op + ' ' + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(value) 
                        end
                        
                    end
                    
                  end
                  
                  if bSimpleHash
                    condition_hash = cond 
                    condition_str = ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                  end

                  condition_str                      
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
                    sql << " AND " + (val_func.length > 0 ? val_func + "(value)" : "value") + ' '
                    
                    if val_op == 'IN' or val_op == 'in'
                        svalue = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(value)
                        sql << val_op + ' ( ' + svalue[1,svalue.length()-2] + ' )'
                    else
                        sql << val_op + ' ' + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(value) 
                    end
                    
                    sql
                end

                def makeCondWhereEx(key,value,srcid_value)
                    sql = ""

					vals = []

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
                    
                    sql << "attrib=?"
					vals << attrib_name
                    sql << " AND source_id=?"
					vals << srcid_value					
                    sql << " AND " + (val_func.length > 0 ? val_func + "(value)" : "value") + ' '
                    
                    if val_op == 'IN' or val_op == 'in'
                        sql << val_op + " ( ? )"
						vals << value.to_s						
                    else
                        sql << val_op + " ?"
						vals << value.to_s
                    end

                    return sql, vals
                end

                def find_objects(condition_hash, op, limit, offset, order_attr)
                    nulls_cond = {}
                    if op == 'AND'
                        condition_hash.each do |key,value|
                            if value.nil?
                                nulls_cond[key] = value
                                condition_hash.delete(key)
                            end
                        end
                    end
                    
                    strLimit = nil
                    if !order_attr 
                        strLimit = " LIMIT " + limit.to_s + " OFFSET " + offset.to_s if limit && offset && condition_hash.length <= 1 && nulls_cond.length == 0
                    end
                    
                    #puts "non-null select start"
                    db = ::Rho::RHO.get_src_db(get_source_name)
                    listObjs = []
                    if op == 'OR' && condition_hash.length > 1
                        srcid_value = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(get_source_id)
					
                        mapObjs = {}
                        bStop = false 
                        condition_hash.each do |key,value|
                            sql = ""
                            sql << "SELECT object,attrib,value FROM object_values WHERE \n"
                            sql << makeCondWhere(key,value,srcid_value)
                            
                            resObjs = db.execute_sql(sql)
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
                        if condition_hash.length > 0
                            sql = ""
						    vals = []
                        
                            condition_hash.each do |key,value|
                                sql << "\nINTERSECT\n" if sql.length > 0 
                                sql << "SELECT object FROM object_values WHERE \n"
								
                                sqlCond, valCond = makeCondWhereEx(key,value,get_source_id)
								sql << sqlCond
								vals = vals + valCond
								
                                sql << strLimit if strLimit
                            end
                            
                            listObjs = db.execute_sql(sql, vals)

                        else
                            sql = ""
						    vals = []
                        
                            sql << "SELECT distinct( object ) FROM object_values WHERE \n"
                            sql << "source_id=?"
							vals << get_source_id
                            sql << strLimit if strLimit
                            
                            listObjs = db.execute_sql(sql, vals)
                            
                        end
                                            
                    end
                    #puts "non-null select end : #{listObjs.length}"
                    
                    nIndex = -1
                    res = []
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

                            attrVal = db.execute_sql(sql, obj['object'], key, get_source_id)
                            #puts 'attrVal: ' + attrVal.inspect  if attrVal
                            if attrVal && attrVal.length>0 && attrVal[0]['value']
                                bSkip = true
                                break
                            end    
                        end
                        
                        next if bSkip
                        res << obj
                    end
                    
                    res
                end

                def find_objects_ex(condition_ar, op, limit, offset, order_attr)
                    mapObjs = {}
                    listObjs = []
                    condition_ar.each do |cond|
                        res = find_objects(cond[:conditions], cond[:op], limit, offset, order_attr)
                        
                        if listObjs.length() == 0
                            if condition_ar.length() > 1
                                res.each do |hash_attrs|
                                    mapObjs[ hash_attrs['object'] ] = 1
                                end
                            end
                            
                            listObjs = res
                        else
                            if op == 'OR'
                                res.each do |hash_attrs|
                                    obj = hash_attrs['object']
                                    if !mapObjs.has_key?(obj)
                                        listObjs << hash_attrs
                                        mapObjs[ obj ] = 1
                                    end    
                                end
                            else
                                andRes = []
                                res.each do |hash_attrs|
                                    obj = hash_attrs['object']
                                    if mapObjs.has_key?(obj)
                                        andRes << hash_attrs
                                    end    
                                end
                                listObjs = andRes
                            end
                        end    
                    end
                    
                    listObjs
                end
                                
                def find_bycondhash(args, &block)                
                    #puts 'find_bycondhash start'
                    
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

                    attribs = nil
                    if select_arr
                      attribs = select_arr
                      if order_attr 
                        order_attr_arr = []
                        order_attr_arr.push(order_attr)
                        attribs = attribs | order_attr_arr
                      end  
                    else
                      #attribs = SyncEngine.get_src_attrs(nSrcID)
                    end

                    #srcid_value = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(get_source_id)

                    db = ::Rho::RHO.get_src_db(get_source_name)
                    db.lock_db
                    begin
                        listObjs = []
                        if condition_hash.is_a?(Hash)
                            listObjs = find_objects(condition_hash, op, limit, offset, order_attr)
                        else
                            listObjs = find_objects_ex(condition_hash, op, limit, offset, order_attr)
                        end

                        nCount = 0;
                        if args.first == :count
                            nCount = listObjs.length
                        else                                
                            listObjs.each do |obj|
                                sql = ""
                                values = []
                                
                                if attribs
                                    nonExistAttrs = attribs.dup
                                    nonExistAttrs.delete(obj['attrib']) if obj['attrib']

                                    if nonExistAttrs.length > 0
                                        sql << "SELECT attrib,value FROM object_values WHERE object=? AND source_id=?"
                                        values << obj['object']
                                        values << get_source_id
                                        
                                        quests = ""
                                        nonExistAttrs.each do |attr|
                                            quests << ',' if quests.length() > 0
                                            quests << '?'
                                            values << attr
                                        end

                                        sql << " AND attrib IN (#{quests})"
                                    end                                    
                                else
                                    sql << "SELECT attrib,value FROM object_values WHERE \n"
                                    sql << "object=? AND source_id=?"
                                    
                                    values << obj['object']
                                    values << get_source_id
                                end
                                
                                #puts "get attribs: #{sql}"
                                listAttrs = sql.length > 0 ? db.execute_sql(sql,values) : []
								
                                new_obj = self.new({:object=>"#{obj['object']}"})
                                #new_obj.vars.merge!({:object=>"#{obj['object']}"})
                                
                                if attribs && obj['attrib']
                                    #new_obj.vars.merge!( {obj['attrib'].to_sym()=>obj['value'] }) if obj['value']
									new_obj.vars[obj['attrib'].to_sym()] = obj['value'] if obj['value']
                                end
                                
                                listAttrs.each do |attrValHash|
                                  attrName = attrValHash['attrib']
                                  attrVal = attrValHash['value']
                                  #new_obj.vars.merge!( { attrName.to_sym()=>attrVal } ) if attrVal
								  new_obj.vars[attrName.to_sym()] = attrVal if attrVal
                                  
                                  #nonExistAttrs.delete(attrName) if nonExistAttrs
                                end
                                
                                #if nonExistAttrs
                                #   nonExistAttrs.each do |attrName|
                                #       new_obj.vars.merge!( { attrName.to_sym()=>nil } )
                                #   end
                                #end
                                
                                ret_list << new_obj
                                break if !order_attr && limit && ret_list.length >= limit
                            end
                        end    
                    ensure
                      db.unlock_db
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

                    #puts "find_bycondhash end: #{ret_list.length} objects"

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

                  #puts "Inside find: args - #{args.inspect}"
                  
                  ret_list = []
                  conditions = {}
                  where_cond = nil
                  limit = nil
                  offset = nil
                  
                  # first find all query objects
                  if args.first == :all || args.first == :first || args.first == :count
                  
                    if !is_schema_source()
                        #!args[1] || !args[1][:conditions] || 
                        if args[1] && args[1][:conditions] && args[1][:conditions].is_a?(Hash)
                            return find_bycondhash(args,&block)
                        end

                        if args[1] && args[1][:op] && args[1][:conditions] && args[1][:conditions].is_a?(Array)
                            return find_bycondhash(args,&block)
                        end
                      
                        where_cond = {"source_id"=>get_source_id}
                    end    
                  elsif args.first.is_a?(String)
                    if !is_schema_source()
                        where_cond = {"object"=>strip_braces(args.first.to_s),"source_id"=>get_source_id}
                    else
                        where_cond = {"object"=>strip_braces(args.first.to_s)}
                    end
                        
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
                    condition_str = convertConditionToStr(args[1][:conditions], args[1][:op], condition_hash)  if args[1][:conditions]
                    
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
                    attribs = '*'
                  end

                  if attribs and attribs.length > 0
                    sql = ""
                    list = []
                    db = ::Rho::RHO.get_src_db(get_source_name)
                    
                    if !is_schema_source()
                      objects = nil
                      if !condition_hash && !condition_str
                        #srcid_value = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(get_source_id)
                        values = [] 

                        if args.first.is_a?(String)                      
                          objects = [ { 'object' => strip_braces(args.first.to_s) } ]
                          
                        else  
                          if !block_given? && order_attr
                              if  !args[1][:dont_ignore_missed_attribs]
                                  sql << "SELECT object FROM object_values WHERE source_id=? "
                                  sql << " AND attrib=? ORDER BY \"value\" " + order_dir
                                  values << get_source_id
                                  values << order_attr
                              end    
                          else
                              #it is more effective to use old find here
                              if attribs && attribs != '*' && attribs.length() != 0 && !args[1][:dont_ignore_missed_attribs]
                                  #sql << "SELECT object FROM object_values WHERE source_id=? AND attrib=?"
                                  #values << get_source_id
                                  #values << attribs[0]
                              else 
                                  #sql << "SELECT distinct(object) FROM object_values WHERE source_id=" << srcid_value 
                              end    
                          end
                          
                          if sql.length() > 0                          
                            sql << strLimit if strLimit

                            #puts "Database query start" # : #{sql}; #{values}"
                            objects = db.execute_sql(sql, values)

                            #puts "Database query end" #: #{objects}"
                          end
                                                  
                        end
                        
                        if objects
                            objects.each do |object|
                                object_id = object['object']
                                sql2 = "SELECT attrib,value FROM object_values WHERE object=? AND source_id=?"
                                values2 = []
                                values2 << object_id
                                values2 << get_source_id
                                
                                if attribs && attribs != '*'    
                                    quests = ""
                                    attribs.each do |attr|
                                        quests << ',' if quests.length() > 0
                                        quests << '?'
                                        values2 << attr
                                    end

                                    sql2 << " AND attrib IN (#{quests})"
                                end                                    
                               
                                item_attribs = db.execute_sql(sql2, values2)
                                if item_attribs && item_attribs.length() > 0
                                    new_item = {'object'=>object_id }
                                    item_attribs.each do |item|
                                       new_item[item['attrib']] = item['value']
                                    end    

                                    list << new_item
                                end    
                            end    
                         end
						 
                         #puts "Processing rhom objects end1, #{list.length} objects"
                      end
                            
                      if objects.nil?
                      
                        if attribs == "*"
                            attribs = SyncEngine.get_src_attrs(Rho::RhoConfig.sources[get_source_name]['partition'].to_s, nSrcID)                            
                        end    
                      
                        if attribs and attribs.length > 0                      
                            sql << "SELECT * FROM (\n" if condition_hash or condition_str
                            sql << "SELECT object, \n"
                            #attribs.reject! {|attrib| select_arr.index(attrib).nil?} if select_arr
                            attribs.each do |attrib|
                              unless attrib.nil? or attrib.length == 0 or ::Rhom::RhomObject.method_name_reserved?(attrib)
                                sql << "MAX(CASE WHEN attrib = '#{attrib}' THEN value ELSE NULL END) AS \'#{attrib}\',\n"
                              end
                            end 
                            sql.chomp!
                            sql.chop!
                            sql << " FROM object_values ov \n"
                            sql << "where " + ::Rhom::RhomDbAdapter.where_str(where_cond) + "\n" if where_cond and where_cond.length > 0
                            sql << "group by object\n"
                            sql << "order by \"#{order_attr}\" " + order_dir if !block_given? && order_attr
                            #sql << ") WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                            sql << ") WHERE " + condition_str if condition_str
                            sql << strLimit if strLimit
                            
                            #puts "Database query start"
                            list = db.execute_sql(sql)
                            #puts "Database query end"
                        end   
                      end  

                    else #schema source
                       attribs = attribs.join(',') if attribs.is_a?(Array)
                       
                       sql << "SELECT #{attribs} FROM #{get_schema_table_name}"
                       if where_cond and where_cond.length > 0
                           sql << " WHERE " + ::Rhom::RhomDbAdapter.where_str(where_cond)
                       else 
                           sql << " WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                           sql << " WHERE " + condition_str if condition_str
                       end
                           
                       sql << " order by \"#{order_attr}\" " + order_dir if !block_given? && order_attr
                       sql << strLimit if strLimit
                       
                       #puts "Database query start" #: #{sql}"
                       list = db.execute_sql(sql)
                       #puts "Database query end"
                       
                    end
                    
                    if args.first != :count
                        list.each do |rowhash|
                          # always return object field with surrounding '{}'
                          #rowhash[:object] = "#{rowhash['object']}"
                          #rowhash[:source_id] = nSrcID
                          new_obj = self.new({:object=>"#{rowhash['object']}"})
                          #new_obj.vars.merge!(rowhash)
                          
                          rowhash.each do |attrName, attrVal|
                            new_obj.vars.merge!( { attrName.to_sym()=>attrVal } ) if attrVal
                          end
                            
                          ret_list << new_obj
                        end
                        
                        #puts "Processing rhom objects end, #{ret_list.length} objects"
                    end
                  else
                      puts "Processing rhom objects end, no attributes found."
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
                  
                  return list.length if args.first == :count
                  if args.first == :first || args.first.is_a?(String) 
                    return ret_list.length > 0 ? ret_list[0] : nil
                  end 
                  
                  ret_list
                end
              
                def search(args)
                    args[:source_names] = [self.name.to_s]

                    SyncEngine.search(args)
                end

                def sync(callback=nil, callback_data="", show_status_popup=nil)
                  src_id = get_source_id.to_i()
                  SyncEngine.set_notification(src_id, callback, callback_data) if callback
                  if !show_status_popup.nil?
                    SyncEngine.dosync_source(src_id, show_status_popup)
                  else
                    SyncEngine.dosync_source(src_id)
                  end
                    
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
                
                # deletes all records matching conditions (optionally nil)
                def delete_all(conditions=nil)
                
                  db = ::Rho::RHO.get_src_db(get_source_name)
                  tableName = is_schema_source() ? get_schema_table_name : 'object_values'
                  begin
                      db.start_transaction
                      if conditions
                        # find all relevant objects, then delete them
                        if is_schema_source()
                            del_conditions = conditions[:conditions]
                            del_objects = db.select_from_table(tableName, 'object', del_conditions)
                        else    
                            del_conditions = get_conditions_hash(conditions[:conditions])
                            del_objects = db.select_from_table(tableName, 'object', del_conditions.merge!({"source_id"=>get_source_id}), {"distinct"=>true})
                        end

                        puts "del_objects : #{del_objects}"
                        del_objects.each do |obj|
                          db.delete_from_table(tableName, {'object'=>obj['object']})
                          db.delete_from_table('changed_values', {'object'=>obj['object']}) if is_sync_source()
                        end
                      else
                        if is_schema_source()
                            db.delete_all_from_table(tableName)
                        else
                            db.delete_from_table(tableName, {"source_id"=>get_source_id})
                        end
                            
                        db.delete_from_table('changed_values', {"source_id"=>get_source_id})  if is_sync_source()
                        #TODO: add delete all update_type
                      end
                      db.commit
 
                  rescue Exception => e
                      puts 'delete_all Exception: ' + e.inspect
                      db.rollback
                      
                      raise    
                  end    
                      
                end

                # saves the current object to the database as a create type
                def create(obj)  		      
                    new_obj = self.new(obj)
                    
                    update_type = 'create'
                    nSrcID = get_source_id
                    obj = new_obj.object
                    src_name = get_source_name
                    db_partition = Rho::RhoConfig.sources[src_name]['partition'].to_s
                    isSchemaSrc = is_schema_source()
                    tableName = isSchemaSrc ? get_schema_table_name() : 'object_values'
                    db = ::Rho::RHO.get_src_db(src_name)
                    begin
                        db.start_transaction

                        if isSchemaSrc
                            db.insert_into_table(tableName, new_obj.vars, {:source_id=>true})
                        end
                        
                        if is_sync_source() || !isSchemaSrc
                            new_obj.vars.each do |key_a,value|
                                key = key_a.to_s
                                next if ::Rhom::RhomObject.method_name_reserved?(key)

                                val = value.to_s #self.inst_strip_braces(value.to_s)
                                
                                # add rows excluding object, source_id and update_type
                                fields = {"source_id"=>nSrcID,
                                          "object"=>obj,
                                          "attrib"=>key,
                                          "value"=>val,
                                          "update_type"=>update_type}
                                fields = new_obj.is_blob_attrib(db_partition, nSrcID, key) ? fields.merge!({"attrib_type" => "blob.file"}) : fields
                                
                                db.insert_into_table('changed_values', fields) if is_sync_source()
                                fields.delete("update_type")
                                fields.delete("attrib_type")
                                
                                db.insert_into_table(tableName, fields) if !isSchemaSrc
                            end
                        end
                        
                        db.commit
                    rescue Exception => e
                        puts 'create Exception: ' + e.inspect
                        db.rollback
                        
                        raise    
                    end
                    
                    new_obj    
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
		        db = ::Rho::RHO.get_src_db(get_inst_source_name)
   				obj = self.object #self.inst_strip_braces(self.object)
                result = db.execute_sql("SELECT object FROM changed_values WHERE source_id=? and object=? and sent>1 LIMIT 1 OFFSET 0", get_inst_source_id().to_i(), obj )
                return !(result && result.length > 0) 
	          end

	          def self.changed?
	            db = ::Rho::RHO.get_src_db(get_source_name)
                result = db.execute_sql("SELECT object FROM changed_values WHERE source_id=? LIMIT 1 OFFSET 0", get_source_id().to_i )
                return result && result.length > 0
	          end

	          def changed?
	            db = ::Rho::RHO.get_src_db(get_inst_source_name)
	            obj = self.object #self.inst_strip_braces(self.object)
                result = db.execute_sql("SELECT object FROM changed_values WHERE source_id=?  and object=? LIMIT 1 OFFSET 0", get_inst_source_id().to_i(), obj )
                return result && result.length > 0
	          end
	            
              # deletes the record from the viewable list as well as
              # adding a delete record to the list of sync operations
              def destroy
                obj = self.object #self.inst_strip_braces(self.object)
                update_type='delete'
                
                if obj
                    db = ::Rho::RHO.get_src_db(get_inst_source_name)
                    begin
                      tableName = is_inst_schema_source() ? get_inst_schema_table_name() : 'object_values'
                    
                      db.start_transaction
                
                      #save list of attrs
                      attrsList = nil
                      if is_inst_schema_source()
                        attrsList = db.select_from_table(tableName, '*', {"object"=>obj}) 
                      else
                        attrsList = db.select_from_table(tableName, '*', {"object"=>obj, "source_id"=>self.get_inst_source_id()}) 
                      end  
                      
                      # first delete the record from viewable list
                      db.delete_from_table(tableName, {"object"=>obj})
                      
                      resUpdateType =  is_inst_sync_source() ? db.select_from_table('changed_values', 'update_type', {"object"=>obj, "update_type"=>'create', "sent"=>0}) : nil 
                      if resUpdateType && resUpdateType.length > 0 
                        update_type = nil                              
                      end
                      
                      db.delete_from_table('changed_values', {"object"=>obj, "sent"=>0}) if is_inst_sync_source()
                      
                      if is_inst_sync_source() && update_type and attrsList and attrsList.length() > 0
                        # now add delete operation
                        if is_inst_schema_source()
                            attrsList[0].each do |attrName, attrValue|
                                next if attrName == 'object'
                                
                                db.insert_into_table('changed_values', 
                                  {"source_id"=>self.get_inst_source_id, "object"=>obj, 
                                    "attrib"=> attrName, "value"=>attrValue, 
                                    "update_type"=>update_type})
                            end        
                        else
                            attrsList.each do |attrName|
                                db.insert_into_table('changed_values', 
                                  {"source_id"=>self.get_inst_source_id, "object"=>obj, 
                                    "attrib"=> attrName['attrib'], "value"=>attrName['value'], 
                                    "update_type"=>update_type})
                            end        
                        end    
                      end
                      
                      db.commit
 
                    rescue Exception => e
                      puts 'destroy Exception: ' + e.inspect
                      db.rollback
                      
                      raise    
                    end    
                      
                end
                true
              end
		
		      def is_blob_attrib(db_partition, nSrcID,attrib_name)  
                SyncEngine.is_blob_attr(db_partition, nSrcID.to_i,attrib_name)
		        #return attrib_name == "image_uri"
		      end

              # saves the current object to the database as a create type
              def save
                # iterate over each instance variable and insert create row to table
				obj = self.object #self.inst_strip_braces(self.object)
				nSrcID = self.get_inst_source_id
                db = ::Rho::RHO.get_src_db(get_inst_source_name)
                db_partition = Rho::RhoConfig.sources[get_inst_source_name]['partition'].to_s
				tableName = is_inst_schema_source() ? get_inst_schema_table_name() : 'object_values'
				isSchemaSrc = is_inst_schema_source()
                begin
                    db.start_transaction

			        update_type = ''
                    if is_inst_sync_source()
                        if isSchemaSrc
                            result = db.execute_sql("SELECT object FROM #{tableName} WHERE object=? LIMIT 1 OFFSET 0",obj)
                        else
                            result = db.execute_sql("SELECT object FROM #{tableName} WHERE object=? AND source_id=? LIMIT 1 OFFSET 0",obj,nSrcID)
                        end

                        if result && result.length > 0                     
                            resUpdateType = is_inst_sync_source() ? db.select_from_table('changed_values', 'update_type', {"object"=>obj, "source_id"=>nSrcID, 'sent'=>0}) : nil
                            if resUpdateType && resUpdateType.length > 0 
                                update_type = resUpdateType[0]['update_type'] 
                            else
        				        update_type = 'update'
        				    end    
    				    else
    				        update_type = 'create'
    				    end
    				end
    				
                    self.vars.each do |key_a,value|
                        key = key_a.to_s
                        next if ::Rhom::RhomObject.method_name_reserved?(key)

                        val = value.to_s #self.inst_strip_braces(value.to_s)
                        
                        # add rows excluding object, source_id and update_type
                        fields = {"source_id"=>nSrcID,
                                  "object"=>obj,
                                  "attrib"=>key,
                                  "value"=>val,
                                  "update_type"=>update_type}
                        fields = is_blob_attrib(db_partition, nSrcID, key) ? fields.merge!({"attrib_type" => "blob.file"}) : fields
                        resValue = nil
                        if isSchemaSrc
                            resValue = db.select_from_table(tableName, key, {"object"=>obj}) 
                        else
                            resValue = db.select_from_table(tableName, 'value', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID}) 
                        end
                            
                        if resValue && resValue.length > 0 
                        
                            isModified = false

                            if is_inst_full_update
                              isModified = true
                            else
                                oldValue = isSchemaSrc ? resValue[0][key] : resValue[0]['value']
                                
                                isModified = oldValue != val
                                if isModified && val && oldValue.nil? && val.to_s().length == 0
                                  isModified = false
                                end  
                                if isModified && oldValue && val.nil? && oldValue.to_s().length == 0
                                  isModified = false
                                end
                            end
                            
                            if isModified
                            
                                if is_inst_sync_source()
                                    resUpdateType = db.select_from_table('changed_values', 'update_type', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID, 'sent'=>0}) 
                                    if resUpdateType && resUpdateType.length > 0 
                                        fields['update_type'] = resUpdateType[0]['update_type'] 
                                        db.delete_from_table('changed_values', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID, "sent"=>0})
                                    end

                                    db.insert_into_table('changed_values', fields)
                                end
                                    
                                if isSchemaSrc
                                    db.update_into_table(tableName, {key=>val}, {"object"=>obj})
                                else
                                    db.update_into_table(tableName, {"value"=>val}, {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID})                                    
                                end    
                            end    
                        else
                            db.insert_into_table('changed_values', fields) if is_inst_sync_source()
                            fields.delete("update_type")
                            fields.delete("attrib_type")
                            
                            if isSchemaSrc
                                db.insert_into_table(tableName, {key=>val, "object"=>obj})
                            else
                                db.insert_into_table(tableName, fields)
                            end    
                        end
                        
                    end
                    db.commit

                rescue Exception => e
                    puts 'save Exception: ' + e.inspect
                    db.rollback
                    
                    raise    
                end    
                
                true
              end
              
              # updates the current record in the viewable list and adds
              # a sync operation to update
              def update_attributes(attrs)
                obj = self.object #self.inst_strip_braces(self.object)
                update_type='update'
                nSrcID = self.get_inst_source_id
                db = ::Rho::RHO.get_src_db(get_inst_source_name)
                tableName = is_inst_schema_source() ? get_inst_schema_table_name() : 'object_values'
                begin
                    db.start_transaction
                    
                    if is_inst_full_update
                        attrs.each do |attrib,val|
                            self.vars[attrib.to_sym()] = val    
                        end    
                        attrs = self.vars
                    end
                    
                    attrs.each do |attrib,val|
                      attrib = attrib.to_s.gsub(/@/,"")
                      next if ::Rhom::RhomObject.method_name_reserved?(attrib)

                      # Don't save objects with braces to database
                      new_val = val.to_s #self.inst_strip_braces(val.to_s)
                      isModified = false
                      
                      if is_inst_full_update
                          isModified = true
                      else
                          old_val = self.send attrib.to_sym unless ::Rhom::RhomObject.method_name_reserved?(attrib)
                          
                          isModified = old_val != new_val
                          if isModified && new_val && old_val.nil? && new_val.to_s().length == 0
                            isModified = false
                          end  
                          if isModified && old_val && new_val.nil? && old_val.to_s().length == 0
                            isModified = false
                          end
                      end
                      
                      # if the object's value doesn't match the database record
                      # then we procede with update
                      if isModified
                          # only one update at a time
                          resUpdateType = is_inst_sync_source() ? db.select_from_table('changed_values', 'update_type', {"object"=>obj, "source_id"=>nSrcID, 'sent'=>0}) : nil 
                          if resUpdateType && resUpdateType.length > 0 
                              update_type = resUpdateType[0]['update_type'] 
                              db.delete_from_table('changed_values', {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID, "sent"=>0})
                          end
                          
                          # add to syncengine queue
                       
                          if is_inst_schema_source() 
                              result = db.select_from_table(tableName, 'object', {"object"=>obj})
                            
                              if result && result.length > 0 
                                db.update_into_table(tableName, {attrib=>new_val}, {"object"=>obj})
                              else
                                db.insert_into_table(tableName, {"object"=>obj, attrib=>new_val})                          
                              end    
                            
                          else  
                              result = db.select_from_table(tableName, 'source_id', {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID}) 
                            
                              if result && result.length > 0 
                                db.update_into_table(tableName, {"value"=>new_val}, {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID})
                              else
                                db.insert_into_table(tableName, {"source_id"=>self.get_inst_source_id, "object"=>obj, "attrib"=>attrib, "value"=>new_val})                          
                              end    
                            
                          end
                          
                          if is_inst_sync_source()  
                              if result && result.length > 0 
                                db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, "value"=>new_val, "update_type"=>update_type})
                              else
                                db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, "value"=>new_val, "update_type"=>update_type})
                              end    
                          end
                                                      
                          # update in-memory object
                          self.vars[attrib.to_sym()] = new_val unless is_inst_full_update
                      end
                    end
                    
                    db.commit

                rescue Exception => e
                    puts 'update_attributes Exception: ' + e.inspect
                    db.rollback
                    
                    raise    
                end    
                    
                true
              end

              def get_inst_source_name
                self.class.name.to_s
              end
	
              def get_inst_source_id
                Rho::RhoConfig.sources[get_inst_source_name]['source_id'].to_s
              end
          
              def is_inst_sync_source
                  Rho::RhoConfig.sources[get_inst_source_name]['sync_type'] != 'none'
              end
          
              def is_inst_schema_source
                 !Rho::RhoConfig.sources[get_inst_source_name]['schema'].nil?
              end
              def get_inst_schema_table_name
                 get_inst_source_name()
              end

              def is_inst_full_update
                 Rho::RhoConfig.sources[get_inst_source_name]['full_update']
              end
              
              def inst_strip_braces(str=nil)
                str ? str.gsub(/\{/,"").gsub(/\}/,"") : nil
              end
            end)
        end #unless
        #modelname = classname.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
        #puts "classname: #{classname}; modelname: #{modelname}"
        #require "#{classname}/#{modelname}" if File.exists? File.join(Rho::RhoFSConnector.get_base_app_path,'app',classname,"#{modelname}.iseq")
      #end
    end #init_object
  end # RhomObjectFactory
end # Rhom