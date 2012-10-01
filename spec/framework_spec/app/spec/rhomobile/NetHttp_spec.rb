require File.expand_path('../../spec_helper', __FILE__)
require 'net/https'

describe "NetHttp#https" do

  it "gets web page content by https" do

    uri = URI('https://docs.rhomobile.com/')

    req = Net::HTTP::Get.new(uri.request_uri)
    res = Net::HTTP.start(uri.host, uri.port,
                   :use_ssl => uri.scheme == 'https',
                   :verify_mode => OpenSSL::SSL::VERIFY_NONE) do |http|
      http.request req
    end

    res.code.should == "200"
    res.message.should == "OK"
    res.class.name.should == "Net::HTTPOK"
  end

end
