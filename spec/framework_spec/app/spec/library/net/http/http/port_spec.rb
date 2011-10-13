require File.expand_path('../../../../../spec_helper', __FILE__)
require 'net/http'

describe "Net::HTTP#port" do
  it "returns the current port number" do
    net = Net::HTTP.new("127.0.0.1", 3454)
    net.port.should eql(3454)
  end
end
