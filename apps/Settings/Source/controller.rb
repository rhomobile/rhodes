require 'rho/rhocontroller'
require 'rhom/rhom_source'

class SourceController < Rho::RhoController
  include Rhom

  def index
    @sources = RhomSource.find(:all)
    render :action => :index
  end
  
  def edit
    @source = RhomSource.find(@params['id'])
    render :action => :edit
  end
  
  def do_sync
    SyncEngine::dosync
    redirect Rho::RhoConfig.start_path
  end
  
  def update
    @source = RhomSource.find(@params['source']['source_id'])
    @source.update_attributes(@params['source'])
    redirect :action => :index
  end
end