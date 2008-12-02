require 'erb'
	
module Rho
  class RhoController
	def self.renderfile(filename)
		ERB.new(IO.read(filename)).result(binding)
	end
    def render(view)
		ERB.new(IO.read(@request[:modelpath]+view.to_s+'.erb')).result(binding)
    end
  end # RhoController
end # Rho