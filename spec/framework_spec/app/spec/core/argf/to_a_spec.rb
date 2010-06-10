require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/readlines'

describe "ARGF.to_a" do
  it_behaves_like :argf_readlines, :to_a
end
