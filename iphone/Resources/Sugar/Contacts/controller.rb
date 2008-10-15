require 'rhocontroller'
require 'rhom'

class ContactsController < RhoController

	def index
		# Test code for orm
		result = Rhom.find 1
		p result[0].fields
		p result[0].types
		p result[0].to_s
		p 'row name' + result[0]['name']
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
