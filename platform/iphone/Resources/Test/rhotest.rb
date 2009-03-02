require 'rhocontroller'

class RhoTest < RhoController

	def list
		@test_string = "Some text string"
		render :hello
	end

end
