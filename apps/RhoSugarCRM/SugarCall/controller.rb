require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','RhoSugarCRM','helpers/application_helper')

class SugarCallController < Rho::RhoController

  #GET /SugarCall
  def index
    @SugarCalls = SugarCall.find(:all)
    
    # sort by name in ascending order
    if (@SugarCalls.length > 0)
      @SugarCalls = @SugarCalls.sort_by {|item| !item.name.nil? ? item.name : ""}
    end
    
    render
  end

  # GET /SugarCall/1
  def show
    @SugarCall = SugarCall.find(@params['id'])
    render :action => :show
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
    redirect :action => :index
  end

  # POST /SugarCall/1/update
  def update
    @SugarCall = SugarCall.find(@params['id'])
    @SugarCall.update_attributes(@params['SugarCall'])
    redirect :action => :index
  end

  # POST /SugarCall/1/delete
  def delete
    @SugarCall = SugarCall.find(@params['id'])
    @SugarCall.destroy
    redirect :action => :index
  end
end
