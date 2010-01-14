require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "ENV.to_s" do
  it "returns \"ENV\"" do
    ENV.to_s.should == "ENV"
  end
end
