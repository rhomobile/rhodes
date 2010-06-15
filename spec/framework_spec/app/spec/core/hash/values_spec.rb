require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'

describe "Hash#values" do
  it "returns an array of values" do
    h = new_hash(1 => :a, 'a' => :a, 'the' => 'lang')
    h.values.class.should == Array
    h.values.sort {|a, b| a.to_s <=> b.to_s}.should == [:a, :a, 'lang']
  end
end
