require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','RhoSugarCRM','helpers/application_helper')

class SugarAccountController < Rho::RhoController
  
  include ApplicationHelper

  #GET /SugarAccount
  def index
    # sort by name in ascending order
    @SugarAccounts = SugarAccount.find(:all, :order => 'name')
    render
  end

  # GET /SugarAccount/1
  def show
    @SugarAccount = SugarAccount.find(@params['id'])
    render :action => :show
  end

  # GET /SugarAccount/new
  def new
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
    redirect :action => :index
  end

  # POST /SugarAccount/1/update
  def update
    @SugarAccount = SugarAccount.find(@params['id'])
    @SugarAccount.update_attributes(@params['SugarAccount'])
    redirect :action => :index
  end

  # POST /SugarAccount/1/delete
  def delete
    @SugarAccount = SugarAccount.find(@params['id'])
    @SugarAccount.destroy
    redirect :action => :index
  end
end
