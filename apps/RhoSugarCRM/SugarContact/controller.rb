require 'rho/rhocontroller'

class SugarContactController < Rho::RhoController

  #GET /SugarContact
  def index
    @SugarContacts = SugarContact.find(:all)
    render
  end

  # GET /SugarContact/1
  def show
    @SugarContacts = SugarContact.find(@params['object'])
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
    redirect :index
  end

  # POST /SugarContact/1/update
  def update
    @SugarContact = SugarContact.find(@params['id'])
    @SugarContact.update_attributes(@params['SugarContact'])
    redirect :index
  end

  # POST /SugarContact/1/delete
  def delete
    @SugarContact = SugarContact.find(@params['id'])
    @SugarContact.destroy
    redirect :index
  end
end
