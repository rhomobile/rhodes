
module Rho
  class RhoController
    def render(view)
		eval(@request[:modelpath]+view.to_s+'_erb', binding )
    end
  end # RhoController
end # Rho