require 'rho/rhocontroller'
require 'helpers/application_helper'

class SugarCaseController < Rho::RhoController

  include ApplicationHelper

  #GET /SugarCase
  def index
    # sort by name in ascending order
    @SugarCases = SugarCase.find(:all, :order => 'name')    
    render
  end

  # GET /SugarCase/1
  def show
    @SugarCase = SugarCase.find(@params['id'])
    render :action => :show
  end

  # GET /SugarCase/new
  def new
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
    redirect :action => :index
  end

  # POST /SugarCase/1/update
  def update
    @SugarCase = SugarCase.find(@params['id'])
    @SugarCase.update_attributes(@params['SugarCase'])
    redirect :action => :index
  end

  # POST /SugarCase/1/delete
  def delete
    @SugarCase = SugarCase.find(@params['id'])
    @SugarCase.destroy
    redirect :action => :index
  end
end
