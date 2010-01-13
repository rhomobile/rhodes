
require 'rho_stubs'
require 'mspec'

require 'mspec/utils/script'
require 'mspec/version'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []

    # turn on exception backtrace
    MSpec.backtrace = false

    #Run all the framework specs we can from the spec app
    spec_folder = File.expand_path(File.dirname(__FILE__) + "/../../spec/framework_spec/app/spec")

    config[:files] <<  spec_folder + '/rhoerror_spec.rb'
    config[:files] <<  spec_folder + '/rhoruby_spec.rb'
    config[:files] <<  spec_folder + '/bsearch_spec.rb'
    config[:files] <<  spec_folder + '/find_spec.rb'
    
  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
end

runner = SpecRunner.new


    retcode = runner.run
    puts "\nMSpec version: "+MSpec::VERSION
    puts "\nTotal:  " + MSpec.count.to_s
    puts "Passed: " + (MSpec.count - MSpec.exc_count).to_s
    puts "Failed: " + MSpec.exc_count.to_s

