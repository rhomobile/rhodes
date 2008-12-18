	
module Rho
  class RhoController
    
    def self.renderfile(filename)
      if File.extname(filename) == '.iseq'
	      eval_compiled_file(filename, binding)
	    else
        IO.read(filename)		    
	    end
	  end
  	
    def render(view)
		  eval_compiled_file(@request[:modelpath]+view.to_s+'_erb'+'.iseq', binding )
    end    
    
  end # RhoController
end # Rho