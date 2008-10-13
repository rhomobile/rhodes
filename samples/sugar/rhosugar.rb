# RhoSugar.rb
# sample app written to allow editing Sugar account data
# to run on Runner using Camping and ActiveRecord
# like camping being written in 4K, we'd like Rhorunner sample CRUD apps to be <100 lines
require 'camping'
Camping.goes :Rhosugar
module Rhosugar::Models
  class Account < Base; end
end

module Rhosugar::Controllers
  class Index < R '/'
    def get
      @accounts = Account.find :all
      render :index
    end
  end

  class Add
    def get
      render :add
    end

    def post
      @account = Account.create :name=>input.account_name,:industry=>input.account_industry
      redirect View,@account
    end
  end

  class View < R '/view/(\d+)'
    def get account_id
      @account=Account.find account_id
      render :view
    end
  end

  class Edit < R '/edit/(\d+)','/edit'
    def get account_id
      @account=Account.find account_id
      render :edit
    end

    def post
      @account=Account.find input.account_id
      @account.update_attributes :name=>input.account_name,:industry=>input.account_industry
      @accounts=Account.find :all
      render :index
    end
  end

  class Delete < R '/delete(\d+)','/edit'
    def get account_id
      @account=Account.find account_id
      @account.destroy
      @accounts=Account.find :all
      render :index
    end
  end
end # Controllers module

module Rhosugar::Views
  def layout
    html do
      body do
        self << yield
      end
    end
  end

  def index
    for account in @accounts
      _account(account)
    end
  end

  def add
    _form(@account,:action=>R(Add))
  end

  def edit
    _form(@account,:action=>R(Edit))
  end

  def view
    _account(@account)
  end

  def _account(account)
    tr do
      td { a(account.name,:href=>R(Edit,account)) }
      td {account.industry}
      td {a('X',:href=>R(Delete,account)) }
    end
  end

  def _form(account,opts)
    form({:method=>'post'}.merge(opts)) do
      label 'Name',:for=>'account_name';br
      input :name=>'account_name',:type=>'text',:value=>account.name;br
      label 'Industry',:for=>'account_industry';br
      input :name=>'account_industry',:type=>'text',:value=>account.industry;br
      input :type=>'hidden',:name=>'account_id',:value=>account.id
      input :type=>'submit';br
    end
  end
end


