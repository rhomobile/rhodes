require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/fixtures/classes"
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/each_name"

describe "Net::HTTPHeader#each_key" do
  it_behaves_like :net_httpheader_each_name, :each_key
end
