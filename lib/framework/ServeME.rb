begin
	require 'rho'
	puts 'RHO loaded'
	Rho::RHO.new
rescue Exception => e
	puts e.message
end