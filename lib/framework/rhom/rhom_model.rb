
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
        false
    end
    
    def property(name,type=:string)
  	  BaseModel.model_params ||= {}
  	  
  	  if type == :blob
  	      BaseModel.model_params['blob_attribs'] ||= []
  	      BaseModel.model_params['blob_attribs'] << name.to_s
  	  end
  	      
  	  if fixed_schema?
  	      BaseModel.model_params['schema'] ||= {}
          BaseModel.model_params['schema']['columns'] ||= []  	  
        
  	      BaseModel.model_params['schema']['columns'] << name.to_s
      	  #TODO: type support
  	  end    
  	  
    end

    def set(name,value)
  	  BaseModel.model_params ||= {}
  	  BaseModel.model_params[name.to_s] = value
    end

    def enable(name)
      set(name,true)
    end
    
    def belongs_to(name, owner)
  	  BaseModel.model_params ||= {}
  	  BaseModel.model_params['links'] ||= {}
  	  
  	  BaseModel.model_params['links'][owner.to_s] = name.to_s
    end
    
    def index(*args)
  	  return unless fixed_schema?
  	  
      BaseModel.model_params['schema'] ||= {}
      BaseModel.model_params['schema']['indexes'] ||= []  	  

      BaseModel.model_params['schema']['indexes'] << args
    end

    def unique_index(*args)
  	  return unless fixed_schema?
  	  
      BaseModel.model_params['schema'] ||= {}
      BaseModel.model_params['schema']['unique_indexes'] ||= []  	  

      BaseModel.model_params['schema']['unique_indexes'] << args
    end
    
  end
  
  module FixedSchema
    include BaseModel

    def self.included(model)
      model.extend FixedSchema
    end
    
    def fixed_schema?
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
