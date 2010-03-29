require 'rho/rhoapplication'

class AppApplication < Rho::RhoApplication
  def initialize
    # Tab items are loaded left->right, @tabs[0] is leftmost tab in the tab-bar
    @tabs = [{ :label => "Home", :action => '/app', :icon => "/public/icons/house.png", :reload => true },
             { :label => "Settings", :action => '/app/Settings', :icon => "/public/icons/gear2.png" },
             { :label => "About", :action => '/app/About', :icon => "/public/icons/about.png" }]
    # Important to call super _after_ you define @tabs!
    super
  end
end
