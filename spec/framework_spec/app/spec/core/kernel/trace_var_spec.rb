require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Kernel#trace_var" do
  it "is a private method" do
    Kernel.should have_private_instance_method(:trace_var)
  end
end

describe "Kernel.trace_var" do
  it "needs to be reviewed for spec completeness"
end
