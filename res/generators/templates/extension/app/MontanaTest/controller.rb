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
    sum = <%= name.camel_case %>.calc_summ(3,7) 
    Alert.show_popup <%= name.camel_case %>::<%= name.camel_case %>StringHelper::process_string('test')+sum.to_s
    render :action => :index, :back => '/app'
  end
  
end
