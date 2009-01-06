require 'rho/settings_controller'

class SourceController < Rho::SettingsController
  def index
    @sources = self.get_all_sources
    render :index
  end
  
  def edit
    @source = self.get_source(@params['id'])
    render :edit
  end
  
  def update
    self.update_source(@params['source'], @params['id'])
    redirect :index
  end
end