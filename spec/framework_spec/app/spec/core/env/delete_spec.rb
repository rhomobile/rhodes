require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "ENV.delete" do
  it "removes the variable and return its value" do
    ENV["foo"] = "bar"
    ENV.delete("foo").should == "bar"
    ENV["foo"].should == nil
  end
end
