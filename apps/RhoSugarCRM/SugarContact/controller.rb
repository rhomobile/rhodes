require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','Lighthouse','helpers/application_helper')

class SugarContactController < Rho::RhoController
  
  include ApplicationHelper

  #GET /SugarContact
  def index
    @SugarContacts = SugarContact.find(:all)
    
    # sort by name in ascending order
		if System::get_property('platform') != 'Blackberry'    
      if (@SugarContacts.length > 0)
        @SugarContacts = @SugarContacts.sort_by {|item| !item.last_name.nil? ? item.last_name : ""}
      end
    end
    
    render
  end

  # GET /SugarContact/1
  def show
    @SugarContact = SugarContact.find(@params['id'])    
    render :action => :show
  end

  # GET /SugarContact/new
  def new
    @SugarContact = SugarContact.new
    render :action => :new
  end

  # GET /SugarContact/1/edit
  def edit
    @SugarContact = SugarContact.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarContact/create
  def create
    @SugarContact = SugarContact.new(@params['SugarContact'])
    @SugarContact.save
    redirect :action => :index
  end

  # POST /SugarContact/1/update
  def update
    @SugarContact = SugarContact.find(@params['id'])
    @SugarContact.update_attributes(@params['SugarContact'])
    redirect :action => :index
  end

  # POST /SugarContact/1/delete
  def delete
    @SugarContact = SugarContact.find(@params['id'])
    @SugarContact.destroy
    redirect :action => :index
  end
end
