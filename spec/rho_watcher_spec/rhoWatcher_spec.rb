require "rspec"
require "webmock/rspec"
require "fileutils"
require_relative "../../lib/build/rhoWatcher"


describe RhoWatcher do

  watcher_test_dir = nil
  watcher = nil
  subscriber = nil

  before :each do
    watcher_test_dir = Dir.mktmpdir
    watcher = RhoWatcher.new
    subscriber = RhoWatcherSubscriber.new
    subscriber.uri = '192.168.0.1:3001'
    subscriber.platform = 'iphone'
    WebMock.disable_net_connect!(allow_localhost: true)
  end

  after :each do
    FileUtils.remove_dir(watcher_test_dir)
  end


  it 'Set server uri' do
    watcher.serverUri = URI('http://192.168.0.1:3000')
    expect(watcher.serverUri.host).to eq('192.168.0.1')
    expect(watcher.serverUri.port).to eq(3000)
  end

  it 'Directory for server root should be created after creating instance of RhoWatcher' do
    expect(File.exist?(watcher.serverRoot)).to be true
  end

  it 'Set application root' do
    watcher.applicationRoot = '/Users/user/projects/project'
    expect(watcher.applicationRoot).to eq('/Users/user/projects/project')
  end

  it 'Create diff files' do
    watcher.applicationRoot = watcher_test_dir

    watcher.createDiffFiles(["#{watcher_test_dir}/public/new_file.js"], ["#{watcher_test_dir}/app/changed_file.rb"], ["#{watcher_test_dir}/removed_file.html"])
    expect(File.read("#{watcher_test_dir}/upgrade_package_add_files.txt")).to eq("public/new_file.js\napp/changed_file.rb\n")
    expect(File.read("#{watcher_test_dir}/upgrade_package_remove_files.txt")).to eq("removed_file.html\n")
  end

  it 'Method hasDeviceWithUri should return true if subscriber with specified uri was added' do
    expect(watcher.hasSubscriberWithUri(subscriber.uri)).to be false

    watcher.addSubscriber(subscriber)
    expect(watcher.hasSubscriberWithUri(subscriber.uri)).to be true
  end

  it 'Method removeDeviceWithUri should remove subscriber' do
    watcher.addSubscriber(subscriber)
    expect(watcher.hasSubscriberWithUri(subscriber.uri)).to be true

    expect(watcher.removeSubsriberWithUri(subscriber.uri))
    expect(watcher.hasSubscriberWithUri(subscriber.uri)).to be false
  end

  it 'Send notification to subscriber' do
    watcher.applicationRoot = watcher_test_dir
    watcher.serverUri = URI('http://192.168.0.1:3000')

    watcher.addSubscriber(subscriber)
    request = 'http://192.168.0.1:3001/development/update_bundle?http://192.168.0.1:3000/iphone/bundle.zip'
    stub_request(:any, request)

    watcher.notifySubscribers
    #expect(WebMock).to have_requested(:get, request)
  end

end