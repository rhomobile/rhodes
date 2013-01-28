#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

module Rhom

    def self.any_model_changed?
        ::Rho::RHO.get_db_partitions.each_value do |db|
            result = db.execute_sql("SELECT object FROM changed_values LIMIT 1 OFFSET 0" )
            
            return true if result && result.length > 0            
        end
        
        false
    end
	           
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
                check_freezing_model(obj)
              
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
                    s_name = name.to_s.chop
                    check_freezing_model(s_name)
                    @vars[s_name.to_sym()] = args[0]  
                  else
                    @vars[name]
                  end
                end
              end

                def check_freezing_model(obj)
                    isSchemaSrc = is_inst_schema_source()
                    return unless obj && (isSchemaSrc || Rho::RhoConfig.sources[get_inst_source_name()]['freezed'])
                     
                    hash_props = isSchemaSrc ? Rho::RhoConfig.sources[get_inst_source_name()]['schema']["property"] : Rho::RhoConfig.sources[get_inst_source_name()]["property"]
                    if obj.is_a?(Hash)
                        obj.each do |key, value|
                            next if key.to_s() == 'object'
                            raise ArgumentError.new( "Non-exist property : #{key}. For model : #{get_inst_source_name()}" ) unless hash_props[key.to_s()]
                        end
                    elsif obj.to_s() != 'object'
                        raise ArgumentError.new( "Non-exist property : #{obj}. For model : #{get_inst_source_name()}" ) unless hash_props[obj.to_s()]
                    end    
                end

              class << self

                def metadata
                  src_name = get_source_name()
                  return Rho::RhoController.cached_metadata[src_name] if Rho::RhoController.cached_metadata.has_key?(src_name)
                  
                  db = ::Rho::RHO.get_src_db(src_name)
                  result = db.select_from_table('sources', 'metadata', {"name"=>src_name} )
                  if result && result.length > 0 && result[0]['metadata']
                    Rho::RhoController.cached_metadata[src_name] = Rho::JSON.parse(result[0]['metadata'])
                  else
                    Rho::RhoController.cached_metadata[src_name] = nil                     
                  end
                  
                  Rho::RhoController.cached_metadata[src_name]
                end
                
                def metadata=(metadata_doc)
                  src_name = get_source_name
                  db = ::Rho::RHO.get_src_db(src_name)
                  db.execute_sql("UPDATE sources set metadata=? where name=?", metadata_doc, src_name)
                end

                def count
                  db = ::Rho::RHO.get_src_db(get_source_name)
                  
                  if is_schema_source()
                    #res = db.select_from_table(get_schema_table_name(),'object').length
                    db_res =  db.execute_sql("SELECT COUNT(*) FROM \"#{get_schema_table_name}\"")
                    #puts "db_res : #{db_res}"
                    if db_res && db_res.length() > 0 
                        res = db_res[0].values[0] 
                    else
                        res = 0
                    end    
                  else
                    res = db.select_from_table('object_values','object', {"source_id"=>get_source_id}, {"distinct"=>true}).length
                  end
                    
                  res.to_i()
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
                  Rho::RhoConfig.sources[get_source_name]['source_id'].to_i
                end

                def convertOpToStr(val_op, value)
                    res = ""
                    if val_op.upcase == 'IN' || val_op.upcase == 'NOT IN'
                    
                        if value.is_a?(String)
                            value = value.split(",")
                            value.each do |item|
                                item.strip!
                                if (item.start_with?("\"") && item.end_with?("\"")) || 
                                   (item.start_with?("\'") && item.end_with?("\'"))
                                    item.slice!(0)
                                    item.chop!
                                end
                            end
                            
                        end
                    
                        if value.is_a?(Array)
                            svalue = ""
                            value.each do |item| 
                                svalue += ',' if svalue.length() > 0
                                svalue += "\'#{item}\'"
                            end
                            res += val_op + ' ( ' + svalue + ' )'                                
                        else
                            raise ArgumentError, 'IN parameters should be String or Array'    
                        end    
                    else
                        res += val_op + ' ' + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(value, false) 
                    end
                    
                    res
                end

                def convertSqlConditionToStr(cond, op)
                  if cond.is_a?(String)
                    return cond
                  end

                  if cond.is_a?(Array) && !op
                    condition_str = cond[0].split(/\?/).each_with_index { |param,i| 
                      param << cond[i+1].to_s
                    }.join(' ').to_s 
                    
                    puts "condition_str : #{condition_str}"
                    return condition_str
                  end
                  
                  nil
                end

                def convertConditionToStatement(cond, op, sql, vals)
                    if cond.is_a?(Array)
                    
                        return unless op
                    
                        sqlRes = ""
                        valsRes = []
                        cond.each do |item|
                            sqlRes += ' ' + op + ' ' if sqlRes.length() > 0
                            
                            sqlCond = ""
                            valsCond = []
                            convertConditionToStatement(item[:conditions], item[:op], sqlCond, valsCond )
                            
                            sqlRes << "(" + sqlCond + ")"
                            valsRes.concat(valsCond)
                        end

                        sql << sqlRes
                        vals.concat(valsRes)
                    
                        return
                    end

                    return if !cond.is_a?(Hash)

                    bSimpleHash = true
                    op = "AND" unless op
                    cond.each do |key, value|
                        if key.is_a?(Hash)
                            sqlCond, valsCond = makeCondWhereEx(key, value, nil)
                            
                            sql << ' ' + op + ' ' if !bSimpleHash
                            sql << sqlCond                        
                            vals.concat(valsCond)
                            
                            bSimpleHash = false
                        end
                    end
                    
                    if bSimpleHash
                        sqlCond, valsCond = ::Rhom::RhomDbAdapter.make_where_params(cond,'AND')
                        sql << sqlCond                        
                        vals.concat(valsCond)
                    end
                end
=begin                                
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
                        
                        condition_str += ' ' + convertOpToStr(val_op, value)
                    end
                    
                  end
                  
                  if bSimpleHash
                    condition_hash = cond 
                    condition_str = ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                  end

                  condition_str                      
                end
=end                
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

                    sql << "attrib=" + ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(
                        val_func.upcase == 'CAST' ? attrib_name.split(' ')[0] : attrib_name)
                    sql << " AND source_id=" + srcid_value 
                    
                    if val_func.upcase == 'CAST'
                        sql << " AND " + val_func + "(value " + attrib_name.split(' ')[1] + ' ' + attrib_name.split(' ')[2] + " ) "
                    else                    
                        sql << " AND " + (val_func.length > 0 ? val_func + "(value)" : "value") + ' '
                    end
                    
                    sql << convertOpToStr(val_op, value)
                    
                    sql
                end

                def getCondAttribName(key)
                    if key.is_a?(Hash)
                        attrib_name = key[:name] if key[:name] 
                    else
                        attrib_name = key
                    end
                    
                    attrib_name
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
                    
                    if srcid_value.nil?
                        sql << (val_func.length > 0 ? val_func + "(#{attrib_name})" : "#{attrib_name}") + ' '
                    else
                        unless attrib_name.is_a?(String) && attrib_name == 'object'
                            sql << "attrib=?"                    
					        vals << (val_func.upcase == 'CAST' ? attrib_name.split(' ')[0] : attrib_name)
					        
                            sql << " AND source_id=?"
					        vals << srcid_value

                            if val_func.upcase == 'CAST'
                                sql << " AND " + val_func + "(value " + attrib_name.split(' ')[1] + ' ' + attrib_name.split(' ')[2] + " ) "
                            else
                                sql << " AND " + (val_func.length > 0 ? val_func + "(value)" : "value") + ' '
                            end    
    					else
                            sql << " source_id=?"
					        vals << srcid_value
    					
                            sql << " AND object "
                        end
                    end
                    
                    if val_op.upcase == 'IN' || val_op.upcase == 'NOT IN'

                        if value.is_a?(String)
                            value = value.split(",")
                            value.each do |item|
                                item.strip!
                                if item.start_with?("\"") && item.end_with?("\"")
                                    item.slice!(0)
                                    item.chop!
                                end
                            end
                            
                        end
                        
                        if value.is_a?(Array)
                            sql << val_op + " ( #{Array.new( value.length(), '?').join(',')} )"
						    vals.concat( value )
                        else
                            raise ArgumentError, 'IN parameters should be String or Array'    
                        end    
                    else
                        sql << val_op + " ?"
						vals << value #.to_s
                    end

                    return sql, vals
                end

                def find_objects(condition_hash, op, limit, offset, order_attr, &block)
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
                    if !(block_given? || order_attr)
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

                                bStop = !(block_given? || order_attr) && limit && offset && nulls_cond.length == 0 && listObjs.length >= offset + limit
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
                                attrib_name = getCondAttribName(key)
                                unless attrib_name && attrib_name.is_a?(String) && attrib_name == 'object'
                                    sql << "SELECT object FROM object_values WHERE \n"
                                else
                                    sql << "SELECT DISTINCT(object) FROM object_values WHERE \n"
                                end    
								
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
                        next if !(block_given? || order_attr) && offset && nIndex < offset && !strLimit
                        
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

                def find_objects_ex(condition_ar, op, limit, offset, order_attr, &block)
                    mapObjs = {}
                    listObjs = []
                    condition_ar.each do |cond|
                        res = find_objects(cond[:conditions], cond[:op], limit, offset, order_attr, &block)
                        
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
                    #puts 'find_bycondhash start' + (block_given? ? 'with block' : "")
                    
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
                        
                        order_dir = args[1][:orderdir]
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
                        if order_attr.is_a?(Array)
                            order_attr_arr = order_attr
                        else    
                            order_attr_arr.push(order_attr)
                        end
                            
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
                            listObjs = find_objects(condition_hash, op, limit, offset, order_attr, &block)
                        else
                            listObjs = find_objects_ex(condition_hash, op, limit, offset, order_attr, &block)
                        end

                        nCount = 0;
                        if args.first == :count
                            nCount = listObjs.length
                        else                                
                            listObjs.each do |obj|
                                sql = ""
                                values = []
                                
                                #if attribs
                                #    nonExistAttrs = attribs.dup
                                #    nonExistAttrs.delete(obj['attrib']) if obj['attrib']

                                #    if nonExistAttrs.length > 0
                                #        sql << "SELECT attrib,value FROM object_values WHERE object=? AND source_id=?"
                                #        values << obj['object']
                                #        values << get_source_id
                                #        
                                #        quests = ""
                                #        nonExistAttrs.each do |attr|
                                #            quests << ',' if quests.length() > 0
                                #            quests << '?'
                                #            values << attr
                                #        end
                                #
                                #        sql << " AND attrib IN (#{quests})"
                                #    end                                    
                                #else
                                    sql << "SELECT attrib,value FROM object_values WHERE \n"
                                    sql << "object=? AND source_id=?"
                                    
                                    values << obj['object']
                                    values << get_source_id
                                #end
                                
                                #puts "get attribs: #{sql}"
                                listAttrs = sql.length > 0 ? db.execute_sql(sql,values) : []
								
                                new_obj = self.new({:object=>"#{obj['object']}"})
                                #new_obj.vars.merge!({:object=>"#{obj['object']}"})
                                
                                #if attribs && obj['attrib']
                                    #new_obj.vars.merge!( {obj['attrib'].to_sym()=>obj['value'] }) if obj['value']
								#	new_obj.vars[obj['attrib'].to_sym()] = obj['value'] if obj['value']
                                #end
                                
                                listAttrs.each do |attrValHash|
                                  attrName = attrValHash['attrib']
                                  attrVal = attrValHash['value']
                                  #new_obj.vars.merge!( { attrName.to_sym()=>attrVal } ) if attrVal
                                  
                                  if attribs && attribs != '*'    
                                     next unless attribs.include? attrName
                                  end
                                  
								  new_obj.vars[attrName.to_sym()] = attrVal if attrVal
                                  
                                  #nonExistAttrs.delete(attrName) if nonExistAttrs
                                end
                                
                                #if nonExistAttrs
                                #   nonExistAttrs.each do |attrName|
                                #       new_obj.vars.merge!( { attrName.to_sym()=>nil } )
                                #   end
                                #end
                                
                                ret_list << new_obj
                                break if !(block_given? || order_attr) && limit && ret_list.length >= limit
                            end
                        end    
                    ensure
                      db.unlock_db
                    end
                  
                    order_array(ret_list, order_attr,order_dir, &block)

                    if (block_given? || order_attr) && limit
                        ret_list = ret_list.slice(offset,limit)
                    end

                    #puts "find_bycondhash end: #{ret_list.length} objects"

                    return nCount if args.first == :count

                    if args.first == :first
                        return ret_list.length > 0 ? ret_list[0] : nil
                    end 

                    ret_list
                end

                def order_array(ret_list, order_attr, order_dir, &block)
                    if order_attr
                        ret_list.sort! { |x,y| 
                        
                            res = 0
                            if order_attr.is_a?(Array)
                                order_attr.each_index { |i|
                                    vx = x.vars[order_attr[i].to_sym()]
                                    vy = y.vars[order_attr[i].to_sym()]
                                    res = vx && vy ? (block_given? ? yield(vx,vy): vx <=> vy) : 0

                                    dir = 'ASC'
                                    if order_dir && order_dir.is_a?(Array) && i < order_dir.length()
                                        dir = order_dir[i].upcase()
                                    else
                                        dir = order_dir.upcase() if order_dir && order_dir.is_a?(String)
                                    end
                                   
                                    res *= -1 if dir && dir == 'DESC'
                                    break if res != 0
                                }
                            else
                                vx = x.vars[order_attr.to_sym()]
                                vy = y.vars[order_attr.to_sym()]
                                res = vx && vy ? (block_given? ? yield(vx,vy): vx <=> vy) : 0
                                res *= -1 if order_dir && order_dir.upcase() == 'DESC'
                            end    
                            
                            res
                        }
                    elsif block_given?
                        ret_list.sort! { |x,y| 
                           res = yield(x,y)
                           res *= -1 if order_dir && order_dir.upcase() == 'DESC'
                           res
                        }
                    end
                    
                end
                
                def make_sql_order(params)
                    order_attr = params[:order]
                    order_dir = params[:orderdir]                    
                    
                    res = ""
                    
                    if order_attr && order_attr.is_a?(Array)
                        order_attr.each_index do |i|
                            res += "," if i > 0
                            
                            res += "\"#{order_attr[i]}\" "
                            if order_dir && order_dir.is_a?(Array) && i < order_dir.length()
                                res += order_dir[i].upcase()
                            else
                                res += order_dir && order_dir.is_a?(String) ? order_dir.upcase() : "ASC"
                            end
                        end                        
                    else
                        res = "\"#{order_attr}\" " + (order_dir ? order_dir.upcase() : "")
                    end
                    
                    res
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
                  #condition_hash = nil
                  select_arr = nil
                  condition_str = nil
                  order_dir=""
                  nSrcID = get_source_id.to_i
                  if args[1]
                    condition_str = convertSqlConditionToStr(args[1][:conditions], args[1][:op])  if args[1][:conditions]
                    
                    if args[1][:per_page] #and args[1][:offset]
                      limit = args[1][:per_page].to_i
                      offset = args[1][:offset] ? args[1][:offset].to_i : 0
                    end
                    
                    select_arr = args[1][:select] if args[1][:select]
                    order_dir = args[1][:orderdir]
                    order_attr = args[1][:order]
                    
                  end
                  
                  if args.first == :first
                    limit = 1                    
                    offset = 0 unless offset
                  end
                  
                  strLimit = nil
                  if !block_given?
                      strLimit = " LIMIT " + limit.to_s + " OFFSET " + offset.to_s if limit && offset
                  end
                  
                  # return horizontal resultset from database
                  # for example, an object that has attributes name,industry:
                  # |               object                 |       name         |  industry   |
                  # ---------------------------------------------------------------------------
                  # | 3560c0a0-ef58-2f40-68a5-48f39f63741b |A.G. Parr PLC 37862 |Entertainment|
                  if select_arr
                    attribs = select_arr
                    #attribs = attribs | condition_hash.keys if condition_hash
                  else
                    attribs = '*'
                  end

                  order_manually = false
                  if attribs and attribs.length > 0
                    sql = ""
                    list = []
                    db = ::Rho::RHO.get_src_db(get_source_name)
                    
                    if !is_schema_source()
                      objects = nil
                      if !condition_str
                        #srcid_value = ::Rhom::RhomDbAdapter.get_value_for_sql_stmt(get_source_id)
                        values = [] 

                        if args.first.is_a?(String)                      
                          objects = [ { 'object' => strip_braces(args.first.to_s) } ]
                          
                        else  
                          if !block_given? && order_attr && order_attr.is_a?(String)
                              if  !args[1][:dont_ignore_missed_attribs]
                                  sql << "SELECT object FROM object_values WHERE source_id=? "
                                  sql << " AND attrib=? ORDER BY \"value\" " + ( order_dir ? order_dir : "")
                                  values << nSrcID
                                  values << order_attr
                              end    
                          else
                              #it is more effective to use old find here
                              if attribs && attribs != '*' && attribs.length() != 0 && !args[1][:dont_ignore_missed_attribs]
                                  sql << "SELECT object FROM object_values WHERE attrib=? AND source_id=?"
                                  values << attribs[0]
                                  values << nSrcID
                              else 
                                  if  limit == 1 && offset == 0 
                                    sql = "SELECT object FROM object_values WHERE source_id=?"
                                    values << nSrcID
                                  elsif strLimit  
                                    sql = "SELECT distinct(object) FROM object_values WHERE source_id=?"
                                    values << nSrcID
                                  end
                                  
                                  #sql << "SELECT distinct(object) FROM object_values WHERE source_id=" << srcid_value 
                              end    
                              
                              order_manually = !order_attr.nil?
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
                                values2 << nSrcID
                                
                                #if attribs && attribs != '*'    
                                #    quests = ""
                                #    attribs.each do |attr|
                                #        quests << ',' if quests.length() > 0
                                #        quests << '?'
                                #        values2 << attr
                                #    end
                                #
                                #    sql2 << " AND attrib IN (#{quests})"
                                #end                                    
                               
                                item_attribs = db.execute_sql(sql2, values2)
                                if item_attribs && item_attribs.length() > 0
                                    new_item = {'object'=>object_id }
                                    item_attribs.each do |item|
                                       if attribs && attribs != '*'    
                                         next unless attribs.include? item['attrib']
                                       end
                                       
                                       new_item[item['attrib']] = item['value']
                                    end    

                                    list << new_item
                                end    
                            end    
                         end
						 
                         #puts "Processing rhom objects end1, #{list.length} objects"
                      end
                            
                      if objects.nil?

                        if !condition_str && !(args[1] && args[1][:dont_ignore_missed_attribs])
                            sql = "SELECT object, attrib, value FROM object_values WHERE source_id=? order by object"
                            values = [nSrcID]
                            objects = db.execute_sql(sql, values)                      

                            new_item = nil
                            objects.each do |item|
                               if ( !new_item || new_item['object'] != item['object'])
                                 list << new_item if new_item
                                 new_item = {'object'=>item['object'] }
                               end
                                
                               new_item[item['attrib']] = item['value'] if item['value']
                            end    
                            list << new_item if new_item
                            
                            order_manually = !order_attr.nil?
                        else
                                                
                            if attribs == "*"
                                raise ArgumentError, "Use Rhom advanced find syntax or specify :select parameter when use sql queries!"
                                #attribs = SyncEngine.get_src_attrs(Rho::RhoConfig.sources[get_source_name]['partition'].to_s, nSrcID)                            
                            end    
                          
                            if attribs and attribs.length > 0                      
                                sql << "SELECT * FROM (\n" if condition_str
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
                                sql << "WHERE " + ::Rhom::RhomDbAdapter.where_str(where_cond) + "\n" if where_cond and where_cond.length > 0
                                sql << "GROUP BY object\n"
                                sql << "ORDER BY " + make_sql_order(args[1]) if !block_given? && order_attr
                                #sql << ") WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                                sql << ") WHERE " + condition_str if condition_str
                                sql << strLimit if strLimit
                                
                                #puts "Database query start"
                                list = db.execute_sql(sql)
                                #puts "Database query end"
                            end   
                          end  
                        end
                    else #schema source
                       attribs = attribs.join(',') if attribs.is_a?(Array)
                       
                       sql << "SELECT #{attribs} FROM \"#{get_schema_table_name}\""
                       vals = []
                       if where_cond and where_cond.length > 0
                           sql << " WHERE " + ::Rhom::RhomDbAdapter.where_str(where_cond)
                       elsif condition_str
                           sql << " WHERE " + condition_str
                       elsif args[1] && args[1][:conditions]
                           sqlCond = ""
                           convertConditionToStatement(args[1][:conditions], args[1][:op], sqlCond, vals)
                           #condHash = {}
                           #sqlCond = convertConditionToStr(args[1][:conditions], args[1][:op], condHash)
                           
                           sql << " WHERE " + sqlCond if sqlCond.length() > 0
                       end
                           
                       sql << " ORDER BY " + make_sql_order(args[1]) if !block_given? && order_attr
                       sql << strLimit if strLimit
                       
                       #puts "Database query start" #: #{sql}"
                       list = db.execute_sql(sql, vals)
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

                  if block_given? || order_manually
                    order_array(ret_list, order_attr, order_dir, &block)
                    ret_list = ret_list.slice(offset,limit) if limit
                  end
                 
                  return list.length if args.first == :count
                  if args.first == :first || args.first.is_a?(String) 
                    return ret_list.length > 0 ? ret_list[0] : nil
                  end 
                  
                  ret_list
                end
              
                def find_by_sql(sql_query)
                    raise ArgumentError, 'find_by_sql only works with FixedSchema models' if !is_schema_source
                    
                    db = ::Rho::RHO.get_src_db(get_source_name)
                    list = db.execute_sql(sql_query)

                    ret_list = []
                    list.each do |rowhash|
                        new_obj = self.new({:object=>"#{rowhash['object']}"})

                        rowhash.each do |attrName, attrVal|
                            new_obj.vars.merge!( { attrName.to_sym()=>attrVal } ) if attrVal
                        end

                        ret_list << new_obj
                    end
                    
                    ret_list
                end
                
                def search(args)
                    args[:source_names] = [self.name.to_s]

                    SyncEngine.search(args)
                end

                def sync(callback=nil, callback_data="", show_status_popup=nil, query_params="")
                  src_id = get_source_id.to_i()
                  SyncEngine.set_notification(src_id, callback, callback_data) if callback
                  if !show_status_popup.nil?
                    SyncEngine.dosync_source(src_id, show_status_popup, query_params)
                  else
                    SyncEngine.dosync_source(src_id, 1, query_params)
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

                def on_sync_delete_error( objects, action )
                    raise ArgumentError, 'on_sync_delete_error action should be :retry' unless action == :retry
                    return  unless is_sync_source()

                    nSrcID = get_source_id()
                    db_partition = Rho::RhoConfig.sources[get_source_name]['partition'].to_s
                    
                    db = ::Rho::RHO.get_src_db(get_source_name)                            
                    db.start_transaction
                    
                    begin                    
                        objects.each do |obj, values|
                          values['attributes'].each do |attrib, value|
                          
                              resUpdateType =  db.select_from_table('changed_values', 'update_type', 
                                {"object"=>obj, "source_id"=>nSrcID, "attrib"=>attrib, 'sent'=>0})
                              next if resUpdateType && resUpdateType.length > 0 
                          
                              attrib_type = SyncEngine.is_blob_attr(db_partition, nSrcID,attrib)  ? "blob.file" : ""
                              db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, 
                                "value"=>value, "update_type"=>'delete', "attrib_type"=>attrib_type })
                          end      
                        end
                        db.commit
                    rescue Exception => e
                        puts 'on_sync_delete_error Exception: ' + e.inspect
                        db.rollback

                        raise    
                    end                    
                    
                end

                def on_sync_update_error( objects, action, rollback_data = nil )
                    raise ArgumentError, 'on_sync_update_error action should be :retry or :rollback' unless action == :retry || action == :rollback
                    return  unless is_sync_source()

                    nSrcID = get_source_id()
                    db_partition = Rho::RhoConfig.sources[get_source_name]['partition'].to_s
                    tableName = is_schema_source() ? get_schema_table_name : 'object_values'
                    is_full_update = Rho::RhoConfig.sources[get_source_name]['full_update']
                    
                    db = ::Rho::RHO.get_src_db(get_source_name)                            
                    db.start_transaction

                    begin                    
                    
                        if action == :rollback
                            raise ArgumentError, 'on_sync_update_error with :rollback action should provide update-rollback parameter' unless rollback_data
                            rollback_data.each do |obj, values|
                              values['attributes'].each do |attrib, value|
                                _insert_or_update_attr(db, is_schema_source(), tableName, nSrcID, obj, attrib, value)
                              end
                            end  
                        else
                    
                            objects.each do |obj, values|
                                #if is_full_update
                                #    resUpdateType =  db.select_from_table('changed_values', 'update_type', {"object"=>obj, "source_id"=>nSrcID, 'sent'=>0})
                                #    
                                #    unless resUpdateType && resUpdateType.length > 0 
                                #        db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>'object', "value"=>"", "update_type"=>'update'})
                                #    end    
                                #else
                            
                                  values['attributes'].each do |attrib, value|
                                      resUpdateType =  db.select_from_table('changed_values', 'update_type', 
                                        {"object"=>obj, "source_id"=>nSrcID, "attrib"=>attrib, 'sent'=>0})
                                      next if resUpdateType && resUpdateType.length > 0 
                                  
                                      attrib_type = SyncEngine.is_blob_attr(db_partition, nSrcID,attrib)  ? "blob.file" : ""
                                      db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, 
                                        "value"=>value, "update_type"=>'update', "attrib_type"=>attrib_type })
                                  end      
                                #end
                            end    
                        end
                            
                        db.commit
                    rescue Exception => e
                        puts 'on_sync_update_error Exception: ' + e.inspect
                        db.rollback

                        raise    
                    end                        
                end

                def push_changes()
                    return  unless is_sync_source()
                    nSrcID = get_source_id()
                    db = ::Rho::RHO.get_src_db(get_source_name)                            
                
                    db.insert_into_table('changed_values', {'update_type'=>'push_changes', 'source_id' => nSrcID } )
                end
                
                def on_sync_create_error( objects, action )
                    raise ArgumentError, 'on_sync_create_error action should be :delete or :recreate' unless action == :delete || action == :recreate
                    return  unless is_sync_source()
                    
                    ar_objs = objects
                    if objects.is_a?(Hash)
                        ar_objs = objects.keys()
                    elsif !objects.is_a?(Array)
                        ar_objs = [objects]
                    end
                        
                    puts "ar_objs : #{ar_objs}"
                    tableName = is_schema_source() ? get_schema_table_name : 'object_values'
                    nSrcID = get_source_id()
                    
                    db = ::Rho::RHO.get_src_db(get_source_name)                            
                    db.start_transaction

                    begin
                        ar_objs.each do |obj|
                            if action == :recreate
                                deletes =  db.select_from_table('changed_values', 'object', {"update_type"=>'delete', "object"=>obj, "source_id"=>nSrcID })
                                unless ( deletes && deletes.length() > 0 )
                                
                                    db.delete_from_table('changed_values', {'object'=>obj, "source_id"=>nSrcID})

                                    db.insert_into_table('changed_values', 
                                        {'update_type'=>'create', 'attrib'=>'object', "source_id"=>nSrcID, "object"=>obj} )

                                    next                                        
                                end
                            end
                            
                            db.delete_from_table('changed_values', {'object'=>obj, "source_id"=>nSrcID})
                            if is_schema_source()                                
                                db.delete_from_table(tableName, {'object'=>obj } )
                            else
                                db.delete_from_table(tableName, {'object'=>obj, "source_id"=>nSrcID} )
                            end
                        end
                        
                        db.commit
                    rescue Exception => e
                        puts 'on_create_error Exception: ' + e.inspect
                        db.rollback

                        raise    
                    end    

                end
                
                # deletes all records matching conditions (optionally nil)
                def delete_all(*args)
                  db = ::Rho::RHO.get_src_db(get_source_name)
                  tableName = is_schema_source() ? get_schema_table_name : 'object_values'
                  op = 'AND'
                  op = args[0][:op].upcase if args && args[0] && args[0][:op]
                  conditions = args[0][:conditions] if args && args[0] && args[0][:conditions]
                  
                  begin
                      db.start_transaction
                      
                      if is_schema_source()
                      
                        vals = []
                        sqlCond = ""
                        convertConditionToStatement(conditions, op, sqlCond, vals)
                      
                        if is_sync_source()
                            sql = "SELECT object FROM \"#{tableName}\""
                            #sql << " WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                            #sql << " WHERE " + condition_str if condition_str
                            sql << " WHERE " + sqlCond if sqlCond.length() > 0
                            listObjs = db.execute_sql(sql, vals)
                            
                            listObjs.each do |item|
                            
                                db.delete_from_table('changed_values', {'object'=>item['object'], "source_id"=>get_source_id(), "sent"=>0})
                                
                                attrsList = db.select_from_table(tableName, '*', {"object"=>item['object']}) 
                                attrsList[0].each do |attrName, attrValue|
                                    next if attrName == 'object'
                                    
                                    db.insert_into_table('changed_values', 
                                      {"source_id"=>get_source_id(), "object"=>item['object'], 
                                        "attrib"=> attrName, "value"=>attrValue, 
                                        "update_type"=>'delete'})
                                end        
                                
                                sql = "DELETE FROM \"#{tableName}\" WHERE object=?"
                                values = [ item['object'] ]
                                
                                db.execute_sql(sql,values)
                            end
                            
                        else #just delete objects
                            sql = "DELETE FROM \"#{tableName}\""
                            #sql << " WHERE " + ::Rhom::RhomDbAdapter.where_str(condition_hash) if condition_hash
                            #sql << " WHERE " + condition_str if condition_str
                            sql << " WHERE " + sqlCond if sqlCond.length() > 0
                            db.execute_sql(sql, vals)
                        end  
                      else
                        listObjs = []
                        
                        if !conditions
                            if is_sync_source()                        
                                listObjs = db.execute_sql("SELECT DISTINCT(object) FROM \"#{tableName}\" WHERE source_id=?", get_source_id() )
                            else
                                db.delete_from_table(tableName, {"source_id"=>get_source_id()} )
                            end    
                        elsif conditions.is_a?(Hash)
                            listObjs = find_objects(conditions, op, nil, 0, nil)
                        else
                            listObjs = find_objects_ex(conditions, op, nil, 0, nil)
                        end
                      
                        listObjs.each do |item|

                            if is_sync_source()                        
                                db.delete_from_table('changed_values', {'object'=>item['object'], "source_id"=>get_source_id(), "sent"=>0})
                                
                                attrsList = db.select_from_table(tableName, '*', {"object"=>item['object'], "source_id"=>get_source_id()}) 
                                attrsList.each do |attrName|
                                    db.insert_into_table('changed_values', 
                                      {"source_id"=>get_source_id(), "object"=>item['object'], 
                                        "attrib"=> attrName['attrib'], "value"=>attrName['value'], 
                                        "update_type"=>'delete'})
                                end        
                            end
                            
                            sql = "DELETE FROM #{tableName} WHERE \n"
                            sql << "object=? AND source_id=?"
                            
                            values = []
                            values << item['object']
                            values << get_source_id
                            
                            db.execute_sql(sql,values)
                        end
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
                    
                    new_obj.create
                    
                    new_obj
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
                        db.delete_from_table(tableName, {"object"=>obj})
                      else
                        attrsList = db.select_from_table(tableName, '*', {"object"=>obj, "source_id"=>self.get_inst_source_id()}) 
                        db.delete_from_table(tableName, {"object"=>obj, "source_id"=>self.get_inst_source_id()})
                      end  
                      
                      resUpdateType =  is_inst_sync_source() ? db.select_from_table('changed_values', 'update_type', {"object"=>obj, "update_type"=>'create', "sent"=>0}) : nil 
                      if resUpdateType && resUpdateType.length > 0 
                        update_type = nil                              
                      end
                      
                      db.delete_from_table('changed_values', {"object"=>obj, "source_id"=>self.get_inst_source_id(), "sent"=>0}) if is_inst_sync_source()
                      
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

              def create
                nSrcID = self.get_inst_source_id
                obj = self.object
                src_name = get_inst_source_name
				tableName = is_inst_schema_source() ? get_inst_schema_table_name() : 'object_values'
				isSchemaSrc = is_inst_schema_source()
                db = ::Rho::RHO.get_src_db(src_name)
                begin
                    db.start_transaction

                    db.insert_into_table('changed_values', 
                        {'update_type'=>'create', 'attrib'=>'object', "source_id"=>nSrcID,"object"=>obj} ) if is_inst_sync_source()

                    if isSchemaSrc                    
                        db.insert_into_table(tableName, self.vars, {:source_id=>true})
                    else    
                        self.vars.each do |key_a,value|
                            key = key_a.to_s
                            next if ::Rhom::RhomObject.method_name_reserved?(key)

                            val = value.to_s #self.inst_strip_braces(value.to_s)
                            
                            db.insert_into_table(tableName, {"source_id"=>nSrcID, "object"=>obj, "attrib"=>key, "value"=>val })
                        end
                    end
                    
                    db.commit
                rescue Exception => e
                    puts 'create Exception: ' + e.inspect
                    db.rollback
                    
                    raise    
                end
                
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

                #call create if item does not exists
                is_new_item = false
                begin
                    db.lock_db()
                    if isSchemaSrc
                        existing_attribs = db.execute_sql("SELECT object FROM \"#{tableName}\" WHERE object=? LIMIT 1 OFFSET 0",obj)
                    else
                        existing_attribs = db.execute_sql("SELECT object FROM \"#{tableName}\" WHERE object=? AND source_id=? LIMIT 1 OFFSET 0",obj,nSrcID)
                    end
                    
                    unless existing_attribs && existing_attribs.length > 0                     
                        is_new_item = true
                        create();
                    end
                    
                    db.unlock_db
                rescue Exception => e
                    puts 'save Exception: ' + e.inspect
                    db.unlock_db
                    
                    raise    
                end    
				
				return if is_new_item
				
                begin
                    db.start_transaction

                    update_type = 'update'
                    ignore_changed_values = true
                    resUpdateType = nil
                    if is_inst_sync_source()
                        resUpdateType =  db.select_from_table('changed_values', 'update_type', {"object"=>obj, "source_id"=>nSrcID, 'sent'=>0})
                        update_type = resUpdateType[0]['update_type'] if resUpdateType && resUpdateType.length > 0 
                        ignore_changed_values = update_type=='create'
                        
                        #if is_inst_full_update
                        #    unless resUpdateType && resUpdateType.length > 0 
                        #        db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>'object', "value"=>"", "update_type"=>update_type})
                        #    end    
                        #    ignore_changed_values = update_type=='update'
                        #end
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

                            #if is_inst_full_update
                            #  isModified = true
                            #else
                                oldValue = isSchemaSrc ? resValue[0][key] : resValue[0]['value']
                                
                                isModified = oldValue != val
                                if isModified && val && oldValue.nil? && val.to_s().length == 0
                                  isModified = false
                                end  
                                if isModified && oldValue && val.nil? && oldValue.to_s().length == 0
                                  isModified = false
                                end
                            #end
                            
                            if isModified
                            
                                unless ignore_changed_values
                                    resUpdateType = db.select_from_table('changed_values', 'update_type', {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID, 'sent'=>0}) 
                                    if resUpdateType && resUpdateType.length > 0 
                                        db.update_into_table('changed_values', {"value"=>val}, {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID})
                                    else
                                        db.insert_into_table('changed_values', fields)        
                                    end
                                    
                                end
                                    
                                if isSchemaSrc
                                    db.update_into_table(tableName, {key=>val}, {"object"=>obj})
                                else
                                    db.update_into_table(tableName, {"value"=>val}, {"object"=>obj, "attrib"=>key, "source_id"=>nSrcID})                                    
                                end    
                            end    
                        else
                            db.insert_into_table('changed_values', fields) unless ignore_changed_values
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

              def self._insert_or_update_attr(db, is_schema, tableName, nSrcID, obj, attrib, new_val)
                  if is_schema
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
                        db.insert_into_table(tableName, {"source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, "value"=>new_val})                          
                      end    
                    
                  end
              end
              
              # updates the current record in the viewable list and adds
              # a sync operation to update
              def update_attributes(attrs)
                check_freezing_model(attrs)
              
                obj = self.object #self.inst_strip_braces(self.object)
                update_type='update'
                nSrcID = self.get_inst_source_id
                db = ::Rho::RHO.get_src_db(get_inst_source_name)
                db_partition = Rho::RhoConfig.sources[get_inst_source_name]['partition'].to_s
                tableName = is_inst_schema_source() ? get_inst_schema_table_name() : 'object_values'
                begin
                
                    db.start_transaction

                    ignore_changed_values = true
                    if is_inst_sync_source()
                        resUpdateType =  db.select_from_table('changed_values', 'update_type', {"object"=>obj, "source_id"=>nSrcID, 'sent'=>0})
                        update_type = resUpdateType[0]['update_type'] if resUpdateType && resUpdateType.length > 0 
                        ignore_changed_values = update_type=='create'
                        
                        #if is_inst_full_update
                        #    unless resUpdateType && resUpdateType.length > 0 
                        #        db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>'object', "value"=>"", "update_type"=>update_type})
                        #    end    
                        #    ignore_changed_values = update_type=='update'
                        #end
                    end
                                    
                    attrs.each do |attrib,val|
                      attrib = attrib.to_s.gsub(/@/,"")
                      next if ::Rhom::RhomObject.method_name_reserved?(attrib)

                      # Don't save objects with braces to database
                      new_val = val.to_s #self.inst_strip_braces(val.to_s)
                      isModified = false
                      
                      old_val = self.send attrib.to_sym unless ::Rhom::RhomObject.method_name_reserved?(attrib)
                      
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
                          self.class._insert_or_update_attr(db, is_inst_schema_source(), tableName, nSrcID, obj, attrib, new_val)
                          
                          unless ignore_changed_values
                          
                              if resUpdateType && resUpdateType.length > 0
                                db.delete_from_table('changed_values', {"object"=>obj, "attrib"=>attrib, "source_id"=>nSrcID, "sent"=>0})
                              end  
                          
                              attrib_type = is_blob_attrib(db_partition, nSrcID, attrib) ? "blob.file" : ""
                              db.insert_into_table('changed_values', {"source_id"=>nSrcID, "object"=>obj, "attrib"=>attrib, 
                                "value"=>new_val, "update_type"=>update_type, "attrib_type"=>attrib_type })
                          end
                                                      
                          # update in-memory object
                          self.vars[attrib.to_sym()] = new_val #unless is_inst_full_update
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
                Rho::RhoConfig.sources[get_inst_source_name]['source_id'].to_i
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
        #require "#{classname}/#{modelname}" if File.exists? File.join(Rho::RhoFSConnector.get_base_app_path,'app',classname,"#{modelname}" + RHO_RB_EXT)
      #end
    end #init_object
  end # RhomObjectFactory
end # Rhom