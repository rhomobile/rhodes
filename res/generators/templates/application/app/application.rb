require 'rho/rhoapplication'

class AppApplication < Rho::RhoApplication
  def initialize
    # Tab items are loaded left->right, @tabs[0] is leftmost tab in the tab-bar
    @tabs = [{ :label => "Index", :action => '/app', :icon => "/public/images/tabs/dashboard.png", :reload => true },
             { :label => "Settings", :action => '/app/Settings', :icon => "/public/images/tabs/accounts.png" },
             { :label => "About", :action => '/app/About', :icon => "/public/images/tabs/contacts.png" }]
    # Important to call super _after_ you define @tabs!
    super
  end
end
