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

    # CORE, not including thread or fiber
    [
      "argf","class","exception","float","marshal","numeric","range","systemexit",
      "array","comparable","false","gc","matchdata","object","regexp",
      "basicobject","continuation","hash","math","objectspace","signal","threadgroup",        
      "bignum","dir","file","integer","method","precision","string","time",   
      "binding","enumerable","filetest","io","module","proc","struct","true", 
      "builtin_constants","env","fixnum","kernel","nil","process","symbol","unboundmethod"
    ].each do |folder|

      specs =  Rho::RhoFSConnector.get_app_path('app') + "spec/core/#{folder}/**/*_spec.iseq"
      Dir.glob(specs) { |file|
        file.gsub!(Rho::RhoFSConnector.get_app_path('app'),"")
        file.gsub!(/\.iseq/,"")
        config[:files] << file
      }
    end
 
    #LANGUAGE
    specs =  Rho::RhoFSConnector.get_app_path('app') + "spec/language/**/*_spec.iseq"
      Dir.glob(specs) { |file|
        file.gsub!(Rho::RhoFSConnector.get_app_path('app'),"")
        file.gsub!(/\.iseq/,"")
        config[:files] << file
      }

    # RHODES
    config[:files] << 'spec/rhom_spec'
    config[:files] << 'spec/rhoruby_spec'
    config[:files] << 'spec/rho_controller_spec'
    config[:files] << 'spec/rhom_object_spec'
    config[:files] << 'spec/rho_spec'
    config[:files] << 'spec/spec_helper'
    config[:files] << 'spec/webview_spec'
    #config[:files] << 'spec/syncengine_spec'
    config[:files] << 'spec/rhom_db_adapter_spec'
    config[:files] << 'spec/rhoerror_spec'
    config[:files] << 'spec/bsearch_spec'
    #config[:files] << 'spec/find_spec'  # find not available on the device

    #LIBRARIES
    specs = Rho::RhoFSConnector.get_app_path('app') + "spec/library/**/*_spec.iseq"
    Dir.glob(specs) { |file|
      file.gsub!(Rho::RhoFSConnector.get_app_path('app'),"")
      file.gsub!(/\.iseq/,"")
      config[:files] << file
    }
  end

  def run

    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
end
