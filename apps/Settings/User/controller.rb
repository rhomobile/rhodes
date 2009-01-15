require 'rho'
require 'rho/rhocontroller'

class UserController < Rho::RhoController

  # GET /User/edit
  def login
    render :login
  end

  # POST /User/login
  def do_login
    if @params['login'] and @params['password']
      success = SyncEngine::login(@params['login'], @params['password'])
    else
      @msg = "You entered an invalid login/password, please try again."
      render :login
    end
    if success > 0
      # run sync if we were successful
      SyncEngine::dosync
      redirect Rho::RhoConfig.start_path
    else
      @msg = "You entered an invalid login/password, please try again."
      render :login
    end
  end
  
  def logout
    SyncEngine::logout
    @msg = "You have been logged out."
    render :login
  end
end
