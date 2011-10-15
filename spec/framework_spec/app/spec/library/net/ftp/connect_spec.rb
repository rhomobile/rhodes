require File.expand_path('../../../../spec_helper', __FILE__)
require 'net/ftp'
require File.expand_path('../fixtures/server', __FILE__)

# TODO: Add specs for using the SOCKSSocket
describe "Net::FTP#connect" do
  before(:each) do
    @server = NetFTPSpecs::DummyFTP.new
    @server.serve_once

    @ftp = Net::FTP.new
  end

  after(:each) do
    @server.connect_message = nil
    @ftp.quit rescue nil
    @ftp.close
    @server.stop
  end

  it "tries to connect to the FTP Server on the given host and port" do
    lambda { @ftp.connect("127.0.0.1", 9876) }.should_not raise_error
  end

  it "returns nil" do
    @ftp.connect("127.0.0.1", 9876).should be_nil
  end

  it "prints a small debug line when in debug mode" do
    @ftp.debug_mode = true
    lambda { @ftp.connect("127.0.0.1", 9876) }.should output(/#{"connect: 127.0.0.1, 9876\\nget: 220 Dummy FTP Server ready!"}/)
    @ftp.debug_mode = false
  end

  it "does not raise any error when the response code is 220" do
    @server.connect_message = "220 Dummy FTP Server ready!"
    lambda { @ftp.connect("127.0.0.1", 9876) }.should_not raise_error
  end

  it "raises a Net::FTPReplyError when the response code is 120" do
    @server.connect_message = "120 Service ready in nnn minutes."
    lambda { @ftp.connect("127.0.0.1", 9876) }.should raise_error(Net::FTPReplyError)
  end

  it "raises a Net::FTPTempError when the response code is 421" do
    @server.connect_message = "421 Service not available, closing control connection."
    lambda { @ftp.connect("127.0.0.1", 9876) }.should raise_error(Net::FTPTempError)
  end
end
