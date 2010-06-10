require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Kernel#taint" do
  it "sets self to be tainted" do
    mock('tainted?').taint.tainted?.should == true
  end

  it "has no effect on immediate values" do
    inmediate_values = [a = nil, b = true, c = false, d = 1, e = "a string"]
    inmediate_values.each{ |v| v.tainted?.should == false}
  end
end
