module Rho
  class RhoController

	def truncate(text, length)
		if text
			omission = '...'
			l = length - omission.length
			(text.length > length ? text[0...l] + omission : text).to_s
		end
	end

	def click_to_call(phone,description=nil)
		description = phone if description.nil?
		return "" if phone.nil? or phone.length == 0
		"<a href=\"tel:#{phone}\" target=\"_self\">#{description}</a>"
	end

	def mailto(address,description=nil)
		description = address if description.nil?
		return "" if address.nil? or address.length == 0
		"<a href=\"mailto:#{address}\" target=\"_self\">#{description}</a>"
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
      return action if action == '/'
      return amurl if action == 'create' or action == 'index'
      return amurl +'/'+ (id.nil? ? action.to_s : id.to_s + '/' + action.to_s)
    end

  end # RhoController
end # Rho