require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "ARGF.to_s" do
  before :each do
    @file1 = fixture File.join(__rhoGetCurrentDir(), __FILE__), "file1.txt"
    @file2 = fixture File.join(__rhoGetCurrentDir(), __FILE__), "file2.txt"
  end

  after :each do
    ARGF.close
  end

  it "returns 'ARGF'" do
    argv [@file1, @file2] do
      ARGF.to_s.should == "ARGF"
    end
  end
end
