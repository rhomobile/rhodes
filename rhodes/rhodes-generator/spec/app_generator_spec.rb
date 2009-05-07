require File.dirname(__FILE__) + '/generator_spec_helper'

describe Rhogen::AppGenerator do

  app_name = 'neatapp'

  it "should complain if no name is specified" do
    lambda {
      @generator = Rhogen::AppGenerator.new('/tmp', {})
    }.should raise_error(::Templater::TooFewArgumentsError)
  end


  it "should generate class_name" do
    @generator = Rhogen::AppGenerator.new('/tmp', {}, 'Class-With-Hyphens')
    @generator.class_name.should == 'ClassWithHyphens'
  end

  before do
    @generator = Rhogen::AppGenerator.new('/tmp', {}, app_name)
  end

  it "should create application.rb, index.erb, and layout.erb files" do
    ['application.rb', 'index.erb', 'layout.erb'].each do |template|
      @generator.should create("/tmp/#{app_name}/app/#{template}")
    end
  end
  
  it "should create rhoconfig.txt, build.yml, Rakefile" do
    ['Rakefile', 'rhoconfig.txt', 'build.yml'].each do |template|
      @generator.should create("/tmp/#{app_name}/#{template}")
    end
  end

  it "should generate valid erb templates" do
    pending "need to figure out how to validate erb"
  end

end
