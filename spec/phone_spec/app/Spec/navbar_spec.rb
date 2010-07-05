class NavBarSpec
  def initialize
  end

  def create_test
    Test_equal( NavBar.started, false )
    
    begin
      NavBar.create
    rescue
      # Nothing
    end
    sleep 2
    Test_equal( NavBar.started, false )

    NavBar.create :title => "", :left => {:action => :back}
    sleep 2
    Test_equal( NavBar.started, true )

    NavBar.remove
    sleep 2
    Test_equal( NavBar.started, false )

  end

end
