require 'rho/rhocontroller'
require File.join(__rhoGetCurrentDir(), 'apps','Lighthouse','helpers/application_helper')
require 'time'

class TicketController < Rho::RhoController
  
  include ApplicationHelper
  
  #GET /Ticket
  def index
    puts "in index with >#{@params['id']}<"
    @title = "All tickets"	
    @tickets = Ticket.find(:all)
    
    # if we pass in an ID them limit by only that project 
    if @params['id']
      @project = Project.find(@params['id'])
      @title = @project.name
      
      @tickets = @tickets.reject {|ticket| ticket.project_id != strip_braces(@params['id']) }
    end
    @tickets = @tickets.sort {|x,y| y.number.to_i <=> x.number.to_i }
    
    render :action => :index
  end

  #GET /Ticket/today
  def today
    @title = "Todays tickets"
    @tickets = Ticket.find(:all)
    today = Time.new
    @tickets = @tickets.reject do |ticket|
      ttime = Time.parse(ticket.created_at)
      (ttime.day != today.day) || (ttime.month != today.month) || (ttime.year != today.year)
    end
    
    render :action => :index
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
    @tickets = @tickets.sort {|x,y| y.number.to_i <=> x.number.to_i }
    
    render :action => :index
  end
  
  # GET /Ticket/1
  def show
    @ticket = Ticket.find(@params['id'])
    render :action => :show
  end

  # GET /Ticket/new
  def new
    @ticket = Ticket.new
    
    # we pass in th ID of the project to create under
    @ticket.project_id = strip_braces(@params['id'])
    @ticket.created_at = Time.new
    
    render :action => :new
  end

  # GET /Ticket/1/edit
  def edit
    @ticket = Ticket.find(@params['id'])
    render :action => :edit
  end

  # POST /Ticket/create
  def create
    puts "in ticket create with #{@params['ticket'].inspect.to_s}\n"
    
    @ticket = Ticket.new(@params['ticket'])
    @ticket.save
    redirect :action => :index
  end

  # POST /Ticket/1/update
  def update
    @ticket = Ticket.find(@params['id'])
    @ticket.update_attributes(@params['ticket'])
    redirect :action => :index
  end

  # POST /Ticket/1/delete
  def delete
    @ticket = Ticket.find(@params['id'])
    @ticket.destroy
    redirect :action => :index
  end
end
