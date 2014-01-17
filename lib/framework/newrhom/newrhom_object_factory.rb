module Rhom         
  class RhomObjectFactory
    
    # Initialize new object with dynamic attributes
    def self.init_object(klass_model_obj)
      return if Object.const_defined?(klass_model_obj.model_name.intern)
      
      klass_instance = Class.new do

        @klass_model = klass_model_obj
        def self.klass_model
          @klass_model
        end

        def self.method_missing(method_sym, *args, &block)
          puts "MZV_DEBUG: we are in method missing #{method_sym}"
          if klass_model.respond_to?(method_sym)
            puts "MZV_DEBUG: we are trying to call ORMModel #{method_sym}, #{args.inspect}, #{block}"
            # convert all symbols into strings
            args.collect! { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
            klass_model.send(method_sym, *args)
          else
            super
          end
        end

        def self._make_conditions_no_op(conditions,op)
          if conditions.is_a?String
            return conditions, []
          end
          if conditions.is_a?Array && !op
            return conditions[0], conditions[1..-1]
          end        
          nil
        end

        def self._make_conditions_with_op(conditions, op, sql, vals)
          if conditions.is_a?(Array)
            return unless op
                    
            sqlRes = ""
            valsRes = []
            conditions.each do |item|
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
          conditions.each do |key, value|
            if key.is_a?(Hash)
              sqlCond, valsCond = _make_cond_where_ex(key, value)
                            
              sql << ' ' + op + ' ' if !bSimpleHash
              sql << sqlCond                        
              vals.concat(valsCond)
                            
              bSimpleHash = false
            end
          end
                    
          if bSimpleHash
            sqlCond, valsCond = _make_where_params(conditions,'AND')
            sql << sqlCond                        
            vals.concat(valsCond)
          end
        end

        def self._make_cond_where_ex(key,value)
          val_op = '='
          val_func = ''
          attrib_name = ''
          if key.is_a?(Hash)
            val_op = key[:op] if key[:op] 
            val_func = key[:func].upcase if key[:func] 
            attrib_name = key[:name] if key[:name] 
          else
            attrib_name = key
          end

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

          puts " make_cond_where_ex: #{attrib_name}, #{value}, #{val_op}, #{val_func}"
          retV = klass_model.buildCondWhereEx(attrib_name, value, val_op, val_func)
          puts " and retVal is : #{retV.inspect}"
          [retV[0], retV[1..-1]]
        end

        def self._make_where_params(condition,op)
          raise ArgumentError if !condition || !op || op.length == 0
          quests = ""
          vals = []
      
          condition.each do |key,val|
            if quests.length > 0
                quests << ' ' << op << ' '
            end

            if val.nil?        
                quests << "\"#{key}\" IS NULL" 
            else
                quests << "\"#{key}\"=?"
                vals << val
            end
          end
        
          return quests,vals
        end

        def self._make_conditions(what, conditions, op)
          puts "MZV_DEBUG : make_conditions : #{what}, #{conditions}, #{op}"
          unless conditions && conditions.length > 0
            arg_what = what.to_sym
            if(arg_what == :all || arg_what == :first || arg_what == :count)
              return _make_cond_where_ex("", "")
            else
              return _make_cond_where_ex("object", what)
            end
          end
          cond_str,quests = _make_conditions_no_op(conditions, op)
          if !cond_str
            cond_str,quests = make_conditions_with_op(conditions, op)
          end
          [cond_str,quests]
        end

        def self.create(obj)
          objHash = self.klass_model.createObject(obj)
          self.new(objHash);
        end

        def self.find(*args)
          puts "MZV_DEBUG: we are in find  #{args.inspect}"
          if args[0] == :count && !args[1]
            return klass_model.count
          end

          # prepare arguments
          select_arr = []
          order_dir = []
          order_attr = []
          conditions_str = ""
          quests = []
          options = {}
          if args[1]
            c, q = _make_conditions(args[0], args[1][:conditions], args[1][:op])
            conditions_str = c if (c && c.length > 0)
            quests = q if (q && q.length > 0)
          else
            c, q = _make_conditions(args[0], nil, nil)
            conditions_str = c if (c && c.length > 0)
            quests = q if (q && q.length > 0)
          end
          options[:conditions] = conditions_str
          if(args[1])
            if args[1][:per_page]
              options[:per_page] = args[1][:per_page].to_i
              options[:offset] = args[1][:offset] ? args[1][:offset].to_i : 0
            end
                    
            select_arr ||= args[1][:select]
            order_dir ||= args[1][:orderdir]
            order_attr ||= args[1][:order]
          end
          if args[0] == :first
            options[:per_page] = 1                    
            options[:offset] = 0 unless options[:offset]
          end

          # normalize ORDER BY attrs
          if(order_attr.is_a?String)
            order_attr = [order_attr]
          end
          if(order_dir.is_a?String)
            order_dir = [order_dir]
          end
          # if ORDER DIRECTION array is smaller - pad it
          pad_number = order_attr.size - order_dir.size
          pad_number.times do
            order_dir << "ASC"
          end
          # merge 2 arrays
          orders_hash = {}
          order_attr.zip(order_dir) do |oattr, odir|
            orders_hash[oattr] = odir 
          end

          # call API function
          retVal = klass_model.findObjects(args[0], options, quests, select_arr, orders_hash)
          puts "MZV_DEBUG: find has returned : #{retVal.inspect}"
          if retVal.is_a?Array
            orm_objs = []
            retVal.each do |obj|
              orm_objs << self.new(obj)
            end  
            puts "MZV_DEBUG: orm_objs : #{orm_objs.inspect}"
            return (args[0] == 'all' ? orm_objs : orm_objs[0])
          end
          retVal
        end

        # This holds the attributes for an instance of
        # the rhom object
        attr_accessor :vars
    
        def initialize(obj={})
          @vars = {}
          objHash = obj
          unless obj[:object] or obj['object']
            objHash = self.class.klass_model.createInstance(obj)
          end
          objHash.each do |key,value|
            self.vars[key.to_sym()] = value
          end
        end

        def update_attributes(attrs)
          objHash = self.class.klass_model.updateObject(self.object, @vars, attrs)
          objHash.each do |key, value|
            self.vars[key.to_sym()] = value
          end
          true
        end

        def save
          #objId = self.object
          #attrs = @vars.collect { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          objHash = self.class.klass_model.saveObject(self.object, @vars)
          objHash.each do |key, value|
            self.vars[key.to_sym()] = value
          end
          true
        end

        def destroy
          self.class.klass_model.deleteObject(self.object)
          true
        end

        def method_missing(method_sym, *args, &block)
          puts "MZV_DEBUG: we are in #{self.class.name} method missing and #{method_sym}, #{args.inspect}"
          unless method_sym == Fixnum
            if method_sym[-1] == '='
              s_name = method_sym.to_s.chop
              self.class.klass_model.validateFreezedAttribute(s_name)
              @vars[s_name.to_sym()] = args[0]  
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
          @vars[:source_id]                
        end
=begin
        def to_s
          @vars.to_s if @vars
        end
              
        def to_json(*args) 
          @vars.to_json(*args) if @vars
        end
=end
      end
      Object.const_set(klass_model_obj.model_name.intern, klass_instance)
    end
  end
end