require 'rho/rhocontroller'
require File.join(File.dirname(__FILE__), '../application')
require File.join(File.dirname(__FILE__), '../helpers/application_helper')

class TicketController < Rho::RhoController
  
  include ApplicationHelper
  
  #GET /Ticket
  def index
    @title = "All tickets"	
    @tickets = Ticket.find(:all)
    render :index
  end

  #GET /Ticket/today
  def today
    @title = "Todays tickets"
    @tickets = Ticket.find(:all)
    today = DateTime.now
    @tickets = @tickets.reject do |ticket|
      ttime = DateTime.parse(ticket.created_at)
      (ttime.day != today.day) || (ttime.month != today.month) || (ttime.year != today.year)
    end
    render :index
  end
  
  #GET /Ticket/assgined_to_me
  def assgined_to_me
    @title = "Assigned to me"
    @tickets = Ticket.find(:all)
    
    # get the ID of user passed from the source
    settings = LighthouseSettings.find(:all)[0]
    
    @tickets = @tickets.reject do |ticket|
      ticket.assigned_user_id != settings.lighthouse_id
    end
    render :index
  end
  
  # GET /Ticket/1
  def show
    @ticket = Ticket.find(@params['id'])
    render :show
  end

  # GET /Ticket/new
  def new
    @ticket = Ticket.new
    render :new
  end

  # GET /Ticket/1/edit
  def edit
    @ticket = Ticket.find(@params['id'])
    render :edit
  end

  # POST /Ticket/create
  def create
    @ticket = Ticket.new(@params['ticket'])
    @ticket.save
    redirect :index
  end

  # POST /Ticket/1/update
  def update
    @ticket = Ticket.find(@params['id'])
    @ticket.update_attributes(@params['ticket'])
    redirect :index
  end

  # POST /Ticket/1/delete
  def delete
    @ticket = Ticket.find(@params['id'])
    @ticket.destroy
    redirect :index
  end
end
