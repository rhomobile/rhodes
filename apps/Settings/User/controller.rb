require 'rho/rhocontroller'

class UserController < Rho::RhoController

  # GET /User/edit
  def edit
    render :edit
  end

  # POST /User/login
  def login
    if @params['login'] and @params['password']
      success = SyncEngine::login(@params['login'], @params['password'])
    else
      @error = "You entered an invalid login/password, please try again."
      render :edit
    end
    if success
      redirect '/'
    else
      @error = "You entered an invalid login/password, please try again."
      render :edit
    end
  end
end
