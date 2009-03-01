require 'rho/render'
require 'rho/rhosupport'
require 'rho/rhoviewhelpers'

module Rho
  class RhoController

    def default_action
      return Hash['GET','show','PUT','update','POST','update',
        'DELETE','delete'][@request['request-method']] unless @request['id'].nil?
      return Hash['GET','index','POST','create'][@request['request-method']]
    end

    def serve(object_mapping,req,res)
      @request, @response = req, res;
      @object_mapping = object_mapping
      @params = RhoSupport::query_params req
      send req['action'].nil? ? default_action : req['action']
    end

    # Returns true if the request's header contains "XMLHttpRequest".
    def xml_http_request?
      not /XMLHttpRequest/i.match(@request['headers']['X-Requested-With']).nil?
    end
    alias xhr? :xml_http_request?

    def redirect(url_params = {},options = {})
      @response['status'] = options['status'] || 302 
      @response['headers']['Location'] = url_for(url_params)
      @response['message'] = options['message'] || 'Moved temporarily'
      return ''
    end
    
  end # RhoController
end # Rho