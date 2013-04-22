require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class SimpleonlystaticmoduleTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end

  def run_calc_test
     res = Rho::Examples::SimpleOnlyStaticModule.calcSumm(2,3)
     title = "Test ERROR:"
     if res.class.to_s == "Fixnum" 
          if res == 5
                title = "Test OK:"
          end
     end	
     Alert.show_popup(
        :message=>("calcSumm(2,3) = "+res.to_s + "\n result.class = "+res.class.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
     render :action => :index, :back => '/app'
  end

  def run_calc_async_test
      Rho::Examples::SimpleOnlyStaticModule.calcSumm(2,3, (url_for :action => :run_calc_async_test_callback) )
      render :action => :index, :back => '/app'
  end

  def run_calc_async_test_callback
     res = @params["result"]
     title = "Test ERROR:"
     if res.class.to_s == "Fixnum" 
          if res == 5
                title = "Test OK:"
          end
     end	
     Alert.show_popup(
        :message=>("Callback with calcSumm(2,3) = "+res.to_s + "\n result.class = "+res.class.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
  end

  def run_join_strings_test
     res = Rho::Examples::SimpleOnlyStaticModule.joinStrings ['111', '222', '333']
     title = "Test ERROR:"
     if res.class.to_s == "String" 
          if res == "111222333"
                title = "Test OK:"
          end
     end	
     Alert.show_popup(
        :message=>("joinStrings ['111', '222', '333'] = "+res.to_s + "\n result.class = "+res.class.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
     render :action => :index, :back => '/app'
  end

  def run_join_strings_async_test
      Rho::Examples::SimpleOnlyStaticModule.joinStrings ['111', '222', '333'], (url_for :action => :run_join_strings_async_test_callback), "&callback_myparam=qwerty" 
      render :action => :index, :back => '/app'
  end

  def run_join_strings_async_test_callback
     res = @params["result"]
     myparam = @params["callback_myparam"]
     title = "Test ERROR:"
     if res.class.to_s == "String" 
          if res == "111222333"
               if myparam == "qwerty"
                   title = "Test OK:"
               end
          end
     end	
     Alert.show_popup(
        :message=>("Callback with joinStrings ['111', '222', '333'] = "+res.to_s + "\n result.class = "+res.class.to_s+"\ncallback_param = "+myparam.to_s),
        :title=>title,
        :buttons => ["Ok"]
     )
  end

  def run_get_platform_test
      res = Rho::Examples::SimpleOnlyStaticModule.getPlatform
      Alert.show_popup "getPlatform() = "+res.to_s + "\n result.class = "+res.class.to_s
      render :action => :index, :back => '/app'
  end
  
  def run_start_test
      Rho::Examples::SimpleOnlyStaticModule.showAlertFromUIThread
      render :action => :index, :back => '/app'
  end

  def test_proc(arg)
    puts "$$$$$ test_proc : #{arg}"
  end

  def test_callback
    puts "$$$$$ test_callback : #{@params}"
  end


  def run_lambda_test
    Rho::Examples::SimpleOnlyStaticModule.calcSumm(1,1, (url_for :action => :test_callback))
    Rho::Examples::SimpleOnlyStaticModule.calcSumm(10,10, lambda{|args| puts "$$$$$ lamda: #{args} "} )
    Rho::Examples::SimpleOnlyStaticModule.calcSumm(100,100, method(:test_proc) )
    Rho::Examples::SimpleOnlyStaticModule.calcSumm(1000,1000, Proc.new{|args| puts "$$$$$ proc: #{args}"} )  
    render :action => :index, :back => '/app'
  end


end
