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
  
  it "should create index.html and application.rb files" do
    puts "#{@generator.inspect}"
    @generator.should create("/tmp/#{app_name}/index.html")
    @generator.should create("/tmp/#{app_name}/application.rb")
  end
end
