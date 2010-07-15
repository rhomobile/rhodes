class BarcodeSpec
 
  def barcode_test

    barcode_01_filename = File.join(Rho::RhoApplication::get_model_path('app','Barcode'), 'barcode_01.png')	
    barcode_01_correct_code = '9876543210128'	
    barcode_02_filename = File.join(Rho::RhoApplication::get_model_path('app','Barcode'), 'barcode_02.jpg')	
    barcode_02_correct_code = '4006170005209'
	

    barcode_01_recognized_code = Barcode.barcode_recognize(barcode_01_filename)
    Test_equal( barcode_01_correct_code, barcode_01_recognized_code )

    barcode_02_recognized_code = Barcode.barcode_recognize(barcode_02_filename)
    Test_equal( barcode_02_correct_code, barcode_02_recognized_code )

  end

end
