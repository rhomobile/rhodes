require 'rho/rhoapplication'

class AppApplication < Rho::RhoApplication
  def initialize
    # Tab items are loaded left->right, @tabs[0] is leftmost tab in the tab-bar
    @tabs = nil

    @@tabbar = nil
    # Important to call super _after_ you define @tabs!
    super
  end
end
