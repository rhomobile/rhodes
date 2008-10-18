require 'rhocontroller'
# the only and only controller for the account model
class CaseController < RhoController

  # GET /cases
  def index
    @cases = Case.find(:all)
  end

  # GET /cases/1
  def show
    @cases = Case.find(params[:id])
  end

  # GET /cases/new
  def new
    @case = Case.new
  end

  # GET /cases/1/edit
  def edit
    @object_value = ObjectValue.find(params[:id])
  end

  # POST /cases
  def create
    @case = Case.new(params[:case])
  end

  # POST /cases/1
  def update
    @case = Case.find(params[:id])
    @case.save
  end

  # POST /cases/1/delete
  def destroy
    @case = Case.find(params[:id])
    @case.destroy
  end

end
