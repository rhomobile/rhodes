require "rspec"
require "webmock/rspec"
require "fileutils"
require_relative "../../lib/build/rhoWatcher"


describe "Serve behaviour" do

  before :each do
    @watcher_test_dir = Dir.mktmpdir
    # WebMock.disable_net_connect!(allow_localhost: true)
=begin
    @watcher = RhoWatcher.new
    @watcher.serverUri = "192.168.0.1"
    @watcher.serverRoot = "watcher_test_dir"
    @watcher.addDirectory(watcher_test_dir)
=end
  end

  after :each do
    @watcher.stop
    FileUtils.remove_dir(@watcher_test_dir)
  end


  it "Set server uri" do
    @watcher = RhoWatcher.new
    @watcher.serverUri = "192.168.0.1"
    expect(@watcher.serverUri).to eq("192.168.0.1")
  end

  it "Set server root" do
    @watcher = RhoWatcher.new
    @watcher.serverRoot = "/usr/server"
    expect(@watcher.serverRoot).to eq("/usr/server")
  end

  it "Set application root" do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = "/usr/server"
    expect(@watcher.applicationRoot).to eq("/usr/server")
  end

  it "Create diff files" do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = @watcher_test_dir

    @watcher.createDiffFiles(["new_file.js"], ["changed_file.rb"], ["removed_file.html"])
    expect(File.read(@watcher_test_dir + "/upgrade_package_add_files.txt")).to eq("new_file.js\nchanged_file.rb\n")
    expect(File.read(@watcher_test_dir + "/upgrade_package_remove_files.txt")).to eq("removed_file.html\n")
  end

  xit "Create bundles and copy they to server root" do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = @watcher_test_dir
    @watcher.serverRoot = @watcher_test_dir + "/server_root"
    @watcher.addDevice(RhoDevice.new("192.168.0.1:6000", "android"))
    FileUtils.mkpath @watcher_test_dir + "/bin/target/android/"
    File.open(@watcher_test_dir + "/bin/target/android/bundle.zip", "w") {}

    @watcher.copyBundlesToServerRoot()
    expect(File.file?(@watcher_test_dir + "/server_root/android/bundle.zip")).to be true
  end

  it "Send notification to device" do
    @watcher = RhoWatcher.new
    @watcher.applicationRoot = @watcher_test_dir
    @watcher.serverUri = "192.168.0.1:3000"
    @watcher.serverRoot = @watcher_test_dir + "/server_root"
    @watcher.addDevice(RhoDevice.new("192.168.0.1:6000", "android"))
    request = "http://192.168.0.1:6000/system/update_bundle?http://" + @watcher.serverUri + "/android/bundle.zip"
    stub_request(:any, request)

    @watcher.sendNotificationsToDevices
    expect(WebMock).to have_requested(:get, request)
  end

end