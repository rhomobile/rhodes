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

describe "rho initializer", :shared => true do

  attr_accessor :rhom, :rho

  before(:all) do
    FileUtils.mkdir_p('build')
    FileUtils.cp_r('spec/syncdbtest.sqlite','build/syncdbtest.sqlite')
    Rho::RhoConfig::add_source("Account", {"url"=>"http://rhosync.rhomobile.com/sources/1", "source_id"=>1})
    Rho::RhoConfig::add_source("Case", {"url"=>"http://rhosync.rhomobile.com/sources/2", "source_id"=>2})
    Rho::RhoConfig::add_source("Employee", {"url"=>"http://rhosync.rhomobile.com/sources/3", "source_id"=>3})
    Object::const_set("SYNC_DB_FILE", "../../build/syncdbtest.sqlite") unless defined? SYNC_DB_FILE
    @rho = Rho::RHO.new(File.dirname(__FILE__) + "/../../../apps/")
    @rhom = Rhom::RhomObjectFactory.new
  end

  after(:all) do
    FileUtils.rm_rf('build')
  end

  def array_print(arr)
    arr.each_with_index do |x,i|
      puts "arr[#{i}] = #{x.inspect}"
    end
  end
end