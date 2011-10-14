require File.expand_path('../../../../../spec_helper', __FILE__)
require 'net/http'

describe "Net::HTTP#port" do
  it "returns the current port number" do
    net = Net::HTTP.new("127.0.0.1", NetHTTPSpecs.server_port)
    net.port.should eql(NetHTTPSpecs.server_port)
  end
end
