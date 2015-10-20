module Rhom

  class RecordNotFound < StandardError
  end
  
  class RhomObjectFactory

    # Initialize new object with dynamic attributes
    def self.init_object(klass_model_obj)
      return if Object.const_defined?(klass_model_obj.model_name.intern)

      klass_instance = Class.new do

        @klass_model = klass_model_obj
        def self.klass_model
          @klass_model
        end

        def self.get_source_id
          @klass_model.source_id
        end

        def self.method_missing(method_sym, *args, &block)
          if klass_model.respond_to?(method_sym)
            # convert all symbols into strings
            args.collect! { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
            klass_model.send(method_sym, *args)
          else
            super
          end
        end

        def self._normalize_complex_conditions(conditions, op)
          return ["", []] unless conditions
          op ||= 'AND'
          sqlRes = ""
          quests = []
          if conditions.is_a?(Array)
            conditions.each do |item|
              sqlRes += ' ' + op + ' ' if sqlRes.length() > 0
              sqlCond, condQuests = _normalize_complex_condition(item[:conditions], item[:op])
              sqlRes += '(' + sqlCond + ')'
              quests.concat(condQuests)
            end
          else
            sqlRes, quests = _normalize_complex_condition(conditions, op)
          end
          [sqlRes, quests]
        end

=begin
        def self._normalize_hash_condition(item, op)
          # Simple Hash condition can be of two forms:
          # 1) {'key1':'value'1, 'key2':'value2' ... }, 'op':'OP1' => translates into : key1=value1 OP1 key2=value2
          # 2) or full form { 'op':'OP1', 'func':'FUNC1', 'attrib':'key1', 'value(s)':'value1' } => translates into FUNC1(key1) OP1 value1
          if item.has_key?(:op)
            itemOp = item[:op] || "="
            itemFunc = item[:func] || ""
            itemName = item[:name] || ""
            itemValues = item[:value] || item[:values] || []
            if itemValues.is_a?String
              itemValues = [itemValues]
            end

            subCond = klass_model.buildComplexWhereCond(itemName.to_s, itemValues, itemOp, itemFunc)
            return [subCond[0], subCond[1..-1]]
          else
            puts "MZV_DEBUG : _normalize_hash_condition : #{item.inspect} , #{op}"
            sqlRes = ""
            condQuests = []
            item.each do |k, v|
              subCond = klass_model.buildComplexWhereCond(k.to_s, [v], "=", '')
              puts "MZV_DEBUG : _received back : #{subCond.inspect}"
              if sqlRes.length > 0
                sqlRes += " " + op + " "
              end
              sqlRes += subCond[0]
              condQuests += subCond[1..-1]
            end
            puts "MZV_DEBUG : _and in the end we have : #{sqlRes} ,  #{condQuests}"
            return [sqlRes, condQuests];
          end
        end
=end

        def self._normalize_complex_condition(conditions, op)
          bSimpleHash = true
          normalized_condition_hash = {}
          normalized_values = []
          op ||= "AND"
          sqlRes = ""
          condQuests = []
          conditions.each do |key, value|
            retVal = nil
            if key.is_a?(Hash)
              if value.is_a?Integer
                value = [value.to_s]
              elsif value.is_a?Float
                value = [value.to_s]
              elsif value.is_a?Time
                value = [value.to_s]
              elsif value.is_a?(String)
                value = value.split(",")
                value.each do |item|
                  item.strip!

                  need_unescape = (item.start_with?('\'') && item.end_with?('\'')) ||
                  (item.start_with?('\"') && item.end_with?('\"')) 
                    
                  item = need_unescape ? item[1..-1] : item
                end
              end

              retVal = self.klass_model.buildComplexWhereCond(key[:name], value, key[:op] || '=', key[:func])
            else # key is a String - simple Hash
              values = (value && value.to_s.length > 0 ? [value] : [])
              retVal = self.klass_model.buildComplexWhereCond(key.to_s, values, '=', '')
            end
            if retVal
              sqlRes << ' ' + op + ' ' if sqlRes.length > 0
              sqlRes += retVal[0]
              condQuests.concat(retVal[1..-1])
            end
          end
          [sqlRes, condQuests]
        end

        def self._normalize_conditions(what, conditions, op)
          if !op
            retV = []
            if !conditions
              retV = self.klass_model.buildSimpleWhereCond(what, [])
              return [retV[0], retV[1..-1]]
            elsif conditions.is_a?String
              retV = self.klass_model.buildSimpleWhereCond(what, [conditions])
              return [retV[0], retV[1..-1]]
            elsif conditions.is_a?Array
              retV = self.klass_model.buildSimpleWhereCond(what, conditions)
              return [retV[0], retV[1..-1]]
            else
              return _normalize_complex_conditions(conditions, 'AND')
            end
          end
          _normalize_complex_conditions(conditions, op)
        end

        def self.changed?
          self.klass_model.anyChangedObjects
        end

        def self.create(obj)
          objHash = self.klass_model.createObject(obj)
          self.new(objHash);
        end

        def self._normalize_args_for_find(what, args_hash, normalized_string_args, normalized_vector_args)
          # 1) Normalize LIMITS
          normalized_string_args.merge!(self.klass_model.buildFindLimits(what, args_hash))

          # 2) Normalize ORDER BY
          order_dir = args_hash[:orderdir] || []
          order_attr = args_hash[:order] || []
          # normalize ORDER BY attrs
          order_attr = [order_attr] if(order_attr.is_a?String)
          order_dir = [order_dir] if(order_dir.is_a?String)
          normalized_vector_args[:order] = self.klass_model.buildFindOrder(order_attr, order_dir)

          # 3) Normalize SELECT
          select_arr = args_hash[:select] || []
          normalized_vector_args.merge!({:select => select_arr})
        end

        def self._order_array(ret_list, order_attr, order_dir, &block)
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

        def self.find(*args, &block)
          raise RecordNotFound if args[0].nil? or args.length == 0
          args[0] = args[0].to_s
          args[1] = args[1] || {}
          retVal = nil

          # prepare arguments
          if klass_model.fixed_schema
            normalized_vector_args = {}
            normalized_string_args = {}
            _normalize_args_for_find(args[0], args[1], normalized_string_args, normalized_vector_args)
            # Build Where Conditions
            cond_str, quests = _normalize_conditions(args[0], args[1][:conditions], args[1][:op])
            normalized_string_args[:conditions] = cond_str || ""
            normalized_vector_args[:quests] = quests || []
            retVal = klass_model.findObjects(args[0],
                                           normalized_string_args,
                                           normalized_vector_args[:quests],
                                           normalized_vector_args[:select],
                                           normalized_vector_args[:order])
          else
            # property bag
            # only LIMIT is supported
            normalized_string_args = {};
            normalized_vector_args = {}
            _normalize_args_for_find(args[0], args[1], normalized_string_args, normalized_vector_args)
            # 2) Normalize ORDER BY
            order_dir = args[1][:orderdir] || []
            order_attr = args[1][:order] || []
            # normalize ORDER BY attrs for PropertyBag Model
            order_attr = [order_attr] if(order_attr.is_a?String)
            order_dir = [order_dir] if(order_dir.is_a?String)
            normalized_string_args[:order] = self.klass_model.buildFindOrderString(order_attr, order_dir)
            normalized_string_args[:op] = args[1] || 'AND';
            if args[1][:conditions].is_a?Hash
              retVal = klass_model.findObjectsPropertyBagByCondHash(args[0],
                                                                    args[1][:conditions],
                                                                    normalized_string_args,
                                                                    normalized_vector_args[:select])
            else # the only other supported case is simple string (WHERE sql) or array (WHERE sql + quests)
              args[1][:conditions] = args[1][:conditions] || [""]
              args[1][:quests] = []
              if args[1][:conditions].is_a?Array
                args[1][:quests] = args[1][:conditions][1..-1]
                args[1][:conditions] = args[1][:conditions][0]
              end
              retVal = klass_model.findObjectsPropertyBagByCondArray(args[0],
                                                                    args[1][:conditions],
                                                                    args[1][:quests],
                                                                    normalized_string_args,
                                                                    normalized_vector_args[:select])
            end
          end

          if retVal.is_a?Array
            # as per description http://apidock.com/rails/v2.3.8/ActiveRecord/Base/find/class
            if retVal.empty?
              case args[0]
                when 'first', 'last'
                  return nil
                when 'all'
                  return retVal
                else
                  return nil          
              end
            end

            # if arg[0] is :first or objId return one object
            return self.new(retVal[0]) if (args[0] != 'all')
            # otherwise - return an array
            orm_objs = []
            retVal.each do |obj|
              orm_objs << self.new(obj)
            end
            _order_array(orm_objs, args[1][:order], args[1][:orderdir], &block)
            return orm_objs
          end
          retVal
        end

        def self.find_by_sql(sql_query)
          retVal = klass_model.find_by_sql(sql_query)
          orm_objs = []
          retVal.each do |obj|
            orm_objs << self.new(obj)
          end
          return orm_objs
        end

        # Alias for find(:all, args)
        def self.find_all(args=nil)
          find(:all, args)
        end

        # Returns a set of rhom objects, limiting the set to length :per_page
        # If no :per_page is specified, the default size is 10
        def self.paginate(args = {})
          # Default to 10 items per page
          args ||= {}
          args[:page] ||= 0
          args[:per_page] ||= 10
          args[:offset] = args[:page] * args[:per_page]
          find(:all, args)
        end

        # deletes all records matching conditions (optionally nil)
        def self.delete_all(*args)
          args[0] ||= {}
          retVal = nil

          # prepare arguments
          if klass_model.fixed_schema
            normalized_vector_args = {}
            normalized_string_args = {}
            _normalize_args_for_find("all", args[0], normalized_string_args, normalized_vector_args)
            # Build Where Conditions
            cond_str, quests = _normalize_conditions("all", args[0][:conditions], args[0][:op])
            normalized_string_args[:conditions] = cond_str || ""
            normalized_vector_args[:quests] = quests || []
            # call API function
            retVal = klass_model.deleteObjects(normalized_string_args,
                                               normalized_vector_args[:quests])
          else # property bag
            normalized_vector_args = {}
            normalized_string_args = {}
            _normalize_args_for_find("all", args[0], normalized_string_args, normalized_vector_args)
            normalized_string_args[:op] = args[0][:op] || 'AND'
            if args[0][:conditions].is_a?Hash
                retVal = klass_model.deleteObjectsPropertyBagByCondHash(args[0][:conditions], normalized_string_args)
            else # the only other supported case is simple string (WHERE sql) or array (WHERE sql + quests)
              conditions = args[0][:conditions] || [""]
              quests = []
              if conditions.is_a?Array
                quests = conditions[1..-1]
                conditions = conditions[0]
              end
              retVal = klass_model.deleteObjectsPropertyBagByCondArray(conditions, quests, normalized_string_args)
            end
          end
          retVal
        end

        def self.on_sync_delete_error( objects, action )
          objects.each do |obj,val|
            klass_model.onSyncDeleteError(obj, val['attributes'], action.to_s) 
          end
        end

        def self.on_sync_update_error( objects, action, rollback_data = nil )
          if action == :rollback
            rollback_data.each do |obj,val|
              klass_model.onSyncUpdateError(obj, {}, val['attributes'], action.to_s)
            end
          else
            objects.each do |obj,val|
              klass_model.onSyncUpdateError(obj, val['attributes'], {}, action.to_s) 
            end
          end
        end

        def self.on_sync_create_error( objects, action )
          ar_objs = objects
          if objects.is_a?(Hash)
            ar_objs = objects.keys
          elsif !objects.is_a?(Array)
            ar_objs = [objects]
          end
          klass_model.onSyncCreateError(ar_objs, action.to_s)
        end

        def self.push_changes
          klass_model.pushChanges
        end

        def update_var(key, value)
          key_s = key.to_sym()
          if value
            @vars[key_s] = value 
          else
            @vars.delete(key_s)
          end
        end

        attr_accessor :vars

        def initialize(obj={})
          @vars = {}
          objHash = obj
          unless obj[:object] or obj['object']
            objHash = self.class.klass_model.createInstance(obj)
          end
          objHash.each { |key,value| update_var(key, value)}
        end

        def update_attributes(attrs)
          objHash = self.class.klass_model.updateObject(self.object, @vars, attrs)
          objHash.each { |key,value| update_var(key, value)}
          true
        end

        def save
          #objId = self.object
          #attrs = @vars.collect { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          objHash = self.class.klass_model.saveObject(self.object, @vars)
          objHash.each { |key,value| update_var(key, value)}
          true
        end

        def destroy
          self.class.klass_model.deleteObject(self.object)
          true
        end

        def can_modify
          self.class.klass_model.canModify(self.object)
        end

        def changed?
          self.class.klass_model.hasChanges(self.object)
        end

        def method_missing(method_sym, *args, &block)
          unless method_sym == Fixnum
            if method_sym[-1] == '='
              s_name = method_sym.to_s.chop
              self.class.klass_model.validateFreezedAttribute(s_name)
              update_var(s_name.to_sym(), args[0])
            else
              @vars[method_sym]
            end
          else
            super
          end
        end

        def object
          @vars[:object]
        end

        def source_id
          @vars[:source_id] ||= self.class.klass_model.source_id
        end

        def to_s
          @vars.to_s if @vars
        end

        def to_json(*args)
          @vars.to_json(*args) if @vars
        end
      end
      Object.const_set(klass_model_obj.model_name.intern, klass_instance)
    end
  end
end
