require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes.rb'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/succ.rb'

describe "String#succ" do
  it_behaves_like(:string_succ, :succ)
end

describe "String#succ!" do
  it_behaves_like(:string_succ_bang, :"succ!")
end