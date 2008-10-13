require 'rhocontroller'

class ContactsController < RhoController

	def index
		@message = "List of contacts"
		render :index
	end

	def new
		render :new
	end
	
	def create
		@message = "Created new contact w/ params: " + @params.to_s
		render :index
	end 
end
