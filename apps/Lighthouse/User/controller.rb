require 'rho/rhocontroller'

class UserController < Rho::RhoController
  
  #GET /User
  def index
    @users = User.find(:all)
    render :index
  end

  # GET /User/1
  def show
    @user = User.find(@params['id'])
    render :show
  end
end
