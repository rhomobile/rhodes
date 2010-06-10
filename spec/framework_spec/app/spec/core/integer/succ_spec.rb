require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/next'

describe "Integer#succ" do
  it_behaves_like(:integer_next, :succ)
end
