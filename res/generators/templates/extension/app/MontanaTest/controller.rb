require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
require '<%= name.downcase %>'

class <%= name.camel_case %>TestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test
    res = Rho::<%= name.camel_case %>.enumerate 
    Alert.show_popup "Rho::<%= name.camel_case %>.enumerate return : #{res}"
    render :action => :index, :back => '/app'
  end
  
end
