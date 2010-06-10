require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "NilClass#to_a" do
  it "returns an empty array" do
    nil.to_a.should == []
  end
end
