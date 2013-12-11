module Rhom
  module BaseModel
    def init_model
      puts "MZV_DEBUG: Model '#{klass_model.model_name}': init_model"
      klass_model.initModel
    end

    def set(name,value)
  	  if ( name == :sync )
  	    if ( value )
  	        klass_model.sync_type = 'incremental' unless klass_model.sync_type != 'none'
            klass_model.partition = 'user' unless klass_model.partition != 'local'
  	    else
  	        klass_model.sync_type = 'none'
            klass_model.partition = 'local'
  	    end        
  	  else
  	    klass_model.setProperty(name.to_s, value.to_s)
  	  end
    end

    def enable(name)
      set(name,true)
    end

    def fixed_schema?
      klass_model.fixed_schema == true
    end

    def property(name,type=:string, option=nil)
      klass_model.setModelProperty(name.to_s, type.to_s, option.to_s)
    end

    def belongs_to(name, owner)
      if owner.is_a?(Array)       
        owner.each do |src|        
          klass_model.setBelongsTo(name.to_s, src.to_s)
        end
      else
        klass_model.setBelongsTo(name.to_s, owner.to_s)
      end  
    end
   
    def index(name,cols)
      return unless fixed_schema?
      
      klass_model.setSchemaIndex(name.to_s, cols, false)
    end

    def unique_index(name,cols)
      return unless fixed_schema?
      
      klass_model.setSchemaIndex(name.to_s, cols, true)
    end
  end
  
  module FixedSchema
    include BaseModel

    def self.included(model)
      model.extend FixedSchema
      model.klass_model.fixed_schema = true
    end
  end

  module PropertyBag
    include BaseModel
    
    def self.included(model)
      model.extend PropertyBag
      model.klass_model.fixed_schema = false
    end
  end
end
