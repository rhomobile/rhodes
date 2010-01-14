require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Numeric#quo" do
  it "returns the result of calling self#/ with other" do
    obj = NumericSub.new
    obj.should_receive(:/).with(19).and_return(:result)
    
    obj.quo(19).should == :result
  end
end
