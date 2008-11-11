require 'rho'
puts 'RHO loaded'
r = Rho::RHO.new
req = Hash.new
req['application']="Rhosugar"
req['model'] = 'Account'
req['request-method']='GET'
puts r.serve(req)

