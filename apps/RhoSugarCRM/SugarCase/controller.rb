require 'rho/rhocontroller'

class SugarCaseController < Rho::RhoController

  #GET /SugarCase
  def index
    @SugarCases = SugarCase.find(:all)
    render
  end

  # GET /SugarCase/1
  def show
    @SugarCases = SugarCase.find(@params['object'])
  end

  # GET /SugarCase/new
  def new
    @SugarCase = SugarCase.new
    render :action => :new
  end

  # GET /SugarCase/1/edit
  def edit
    @SugarCase = SugarCase.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarCase/create
  def create
    @SugarCase = SugarCase.new(@params['SugarCase'])
    @SugarCase.save
    redirect :index
  end

  # POST /SugarCase/1/update
  def update
    @SugarCase = SugarCase.find(@params['id'])
    @SugarCase.update_attributes(@params['SugarCase'])
    redirect :index
  end

  # POST /SugarCase/1/delete
  def delete
    @SugarCase = SugarCase.find(@params['id'])
    @SugarCase.destroy
    redirect :index
  end
end
