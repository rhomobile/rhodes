class NativeBarSpec
  def initialize
  end

  def create_test
    NativeBar.create Rho::RhoApplication::NOBAR_TYPE, []
    sleep 2
    Test_equal( NativeBar.started, false )

    NativeBar.create Rho::RhoApplication::TOOLBAR_TYPE, [{:action => :back}]
    sleep 2
    Test_equal( NativeBar.started, true )

    NativeBar.remove
    sleep 2
    Test_equal( NativeBar.started, false )

  end

end
