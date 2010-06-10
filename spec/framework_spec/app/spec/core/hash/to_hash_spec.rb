require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Hash#to_hash" do
  it "returns self" do
    h = new_hash
    h.to_hash.should equal(h)
  end
end
