require 'mspec'

class SpecRunner < MSpecScript
  def initialize
    config[:files] = []
	  
	results_path = File.join( Rho::RhoApplication.get_base_app_path(), 'test_results.xml' )
	@formatter = JUnitFormatter.new(results_path)
	@formatter.register
	  
	# turn on exception backtrace
	MSpec.backtrace = true

	config[:files] << [ "spec/syncengine_spec", [ {:schema_model=>true }, {:schema_model=>false } ] ]
	config[:files] << [ "spec/blobsync_spec", [ {:schema_model=>true }, {:schema_model=>false } ] ]

if !defined?(RHO_WP7)
	config[:files] << "spec/bulksync_spec"
end
	config[:files] << "spec/blob_bulksync_spec"

  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
  
end
