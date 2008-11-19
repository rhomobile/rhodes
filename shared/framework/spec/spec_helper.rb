begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end
 
$:.unshift(File.join(File.dirname(__FILE__), '..'))
 
# Use the rubygem for local testing
require 'spec/stubs'
require 'rho/rho'
require 'rhom/rhom'