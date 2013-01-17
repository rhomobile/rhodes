require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
require 'barcode1'

class Barcode1TestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test
    sum = Barcode1.calc_summ(3,7) 
    Alert.show_popup Barcode1::Barcode1StringHelper::process_string('test')+sum.to_s
    render :action => :index, :back => '/app'
  end
  
end
