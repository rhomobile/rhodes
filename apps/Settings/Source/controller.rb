require 'rho/rhocontroller'
require 'rhom/rhom_source'

class SourceController < Rho::RhoController
  layout :source_layout
  include Rhom
  def index
    @sources = RhomSource.find(:all)
    render :action => :index, :layout => :source_layout
  end
  
  def edit
    @source = RhomSource.find(@params['id'])
    render :action => :edit, :layout => :source_layout
  end
  
  def update
    @source = RhoSource.find(@params['id'])
    @source.update_attributes
    redirect :action => :index, :layout => :source_layout
  end
end