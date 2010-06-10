require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/getc'

describe "ARGF.getc" do
  it_behaves_like :argf_getc, :readchar
end

describe "ARGF.readchar" do
  before :each do
    @file1 = fixture File.join(__rhoGetCurrentDir(), __FILE__), "file1.txt"
    @file2 = fixture File.join(__rhoGetCurrentDir(), __FILE__), "file2.txt"
  end

  after :each do
    ARGF.close
  end

  it "raises EOFError when end of stream reached" do
    argv [@file1, @file2] do
      lambda { while c = ARGF.readchar; end }.should raise_error(EOFError)
    end
  end
end
