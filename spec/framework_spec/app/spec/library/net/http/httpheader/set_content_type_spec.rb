require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/fixtures/classes"
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/set_content_type"

describe "Net::HTTPHeader#set_content_type" do
  it_behaves_like :net_httpheader_set_content_type, :set_content_type
end
