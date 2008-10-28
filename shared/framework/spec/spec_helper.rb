begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

$:.unshift(File.join(File.dirname(__FILE__), '..'))
require 'spec/stubs'

# Use the rubygem for local testing
require 'rubygems'
require 'sqlite3'
require 'rhom/rhom'