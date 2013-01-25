require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
require 'barcode'

class BarcodeTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
  
    res = Rho::Barcode.getProperty("display")
    puts "res: #{res}"
    
    Rho::Barcode.getProperty("display", "/app/BarcodeTest/test_callback")
    
    arBarcodes = Rho::Barcode.enumerate
    puts "arBarcodes : #{arBarcodes}"
    id = arBarcodes[0].getProperty("ID")
    puts "id: #{id}"
    arBarcodes[1].getProperty("sound", "/app/BarcodeTest/test_callback")
    
    arBarcodes[1].getAllProperties( "/app/BarcodeTest/test_callback", "param1=123")
    
    render :back => '/app'
  end

  def test_callback
    puts "test_callback: #{@params}"
  end

  def run_test
    sum = Barcode.calc_summ(3,7) 
    Alert.show_popup Barcode::BarcodeStringHelper::process_string('test')+sum.to_s
    render :action => :index, :back => '/app'
  end
  
end
