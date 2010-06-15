require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes.rb'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/to_sym.rb'

describe "String#to_sym" do
  it_behaves_like(:string_to_sym, :to_sym)
end