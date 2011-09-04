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
      
      if ( System.get_property('platform') == 'WINDOWS' )
        next if file =~ /\/execution_spec$/           
      end
      
      config[:files] << file
    end
    # CORE
    core = []

    core << 'argf' unless System.get_property('platform') == 'WINDOWS'
    core << 'array'
    core << 'basicobject'    
    core << 'bignum'    
    core << 'binding'
    core << 'builtin_constants'    
    core << 'class'
    core << 'comparable'
    core << 'complex'
    core << 'continuation'
    core << 'dir'
    core << 'encoding'    
    core << 'enumerable'   
    #has multiple crashes
    #core << 'enumerator'   
    #core << 'env' unless System.get_property('platform') == 'WINDOWS'
    core << 'exception'
    core << 'false'
    core << 'file'
    core << 'filetest'
    core << 'fixnum'        
    core << 'float'
    core << 'gc'
    core << 'hash'
    core << 'integer'
    core << 'io'
    core << 'kernel'
    core << 'marshal'
    core << 'matchdata'    
    core << 'math'
    core << 'method'
    core << 'module'
    core << 'mutex'
    core << 'nil'
    core << 'numeric'
    core << 'object'
    core << 'objectspace'
    core << 'precision'
    core << 'proc'
    core << 'process' unless System.get_property('platform') == 'WINDOWS'
    core << 'random'    
    core << 'range'
    core << 'rational'
    core << 'regexp'
    core << 'signal'  unless System.get_property('platform') == 'WINDOWS'
    core << 'string'
    core << 'struct'
    core << 'symbol'
    core << 'systemexit'
    core << 'time'
    core << 'true'
    core << 'unboundmethod'
    
    core << 'thread'
    core << 'threadgroup'
    core << 'fiber'
    
    core.each do |folder|
      specs =  app_folder + "spec/core/#{folder}/**/*_spec.iseq"
      Dir.glob(specs) do |file|
        file.gsub!(app_folder,"")
        file.gsub!(/\.iseq/,"")
 
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
