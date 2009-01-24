require 'rho/rhocontroller'

class SugarCallController < Rho::RhoController

  #GET /SugarCall
  def index
    @SugarCalls = SugarCall.find(:all)
    render
  end

  # GET /SugarCall/1
  def show
    @SugarCalls = SugarCall.find(@params['object'])
  end

  # GET /SugarCall/new
  def new
    @SugarCall = SugarCall.new
    render :action => :new
  end

  # GET /SugarCall/1/edit
  def edit
    @SugarCall = SugarCall.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarCall/create
  def create
    @SugarCall = SugarCall.new(@params['SugarCall'])
    @SugarCall.save
    redirect :index
  end

  # POST /SugarCall/1/update
  def update
    @SugarCall = SugarCall.find(@params['id'])
    @SugarCall.update_attributes(@params['SugarCall'])
    redirect :index
  end

  # POST /SugarCall/1/delete
  def delete
    @SugarCall = SugarCall.find(@params['id'])
    @SugarCall.destroy
    redirect :index
  end
end
