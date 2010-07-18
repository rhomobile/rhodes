require 'rho/rhocontroller'
require 'rhospec'

class SpecController < Rho::RhoController

  #GET /Spec
  def index
    if $rhospec.nil?
      puts "SpecController.index"
      $rhospec = RhoSpec.new
      $rhospec.start

    end
    
    render    
  end
  
end
