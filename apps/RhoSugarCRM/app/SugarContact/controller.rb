require 'rho/rhocontroller'
require 'rho/rhocontact'
require 'helpers/application_helper'

class SugarContactController < Rho::RhoController
  
  include ApplicationHelper

  #GET /SugarContact
  def index
    # sort by name in ascending order
    @SugarContacts = SugarContact.find(:all, :order => 'last_name')    
    render
  end

  # GET /SugarContact/1
  def show
    @SugarContact = SugarContact.find(@params['id'])    
    render :action => :show
  end

  # GET /SugarContact/new
  def new
    @contactdefault = Hash.new
    @contactdefault['first_name'] = ""
    @contactdefault['last_name'] = ""
    @contactdefault['company_name'] = ""
    @contactdefault['phone_mobile'] = ""
    @contactdefault['phone_home'] = ""   
    @contactdefault['phone_work'] = ""
    @contactdefault['email1'] = ""
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
  
  def devicecontacts
    @DeviceContacts = Rho::RhoContact.find(:all).to_a.sort! {|x,y| x[1]['first_name'] <=> y[1]['first_name'] }    
    render :action => :devicecontacts
  end
  
  def newfromdevice
    @contactdefault = Hash.new
    @contact = Rho::RhoContact.find(@params['id'])
    @contactdefault['first_name'] = @contact['first_name']
    @contactdefault['last_name'] = @contact['last_name']
    @contactdefault['company_name'] = @contact['company_name']
    @contactdefault['phone_mobile'] = @contact['mobile_number']
    @contactdefault['phone_home'] = @contact['home_number']    
    @contactdefault['phone_work'] = @contact['business_number']
    @contactdefault['email1'] = @contact['email_address']
    
    render :action => :new    
  end
  
  def savetodevice
    @SugarContact = SugarContact.find(@params['id'])
    @contact = Hash.new

    @contact['first_name'] = @SugarContact.first_name.nil? ? "" : @SugarContact.first_name
    @contact['last_name'] = @SugarContact.last_name.nil? ? "" : @SugarContact.last_name
    @contact['company_name'] = @SugarContact.company_name.nil? ? "" : @SugarContact.company_name
    @contact['mobile_number'] = @SugarContact.phone_mobile.nil? ? "" : @SugarContact.phone_mobile
    @contact['home_number'] = @SugarContact.phone_home.nil? ? "" : @SugarContact.phone_home
    @contact['business_number'] = @SugarContact.phone_work.nil? ? "" : @SugarContact.phone_work
    @contact['email_address'] = @SugarContact.email1.nil? ? "" : @SugarContact.email1      
    @DeviceContact = Rho::RhoContact.create!(@contact)
    
    redirect :action => :index    
  end
  
  private
  
  def contactexists?(first_name, last_name, company_name, phone_mobile, phone_home, phone_work, email1)
    @DeviceContacts = Rho::RhoContact.find(:all).to_a.sort! {|x,y| x[1]['first_name'] <=> y[1]['first_name'] }    

    found = false
    @DeviceContacts.each do |contact|
      found |= (!contact.first_name.nil? && contact.first_name == first_name && 
      !contact.last_name.nil? && contact.last_name == last_name && 
      !contact.company_name.nil? && contact.company_name == company_name)
    end
    found
  end
  
end
