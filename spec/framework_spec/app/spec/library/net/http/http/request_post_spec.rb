require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/fixtures/http_server"
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/request_post"

describe "Net::HTTP#request_post" do
  it_behaves_like :net_ftp_request_post, :request_post
end
