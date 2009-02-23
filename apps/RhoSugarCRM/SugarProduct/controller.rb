require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','RhoSugarCRM','helpers/application_helper')

class SugarProductController < Rho::RhoController

  include ApplicationHelper

  #GET /SugarProduct
  def index
    # sort by name in ascending order
    @SugarProducts = SugarProduct.find(:all, :order => 'name')    
    render
  end

  # GET /SugarProduct/1
  def show
    @SugarProduct = SugarProduct.find(@params['id'])
    render :action => :show
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
    redirect :action => :index
  end

  # POST /SugarProduct/1/update
  def update
    @SugarProduct = SugarProduct.find(@params['id'])
    @SugarProduct.update_attributes(@params['SugarProduct'])
    redirect :action => :index
  end

  # POST /SugarProduct/1/delete
  def delete
    @SugarProduct = SugarProduct.find(@params['id'])
    @SugarProduct.destroy
    redirect :action => :index
  end
end
