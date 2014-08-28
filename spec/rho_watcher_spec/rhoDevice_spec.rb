require 'rspec'
require_relative "../../lib/build/rhoWatcher"

describe RhoDevice do

  it 'return build platform task name' do
    device = RhoDevice.new('192.168.0.1:3000','iphone')
    expect(device.buildTask).to eq('build:iphone:upgrade_package_partial')
  end

end