require 'rho/rhocontroller'

class PeopleController < Rho::RhoController
  
  #GET /People
  def index
    @peoples = People.find(:all)
    render :index
  end

  # GET /People/1
  def show
    @peoples = People.find(@params['object'])
  end

  # GET /People/new
  def new
    @people = People.new
    render :new
  end

  # GET /People/1/edit
  def edit
    @people = People.find(@params['id'])
    render :edit
  end

  # POST /People/create
  def create
    @people = People.new(@params['people'])
    @people.save
    redirect :action => :index
  end

  # POST /People/1/update
  def update
    @people = People.find(@params['id'])
    @people.update_attributes(@params['people'])
    redirect :action => :index
  end

  # POST /People/1/delete
  def delete
    @people = People.find(@params['id'])
    @people.destroy
    redirect :action => :index
  end
end
