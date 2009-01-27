require 'rho/rhocontroller'

class SugarAccountController < Rho::RhoController

  #GET /SugarAccount
  def index
    @SugarAccounts = SugarAccount.find(:all)
    render
  end

  # GET /SugarAccount/1
  def show
    @SugarAccount = SugarAccount.find(@params['id'])
    render :action => :show
  end

  # GET /SugarAccount/new
  def new
    @SugarAccount = SugarAccount.new
    render :action => :new
  end

  # GET /SugarAccount/1/edit
  def edit
    @SugarAccount = SugarAccount.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarAccount/create
  def create
    @SugarAccount = SugarAccount.new(@params['SugarAccount'])
    @SugarAccount.save
    redirect :index
  end

  # POST /SugarAccount/1/update
  def update
    @SugarAccount = SugarAccount.find(@params['id'])
    @SugarAccount.update_attributes(@params['SugarAccount'])
    redirect :index
  end

  # POST /SugarAccount/1/delete
  def delete
    @SugarAccount = SugarAccount.find(@params['id'])
    @SugarAccount.destroy
    redirect :index
  end
end
