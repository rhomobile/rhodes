require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "IO#fcntl" do
  it "raises IOError on closed stream" do
    lambda { IOSpecs.closed_file.fcntl(5, 5) }.should raise_error(IOError)
  end
end
