require 'rho/rhocontroller'

class Barcode1TestController < Rho::RhoController
  #@layout = :simplelayout
  @layout = 'Barcode1Test/layout'
  
  def index
    puts "Barcode1Test index controller"
    render :back => '/app'
  end


  def run_test
   Rho::Barcode1.getProps("display", url_for(:action => :test_callback1))
   arBarcodes = Rho::Barcode1.enumerate
   puts "arBarcodes : #{arBarcodes}"
   Alert.show_popup "arBarcodes : #{arBarcodes}"
   id = arBarcodes[0].getProps("ID")
   Alert.show_popup "first ID =  : #{id}"

   arBarcodes[1].getProps("sound", url_for(:action => :test_callback2))
   
   arBarcodes[1].getProps(nil, url_for(:action => :test_callback3), "param1=123")

    render :action => :index, :back => '/app'
   
  end

  def test_callback1
      puts "callback1 = #{@params}"
      Alert.show_popup "callback1 = #{@params}"
  end

  def test_callback2
      puts "callback2 = #{@params}"
      Alert.show_popup "callback2 = #{@params}"
  end

  def test_callback3
      puts "callback3 = #{@params}"
      Alert.show_popup "callback3 = #{@params}"
  end



end
