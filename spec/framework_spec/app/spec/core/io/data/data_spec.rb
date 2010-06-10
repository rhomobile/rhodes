require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'

describe "DATA" do
  before :each do
    @data1_name = fixture File.join(__rhoGetCurrentDir(), __FILE__), "data1.rb"
  end

# XXX We dont have an external interpreter
#  it "presents $0 file data after __END__ as a File object" do
#    data = ruby_exe(@data1_name)
#    data.should == "[File, \"hello\"]"
#  end
end