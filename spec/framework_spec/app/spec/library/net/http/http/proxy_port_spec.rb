require File.expand_path('../../../../../spec_helper', __FILE__)
require 'net/http'

describe "Net::HTTP.proxy_port" do
  describe "when self is no proxy class" do
    it "returns nil" do
      Net::HTTP.proxy_port.should be_nil
    end
  end

  describe "when self is a proxy class" do
    it "returns 80 if no port was set for self's proxy connection" do
      Net::HTTP.Proxy("127.0.0.1").proxy_port.should eql(80)
    end

    it "returns the port for self's proxy connection" do
      Net::HTTP.Proxy("127.0.0.1", 1234, "rspec", "rocks").proxy_port.should eql(1234)
    end
  end
end

describe "Net::HTTP#proxy_port" do
  describe "when self is no proxy class instance" do
    it "returns nil" do
      Net::HTTP.new("127.0.0.1", NetHTTPSpecs.server_port).proxy_port.should be_nil
    end
  end

  describe "when self is a proxy class instance" do
    it "returns 80 if no port was set for self's proxy connection" do
      Net::HTTP.Proxy("127.0.0.1").new("127.0.0.1", NetHTTPSpecs.server_port).proxy_port.should eql(80)
    end

    it "returns the port for self's proxy connection" do
      http_with_proxy = Net::HTTP.Proxy("127.0.0.1", 1234, "rspec", "rocks")
      http_with_proxy.new("127.0.0.1", NetHTTPSpecs.server_port).proxy_port.should eql(1234)
    end
  end
end
