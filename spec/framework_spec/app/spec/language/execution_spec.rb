require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../spec_helper'

describe "``" do
  platform_is_not :android do
    it "returns the output of the executed sub-process" do
      ip = 'world'
      `echo disc #{ip}`.should == "disc world\n"
    end
  end
end

describe "%x" do
  platform_is_not :android do
    it "is the same as ``" do
      ip = 'world'
      %x(echo disc #{ip}).should == "disc world\n"
    end
  end
end
