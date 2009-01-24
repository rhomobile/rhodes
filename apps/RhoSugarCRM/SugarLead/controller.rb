require 'rho/rhocontroller'

class SugarLeadController < Rho::RhoController

  #GET /SugarLead
  def index
    @SugarLeads = SugarLead.find(:all)
    render
  end

  # GET /SugarLead/1
  def show
    @SugarLeads = SugarLead.find(@params['object'])
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
    redirect :index
  end

  # POST /SugarLead/1/update
  def update
    @SugarLead = SugarLead.find(@params['id'])
    @SugarLead.update_attributes(@params['SugarLead'])
    redirect :index
  end

  # POST /SugarLead/1/delete
  def delete
    @SugarLead = SugarLead.find(@params['id'])
    @SugarLead.destroy
    redirect :index
  end
end
