require 'rho/rhocontroller'

class SugarMeetingController < Rho::RhoController

  #GET /SugarMeeting
  def index
    @SugarMeetings = SugarMeeting.find(:all)
    
    # sort by name in ascending order
		if System::get_property('platform') != 'Blackberry'    
      if (@SugarMeetings.length > 0)
        @SugarMeetings = @SugarMeetings.sort_by {|item| !item.name.nil? ? item.name : ""}
      end
    end
  
    render
  end

  # GET /SugarMeeting/1
  def show
    @SugarMeeting = SugarMeeting.find(@params['id'])
    render :action => :show
  end

  # GET /SugarMeeting/new
  def new
    @SugarMeeting = SugarMeeting.new
    render :action => :new
  end

  # GET /SugarMeeting/1/edit
  def edit
    @SugarMeeting = SugarMeeting.find(@params['id'])
    render :action => :edit
  end

  # POST /SugarMeeting/create
  def create
    @SugarMeeting = SugarMeeting.new(@params['SugarMeeting'])
    @SugarMeeting.save
    redirect :action => :index
  end

  # POST /SugarMeeting/1/update
  def update
    @SugarMeeting = SugarMeeting.find(@params['id'])
    @SugarMeeting.update_attributes(@params['SugarMeeting'])
    redirect :action => :index
  end

  # POST /SugarMeeting/1/delete
  def delete
    @SugarMeeting = SugarMeeting.find(@params['id'])
    @SugarMeeting.destroy
    redirect :action => :index
  end
end
