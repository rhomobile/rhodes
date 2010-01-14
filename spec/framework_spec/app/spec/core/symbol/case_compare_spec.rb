require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe "Symbol#===" do
  it "returns true when the other is a Symbol" do
    (Symbol === :ruby).should == true
    (Symbol === :"ruby").should == true
    (Symbol === :'ruby').should == true
    (Symbol === 'ruby').should == false
  end
end

# Symbol === :fnord