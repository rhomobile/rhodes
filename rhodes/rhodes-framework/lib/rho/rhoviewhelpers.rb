module Rho
  class RhoController

    def truncate(text,length)
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

    # Examples of how to use link_to method:
    #
    # link_to "Visit Other Site", "http://www.rhomobile.com/"
    # ==> <a href=\"http://www.rhomobile.com/\" >Visit Other Site</a>
    #
    # link_to "Help", { :action => "help" }
    # ==> <a href=\"/application/model/help\" >Help</a>
    #
    # link_to "Delete", { :action => "delete", :id => '{12}' }
    # ==> <a href="/application/model/{12}/delete"  onclick="var f = document.createElement('form'); 
    # f.style.display = 'none';this.parentNode.appendChild(f); f.method = 'POST'; 
    # f.action = this.href;f.submit();return false;">Delete</a>
    #
    # link_to "Show", { :action => "show", :id => '{12}'},"style=\"height:4px;width:7px;border-width:0px;\"" 
    # ==> <a href="/application/model/{12}/show" style="height:4px;width:7px;border-width:0px;">Show</a>
    #
    # link_to "Delete", { :action => "delete", :id => '{12}' }, "class=\"delete_link\""
    # ==> <a href="/application/model/{12}/delete" class="delete_link" onclick="var f = document.createElement('form'); 
    # f.style.display = 'none';this.parentNode.appendChild(f); f.method = 'POST'; 
    # f.action = this.href;f.submit();return false;\">Delete</a>"
    #
    # link_to "Invate",:action => :invite, :query => {:name => 'John Smith', 'address' => "http://john.smith.com"}
    # ==> <a href="/application/model/invite?name=John%20Smith&address=http%3A%2F%2Fjohn.smith.com" >Invate</a>
    #
    def link_to(name,url_params = {},html_options = "",confirm = nil)
      url = url_for(url_params)
      if (url_params.is_a?(String) || url_params[:action].to_s != 'delete')
        "<a href=\"#{url}\" #{html_options}>#{name || url}</a>"
      else
        "<a href=\"#{url}\" #{html_options} onclick=\""+  #if (confirm('#{confirm}')) {
        "var f = document.createElement('form'); f.style.display = 'none';" +
        "this.parentNode.appendChild(f); f.method = 'POST'; f.action = this.href;f.submit();"+
        "return false;\">#{name || url}</a>"
      end
    end

    # Examples of how to use url_for method:
    #
    # url_for '/some_url'
    # ==> /some_url
    #
    # When generating a new URL, missing values may be filled in from the current request's parameters.
    # For example, if application name or model are not specifyed in the call parameters, they would be filled from the request.
    #
    # url_for :action => :index
    # ==> /application/model
    #
    # url_for :action => :create
    # ==> /application/model
    #
    # url_for :action => :new
    # ==> /application/model/new
    #
    # url_for :action => :show, :id => '{12}'
    # ==> /application/model/{12}/show
    #
    # url_for :model => :another_model, :action => :show, :id => '{12}'
    # ==> /application/another_model/{12}/show
    #
    # url_for :controller => :another_controller, :action => :show, :id => '{12}'
    # ==> /application/another_controller/{12}/show
    #
    # url_for :application => :another_app, :model => :another_model, :action => :show, :id => '{12}'
    # ==> /another_app/another_model/{12}/show
    # 
    # url_for :action => :create, :query => {:name => 'John Smith', 'address' => "http://john.smith.com"}
    # ==> /application/model?name=John%20Smith&address=http%3A%2F%2Fjohn.smith.com
    #
    # url_for :action => :show, :id => '{12}', :fragment => "an-anchor" 
    # ==> /application/model/{12}/show#an-anchor
    #
    def url_for(params = {})
      return params.to_s if params.is_a? String or params.is_a? Symbol
      return '/' if not params.is_a? Hash or params.nil?

      application = params[:application] || @request['application']
      model = params[:controller] || params[:model] || @request['model'] 
      action = params[:action].nil? ? nil : params[:action].to_s
      id = params[:id].nil? ? nil : params[:id].to_s
      query = query_to_s(params[:query])
      fragment = params[:fragment].nil? ? '' : '#' + params[:fragment]
   
      amurl = ''
      amurl << '/' << application.to_s << '/' << model.to_s

      if action.nil? or action == 'create' or action == 'index'  
        amurl << query << fragment 
      else
        amurl << '/' << (id.nil? ? action : id + '/' + action) << query << fragment          
      end
        
      amurl
      
      #amurl = '/' + application.to_s + '/' + model.to_s

      #return amurl + query + fragment if action.nil? or action == 'create' or action == 'index'
      #return amurl +'/'+ (id.nil? ? action : id + '/' + action) + query + fragment
    end

  private
    def query_to_s(query)
      return '' if query.nil?
      qstring = '?'
      query.each do |key,value|
        qstring += '&' if qstring.length > 1
        qstring += Rho::RhoSupport.url_encode(key.to_s) + '=' + Rho::RhoSupport.url_encode(value.to_s)
      end
      qstring
    end
    
    # def url_encode(s)
    #   s.to_s.dup.force_encoding("ASCII-8BIT").gsub(/[^a-zA-Z0-9_\-.]/n) {
    #     sprintf("%%%02X", $&.unpack("C")[0])
    #   }
    # end
    
  end # RhoController
end # Rho