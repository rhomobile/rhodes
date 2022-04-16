module HostPlatform
	def windows?
		RUBY_PLATFORM =~ /(win|w)32$/
	end
	module_function :windows?

	def linux?
		RUBY_PLATFORM =~ /linux/
	end
	module_function :linux?

	def mac?
		RUBY_PLATFORM =~ /darwin/
	end
	module_function :mac?

	def exe_ext
		if HostPlatform.windows?
			'.exe'
		else
			''
		end
	end
	module_function :exe_ext

	def bat_ext
		if HostPlatform.windows?
			'.bat'
		else
			''
		end
	end
	module_function :bat_ext

	def num_cpus
		num = nil
	 	if HostPlatform.linux?
	    	num = `cat /proc/cpuinfo | grep processor | wc -l`.gsub("\n", '')
	 	elsif HostPlatform.mac?
	    	num = `sysctl -n hw.ncpu`.gsub("\n", '')
	 	elsif HostPlatform.windows?
	    	num = ENV['NUMBER_OF_PROCESSORS']
	  	else
	    	num = 1
	  	end
	  	num = num.to_i
	  	num = 1 if num == 0
	  	num
	end
	module_function :num_cpus
end
