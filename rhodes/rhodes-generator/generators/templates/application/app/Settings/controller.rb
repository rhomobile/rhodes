require 'rho'
require 'rho/rhocontroller'
require 'rho/rhoerror'

class SettingsController < Rho::RhoController
  
  def index
    @msg = @params['msg']
    render
  end

  def login
    @msg = @params['msg']
    render :action => :login
  end

  def login_callback
    errCode = @params['error_code'].to_i
    if errCode == 0
      # run sync if we were successful
      WebView.navigate Rho::RhoConfig.start_path
      SyncEngine::dosync
    else
      @msg = @params['error_message']
      if @msg == nil or @msg.length == 0 
        @msg = Rho::RhoError.new(errCode).message
      end
      WebView.navigate ( url_for :action => :login, :query => {:msg => @msg} )
    end  
  end

  def do_login
    if @params['login'] and @params['password']
      begin
        SyncEngine::login(@params['login'], @params['password'], (url_for :action => :login_callback) )
        render :action => :wait
      rescue RhoError => e
          @msg = e.message
          render :action => :login, :query => {:msg => @msg}
      end
    else
        @msg = "You entered an invalid login/password, please try again." unless @msg.length    
        render :action => :login, :query => {:msg => @msg}
    end
  end
  
  def logout
    SyncEngine.logout
    @msg = "You have been logged out."
    render :action => :login, :query => {:msg => @msg}
  end
  
  def reset
    render :action => :reset
  end
  
  def do_reset
    SyncEngine.database_full_reset
    @msg = "Database has been reset."
    redirect :action => :index, :query => {:msg => @msg}
  end
  
  def do_sync
    SyncEngine.dosync
    @msg =  "Sync has been triggered."
    redirect :action => :index, :query => {:msg => @msg}
  end
end
