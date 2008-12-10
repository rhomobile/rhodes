require File.dirname(__FILE__) + '/generator_spec_helper'

describe Rhogen::SourceGenerator do
  
  source_name = 'NeatSource'

  it "should complain if no name is specified" do
    lambda {
      @generator = Rhogen::SourceGenerator.new('/tmp', {})
    }.should raise_error(::Templater::TooFewArgumentsError)
  end
  
  before do
    @generator = Rhogen::SourceGenerator.new('/tmp', {}, source_name)
  end
  
  it "should create neat_source.rb files" do
    ['neat_source.rb'].each do |template|
      @generator.should create("/tmp/lib/#{template}")
    end
  end
  
  it "should generate valid erb templates" do
    pending "need to figure out how to validate erb"
  end
  
end
