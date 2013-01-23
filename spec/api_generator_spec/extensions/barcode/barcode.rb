module Barcode

class BarcodeStringHelper

   def self.process_string(str)
       return Barcode::native_process_string(str)
   end 
    
end


end

