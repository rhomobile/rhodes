require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Module#freeze" do
  it "prevents further modifications to self" do
    m = Module.new.freeze
    m.frozen?.should == true

    # Does not raise
    class << m; end

    lambda {
      class << m
        def test() "test" end
      end
    }.should raise_error(TypeError)

    lambda { def m.test() "test" end }.should raise_error(TypeError)
  end
end
