require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Numeric#modulo" do 
  it "returns the result of calling self#% with other" do
    obj = NumericSub.new
    obj.should_receive(:%).with(20).and_return(:result)
    
    obj.modulo(20).should == :result
  end
end
