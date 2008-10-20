require 'rhom'

class RhoApplication
	
	def initialize(modelname, source_id)
		puts 'inside initialize model, source_id: ' + modelname + ', ' + source_id
		if @rhom.nil?
		  @rhom = Rhom.new(modelname, source_id)
		end
		puts 'rhom: ' + @rhom.inspect
	end
	
	class << self
	
		def get_app_path(appname)
			File.join(File.dirname(File.expand_path(__FILE__)), '../apps/'+appname+'/')
		end

		def get_model_path(appname, modelname)
			File.join(File.dirname(File.expand_path(__FILE__)), '../apps/'+appname+'/'+modelname+'/')
		end
	
	end

	def serve(req,res)
		req[:modelpath] = self.class.get_model_path req['application'], req['model']
		puts 'trying to require controller...'
		require req[:modelpath]+'controller'
		puts 'require controller successful...'
		res['request-body'] = (Object.const_get(req['model']+'Controller').new).send :serve, @rhom, req, res
	end

end	