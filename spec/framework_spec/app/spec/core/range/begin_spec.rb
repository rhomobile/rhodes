require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/begin'

describe "Range#begin" do
  it_behaves_like(:range_begin, :begin)
end
