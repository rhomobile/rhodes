require 'rho/rhocontroller'
require 'rhospec'

class SpecController < Rho::RhoController

  #GET /Spec
  def index
    if $tests_done.nil?
      puts "SpecController.index"
      @rhospec = RhoSpec.new
      @rhospec.start

      $tests_done = true
    end
    
    render    
  end
  
end
