require 'rho/settings_controller'

class SourceController < Rho::SettingsController
  def index
    @sources = self.get_all_sources
    render :action => :index
  end
  
  def show
    @source = self.get_source(@params['id'])
    render :action => :show
  end
  
  def edit
    @source = self.get_source(@params['id'])
    render :action => :edit
  end
  
  def update
    self.update_source(@params['source'], @params['id'])
    redirect :action => :index
  end
end