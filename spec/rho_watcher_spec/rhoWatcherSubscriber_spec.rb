require 'rspec'
require_relative "../../lib/build/rhoWatcher"

describe RhoWatcherSubscriber do

  it 'Method buildTask should return build platform task name' do
    subscriber = RhoWatcherSubscriber.new
    subscriber.uri = '192.168.0.1:3000'
    subscriber.platform = 'iphone'
    expect(subscriber.buildTask).to eq('build:iphone:upgrade_package_partial')
  end

  it 'Method buildTask should return build platform task name' do
    subscriber = RhoWatcherSubscriber.new
    subscriber.uri = '192.168.0.1:3000'
    subscriber.platform = 'APPLE'
    expect(subscriber.buildTask).to eq('build:iphone:upgrade_package_partial')
  end

end