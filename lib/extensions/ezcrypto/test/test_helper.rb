$:.unshift(File.dirname(__FILE__) + '/../lib')

require 'test/unit'
require 'rubygems'
require 'action_pack'
require 'active_record'
#require 'active_support/binding_of_caller'
#require 'active_support/breakpoint'


unless defined?(RAILS_ROOT)
  root_path = File.join(File.dirname(__FILE__), '..')
  unless RUBY_PLATFORM =~ /mswin32/
    require 'pathname'
    root_path = Pathname.new(root_path).cleanpath.to_s
  end
  RAILS_ROOT = root_path
end


require 'active_record/fixtures'
require 'initializer'

Rails::Initializer.run(:set_load_path)

ActiveRecord::Base.logger = Logger.new(File.dirname(__FILE__) + "/debug.log")
ActiveRecord::Base.establish_connection(
    :adapter  => "mysql",
    :host     => "localhost",
    :username => "root",
    :password => "",
    :database => "crypto_test"
  )
#load(File.dirname(__FILE__) + "/schema.rb")

