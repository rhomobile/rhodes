require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "Dir" do
  it "includes Enumerable" do
    Dir.include?(Enumerable).should == true
  end
end
