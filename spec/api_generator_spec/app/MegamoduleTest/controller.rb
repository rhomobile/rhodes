require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'

class MegamoduleTestController < Rho::RhoController
  include BrowserHelper
  include ApplicationHelper
  
  def index
    render :back => '/app'
  end


  def run_test
    render :action => :index, :back => '/app'
    # test int property
    defItem = Rho::Examples::Megamodule.default
    defItem.IntegerProperty = 3
    if defItem.IntegerProperty != 3
       Alert.show_popup "Test FAILED : [defItem.IntegerProperty != 3] !"
       return
    end
    defItem.IntegerProperty = 7
    if defItem.IntegerProperty != 7
       Alert.show_popup "Test FAILED : [defItem.IntegerProperty != 3] !"
       return
    end
    itemCount = Rho::Examples::Megamodule.getObjectsCount
    if itemCount != 3
       Alert.show_popup "Test FAILED : [itemCount != 3 != 3] !"
       return
    end

    secondItem = Rho::Examples::Megamodule.getObjectByIndex 1
    secondItem.setProperty("StringProperty", "ABC")
    sip = secondItem.StringProperty
    if sip != "ABC"
      Alert.show_popup "Test FAILED : [secondItem.StringProperty != ABC] !"
      return
    end

    res1 = secondItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"})
    res2 = Rho::Examples::Megamodule.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p2 => "qwe"})
    res3 = secondItem.typesTest("ABC", true, 555123, 3.14, [1,2,3], {:p1 => "abc", :p3 => "qwa"})
    res4 = Rho::Examples::Megamodule.typesTest("ABC", true, 555123, 3.15, [1,2,3], {:p1 => "abc", :p2 => "qwe"})
    if (!res1) || (!res2) || (res3) || (res4)
      Alert.show_popup "Test FAILED : [secondItem.typesTest ] ("+res1.to_s+","+res2.to_s+","+res3.to_s+","+res4.to_s+")"
      return
    end



    Alert.show_popup "Test DONE OK!"
    render :action => :index, :back => '/app'
  end

  def run_show_alert_test
    Rho::Examples::Megamodule.showAlertFromUIThread "message text from Ruby controller !"
    render :action => :index, :back => '/app'
  end



end
