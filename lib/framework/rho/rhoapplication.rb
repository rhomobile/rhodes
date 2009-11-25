require 'rhom'
require 'rhofsconnector'

module Rho
  class RhoApplication
  	attr_accessor :default_menu
  	
  	TOOLBAR_TYPE = 0
  	TABBAR_TYPE = 1
  	NOBAR_TYPE = 2
  	
  	@@toolbar = []

    def init_locale
        curLocale = System::get_locale()
        puts 'Current locale: ' + curLocale.to_s
        
        begin
            require 'rholang/lang_' + curLocale
        rescue Exception => e
            puts 'Could not load locale: ' + curLocale.to_s + '. Load english.'
            require 'rholang/lang_en' unless curLocale == 'en'
        end
        
        begin
            require 'lang/lang_' + curLocale
        rescue Exception => e
            puts 'Could not load app resources for locale: ' + curLocale.to_s
            if curLocale != 'en'
                begin
                    puts 'Load english resources.'
                    require 'lang/lang_en' 
                rescue Exception => e
                end    
            end    
        end
        
        require 'rholang/localization_simplified'
    end
    	
    def initialize
      init_locale()
      
      unless @rhom
        @rhom = Rhom::Rhom.new
      end
      unless @default_menu
      	@default_menu = { "Home" => :home, "Refresh" => :refresh, 
      		"Sync" => :sync, "Options" => :options, "Log" => :log, :separator => nil, "Close" => :close }
  	  end
  	  if @tabs
  	    # normalize the list
  	    @tabs.map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
  	    puts "Initializing application with tabs: #{@tabs.inspect}" 
  	    NativeBar.create(TABBAR_TYPE, @tabs)
	    elsif @@toolbar
	      NativeBar.create(TOOLBAR_TYPE, @@toolbar)
      else
        NativeBar.create(NOBAR_TYPE, [])
	    end
    end
    
    def on_activate_app
    end
    
    def set_menu(menu=nil,back_action=nil)
      disp_menu = menu ? menu.dup : @default_menu.dup
      disp_menu['Back'] = back_action if back_action
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