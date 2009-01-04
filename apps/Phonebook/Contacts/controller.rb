require 'rho/rhocontroller'
require 'rho/rhocontact'

class ContactsController < Rho::RhoController

  # GET /Contacts
  def index
    @contacts = Rho::RhoContact.find(:all)
    render :index
  end

  # GET /Contacts/1
  def show
    @contact = Rho::RhoContact.find(@params['id'])
  end

  # GET /Contacts/new
  def new
    render :new
  end

  # GET /Contacts/1/edit
  def edit
    @contact = Rho::RhoContact.find(@params['id'])
    render :edit
  end

  # POST /Contacts
  def create
    @contact = Rho::RhoContact.create!(@params['contact'])
    redirect :index
  end

  # POST /Contacts/1
  def update
    Rho::RhoContact.update_attributes(@params['contact'])
    redirect :index
  end

  # POST /Contacts/1/delete
  def delete
    Rho::RhoContact.destroy(@params['id'])
    redirect :index
  end

end
