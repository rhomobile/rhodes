require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "Symbol#to_sym" do
  it "returns self" do
    [:rubinius, :squash, :[], :@ruby, :@@ruby].each do |sym|
      sym.to_sym.should == sym
    end
  end
end
