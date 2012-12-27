require 'rubygems'

# Set environment to test
ENV['RHO_ENV'] = 'test'
ROOT_PATH = File.expand_path(File.join(File.dirname(__FILE__),'..'))
Bundler.require(:default, ENV['RHO_ENV'].to_sym)

# Try to load vendor-ed rhoconnect, otherwise load the gem
begin
  require 'vendor/rhoconnect/lib/rhoconnect'
rescue LoadError
  require 'rhoconnect'
end

$:.unshift File.join(File.dirname(__FILE__), "..") # FIXME:
# Load our rhoconnect application
require 'application'
include Rhoconnect

require 'rhoconnect/test_methods'

# Monkey patch to fix the following issue:
# /Library/Ruby/Gems/1.8/gems/rspec-core-2.5.1/lib/rspec/core/shared_example_group.rb:45:
# in `ensure_shared_example_group_name_not_taken': Shared example group '...' already exists (ArgumentError)
module RSpec
  module Core
    module SharedExampleGroup
    private
      def ensure_shared_example_group_name_not_taken(name)
      end
    end
  end
end

shared_examples_for "SpecHelper" do
  include Rhoconnect::TestMethods
  
  before(:each) do
    Store.db.flushdb
    Application.initializer(ROOT_PATH)
  end  
end