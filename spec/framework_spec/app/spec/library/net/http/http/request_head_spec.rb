require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/fixtures/http_server"
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/request_head"

describe "Net::HTTP#request_head" do
  it_behaves_like :net_ftp_request_head, :request_head
end
