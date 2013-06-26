require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class <%= namecamelcase %>TestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test
    res = Rho::<%= namecamelcase %>.enumerate 
    Alert.show_popup "Rho::<%= namecamelcase %>.enumerate return : #{res.to_s}"
    render :action => :index, :back => '/app'
  end
  
end
