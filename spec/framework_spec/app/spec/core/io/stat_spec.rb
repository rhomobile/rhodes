require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "IO#stat" do
  it "raises IOError on closed stream" do
    lambda { IOSpecs.closed_io.stat }.should raise_error(IOError)
  end

  it "returns a File::Stat object for the stream" do
    io = IO.new $stderr.fileno
    io.stat.class.should == File::Stat
  end
end
