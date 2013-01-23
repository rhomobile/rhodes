require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
require 'barcode'

class BarcodeTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_test
    sum = Barcode.calc_summ(3,7) 
    Alert.show_popup Barcode::BarcodeStringHelper::process_string('test')+sum.to_s
    render :action => :index, :back => '/app'
  end
  
end
