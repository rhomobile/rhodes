require 'rho/rhocontroller'

class ContactsController < Rho::RhoController

  def index
    @accounts = Contacts.find(:all)
    @message = "List of contacts"
    puts "Triggering syncronization w/ remote DB"
    SyncEngine::dosync

    render :index
  end

  def new
    render :new
  end
	
  def create
    @message = "Created new contact w/ params: " + @params.to_s
    redirect :show, '{10}'
  end
	
  def show
    @message = "Show contact record: " + @params['id']
    render :show
  end
	
  def edit
    render :edit
  end
	
  def update
    puts "Updated record: " + @params['id']
    redirect :show, @params['id']
  end
	
  def delete
    puts "Deleted record: " + @params['id']
    redirect :index
  end
  
end
