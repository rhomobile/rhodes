require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
#require 'barcode'

class BarcodeTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def test_proc(arg)
    puts "test_proc : #{arg}"
  end
  
  #@@test_proc = lambda{|args| puts "lamda: #{args}"}
  def index
=begin
    str1 = ""
    #Rho::Barcode.myTest(str1)
    Rho::Barcode.test    
    Rho::Barcode.test= "123"
    Rho::Barcode.test = "123"
    
    hash = { name: "David", age: 49 }
    #puts "hash: #{hash.name}"

    Rho::Barcode.enable(hash, url_for(:action => :test_callback))

    my_test = "123"
    Rho::Barcode.getProperty("display", lambda{|args| puts "lamda: #{args} #{my_test}"} )
    Rho::Barcode.getProperty("display", method(:test_proc) )
    Rho::Barcode.getProperty("display", Proc.new{|args| puts "proc: #{args}"} )
    

    Rho::Barcode.getProperty("display", "/app/BarcodeTest/test_callback")
    
    Rho::Barcode.getProperty("display", "/app/BarcodeTest/test_callback")
    
    arBarcodes = Rho::Barcode.enumerate
    puts "arBarcodes : #{arBarcodes}"
    id = arBarcodes[0].getProperty("ID")
    puts "id: #{id}"
    
    Rho::Barcode.myTest(arBarcodes[0])
    
    arBarcodes[1].getProperty("sound", "/app/BarcodeTest/test_callback")
    
    arBarcodes[1].getAllProperties( "/app/BarcodeTest/test_callback", "param1=123")

    #hash = { name: "David", age: 49 }
    Rho::Barcode.setProperties(  Barcode128: 'enabled' )
=end  
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
