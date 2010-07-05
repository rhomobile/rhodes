require 'rho/rhocontroller'
require 'rhospec'

class SpecController < Rho::RhoController

  #GET /Spec
  def index
    @rhospec = RhoSpec.new
    @rhospec.start
    
    render    
  end
  
end
