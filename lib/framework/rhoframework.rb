if defined?( RHO_WP7 )
    RHO_RB_EXT = '.rb'
    RHO_ERB_EXT = '_erb' + RHO_RB_EXT
    RHO_APPS_DIR = 'apps/'
elsif defined?( RHODES_EMULATOR )
    RHO_RB_EXT = '.rb'
    RHO_ERB_EXT = '.erb'
    RHO_APPS_DIR = ''
    RHO_EMULATOR_DIR = 'rhosimulator'    
    module Kernel   
	    def eval_compiled_file(fname, bind)
	        require 'erb'
	        
	        puts "eval_compiled_file : #{fname}"
	        strFile = IO.read(fname)
	        code = ERB.new(strFile).src
	        
		    eval(code, bind)
	    end
    end
else
    RHO_RB_EXT = '.iseq'
    RHO_ERB_EXT = '_erb' + RHO_RB_EXT
    RHO_APPS_DIR = 'apps/'
end

module Rho
  def self.file_exist?(fname)
      return File.exist_injar?(fname) if defined? RHO_ME

	  return __rho_exist_in_resources(fname) if defined? RHO_WP7
	              
      File.exist?(fname)
  end
end

if defined? RHO_WP7

module Kernel   
	def __rhoGetCurrentDir
		""
	end

	alias :orig_require :require

	def require(file_name)
		 file_name += '.rb' unless file_name.end_with?('.rb')
		 orig_require( file_name )
	end

	def eval_compiled_file(fname, bind)
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

if defined?( RHODES_EMULATOR )
    require 'debugger'
end
	
    puts 'RHO loaded'
    RHO_FRAMEWORK = Rho::RHO.new
rescue Exception => e
    trace_msg = e.backtrace.join("\n")
    puts 'Create RHO framework failed: ' + e.inspect + ";Trace: #{trace_msg}"
end    
