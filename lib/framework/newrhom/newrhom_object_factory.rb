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

        def self.create(obj)
          objHash = self.klass_model.createObject(obj)
          self.new(objHash);
        end

        def self.find(*args)
          puts "MZV_DEBUG: we are in find  #{args.inspect}"
          args.collect! { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          retVal = klass_model.find(*args)
          puts "MZV_DEBUG: find has returned : #{retVal.inspect}"
          orm_objs = []
          retVal.each do |obj|
            orm_objs << self.new(obj)
          end  
          puts "MZV_DEBUG: orm_objs : #{orm_objs.inspect}"

          args[0] == 'all' ? orm_objs : orm_objs[0] 
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
          #attrs.collect! { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          #oldAttrs = @vars.collect { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          objHash = self.class.klass_model.updateObject(self.object, @vars, attrs)
          objHash.each do |key, value|
            self.vars[key.to_sym()] = value
          end
          true
        end

        def save
          objId = self.object
          attrs = @vars.collect { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          objHash = self.class.klass_model.saveObject(self.object, attrs)
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