require 'rho/rhocontroller'

class SugarCampaignController < Rho::RhoController

  #GET /SugarCampaign
  def index
    @SugarCampaigns = SugarCampaign.find(:all)
    render
  end

  # GET /SugarCampaign/1
  def show
    @SugarCampaigns = SugarCampaign.find(@params['object'])
  end

  # GET /SugarCampaign/new
  def new
    @SugarCampaign = SugarCampaign.new
    render :action => :new
  end

  # GET /SugarCampaign/1/edit
  def edit
    @SugarCampaign = SugarCampaign.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarCampaign/create
  def create
    @SugarCampaign = SugarCampaign.new(@params['SugarCampaign'])
    @SugarCampaign.save
    redirect :index
  end

  # POST /SugarCampaign/1/update
  def update
    @SugarCampaign = SugarCampaign.find(@params['id'])
    @SugarCampaign.update_attributes(@params['SugarCampaign'])
    redirect :index
  end

  # POST /SugarCampaign/1/delete
  def delete
    @SugarCampaign = SugarCampaign.find(@params['id'])
    @SugarCampaign.destroy
    redirect :index
  end
end
