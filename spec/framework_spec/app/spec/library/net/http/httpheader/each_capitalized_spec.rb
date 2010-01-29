require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../../spec_helper'
require 'net/http'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/fixtures/classes"
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + "/shared/each_capitalized"

describe "Net::HTTPHeader#each_capitalized" do
  it_behaves_like :net_httpheader_each_capitalized, :each_capitalized
end

