if Rho::RHO.use_new_orm
	puts "loading newrhom"
	require 'newrhom/newrhom'
else
	require 'rhom/rhom'
	require 'rhom/rhom_source'
end