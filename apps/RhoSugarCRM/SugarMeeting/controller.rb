require 'rho/rhocontroller'

class SugarMeetingController < Rho::RhoController

  #GET /SugarMeeting
  def index
    @SugarMeetings = SugarMeeting.find(:all)
    render
  end

  # GET /SugarMeeting/1
  def show
    @SugarMeetings = SugarMeeting.find(@params['object'])
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
    redirect :index
  end

  # POST /SugarMeeting/1/update
  def update
    @SugarMeeting = SugarMeeting.find(@params['id'])
    @SugarMeeting.update_attributes(@params['SugarMeeting'])
    redirect :index
  end

  # POST /SugarMeeting/1/delete
  def delete
    @SugarMeeting = SugarMeeting.find(@params['id'])
    @SugarMeeting.destroy
    redirect :index
  end
end
