require 'rho'
puts 'RHO loaded'
Rho::RHO.new
SyncEngine::start
puts 'sleeping for 60 seconds'
sleep(60)