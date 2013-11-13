module NewRhom         
  class NewRhomObjectFactory
    
    # Initialize new object with dynamic attributes
    def self.init_object(klass_model_obj)
      return if Object.const_defined?(klass_model_obj.model_name.intern)
      
      klass_instance = Class.new do

        @klass_model = klass_model_obj
        def self.klass_model
          @klass_model
        end
      #    include ::Rhom::RhomObject
      #    extend ::Rhom::RhomObject
              
          # This holds the attributes for an instance of
          # the rhom object
      #    attr_accessor :vars
          
        def generate_id
          Rho::RhoConfig.generate_id()
        end
              
        def initialize(obj=nil)
      #      check_freezing_model(obj)
              
      #      @vars = {}
      #      self.rhom_init(@vars)
      #      self.vars[:object] = "#{generate_id()}" unless obj && obj[:object]
      #      self.vars[:source_id] = self.get_inst_source_id.to_i
                
      #          if obj
      #            obj.each do |key,value|
      #              self.vars[key.to_sym()] = value if key && key.length > 0
      #            end
      #          end
        end
      end
      Object.const_set(klass_model_obj.model_name.intern, klass_instance)
    end
  end
end