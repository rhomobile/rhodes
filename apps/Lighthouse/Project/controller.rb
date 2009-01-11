require 'rho/rhocontroller'

class ProjectController < Rho::RhoController
  
  #GET /Project
  def index
    @projects = Project.find(:all)
    render :index
  end

  # GET /Project/1
  def show
    @project = Project.find(@params['id'])
    render :show
  end
end
