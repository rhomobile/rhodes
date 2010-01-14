require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/object_id'

describe "Kernel#__id__" do
  it_behaves_like(:kernel_object_id, :__id__) 
end

describe "Kernel#__id__" do
  it "needs to be reviewed for spec completeness"
end
