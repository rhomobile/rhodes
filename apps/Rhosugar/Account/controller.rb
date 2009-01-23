require 'rho/rhocontroller'

class AccountController < Rho::RhoController
  layout :account_layout

  # GET /cases
  def index
    @accounts = Account.find(:all)
    render
  end

  # GET /cases/1
  def show
    @accounts = Account.find(@params['object'])
  end

  # GET /cases/new
  def new
    @account = Account.new
    @industries=["Electronics","Finance","Technology"]
    render :action => :new
  end

  # GET /cases/1/edit
  def edit
    @account = Account.find(@params['id'])
    @industries=["Electronics","Finance","Technology"]
    render :action => :edit
  end

  # POST /cases
  def create
    @account = Account.new(@params['account'])
    @account.save
    redirect :index
  end

  # POST /cases/1
  def update
    @account = Account.find(@params['id'])
    @account.update_attributes(@params['account'])
    redirect :index
  end

  # POST /cases/1/delete
  def delete
    @account = Account.find(@params['id'])
    @account.destroy
    redirect :index
  end
end
