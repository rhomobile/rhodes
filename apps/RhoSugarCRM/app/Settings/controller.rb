require 'rho'
require 'rho/rhocontroller'

class SettingsController < Rho::RhoController
  
  def index
    puts 'inside index'
    @msg = @params['msg']
    render
  end

  def login
    render :action => :login
  end

  def do_login
    if @params['login'] and @params['password']
      success = SyncEngine::login(@params['login'], @params['password'])
    else
      @msg = "You entered an invalid login/password, please try again."
      render :action => :login
    end
    if success > 0
      # run sync if we were successful
      SyncEngine::dosync
      redirect Rho::RhoConfig.start_path
    else
      @msg = "You entered an invalid login/password, please try again."
      render :action => :login
    end
  end
  
  def logout
    SyncEngine::logout
    @msg = "You have been logged out."
    render :action => :login
  end
  
  def reset
    render :action => :reset
  end
  
  def do_reset
    SyncEngine::trigger_sync_db_reset
    @msg = "Database will be deleted on next sync."
    redirect Rho::RhoConfig.start_path
  end
  
  def do_sync
    puts 'inside do_sync'
    SyncEngine::dosync
    @msg =  "Sync has been triggered."
    redirect :action => :index, :query => {:msg => @msg}
  end
end
