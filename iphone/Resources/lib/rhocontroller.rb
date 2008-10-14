require 'erb'
require 'rhosupport'

class RhoController
	
	def default_action
		return Hash['GET','show','PUT','update','POST','update',
		'DELETE','delete'][@request['request-method']] unless @request['id'].nil?
		return Hash['GET','index','POST','create'][@request['request-method']]
	end 
	
	def serve(req,res)
		@request, @response = req, res;
		@params = RhoSupport::query_params req
		send req['action'].nil? ? default_action : req['action']
	end
	
	def render(view)
		ERB.new(IO.read(@request[:modelpath]+view.to_s+'.erb')).result(binding)
	end
	
	def link_to(name,action,id=nil,confirm=nil)
		action = action.to_s
		if (action != 'delete')
			"<a href=\"#{url_for(action,id)}\">#{name}</a>"
		else
			"<a href=\"#{url_for(action,id)}\" onclick=\""+  #if (confirm('#{confirm}')) { 
			"var f = document.createElement('form'); f.style.display = 'none';" + 
			"this.parentNode.appendChild(f); f.method = 'POST'; f.action = this.href;f.submit();"+
			"return false;\">#{name}</a>"
		end	
	end
	
	def url_for(action,id=nil)
		action = action.to_s
		amurl = '/'+@request['application']+'/'+@request['model'] 
		return amurl if action == 'create' or action == 'index'
		return amurl +'/'+ (id.nil? ? action.to_s : id.to_s + '/' + action.to_s)
	end
	
	def redirect(action,id=nil)
		@response['status'] = 302
		@response['headers']['Location'] = url_for(action,id)
		@response['message'] = 'Moved temporarily'
		return ''
	end
end	