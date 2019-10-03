require 'mspec/matchers'
require 'mspec/expectations'
require 'mspec/mocks'
require 'mspec/runner'
require 'mspec/guards'
require 'mspec/helpers'

require 'mspec/utils/script'
require 'mspec/version'

# If the implementation on which the specs are run cannot
# load pp from the standard library, add a pp.rb file that
# defines the #pretty_inspect method on Object or Kernel.
begin
  require 'pp'
rescue LoadError
  module Kernel
    def pretty_inspect
      inspect
    end
  end
end


class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
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
