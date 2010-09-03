require 'rho/mapview'

describe "MapView" do

  it "should create" do
    state = MapView.state
    state.should be_nil

    MapView.create :settings => {:map_type => 'roadmap', :region => [37, -122, 10, 10]}
    w = 5
    #w = 5 if System::get_property('platform') == 'ANDROID'
    sleep w

    state = MapView.state
    state.should_not be_nil
    state.is_a?(Hash).should == true 
    delta = 0.001
    (state[:center][:latitude] - 37.0).abs.should  < delta
    (state[:center][:longitude] - (-122.0)).abs.should  < delta

    MapView.close
    sleep 2

    state = MapView.state
    state.should be_nil
  end

end
