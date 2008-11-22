require File.dirname(__FILE__) + '/generator_spec_helper'
require 'fileutils'

describe Rhogen::ModelGenerator do

  model_name = 'employee'

  it "should complain if no name is specified" do
    lambda {
      @generator = Rhogen::ModelGenerator.new('/tmp', {})
    }.should raise_error(::Templater::TooFewArgumentsError)
  end

  before do
    @generator = Rhogen::ModelGenerator.new('/tmp', {}, model_name, "http://something.com/sources/5", 5)
  end

  it "should create config.rb, controller.rb, index.erb, edit.erb, and new.erb files" do
    ['config.rb', 'controller.rb', 'index.erb', 'edit.erb', 'new.erb'].each do |template|
      @generator.should create("/tmp/#{model_name.camel_case}/#{template}")
    end
  end
end
