module MapView
  def self.state
    state = nil
    if MapView.state_started
      state = {}
      state[:center] = {}
      state[:center][:latitude] = MapView.state_center_lat
      state[:center][:longitude] = MapView.state_center_lon
    end
    state
  end
end
