require File.dirname(__FILE__) + '/generator_spec_helper'
require 'fileutils'

describe Rhogen::ModelGenerator do

  model_name = 'employee'

  it "should complain if no name is specified" do
    lambda {
      @generator = Rhogen::ModelGenerator.new('/tmp', {})
    }.should raise_error(::Templater::TooFewArgumentsError)
  end
  
  it "should require attributes" do
    lambda {
      @generator = Rhogen::ModelGenerator.new('/tmp', {}, model_name)
    }.should raise_error(::Templater::TooFewArgumentsError)
  end
  
  it "should use type argument" do
    @generator = Rhogen::ModelGenerator.new('/tmp', {}, model_name, "name,industry,address", "ask")
    @generator.arguments[2].should == "ask"
  end

  before do
    @generator = Rhogen::ModelGenerator.new('/tmp', {}, model_name, "name,industry,address")
  end
  
  it "should have attributes" do
    @generator.attributes?.should == true
  end
  
  it "should have all attributes" do
    @generator.attributes.should == ['name', 'industry', 'address']
  end
  
  it "should generate valid erb templates" do
    pending "need to figure out how to validate erb"
  end
  
end
