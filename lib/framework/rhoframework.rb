begin
    require 'rational'
    require 'date/format'
    require 'time'

   # require 'sqlite3/constants'
   # require 'sqlite3/errors'
   # require 'sqlite3/pragmas'
    
   # require 'sqlite3/resultset'
   # require 'sqlite3/statement'
    
   # require 'date'
   # require 'sqlite3/translator'
    
   # require 'sqlite3/value'

   # require 'sqlite3/database'
    require 'rhom/rhom_db_adapter'
	
    require 'rhom/rhom_object'
    require 'rhofsconnector'
	
    require 'rhom/rhom_object_factory'
    
    require 'rhom/rhom'
    require 'rhom'
    
    require 'rho/rhoapplication'
    
    require 'rho/rho'
    require 'rho'
	
    puts 'RHO loaded'
    Rho::RHO.new
rescue Exception => e
    puts e.message
end