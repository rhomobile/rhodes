require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','Lighthouse','helpers/application_helper')

class SugarLeadController < Rho::RhoController
  
  include ApplicationHelper

  #GET /SugarLead
  def index
    # sort by name in ascending order and only if all items are non-nil to prevent sort error
    @SugarLeads = SugarLead.find(:all, :order => 'last_name')  
    render
  end

  # GET /SugarLead/1
  def show
    @SugarLead = SugarLead.find(@params['id'])
    render :action => :show
  end

  # GET /SugarLead/new
  def new
    @SugarLead = SugarLead.new
    render :action => :new
  end

  # GET /SugarLead/1/edit
  def edit
    @SugarLead = SugarLead.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarLead/create
  def create
    @SugarLead = SugarLead.new(@params['SugarLead'])
    @SugarLead.save
    redirect :action => :index
  end

  # POST /SugarLead/1/update
  def update
    @SugarLead = SugarLead.find(@params['id'])
    @SugarLead.update_attributes(@params['SugarLead'])
    redirect :action => :index
  end

  # POST /SugarLead/1/delete
  def delete
    @SugarLead = SugarLead.find(@params['id'])
    @SugarLead.destroy
    redirect :action => :index
  end
end
