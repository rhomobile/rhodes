#!/usr/bin/env ruby

$LOAD_PATH << "../../lib/extensions/mspec"
# puts $:
require 'mspec/matchers'
require 'mspec/expectations'
require 'mspec/mocks'
require 'mspec/runner'
require 'mspec/runner/mspec'
require 'mspec/guards'
require 'mspec/helpers'

# If the implementation on which the specs are run cannot
# load pp from the standard library, add a pp.rb file that
# defines the #pretty_inspect method on Object or Kernel.
require 'mspec/pp'

require 'mspec/utils/script'
require 'mspec/version'


class SpecRunner < MSpecScript
  def initialize
    super
    @results_path = File.join(File.dirname(__FILE__), 'test_results.xml' )
    # for android path is  /data/data/com.rhomobile.rhoconnect_spec/rhodata/apps/test_results.xml

    @formatter = JUnitFormatter.new(@results_path)
    @formatter.register

    # turn on exception backtrace
    MSpec.backtrace = true
 end

  def run
    MSpec.register_files [ File.join(File.dirname(__FILE__), 'gcm_push_spec.rb') ]
    MSpec.process
    MSpec.exit_code
  end
end

runner = SpecRunner.new
retcode = runner.run

$total  = MSpec.count
$failed = MSpec.exc_count

puts "\nTotal:  " + MSpec.count.to_s
puts "Passed: " + (MSpec.count - MSpec.exc_count).to_s
puts "Failed: " + MSpec.exc_count.to_s

