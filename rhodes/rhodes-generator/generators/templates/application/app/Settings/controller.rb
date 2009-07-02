require 'rho'
require 'rho/rhocontroller'

class SettingsController < Rho::RhoController
  
  def index
    @msg = @params['msg']
    render
  end

  def login
    @msg = @params['msg']
    render :action => :login
  end

  def do_login
    success = 0
    @msg = ""
    if @params['login'] and @params['password']
      begin
        success = SyncEngine::login(@params['login'], @params['password'])
      rescue RhoError => e
          @msg = e.message
      end  
    end
    
    if success > 0
      # run sync if we were successful
      SyncEngine::dosync
      redirect Rho::RhoConfig.start_path
    else
      @msg = "You entered an invalid login/password, please try again." unless @msg.length
      render :action => :login, :query => {:msg => @msg}
    end
  end
  
  def logout
    SyncEngine::logout
    @msg = "You have been logged out."
    render :action => :login, :query => {:msg => @msg}
  end
  
  def reset
    render :action => :reset
  end
  
  def do_reset
    SyncEngine::trigger_sync_db_reset
    SyncEngine::dosync
    @msg = "Database has been reset."
    redirect :action => :index, :query => {:msg => @msg}
  end
  
  def do_sync
    SyncEngine::dosync
    @msg =  "Sync has been triggered."
    redirect :action => :index, :query => {:msg => @msg}
  end
end
