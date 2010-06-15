require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes.rb'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/concat.rb'

describe "String#concat" do
  it_behaves_like :string_concat, :concat
end

describe "String#concat with Fixnum" do
  it_behaves_like :string_concat_fixnum, :concat
end
