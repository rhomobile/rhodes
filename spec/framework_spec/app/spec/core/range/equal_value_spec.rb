require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/equal_value'

describe "Range#==" do
  it_behaves_like(:range_eql, :==)
end
