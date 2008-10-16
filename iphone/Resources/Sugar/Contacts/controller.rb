require 'rhocontroller'
require 'rhom'

class ContactsController < RhoController

  def index
    # Test code for orm
    #    result = @rhom.find 1
    #    p result.inspect
    #    p 'source id: ' + result[0].source_id.to_s
    # End test code
    @message = "List of contacts"
    render :index
  end

  def new
    render :new
  end
	
  def create
    @message = "Created new contact w/ params: " + @params.to_s
    redirect :show, 10
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
