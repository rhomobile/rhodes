module MapView
  def self.state
    return nil unless MapView.state_started
    state = {:center => {:latitude => MapView.state_center_lat, :longitude => MapView.state_center_lon}}
    state
  end
end
