require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/collect'

describe "Array#collect" do
  it_behaves_like(:array_collect, :collect)
end

describe "Array#collect!" do
  it_behaves_like(:array_collect_b, :collect!)
end
