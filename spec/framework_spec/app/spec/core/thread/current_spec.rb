require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Thread.current" do
  it "returns a thread" do
    current = Thread.current
    current.class.should == Thread
  end

  it "returns the current thread" do
    t = Thread.new { Thread.current }
    t.value.should equal(t)
    Thread.current.should_not equal(t.value)
  end
end
