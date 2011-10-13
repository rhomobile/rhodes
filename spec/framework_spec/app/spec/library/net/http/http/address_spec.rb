require File.expand_path('../../../../../spec_helper', __FILE__)
require 'net/http'

describe "Net::HTTP#address" do
  it "returns the current host name" do
    net = Net::HTTP.new("127.0.0.1")
    net.address.should == "127.0.0.1"
  end
end
