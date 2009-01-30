require 'rho/rhocontroller'

class ServiceRequestController < Rho::RhoController
  
  #GET /ServiceRequest
  def index
    @service_requests = ServiceRequest.find(:all)
    render :index
  end

  # GET /ServiceRequest/1
  def show
    @service_requests = ServiceRequest.find(@params['object'])
  end

  # GET /ServiceRequest/new
  def new
    @service_request = ServiceRequest.new
    render :new
  end

  # GET /ServiceRequest/1/edit
  def edit
    @service_request = ServiceRequest.find(@params['id'])
    render :edit
  end

  # POST /ServiceRequest/create
  def create
    @service_request = ServiceRequest.new(@params['service_request'])
    @service_request.save
    redirect :action => :index
  end

  # POST /ServiceRequest/1/update
  def update
    @service_request = ServiceRequest.find(@params['id'])
    @service_request.update_attributes(@params['service_request'])
    redirect :action => :index
  end

  # POST /ServiceRequest/1/delete
  def delete
    @service_request = ServiceRequest.find(@params['id'])
    @service_request.destroy
    redirect :action => :index
  end
end
