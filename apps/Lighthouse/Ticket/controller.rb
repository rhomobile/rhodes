require 'rho/rhocontroller'
require File.join(File.dirname(__FILE__), '../helpers/application_helper')

class TicketController < Rho::RhoController
  
  include ApplicationHelper
  
  #GET /Ticket
  def index
    @tickets = Ticket.find(:all)
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
