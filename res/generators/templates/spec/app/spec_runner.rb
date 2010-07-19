require 'mspec'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
    # turn on exception backtrace
    MSpec.backtrace = true

    #MSpec.guard

    specs = Rho::RhoFSConnector.get_app_path('app') + "**/*_spec.iseq"
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
