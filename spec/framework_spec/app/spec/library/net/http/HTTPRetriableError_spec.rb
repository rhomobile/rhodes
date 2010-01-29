require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'
require 'net/http'

describe "Net::HTTPRetriableError" do
  it "is a subclass of Net::ProtoRetriableError" do
    Net::HTTPRetriableError.should < Net::ProtoRetriableError
  end
  
  it "includes the Net::HTTPExceptions module" do
    Net::HTTPRetriableError.should < Net::HTTPExceptions
  end
end
