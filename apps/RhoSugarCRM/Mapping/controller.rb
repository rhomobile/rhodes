require 'rho/rhocontroller'
require 'helpers/application_helper'

class MappingController < Rho::RhoController

  include ApplicationHelper

  def index
    render
  end
  
  def showmypos
    @show_addresses = false
    @zoom_on_last_contact = false
    render :action => :showmypos, :layout => false
  end
  
  def showcontact
    @show_addresses = true
    @zoom_on_last_contact = true
    @contact = SugarContact.find(@params['id'])
    @contacts = [@contact]
    render :action => :showmypos, :layout => false
  end
  
  def showallcontacts
    @show_addresses = true
    @zoom_on_last_contact = false
    @contacts = SugarContact.find(:all, :order => 'last_name')        
    render :action => :showmypos, :layout => false    
  end
  
end