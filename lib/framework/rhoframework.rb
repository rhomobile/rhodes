module Rho
  def self.file_exist?(fname)
      return File.exist_injar?(fname) if defined? RHO_ME

	  if defined? RHO_WP7
	     if fname.end_with?('_erb.iseq')
			fname = fname[0,fname.length-9] + '_erb.rb'
		 elsif fname.end_with?('.iseq')
			fname = fname[0,fname.length-5] + '.rb'
		 end
	  end
	              
      File.exist?(fname)
  end
end

if defined? RHO_WP7

module Kernel   
	def __rhoGetCurrentDir
		"/rho"
	end

	alias :orig_require :require

	def require(file_name)
		 file_name += '.rb' unless file_name.end_with?('.rb')
		 orig_require( file_name )
	end

	def eval_compiled_file(fname, bind)
		if fname.end_with?('_erb.iseq')
			fname = fname[0,fname.length-9] + '_erb.rb'
		end

	    code = IO.read(fname)
		eval(code, bind)
	end
end

end

begin
	if defined? RHO_WP7
		require 'rhosystem'
	end

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

    require 'rho/rho'
    require 'rho/render'
    
    require 'rho'
	
    puts 'RHO loaded'
    RHO_FRAMEWORK = Rho::RHO.new
rescue Exception => e
    trace_msg = e.backtrace.join("\n")
    puts 'Create RHO framework failed: ' + e.inspect + ";Trace: #{trace_msg}"
end    
