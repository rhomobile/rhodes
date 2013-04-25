require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class GenPropBag2TestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test
    res = Rho::GenPropBag2.take 
    Alert.show_popup "Rho::GenPropBag2.take return : #{res}"
    render :action => :index, :back => '/app'
  end
  
end
