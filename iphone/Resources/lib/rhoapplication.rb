require 'rhom'

class RhoApplication
	attr_accessor :rhom
	
	def initialize(modelname)
		puts 'in rhoapplication initialize ' + modelname
		@rhom = Rhom.new(modelname)
	end
	
	class << self
	
		def get_app_path(appname)
			File.join(File.dirname(File.expand_path(__FILE__)), '../'+appname+'/')
		end

		def get_model_path(appname, modelname)
			File.join(File.dirname(File.expand_path(__FILE__)), '../'+appname+'/'+modelname+'/')
		end
	
	end

	def serve(req,res)
		req[:modelpath] = self.class.get_model_path req['application'], req['model']
		require req[:modelpath]+'controller'
		res['request-body'] = (Object.const_get(req['model']+'Controller').new).send :serve, req, res		
	end

end	