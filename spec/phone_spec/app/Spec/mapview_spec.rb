require 'rho/mapview'

class MapViewSpec
  def initialize
  end

  def create_test
    state = MapView.state
    Test_equal( state, nil )

    MapView.create :settings => {:map_type => 'roadmap', :region => [37, -122, 10, 10]}
    w = 2
    w = 5 if System::get_property('platform') == 'ANDROID'
    sleep w

    state = MapView.state
    Test_not_equal( state, nil )
    Test_equal( state.is_a?(Hash), true )
    delta = 0.001
    Test_equal( (state[:center][:latitude] - 37.0).abs < delta, true )
    Test_equal( (state[:center][:longitude] - (-122.0)).abs < delta, true )

    MapView.close
    sleep 2

    state = MapView.state
    Test_equal( state, nil )
  end

end
