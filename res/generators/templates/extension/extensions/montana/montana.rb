module <%= name.camel_case %>

class <%= name.camel_case %>StringHelper

   def self.process_string(str)
       return <%= name.camel_case %>::native_process_string(str)
   end 
    
end


end

