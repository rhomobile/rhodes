require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "Fixnum#~" do
  it "returns self with each bit flipped" do
    (~0).should == -1
    (~1221).should == -1222
    (~-2).should == 1
    (~-599).should == 598
  end
end
