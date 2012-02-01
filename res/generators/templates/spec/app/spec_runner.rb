require 'mspec'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
    # turn on exception backtrace
    MSpec.backtrace = true

    #MSpec.guard
    app_folder = Rho::RhoFSConnector.get_app_path('app')
    app_folder.gsub!(/\\/, '/')

    specs = app_folder + "**/*_spec" + RHO_RB_EXT
    Dir.glob(specs) { |file|
      file.gsub!(app_folder,"")
      
      if RHO_RB_EXT == '.rb'
        file.gsub!(/\.rb/,"")
      else
        file.gsub!(/\.iseq/,"")
      end
      
      config[:files] << file
    }

  end

  def run

    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
end
