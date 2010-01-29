require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/request_put"

describe "Net::HTTP#put2" do
  it_behaves_like :net_ftp_request_put, :put2
end
