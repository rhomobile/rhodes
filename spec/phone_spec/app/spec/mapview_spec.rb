require 'rho/mapview'

describe "MapView" do

  it "should create" do
    state = MapView.state
    state.should be_nil

    MapView.create :settings => {:map_type => 'roadmap', :region => [37, -122, 10, 10]}
    w = 5
    #w = 5 if System::get_property('platform') == 'ANDROID'
    sleep 20

    state = MapView.state
    state.should_not be_nil
    state.is_a?(Hash).should == true 
    delta = 0.001
    (state[:center][:latitude] - 37.0).abs.should  < delta
    (state[:center][:longitude] - (-122.0)).abs.should  < delta

    MapView.close
    sleep 20

    state = MapView.state
    state.should be_nil
  end

  it "should create 500 annotations" do
    state = MapView.state
    state.should be_nil

     myannotations = []
     500.times do |j|
          annotation = {:latitude => '37.349691', :longitude => '-121.983261', :title => "Test Location", :subtitle => "test", :url => "/app/GeoLocation/show?city=Current Location"}	
          myannotations << annotation
     end

     myannotations <<   {:street_address => "Cupertino, CA 95014", :title => "Cupertino", :subtitle => "zip: 95014", :url => "/app/GeoLocation/show?city=Cupertino"}
     myannotations << {:street_address => "Santa Clara, CA 95051", :title => "Santa Clara", :subtitle => "zip: 95051", :url => "/app/GeoLocation/show?city=Santa%20Clara"}

     map_params = {
          :settings => {:map_type => "roadmap", :region => [37, -122, 10, 10],
                        :zoom_enabled => true, :scroll_enabled => true, :shows_user_location => true, :api_key => '0tLch9tZSX1G7PQRWwmqvINECUGtJ8ReArpPJxw'},
          :annotations => myannotations
     }

    MapView.create map_params

    w = 5
    #w = 5 if System::get_property('platform') == 'ANDROID'
    sleep 40

    state = MapView.state
    state.should_not be_nil
    state.is_a?(Hash).should == true 
    delta = 0.001
    (state[:center][:latitude] - 37.0).abs.should  < delta
    (state[:center][:longitude] - (-122.0)).abs.should  < delta

    MapView.close
    sleep 20

    state = MapView.state
    state.should be_nil
  end




end
