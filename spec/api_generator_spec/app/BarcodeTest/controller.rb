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
    #Rho::Barcode.enumerate( lambda{|args| puts "lamda: #{args}"});

    

    puts "Before Rho::Barcode.boolProp : #{Rho::Barcode.boolProp} type:#{Rho::Barcode.boolProp.class.to_s}"
    Rho::Barcode.boolProp = true
    puts "After Rho::Barcode.boolProp : #{Rho::Barcode.boolProp} type:#{Rho::Barcode.boolProp.class.to_s}"
    
    puts "Before Rho::Barcode.intProp : #{Rho::Barcode.intProp} type:#{Rho::Barcode.intProp.class.to_s}"
    Rho::Barcode.intProp = 123
    puts "After Rho::Barcode.intProp : #{Rho::Barcode.intProp} type:#{Rho::Barcode.intProp.class.to_s}"

    puts "Before Rho::Barcode.floatProp : #{Rho::Barcode.floatProp} type:#{Rho::Barcode.floatProp.class.to_s}"
    Rho::Barcode.floatProp = 123.78
    puts "After Rho::Barcode.floatProp : #{Rho::Barcode.floatProp} type:#{Rho::Barcode.floatProp.class.to_s}"

    puts "Before Rho::Barcode.stringProp : #{Rho::Barcode.stringProp} type:#{Rho::Barcode.stringProp.class.to_s}"
    Rho::Barcode.stringProp = "geny"
    puts "After Rho::Barcode.stringProp : #{Rho::Barcode.stringProp} type:#{Rho::Barcode.stringProp.class.to_s}"

    puts "Before Rho::Barcode.boolPropDef : #{Rho::Barcode.boolPropDef} type:#{Rho::Barcode.boolPropDef.class.to_s}"
    Rho::Barcode.boolPropDef = true
    puts "After Rho::Barcode.boolPropDef : #{Rho::Barcode.boolPropDef} type:#{Rho::Barcode.boolPropDef.class.to_s}"
    
    puts "Before Rho::Barcode.intPropDef : #{Rho::Barcode.intPropDef} type:#{Rho::Barcode.intPropDef.class.to_s}"
    Rho::Barcode.intPropDef = 123
    puts "After Rho::Barcode.intPropDef : #{Rho::Barcode.intPropDef} type:#{Rho::Barcode.intPropDef.class.to_s}"

    puts "Before Rho::Barcode.floatPropDef : #{Rho::Barcode.floatPropDef} type:#{Rho::Barcode.floatPropDef.class.to_s}"
    Rho::Barcode.floatPropDef = 123.78
    puts "After Rho::Barcode.floatPropDef : #{Rho::Barcode.floatPropDef} type:#{Rho::Barcode.floatPropDef.class.to_s}"

    puts "Before Rho::Barcode.stringPropDef : #{Rho::Barcode.stringPropDef} type:#{Rho::Barcode.stringPropDef.class.to_s}"
    Rho::Barcode.stringPropDef = "geny"
    puts "After Rho::Barcode.stringPropDef : #{Rho::Barcode.stringPropDef} type:#{Rho::Barcode.stringPropDef.class.to_s}"

    
=begin
 
    Rho::Barcode.setProperties :autoEnter => 'enabled', :code128 => 'enabled', :code39 => 'enabled'
    Rho::Barcode.setProperties code128: 'enabled', code39: 'enabled' 
    Rho::Barcode.enable( {:code128 => 'enabled', :code39 => 'enabled'}, "" )
    Rho::Barcode.enable( {code128: 'enabled', code39: 'enabled'}, "" )
    

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
