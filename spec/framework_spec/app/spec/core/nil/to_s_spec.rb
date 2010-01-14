require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "NilClass#to_s" do
  it "returns the string ''" do
    nil.to_s.should == ""
  end
end
