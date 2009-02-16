require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','Lighthouse','helpers/application_helper')

class SugarCaseController < Rho::RhoController

  include ApplicationHelper

  #GET /SugarCase
  def index
    @SugarCases = SugarCase.find(:all)
    
    # sort by name in ascending order
    if (@SugarCases.length > 0)
      @SugarCases = @SugarCases.sort_by {|item| !item.name.nil? ? item.name : ""}
    end
    
    render
  end

  # GET /SugarCase/1
  def show
    @SugarCase = SugarCase.find(@params['id'])
    render :action => :show
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
