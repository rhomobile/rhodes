require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class GenPropBagTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test
    res = Rho::GenPropBag.take 
    Alert.show_popup "Rho::GenPropBag.take return : #{res}"
    render :action => :index, :back => '/app'
  end
  
end
