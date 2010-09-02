require 'mspec'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
    # turn on exception backtrace
    MSpec.backtrace = true

    #MSpec.guard
    
    #LANGUAGE
    app_folder = Rho::RhoFSConnector.get_app_path('app')
    app_folder.gsub!(/\\/, '/')
    
    specs =  app_folder + "spec/language/**/*_spec.iseq"
    
    Dir.glob(specs) do |file|
      file.gsub!(app_folder,"")
      file.gsub!(/\.iseq/,"")
      # Temporary disable for_spec until https://www.pivotaltracker.com/story/show/3876583 will be fixed
      next if file =~ /\/for_spec$/
      
      if ( System.get_property('platform') == 'WINDOWS' )
        next if file =~ /\/execution_spec$/      
        next if file =~ /\/file_spec$/      
      end
      
      config[:files] << file
    end
    # CORE
    core = []

    core << 'argf' unless System.get_property('platform') == 'WINDOWS'
    core << 'class'
    core << 'exception'
    core << 'float'
    # TODO: enable it, right now it reports many fails because of encoding mismatch
    #core << 'marshal'
    core << 'numeric'
    core << 'range'
    core << 'systemexit'
    core << 'array'
    core << 'comparable'
    core << 'false'
    core << 'gc'
    core << 'matchdata'
    core << 'object'
    core << 'regexp'
    core << 'basicobject'
    core << 'continuation'
    core << 'hash'
    core << 'math'
    core << 'objectspace'
    core << 'signal'
    # TODO: see if thread relating testing could be enabled
    #core << 'thread'
    #core << 'threadgroup'
    #core << 'fiber'
    core << 'bignum'
    core << 'dir'
    core << 'file'
    core << 'integer'
    core << 'method'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3915975 will be fixed
    #core << 'precision'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3916078 will be fixed
    #core << 'string'
    core << 'binding'
    core << 'enumerable'
    core << 'filetest'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3915946 will be fixed
    #core << 'io'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3916275 will be fixed
    #core << 'module'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3916389 will be fixed
    #core << 'proc'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3916570 will be fixed
    #core << 'struct'
    #core << 'fixnum'
    #core << 'time'
    #core << 'symbol'
    core << 'true'
    core << 'builtin_constants'
    core << 'env' unless System.get_property('platform') == 'WINDOWS'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3916324 will be fixed
    #core << 'kernel'
    core << 'nil'
    core << 'process' unless System.get_property('platform') == 'WINDOWS'
    core << 'unboundmethod'

    core.each do |folder|
      specs =  app_folder + "spec/core/#{folder}/**/*_spec.iseq"
      Dir.glob(specs) do |file|
        file.gsub!(app_folder,"")
        file.gsub!(/\.iseq/,"")
        
        if ( System.get_property('platform') == 'WINDOWS' )
          next if file =~ /\/file\/expand_path_spec$/      
          next if file =~ /\/file\/executable_real_spec$/      
          next if file =~ /\/file\/lstat_spec$/      
          next if file =~ /\/file\/mtime_spec$/      
          next if file =~ /\/file\/new_spec$/      
          next if file =~ /\/file\/open_spec$/      
          next if file =~ /\/file\/readable_real_spec$/                
          next if file =~ /\/file\/setgid_spec$/                
          next if file =~ /\/file\/readlink_spec$/                
          next if file =~ /\/file\/setuid_spec$/                
          next if file =~ /\/file\/size_spec$/                
          next if file =~ /\/file\/stat_spec$/      
          next if file =~ /\/file\/truncate_spec$/      
          next if file =~ /\/file\/umask_spec$/      
          next if file =~ /\/file\/ulink_spec$/      
          next if file =~ /\/file\/writable_real_spec$/      
          next if file =~ /\/file\/zero_spec$/      
          
          next if file =~ /\/file\/stat\//                
          
          next if file =~ /\/filetest\/executable_real_spec$/      
          next if file =~ /\/filetest\/zero_spec$/      
          
          next if file =~ /\/process\/kill_spec$/      
        end
        
        config[:files] << file
      end
    end

    # LIBRARIES
    specs = app_folder + "spec/library/**/*_spec.iseq"
    Dir.glob(specs) do |file|
      next if file =~ /sha1/      
    
      file.gsub!(app_folder,"")
      file.gsub!(/\.iseq/,"")
      config[:files] << file
    end
  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
end
