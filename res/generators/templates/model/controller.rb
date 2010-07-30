require 'rho/rhocontroller'
require 'helpers/browser_helper'

class <%= class_name %>Controller < Rho::RhoController
  include BrowserHelper

  #GET /<%= class_name %>
  def index
    @<%= name.downcase.pluralize %> = <%= class_name %>.find(:all)
    render
  end

  # GET /<%= class_name %>/{1}
  def show
    @<%= name.downcase %> = <%= class_name %>.find(@params['id'])
    if @<%= name.downcase %>
      render :action => :show
    else
      redirect :action => :index
    end
  end

  # GET /<%= class_name %>/new
  def new
    @<%= name.downcase %> = <%= class_name %>.new
    render :action => :new
  end

  # GET /<%= class_name %>/{1}/edit
  def edit
    @<%= name.downcase %> = <%= class_name %>.find(@params['id'])
    if @<%= name.downcase %>
      render :action => :edit
    else
      redirect :action => :index
    end
  end

  # POST /<%= class_name %>/create
  def create
    @<%= name.downcase %> = <%= class_name %>.create(@params['<%= name.downcase %>'])
    redirect :action => :index
  end

  # POST /<%= class_name %>/{1}/update
  def update
    @<%= name.downcase %> = <%= class_name %>.find(@params['id'])
    @<%= name.downcase %>.update_attributes(@params['<%= name.downcase %>']) if @<%= name.downcase %>
    redirect :action => :index
  end

  # POST /<%= class_name %>/{1}/delete
  def delete
    @<%= name.downcase %> = <%= class_name %>.find(@params['id'])
    @<%= name.downcase %>.destroy if @<%= name.downcase %>
    redirect :action => :index
  end
end
