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

#require 'rhom'
#require 'rhofsconnector'
require 'rholang/localization_simplified'
require 'rho/rhomsg'
require 'rho/rhotabbar'
require 'rho/rhotoolbar'

::System::set_locale(nil, nil)

module Rho
  class Application
  	attr_accessor :default_menu
  	
  	TOOLBAR_TYPE = 0
  	TABBAR_TYPE = 1
  	NOBAR_TYPE = 2
  	VTABBAR_TYPE = 3
  	
  	@@toolbar = [{:action => :back}, {:action => :forward}, {:action => :separator},
      {:action => :home}, {:action => :refresh}, {:action => :options} ]
    
    #def self.make_default_native_menu
    #    { Rho::RhoMessages.get_message('home_menu') => :home, Rho::RhoMessages.get_message('refresh_menu') => :refresh, 
    #  		Rho::RhoMessages.get_message('sync_menu') => :sync, Rho::RhoMessages.get_message('options_menu') => :options, Rho::RhoMessages.get_message('log_menu') => :log, :separator => nil, Rho::RhoMessages.get_message('close_menu') => :close }
    #end
    
    def self.make_default_native_menu
        NativeMenubar_defaultMainMenu()
    end
    
    def self.NativeMenubar_defaultMainMenu
        [ {:label=>Rho::RhoMessages.get_message('home_menu'), :action=>:home}, 
          { :label =>Rho::RhoMessages.get_message('refresh_menu'), :action => :refresh }, 
      	  { :label => Rho::RhoMessages.get_message('sync_menu'), :action => :sync}, 
      	  { :label =>Rho::RhoMessages.get_message('options_menu'), :action => :options},
      	  { :label=>Rho::RhoMessages.get_message('log_menu'), :action => :log}, 
      	  { :label=>:separator, :action => nil}, 
      	  { :label => Rho::RhoMessages.get_message('close_menu'), :action => :close }
      	]
    end
        	
    def initialize
      #LocalizationSimplified.requre_loc(Rho::RhoFSConnector::get_app_path('app') + 'lang/lang_',true)
      
      unless @rhom
        @rhom = Rhom::Rhom.get_instance
      end
      unless @default_menu
      	@default_menu = Rho::Application::make_default_native_menu()
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
        if tabs.kind_of? Array
             tabs.map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
        else
             if tabs.kind_of? Hash
                 tabs[:tabs].map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
             end
        end
        puts "Initializing application with vertical tabs: #{tabs.inspect}"
        NativeTabbar.create_vertical(tabs)
        #NativeTabbar.create(tabs)
        NativeTabbar.switch_tab(0)
      elsif @@native_bar_data[:type] == :tabbar
        tabs = @@native_bar_data[:data]
        # normalize the list
        if tabs.kind_of? Array
             tabs.map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
        else
             if tabs.kind_of? Hash
                  tabs[:tabs].map! { |tab| tab[:refresh] = false unless tab[:refresh]; tab }
             end
        end
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
      Rho::Application.processApplicationEvent({'applicationEvent'=>'Activated'})
    end

    def on_deactivate_app
      Rho::Application.processApplicationEvent({'applicationEvent'=>'Deactivated'})
    end

    def on_ui_created
      Rho::Application.processApplicationEvent({'applicationEvent'=>'UICreated'})
    end

    def on_ui_close_request
      Rho::Application.quit()
    end

    def on_ui_destroyed
      Rho::Application.processApplicationEvent({'applicationEvent'=>'UIDestroyed'})
    end

    def on_sync_user_changed
      Rho::Application.processApplicationEvent({'applicationEvent'=>'SyncUserChanged'})
    end

    def on_reinstall_config_update(conflicts)
      Rho::Application.processApplicationEvent({'applicationEvent'=>'ConfigConflict','eventData'=>{ 'conflicts' => conflicts}})
    end

    # works for schema sources
    #return true to run script creating table    
    def on_migrate_source(old_version, new_src)
      Rho::Application.processApplicationEvent({'applicationEvent'=>'DBMigrateSource','eventData'=>{ 'old_version' => old_version, 'new_src' =>  new_src}})
    end

    def set_menu(menu=nil,back_action=nil)
      @default_menu = {} if @default_menu.nil?
      disp_menu = menu ? menu.dup : @default_menu.dup
      if back_action
        if disp_menu.is_a?(Array)
            disp_menu << {:label => 'Back', :action => back_action }
        else
           disp_menu['Back'] = back_action
        end

      end
      #puts "RhoApplication: Using menu - #{disp_menu.inspect}"
  	  WebView.set_menu_items(disp_menu)
    end

    @@current_controller = nil
    def self.current_controller()
        @@current_controller
    end
    def self.set_current_controller(ctrl)
        @@current_controller = ctrl
    end

    def serve(req,res)
      req[:modelpath] = self.class.get_model_path req['application'], req['model']
      controller_class = req['model']+'Controller'
      undercase = controller_class.split(/(?=[A-Z])/).map{|w| w.downcase}.join("_")
	  undercase.slice!(0) if undercase.start_with?('_')
	  #TODO: WP7 - for some reason it added _ at the start

      is_found = false

      if (Rho::file_exist?(  req[:modelpath]+ undercase +RHO_RB_EXT ) || Rho::file_exist?(  req[:modelpath]+ undercase +RHO_RB_EXT + RHO_ENCRYPTED_EXT))
        require req['model'] + '/' + undercase #req[:modelpath]+ undercase

        is_found = true
      elsif Rho::System.isRhoSimulator
        begin
            require req['model'] + '/' + undercase #req[:modelpath]+ undercase
            is_found = true
        rescue Exception => exc
        end

      end

      unless is_found
        require req['model'] + '/controller' #req[:modelpath]+'controller'
      end

      @@current_controller = (Object.const_get(req['model']+'Controller').new)
      res['request-body'] = @@current_controller.send :serve, self, @rhom, req, res
    end

  end # Application
end # Rho

module Rho
    RhoApplication = Rho::Application
end
