require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class MegamoduleTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end


 def run_array_async_test
      Rho::Examples::Megamodule.produceArray  (url_for :action => :run_array_async_test_callback) 
      render :action => :index, :back => '/app'
  end

  def run_array_async_test_callback
     res = @params["result"]
     title = "Test ERROR:"
     if res.class.to_s == "Array" 
          if res == [1,2,3,4,5,6,7,8,9,10]
               title = "Test OK:"
          end
     end	
     Alert.show_popup(
        :message=>("Callback with produceArray() = "+res.to_s + "\n result.class = "+res.class.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
  end

 def run_hash_async_test
      Rho::Examples::Megamodule.produceHash  (url_for :action => :run_hash_async_test_callback), "&callback_myparam=qwerty" 
      render :action => :index, :back => '/app'
  end

  def run_hash_async_test_callback
     # check { :parame1 => "value1", :parama2 = > 55 }
     parame1 = @params["parame1"]
     parama2 = @params["parama2"]
     myparam = @params["callback_myparam"]
     title = "Test ERROR:"
     if parame1.class.to_s == "String" 
          if parame1 == "value1"
              if parama2.class.to_s == "Fixnum"
                  if parama2 == 55
                       if myparam == "qwerty"
                           title = "Test OK:"
                       end
                  end
               end
          end
     end	
     Alert.show_popup(
        :message=>("Callback with produceHash() \nparame1= "+parame1.to_s + "\n parame1.class = "+parame1.class.to_s+"\nparama2= "+parama2.to_s + "\n parama2.class = "+parama2.class.to_s+   "\ncallback_param = "+myparam.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
  end

  def run_get_int_prop_test
    res = Rho::Examples::Megamodule.IntegerProperty
     title = "Test ERROR:"
     if res.class.to_s == "Fixnum" 
          if res == 12345
                title = "Test OK:"
          end
     end	
     Alert.show_popup(
        :message=>("integer_property = "+res.to_s + "\n result.class = "+res.class.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
     render :action => :index, :back => '/app'  end

  def run_show_alert_test
    Rho::Examples::Megamodule.showAlertFromUIThread "message text from Ruby controller !"
    render :action => :index, :back => '/app'
  end



end
