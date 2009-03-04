require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','RhoSugarCRM','helpers/application_helper')

class SugarCallController < Rho::RhoController

  include ApplicationHelper

  #GET /SugarCall
  def index
    # sort by name in ascending order
    @SugarCalls = SugarCall.find(:all, :order => 'name')  
    render
  end

  # GET /SugarCall/1
  def show
    @SugarCall = SugarCall.find(@params['id'])
    render :action => :show
  end

  # GET /SugarCall/new
  def new
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
