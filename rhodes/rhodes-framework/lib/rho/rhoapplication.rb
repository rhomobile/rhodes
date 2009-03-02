require 'rhom'
require 'rhofsconnector'

module Rho
  class RhoApplication
	
    def initialize
      if @rhom.nil?
        @rhom = Rhom::Rhom.new
      end
    end
	
    class << self
	
      def get_app_path(appname)
        Rho::RhoFSConnector::get_app_path(appname)
      end
      
      def get_base_app_path
        Rho::RhoFSConnector::get_base_app_path
      end

      def get_model_path(appname, modelname)
        Rho::RhoFSConnector::get_model_path(appname, modelname)
      end
	
    end

    def serve(req,res)
      req[:modelpath] = self.class.get_model_path req['application'], req['model']
      require req[:modelpath]+'controller'
      res['request-body'] = (Object.const_get(req['model']+'Controller').new).send :serve, @rhom, req, res
    end

  end # RhoApplication
end # Rho