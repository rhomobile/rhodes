require File.dirname(__FILE__) + '/generator_spec_helper'

describe Rhogen::AppGenerator do
  
  app_name = 'NeatApp'

  it "should complain if no name is specified" do
    lambda {
      @generator = Rhogen::AppGenerator.new('/tmp', {})
    }.should raise_error(::Templater::TooFewArgumentsError)
  end
  
  before do
    @generator = Rhogen::AppGenerator.new('/tmp', {}, app_name)
  end
  
  it "should create application.rb and index.html files" do
    ['application.rb', 'index.html'].each do |template|
      @generator.should create("/tmp/#{app_name}/#{template}")
    end
  end
end
