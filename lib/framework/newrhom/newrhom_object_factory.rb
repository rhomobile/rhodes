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
          obj_inst = self.new(obj)
          self.klass_model.createObject(obj_inst.vars)
          obj_inst
        end

        def self.find(*args)
          puts "MZV_DEBUG: we are in find  #{args.inspect}"
          args.collect! { |arg| (arg.is_a?Symbol) ? arg.to_s : arg }
          objs = klass_model.find(*args)
          puts "MZV_DEBUG: find has returned : #{objs.inspect}"
          orm_objs = []
          objs.each do |obj|
            orm_objs << self.new(obj)
          end  
          puts "MZV_DEBUG: orm_objs : #{orm_objs.inspect}"
          orm_objs
        end
        
        # This holds the attributes for an instance of
        # the rhom object
        attr_accessor :vars
    
        def initialize(obj=nil)
          @vars = {}
          if obj
            self.class.klass_model.validateFreezedAttributes(obj)
            obj.each do |key,value|
              self.vars[key.to_sym()] = value if key && key.length > 0
            end
          end
          
          self.vars[:object] = "#{::Rho::NewORM.generateId}" unless self.vars[:object]
          self.vars[:source_id] = self.class.klass_model.source_id
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