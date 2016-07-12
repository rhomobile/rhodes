require 'rho/rhocontroller'
require 'spec_runner'

class SpecRunnerController < Rho::RhoController

  #GET /SpecRunner
  def index
    @msg = 'MSpec version: '+MSpec::VERSION
    @code = 0
    render
  end

  def run_specs
    GC.enable()    
    @runner = SpecRunner.new
    @code = @runner.run
    @exc_count = MSpec.exc_count
    @count = MSpec.count
    @errorMessages = MSpec.errorMessages

    total = @count.to_s
    passed = (@count - @exc_count).to_s
    failed = @exc_count.to_s
    messages = @errorMessages

    puts "***Total:  " + total
    puts "***Passed: " + passed
    puts "***Failed: " + failed

    render(string: "{ total:#{total}, passed:#{passed}, failed:#{failed}, messages:#{messages} }")
  end

end