require 'erb'
require 'rhosupport'

class RhoController
	
	def default_action
		Hash['GET','index','POST','create','PUT','update','DELETE','delete'][@request['request-method']]
	end 
	
	def serve(req,res)
		@request, @response = req, res;
		@params = RhoSupport::query_params req
		send req['action'].nil? ? default_action : req['action']
	end
	
	def render(view)
		ERB.new(IO.read(@request[:modelpath]+view.to_s+'.erb')).result(binding)
	end

end	