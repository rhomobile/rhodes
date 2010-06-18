require 'rho/mapview'

class MapViewTest
  def initialize
  end

  def create_test
    state = MapView.state
    Test_equal( state, nil )

    MapView.create :settings => {:map_type => 'roadmap', :region => [37, -122, 10, 10]}
    sleep 2

    state = MapView.state
    Test_not_equal( state, nil )
    Test_equal( state.is_a?(Hash), true )
    Test_equal( state[:center], {:latitude => 37.0, :longitude => -122.0} )

    MapView.close
    sleep 2

    state = MapView.state
    Test_equal( state, nil )
  end

end
