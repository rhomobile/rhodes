require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/version_1_2'

describe "Net::HTTP.is_version_1_2?" do
  it_behaves_like :net_http_version_1_2_p, :is_version_1_2?
end
