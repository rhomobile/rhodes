require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "Array" do
  it "includes Enumerable" do
    Array.ancestors.include?(Enumerable).should == true
  end
end
