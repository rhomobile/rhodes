require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Kernel#chomp" do
  it "is a private method" do
    Kernel.should have_private_instance_method(:chomp)
  end
end

describe "Kernel#chomp!" do
  it "is a private method" do
    Kernel.should have_private_instance_method(:chomp!)
  end
end

describe "Kernel.chomp" do
  it "needs to be reviewed for spec completeness"
end

describe "Kernel.chomp!" do
  it "needs to be reviewed for spec completeness"
end
