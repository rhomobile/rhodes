require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class <%= namecamelcase %>TestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test_get_platform
    objects = Rho::<%= namecamelcase %>.enumerate
    obj = objects[0]
    res = obj.getPlatformName
    Alert.show_popup "Rho::<%= namecamelcase %>.getPlatformName return : #{res.to_s}"
    render :action => :index, :back => '/app'
  end

  def run_test_calc_summ
    objects = Rho::<%= namecamelcase %>.enumerate
    obj = objects[0]
    res = obj.calcSumm(2,3) 
    Alert.show_popup "Rho::<%= namecamelcase %>.calcSumm(2,3) return : #{res.to_s}"
    render :action => :index, :back => '/app'
  end

  def run_test_join_strings
    objects = Rho::<%= namecamelcase %>.enumerate
    obj = objects[0]
    res = obj.joinStrings("aaa","bbb") 
    Alert.show_popup "Rho::<%= namecamelcase %>.joinStrings(aaa,bbb) return : #{res.to_s}"
    render :action => :index, :back => '/app'
  end

  
end
