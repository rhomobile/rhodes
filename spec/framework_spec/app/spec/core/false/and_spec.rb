require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "FalseClass#&" do
  it "returns false" do
    (false & false).should == false
    (false & true).should == false
    (false & nil).should == false
    (false & "").should == false
    (false & mock('x')).should == false
  end
end
