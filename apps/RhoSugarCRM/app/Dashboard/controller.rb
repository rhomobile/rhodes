require 'rho/rhocontroller'

class DashboardController < Rho::RhoController

  def index
    render :action => :index
  end
end