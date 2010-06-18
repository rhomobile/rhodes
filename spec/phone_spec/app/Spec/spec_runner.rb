require 'rho/rhocontroller'

class SpecController < Rho::RhoController

  def run_all_tests
    run_spec('AsyncHttp')
    run_spec('Json')
    run_spec('Xml')
    run_spec('Contacts')
    run_spec('RhoFile')
    run_spec('MapView')
    run_spec('NativeBar')
    run_spec('NavBar')
  end
  
end
