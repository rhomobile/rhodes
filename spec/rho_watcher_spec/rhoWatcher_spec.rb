require "rspec"
require "webmock/rspec"
require "fileutils"
require_relative "../../lib/build/rhoWatcher"


describe RhoWatcher do

  before :each do
    @watcher_test_dir = Dir.mktmpdir
    WebMock.disable_net_connect!(allow_localhost: true)
  end

  after :each do
    FileUtils.remove_dir(@watcher_test_dir)
  end


  it 'Set server uri' do
    @watcher = RhoWatcher.new
    @watcher.serverUri = URI('http://192.168.0.1:3000')
    expect(@watcher.serverUri.host).to eq('192.168.0.1')
    expect(@watcher.serverUri.port).to eq(3000)
  end

  it 'Server root should be created after creating instance of RhoWatcher' do
    @watcher = RhoWatcher.new
    expect(File.exists?(@watcher.serverRoot)).to be true
  end

  it 'Set application root' do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = '/Users/user/projects/project'
    expect(@watcher.applicationRoot).to eq('/Users/user/projects/project')
  end

  it 'Create diff files' do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = @watcher_test_dir

    @watcher.createDiffFiles(["#{@watcher_test_dir}/public/new_file.js"], ["#{@watcher_test_dir}/app/changed_file.rb"], ["#{@watcher_test_dir}/removed_file.html"])
    expect(File.read("#{@watcher_test_dir}/upgrade_package_add_files.txt")).to eq("public/new_file.js\napp/changed_file.rb\n")
    expect(File.read("#{@watcher_test_dir}/upgrade_package_remove_files.txt")).to eq("removed_file.html\n")
  end

  it 'Send notification to device' do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = @watcher_test_dir
    @watcher.serverUri = URI('http://192.168.0.1:3000')
    @watcher.addDevice(RhoDevice.new('192.168.0.1:3001', 'iphone'))
    request = 'http://192.168.0.1:3001/development/update_bundle?http://192.168.0.1:3000/iphone/bundle.zip'
    stub_request(:any, request)

    @watcher.sendNotificationsToDevices
    #expect(WebMock).to have_requested(:get, request)
  end

end