require 'rho/rhocontroller'

class MappingController < Rho::RhoController

  def index
    render
  end
  
  def showmypos
    render :action => :showmypos, :layout => false
  end
  
  def showcontactpos
    render :action => :showcontactpos, :layout => false
  end
  
end