begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

require File.join(File.expand_path(__FILE__), 'stubs')

# Use the rubygem for local testing
require 'rubygems'
require 'sqlite3'
require 'rhom/rhom'