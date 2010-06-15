require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'

describe "Time#to_f" do
  it "returns the float number of seconds + usecs since the epoch" do
    Time.at(100, 100).to_f.should == 100.0001
  end
end
