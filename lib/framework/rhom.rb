if Rho::RHO.use_new_rhom
	puts "loading newrhom"
	require 'newrhom/newrhom'
else
	require 'rhom/rhom'
	require 'rhom/rhom_source'
end