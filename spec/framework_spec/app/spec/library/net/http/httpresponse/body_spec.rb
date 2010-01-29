require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/body"

describe "Net::HTTPResponse#body" do
  it_behaves_like :net_httpresponse_body, :body
end
