require 'rhom'
require 'rhofsconnector'

module Rho
  class RhoApplication
  	attr_accessor :default_menu
	
    def initialize
      if @rhom.nil?
        @rhom = Rhom::Rhom.new
      end
      @default_menu = { "Home" => :home, "Refresh" => :refresh, "Sync" => :sync, "Options" => :options, "Log" => :log }
    end
    
    def set_menu(menu=nil)
      disp_menu = menu ? menu : self.default_menu
      puts "RhoApplication: Using menu - #{disp_menu.inspect}"
  	  WebView.set_menu_items(disp_menu)
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
      res['request-body'] = (Object.const_get(req['model']+'Controller').new).send :serve, self, @rhom, req, res
    end

  end # RhoApplication
end # Rho