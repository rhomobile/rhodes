require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','Lighthouse','helpers/application_helper')

class SugarContactController < Rho::RhoController
  
  include ApplicationHelper

  #GET /SugarContact
  def index
    @SugarContacts = SugarContact.find(:all)
    render
  end

  # GET /SugarContact/1
  def show
    @SugarContact = SugarContact.find(@params['id'])
    puts @SugarContact.inspect
    
    puts "lead source = #{@SugarContact.lead_source}"
    puts "lead source.to_s = #{@SugarContact.lead_source.to_s}"
    puts "lead source.length = #{@SugarContact.lead_source.length}"
    puts "lead inspect = #{@SugarContact.lead_source.inspect}"
    puts "lead class = #{@SugarContact.lead_source.class}"
    
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
