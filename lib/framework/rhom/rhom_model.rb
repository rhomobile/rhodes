
module Rhom

  module BaseModel
    class << self
      attr_accessor :model_params
      
    end

    def get_model_params
      BaseModel.model_params  
    end

    def reset_model_params    
        BaseModel.model_params = {}
    end
    
    def fixed_schema?
        BaseModel.model_params ||= {}
        BaseModel.model_params["model_type"] = "property_bag" unless BaseModel.model_params["model_type"]
        
        false
    end
    
    def property(name,type=:string,option=nil)
      fixed_schema?()

  	  BaseModel.model_params ||= {}

      BaseModel.model_params['schema'] ||= {}
      BaseModel.model_params['schema']['property'] ||= {}
      BaseModel.model_params['schema']['property'][name.to_s] = [type,option] 
      

    end

    def set(name,value)
  	  BaseModel.model_params ||= {}
  	  
  	  if ( name == :sync )
  	    if ( value )
  	        BaseModel.model_params['sync_type'] = 'incremental' unless BaseModel.model_params['sync_type']
  	    else
  	        BaseModel.model_params.delete('sync_type')
  	    end        
  	  else
  	    BaseModel.model_params[name.to_s] = value
  	  end
    end

    def enable(name)
      set(name,true)
    end
    
    def belongs_to(name, owner)
  	  BaseModel.model_params ||= {}
  	  BaseModel.model_params['belongs_to'] ||= {}
  	  
  	  BaseModel.model_params['belongs_to'][name.to_s] = owner.to_s
    end
    
    def index(name,cols)
  	  return unless fixed_schema?
  	  
      BaseModel.model_params['schema'] ||= {}
      BaseModel.model_params['schema']['index'] ||= []  	  

      BaseModel.model_params['schema']['index'] << {name.to_s=>cols}
    end

    def unique_index(name,cols)
  	  return unless fixed_schema?
  	  
      BaseModel.model_params['schema'] ||= {}
      BaseModel.model_params['schema']['unique_index'] ||= []  	  

      BaseModel.model_params['schema']['unique_index'] << {name.to_s=>cols}
    end
    
  end
  
  module FixedSchema
    include BaseModel

    def self.included(model)
      model.extend FixedSchema
    end
    
    def fixed_schema?
        BaseModel.model_params ||= {}
        BaseModel.model_params["model_type"] = "fixed_schema" unless BaseModel.model_params["model_type"]
        
        true
    end
    
  end

  module PropertyBag
    include BaseModel
    
    def self.included(model)
      model.extend PropertyBag
    end
    
  end
  
end # Rhom
