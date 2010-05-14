require 'rhom'
require 'rhofsconnector'
require 'rholang/localization_simplified'
require 'rho/rhomsg'

module Rho
  class RhoApplication
  	attr_accessor :default_menu
  	
  	TOOLBAR_TYPE = 0
  	TABBAR_TYPE = 1
  	NOBAR_TYPE = 2
  	
  	@@toolbar = [{:action => :back}, {:action => :forward}, {:action => :separator},
      {:action => :home}, {:action => :refresh}, {:action => :options} ]
    	
    def initialize
      LocalizationSimplified.requre_loc(Rho::RhoFSConnector::get_app_path('app') + 'lang/lang_',true)
      
      unless @rhom
        @rhom = Rhom::Rhom.new
      end
      unless @default_menu
      	@default_menu = { Rho::RhoMessages.get_message('home_menu') => :home, Rho::RhoMessages.get_message('refresh_menu') => :refresh, 
      		Rho::RhoMessages.get_message('sync_menu') => :sync, Rho::RhoMessages.get_message('options_menu') => :options, Rho::RhoMessages.get_message('log_menu') => :log, :separator => nil, Rho::RhoMessages.get_message('close_menu') => :close }
  	  end
  	  if @tabs
  	    # normalize the list
  	    @tabs.map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
  	    puts "Initializing application with tabs: #{@tabs.inspect}" 
  	    NativeBar.create(TABBAR_TYPE, @tabs)
        NativeBar.switch_tab(0)
	    elsif @@toolbar
	      NativeBar.create(TOOLBAR_TYPE, @@toolbar)
      else
        NativeBar.create(NOBAR_TYPE, [])
	    end
    end
    
    def on_activate_app
    end

    # works for schema sources    
    def on_migrate_source(old_version, new_src)
        puts "on_migrate_source; old_version :#{old_version}; new_src : #{new_src}"
        db = ::Rho::RHO.get_src_db(new_src['name'])
        if new_src['schema']
            db.delete_table(new_src['name'])
        end    
    end
    
    def set_menu(menu=nil,back_action=nil)
      @default_menu = {} if @default_menu.nil?
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
      controller_class = req['model']+'Controller'
      undercase = controller_class.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")

      if Rho::file_exist?(  req[:modelpath]+ undercase +'.iseq' )
        require req['model'] + '/' + undercase #req[:modelpath]+ undercase
      else
        require req['model'] + '/controller' #req[:modelpath]+'controller'
      end
      
      res['request-body'] = (Object.const_get(req['model']+'Controller').new).send :serve, self, @rhom, req, res
    end

  end # RhoApplication
end # Rho
