#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

require 'rhom'
require 'rhofsconnector'
require 'rholang/localization_simplified'
require 'rho/rhomsg'
require 'rho/rhotabbar'
require 'rho/rhotoolbar'

::System::set_locale(nil, nil)

module Rho
  class RhoApplication
  	attr_accessor :default_menu
  	
  	TOOLBAR_TYPE = 0
  	TABBAR_TYPE = 1
  	NOBAR_TYPE = 2
  	VTABBAR_TYPE = 3
  	
  	@@toolbar = [{:action => :back}, {:action => :forward}, {:action => :separator},
      {:action => :home}, {:action => :refresh}, {:action => :options} ]
    	
    def initialize
      #LocalizationSimplified.requre_loc(Rho::RhoFSConnector::get_app_path('app') + 'lang/lang_',true)
      
      unless @rhom
        @rhom = Rhom::Rhom.new
      end
      unless @default_menu
      	@default_menu = { Rho::RhoMessages.get_message('home_menu') => :home, Rho::RhoMessages.get_message('refresh_menu') => :refresh, 
      		Rho::RhoMessages.get_message('sync_menu') => :sync, Rho::RhoMessages.get_message('options_menu') => :options, Rho::RhoMessages.get_message('log_menu') => :log, :separator => nil, Rho::RhoMessages.get_message('close_menu') => :close }
  	  end

      if @vtabs != nil
        @@native_bar_data = {:type => :vtabbar, :data => @vtabs}
      elsif @tabs != nil
        @@native_bar_data = {:type => :tabbar, :data => @tabs}
      elsif @@toolbar != nil
        @@native_bar_data = {:type => :toolbar, :data => @@toolbar}
      else
        @@native_bar_data = nil #{:type => :nobar}
      end
	  
      #::Rho::RHO.get_instance().check_sources_migration(self)

      @initialized = true
      
      ::Rho::RHO.get_instance().set_app(::Rho::RHO::APPNAME, self)
    end

    def init_nativebar
      return unless @@native_bar_data
      
      if @@native_bar_data[:type] == :vtabbar
        tabs = @@native_bar_data[:data]
        # normalize the list
        tabs.map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
        puts "Initializing application with vertical tabs: #{tabs.inspect}"
        NativeTabbar.create_vertical(tabs)
        #NativeTabbar.create(tabs)
        NativeTabbar.switch_tab(0)
      elsif @@native_bar_data[:type] == :tabbar
        tabs = @@native_bar_data[:data]
        # normalize the list
        tabs.map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
        puts "Initializing application with tabs: #{tabs.inspect}"
        NativeTabbar.create(tabs)
        NativeTabbar.switch_tab(0)
      elsif @@native_bar_data[:type] == :toolbar
        NativeToolbar.create(@@native_bar_data[:data])
      #else
      #  NativeBar.create(NOBAR_TYPE, [])
      end
      
      @@native_bar_data = nil
    end

    def initialized?
      @initialized
    end

    def on_activate_app
    end

    def on_deactivate_app
    end

    def on_ui_created
        start_url = Rho::RhoConfig.start_path
        start_url = "" unless start_url
        
        security_token_not_passed = System.get_property('security_token_not_passed')
        security_token_not_passed = false if security_token_not_passed.nil?
        invalid_security_token_start_path_exist = Rho::RhoConfig.exists? 'invalid_security_token_start_path'
        invalid_security_token_start_path = Rho::RhoConfig.invalid_security_token_start_path
        
        if security_token_not_passed
            if invalid_security_token_start_path_exist
                start_url = invalid_security_token_start_path
            else
                # exit from application - old way
                puts 'security_token is not passed - application will closed'
                System.exit
            end
        end

        puts "on_ui_created.navigate to start url: '#{start_url}'"
        WebView.navigate(start_url)
    end

    def on_ui_destroyed
    end

    def on_sync_user_changed
        Rhom::Rhom.database_full_reset(false, false)    
        ::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET client_id=?", "")
    end
    
    def on_reinstall_config_update(conflicts)
        puts "on_reinstall_config_update: #{conflicts}"
    end

    # works for schema sources
    #return true to run script creating table    
    def on_migrate_source(old_version, new_src)
        puts "default on_migrate_source - do nothing; old_version :#{old_version}; new_src : #{new_src}"
        #if new_src['schema']
        #    db = ::Rho::RHO.get_src_db(new_src['name'])
        #    db.delete_table(new_src['name'])
            
        #    return false  #create new table
        #end
        
        return true
    end
    
    def set_menu(menu=nil,back_action=nil)
      @default_menu = {} if @default_menu.nil?
      disp_menu = menu ? menu.dup : @default_menu.dup
      disp_menu['Back'] = back_action if back_action
      #puts "RhoApplication: Using menu - #{disp_menu.inspect}"
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
      
      def get_blob_folder()
        Rho::RhoFSConnector::get_blob_folder()
      end

      def get_public_folder()
        Rho::RhoFSConnector::get_public_folder()
      end
      
      def get_blob_path(relative_path)
        Rho::RhoFSConnector::get_blob_path(relative_path)      
      end
      
    end

    def serve(req,res)
      req[:modelpath] = self.class.get_model_path req['application'], req['model']
      controller_class = req['model']+'Controller'
      undercase = controller_class.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
	  undercase.slice!(0) if undercase.start_with?('_')
	  #TODO: WP7 - for some reason it added _ at the start
	  	
      if Rho::file_exist?(  req[:modelpath]+ undercase +RHO_RB_EXT )
        require req['model'] + '/' + undercase #req[:modelpath]+ undercase
      else
        require req['model'] + '/controller' #req[:modelpath]+'controller'
      end
      
      res['request-body'] = (Object.const_get(req['model']+'Controller').new).send :serve, self, @rhom, req, res
    end

  end # RhoApplication
end # Rho
