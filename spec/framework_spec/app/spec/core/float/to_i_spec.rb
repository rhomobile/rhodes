require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/to_i'

describe "Float#to_i" do
  it_behaves_like(:float_to_i, :to_i)
end
