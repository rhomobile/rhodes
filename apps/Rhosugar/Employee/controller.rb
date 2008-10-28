require 'rho/rhocontroller'
# the only and only controller for the account model
class EmployeeController < Rho::RhoController

  # GET /Employees
  def index
    @employees = Employee.find(:all)
    render :index
  end

  # GET /Employees/1
  def show
    @employees = Employee.find(@params['id'])
  end

  # GET /Employees/new
  def new
    @accounts = Account.find :all
    @employee = Employee.new
    render :new
  end

  # GET /Employees/1/edit
  def edit
    @accounts = Account.find :all
    @employee = Employee.find(@params['id'])
    @account = Account.find @employee.id
    render :edit
  end

  # POST /Employees
  def create
    @employee = Employee.new(@params['Employee'])
    redirect :index
  end

  # POST /Employees/1
  def update
    @employee = Employee.find(@params['id'])
    @employee.update_attributes(@params['employee'])
    redirect :index
  end

  # POST /Employees/1/delete
  def destroy
    @employee = Employee.find(@params['id'])
    @employee.destroy
    redirect :index
  end

end
