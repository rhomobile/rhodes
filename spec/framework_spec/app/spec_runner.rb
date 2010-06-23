require 'mspec'
require 'mspec/utils/script'
require 'mspec/version'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
    # turn on exception backtrace
    MSpec.backtrace = true

    #MSpec.guard
    
    #LANGUAGE
    specs =  Rho::RhoFSConnector.get_app_path('app') + "spec/language/**/*_spec.iseq"
    Dir.glob(specs) do |file|
      file.gsub!(Rho::RhoFSConnector.get_app_path('app'),"")
      file.gsub!(/\.iseq/,"")
      # Temporary disable for_spec until https://www.pivotaltracker.com/story/show/3876583 will be fixed
      next if file =~ /\/for_spec$/
      config[:files] << file
    end

    # CORE
    core = []
    core << 'argf'
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
    core << 'env'
    # TODO: enable when bug https://www.pivotaltracker.com/story/show/3916324 will be fixed
    #core << 'kernel'
    core << 'nil'
    core << 'process'
    core << 'unboundmethod'

    core.each do |folder|
      specs =  Rho::RhoFSConnector.get_app_path('app') + "spec/core/#{folder}/**/*_spec.iseq"
      Dir.glob(specs) do |file|
        file.gsub!(Rho::RhoFSConnector.get_app_path('app'),"")
        file.gsub!(/\.iseq/,"")
        config[:files] << file
      end
    end

    # LIBRARIES
    specs = Rho::RhoFSConnector.get_app_path('app') + "spec/library/**/*_spec.iseq"
    Dir.glob(specs) do |file|
      file.gsub!(Rho::RhoFSConnector.get_app_path('app'),"")
      file.gsub!(/\.iseq/,"")
      config[:files] << file
    end
 
    # RHODES
    config[:files] << 'spec/rhom_spec'
    config[:files] << 'spec/rhoruby_spec'
    config[:files] << 'spec/rho_controller_spec'
    config[:files] << 'spec/rhom_object_spec'
    config[:files] << 'spec/rho_spec'
    config[:files] << 'spec/spec_helper'
    #config[:files] << 'spec/syncengine_spec'
    #config[:files] << 'spec/blobsync_spec'
    #config[:files] << 'spec/bulksync_spec'
    config[:files] << 'spec/rhom_db_adapter_spec'
    config[:files] << 'spec/rhoerror_spec'
    config[:files] << 'spec/bsearch_spec'
    #config[:files] << 'spec/find_spec'  # find not available on the device

  end

  def run

    MSpec.register_files config[:files]
    MSpec.backtrace=true

    MSpec.process
    MSpec.exit_code
  end
end
