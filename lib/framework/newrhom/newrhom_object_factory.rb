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

=begin
        def self.generate_id
          Rho::RhoConfig.generate_id()
        end
=end
        
        # This holds the attributes for an instance of
        # the rhom object
        attr_accessor :vars
=begin     
        def initialize(obj=nil)
          #klass_model.check_freezing_model(obj)
              
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

        def method_missing(method_sym, *args, &block)
          unless name == Fixnum
            if name[name.length()-1] == '='
              s_name = name.to_s.chop
              #check_freezing_model(s_name)
              @vars[s_name.to_sym()] = args[0]  
            else
              @vars[name]
            end
          else
            super
          end
        end
=end

=begin
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
=end
      end
      Object.const_set(klass_model_obj.model_name.intern, klass_instance)
    end
  end
end