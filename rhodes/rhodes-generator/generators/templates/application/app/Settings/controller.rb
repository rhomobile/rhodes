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
    if @params['login'] and @params['password']
      success = SyncEngine::login(@params['login'], @params['password'])
    else
      @msg = "You entered an invalid login/password, please try again."
      render :action => :login, :query => {:msg => @msg}
    end
    if success > 0
      # run sync if we were successful
      SyncEngine::dosync
      redirect Rho::RhoConfig.start_path
    else
      @msg = "You entered an invalid login/password, please try again."
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
