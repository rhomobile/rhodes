
module Rho
  class RhoController
	def self.renderfile(filename)
		eval(filename+'_erb', binding)
	end  
    def render(view)
		eval(@request[:modelpath]+view.to_s+'_erb', binding )
    end
  end # RhoController
end # Rho