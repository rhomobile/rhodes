require 'rho/rhocontroller'

class SugarProductController < Rho::RhoController

  #GET /SugarProduct
  def index
    @SugarProducts = SugarProduct.find(:all)
    render
  end

  # GET /SugarProduct/1
  def show
    @SugarProducts = SugarProduct.find(@params['object'])
  end

  # GET /SugarProduct/new
  def new
    @SugarProduct = SugarProduct.new
    render :action => :new
  end

  # GET /SugarProduct/1/edit
  def edit
    @SugarProduct = SugarProduct.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarProduct/create
  def create
    @SugarProduct = SugarProduct.new(@params['SugarProduct'])
    @SugarProduct.save
    redirect :index
  end

  # POST /SugarProduct/1/update
  def update
    @SugarProduct = SugarProduct.find(@params['id'])
    @SugarProduct.update_attributes(@params['SugarProduct'])
    redirect :index
  end

  # POST /SugarProduct/1/delete
  def delete
    @SugarProduct = SugarProduct.find(@params['id'])
    @SugarProduct.destroy
    redirect :index
  end
end
