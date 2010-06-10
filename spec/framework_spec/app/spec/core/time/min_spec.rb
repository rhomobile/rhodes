require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/methods'

describe "Time#min" do
  it "returns the minute of the hour (0..59) for time" do
    with_timezone("CET", 1) do
      Time.at(0).min.should == 0
    end
  end
end
