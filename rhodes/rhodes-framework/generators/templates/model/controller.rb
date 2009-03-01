require 'rho/rhocontroller'

class <%= class_name %>Controller < Rho::RhoController

  #GET /<%= class_name %>
  def index
    @<%= name.pluralize %> = <%= class_name %>.find(:all)
    render
  end

  # GET /<%= class_name %>/1
  def show
    @<%= name %> = <%= class_name %>.find(@params['id'])
    render :action => :show
  end

  # GET /<%= class_name %>/new
  def new
    @<%= name %> = <%= class_name %>.new
    render :action => :new
  end

  # GET /<%= class_name %>/1/edit
  def edit
    @<%= name %> = <%= class_name %>.find(@params['id'])
    render :action => :edit
  end

  # POST /<%= class_name %>/create
  def create
    @<%= name %> = <%= class_name %>.new(@params['<%= name %>'])
    @<%= name %>.save
    redirect :action => :index
  end

  # POST /<%= class_name %>/1/update
  def update
    @<%= name %> = <%= class_name %>.find(@params['id'])
    @<%= name %>.update_attributes(@params['<%= name %>'])
    redirect :action => :index
  end

  # POST /<%= class_name %>/1/delete
  def delete
    @<%= name %> = <%= class_name %>.find(@params['id'])
    @<%= name %>.destroy
    redirect :action => :index
  end
end
