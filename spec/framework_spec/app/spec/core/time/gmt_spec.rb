require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'

describe "Time#gmt?" do
  it "returns true if time represents a time in UTC (GMT)" do
    Time.now.gmt?.should == false
    Time.now.gmtime.gmt?.should == true
  end
end
