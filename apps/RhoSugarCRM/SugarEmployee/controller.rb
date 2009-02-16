require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','RhoSugarCRM','helpers/application_helper')

class SugarEmployeeController < Rho::RhoController

  include ApplicationHelper
  
  #GET /SugarEmployee
  def index
    @SugarEmployees = SugarEmployee.find(:all)
    
    # sort by name in ascending order
		if System::get_property('platform') != 'Blackberry'    
      if (@SugarEmployees.length > 0)
        @SugarEmployees = @SugarEmployees.sort_by {|item| !item.last_name.nil? ? item.last_name : ""}
      end
    end
    
    render
  end

  # GET /SugarEmployee/1
  def show
    @SugarEmployee = SugarEmployee.find(@params['id'])
    render :action => :show
  end

  # GET /SugarEmployee/new
  def new
    @SugarEmployee = SugarEmployee.new
    render :action => :new
  end

  # GET /SugarEmployee/1/edit
  def edit
    @SugarEmployee = SugarEmployee.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarEmployee/create
  def create
    @SugarEmployee = SugarEmployee.new(@params['SugarEmployee'])
    @SugarEmployee.save
    redirect :action => :index
  end

  # POST /SugarEmployee/1/update
  def update
    @SugarEmployee = SugarEmployee.find(@params['id'])
    @SugarEmployee.update_attributes(@params['SugarEmployee'])
    redirect :action => :index
  end

  # POST /SugarEmployee/1/delete
  def delete
    @SugarEmployee = SugarEmployee.find(@params['id'])
    @SugarEmployee.destroy
    redirect :action => :index
  end
end
