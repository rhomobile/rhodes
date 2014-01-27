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

        def self._make_complex_where_conditions(what, conditions, op)
          return ["", []] unless conditions
          op ||= 'AND'
          sqlRes = ""
          quests = []
          if conditions.is_a?(Array)
            conditions.each do |item|
              sqlRes += ' ' + op + ' ' if sqlRes.length() > 0
              sqlCond, condQuests = _make_complex_where_cond(item[:conditions], item[:op])
              sqlRes += '(' + sqlCond + ')'
              quests.concat(condQuests)
            end
          else
            sqlRes, quests = _make_complex_where_cond(conditions, op)
          end
          [sqlRes, quests]
        end

        def self._make_complex_where_cond(conditions, op)
          bSimpleHash = true
          normalized_condition_hash = {}
          normalized_values = []
          op ||= "AND"
          sqlRes = ""
          condQuests = []
          conditions.each do |key, value|
            retVal = nil
            if key.is_a?(Hash)
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

              retVal = self.klass_model.buildComplexWhereCond(key[:name], values, key[:val_op], key[:val_func])
            else # key is a String
              values = (value && value.length > 0 ? [value] : [])
              retVal = self.klass_model.buildComplexWhereCond(key, values, '=', '')
            end
            if retVal
              sqlRes << ' ' + op + ' ' if sqlRes.length > 0
              sqlRes += retV[0]
              condQuests.concat(retV[1..-1])
            end
          end
          [sqlRes, condQuests]  
        end

        def self._make_conditions(what, conditions, op)
          puts "MZV_DEBUG : make_conditions : #{what}, #{conditions}, #{op}"
          if !op
            if !conditions or conditions.is_a?String
              return self.klass_model.buildSimpleWhereCond(what, [conditions]) 
            elsif conditions.is_a?Array
              return self.klass_model.buildSimpleWhereCond(what, conditions)
            end
          end
          _make_complex_where_conditions(conditions, op)
        end

        def self.create(obj)
          objHash = self.klass_model.createObject(obj)
          self.new(objHash);
        end

        def self._normalize_args_for_find(what, args_hash = {}, normalized_string_args, normalized_vector_args)
          # 1) Normalize LIMITS
          normalized_string_args = self.klass_model.buildFindLimits(what, args_hash)

          # 2) Normalize ORDER BY
          order_dir = []
          order_dir ||= args_hash[:orderdir]
          order_attr = []
          order_attr ||= args_hash[:order]
          # normalize ORDER BY attrs
          if(order_attr.is_a?String)
            order_attr = [order_attr]
          end
          if(order_dir.is_a?String)
            order_dir = [order_dir]
          end
          normalized_vector_args = {}
          normalized_vector_args[:order] = self.klass_model.buildFindOrder(what, order_dir, order_attr)
          
          # 3) Normalize SELECT
          select_arr = args_hash[:select] || []
          normalized_vector_args.merge!({:select => select_arr})

          # 4) Build Where Conditions
          cond_str, quests = _make_conditions(what, args_hash[:conditions], args_hash[:op])
          normalized_string_args[:conditions] = cond_str || ""
          normalized_vector_args[:quests] = quests || []
        end

        def self.find(*args)
          puts "MZV_DEBUG: we are in find  #{args.inspect}"
          #if args[0] == :count && !args[1]
          #  return klass_model.count
          #end

          # prepare arguments
          normalized_vector_args = {}
          normalized_string_args = {}
          _normalize_args_for_find(args[0], args[1], normalized_string_args, normalized_vector_args)
          # call API function
          retVal = klass_model.findObjects(args[0], 
                                           normalized_string_args, 
                                           normalized_vector_args[:quests],
                                           normalized_vector_args[:select],
                                           normalized_vector_args[:order])
          puts "MZV_DEBUG: find has returned : #{retVal.inspect}"
          if retVal.is_a?Array
            return retVal unless retVal.size() > 0
            # if arg[0] is :first or objId return one object
            return self.new(retVal[0]) if (args[0] != 'all')
            # otherwise - return an array 
            orm_objs = []
            retVal.each do |obj|
              orm_objs << self.new(obj)
            end  
            puts "MZV_DEBUG: orm_objs : #{orm_objs.inspect}"
            return orm_objs
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