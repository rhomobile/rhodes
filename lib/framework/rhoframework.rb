begin

    if defined? RHO_ME
	    require 'rationalME'
    else	
        require 'rational'
    end
    
    require 'rhom/rhom_db_adapter'
	
    require 'rhom/rhom_object'
    require 'rhofsconnector'
	
    require 'rhom/rhom_object_factory'
    
    require 'rhom/rhom'
    require 'rhom'

    require 'rho/rhoapplication'

    require 'indifferent_access'
    require 'rho/rhosupport'    
    require 'rho/rhocontroller'    
    require 'rho/render'

    require 'rho/rho'
    require 'rho'
	
    puts 'RHO loaded'
    Rho::RHO.new
rescue Exception => e
    puts e.message
end