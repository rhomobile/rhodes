require 'rho/rhocontroller'

class SugarCampaignController < Rho::RhoController

  #GET /SugarCampaign
  def index
    @SugarCampaigns = SugarCampaign.find(:all)
    
    # sort by name in ascending order
    if (@SugarCampaigns.length > 0)
      @SugarCampaigns = @SugarCampaigns.sort_by {|item| !item.name.nil? ? item.name : ""}
    end
    
    render
  end

  # GET /SugarCampaign/1
  def show
    @SugarCampaign = SugarCampaign.find(@params['id'])
    render :action => :show
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
    redirect :action => :index
  end

  # POST /SugarCampaign/1/update
  def update
    @SugarCampaign = SugarCampaign.find(@params['id'])
    @SugarCampaign.update_attributes(@params['SugarCampaign'])
    redirect :action => :index
  end

  # POST /SugarCampaign/1/delete
  def delete
    @SugarCampaign = SugarCampaign.find(@params['id'])
    @SugarCampaign.destroy
    redirect :action => :index
  end
end
