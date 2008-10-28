require 'rho/rhocontroller'
# the only and only controller for the account model
class CaseController < Rho::RhoController

  # GET /cases
  def index
    @cases = Case.find(:all)
    render :index
  end

  # GET /cases/1
  def show
    @cases = Case.find(@params['object'])
  end

  # GET /cases/new
  def new
    @accounts = Account.find :all
    @case = Case.new
    render :new
  end

  # GET /cases/1/edit
  def edit
    @accounts = Account.find :all
    @case = Case.find(@params['id'])
    @account = Account.find @case.account_id
    @employees = Employee.find :all
    @employee = Employee.find @case.assigned_user_id
    render :edit
  end

  # POST /cases
  def create
    @case = Case.new(@params['case'])
    redirect :index
  end

  # POST /cases/1
  def update
    @case = Case.find(@params['object'])
    @case.update_attributes(@params['case'])
    redirect :index
  end

  # POST /cases/1/delete
  def destroy
    @case = Case.find(@params['object'])
    @case.destroy
	redirect :index
  end

end
