$:.push File.join(File.dirname(__FILE__), '..', 'lib')

require 'rubygems'
require 'spec'
require 'templater/spec/helpers'

$:.push File.join(File.dirname(__FILE__), '..', 'generators')
require 'rhogen'

Spec::Runner.configure do |config|
  config.include Templater::Spec::Helpers
end