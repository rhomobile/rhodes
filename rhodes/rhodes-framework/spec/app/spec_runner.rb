require 'mspec'
require 'mspec/utils/script'
require 'mspec/version'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
    # turn on exception backtrace
    MSpec.backtrace = true
    
#    Dir.entries(File.join(File.dirname(__FILE__),'spec')).each do |x|
#      x = nil if not x =~ /.iseq/
#      puts "Adding spec file: #{x}" if x
#      puts "Adding spec file: " + File.join(File.dirname(__FILE__),'spec',File.basename(x,'.iseq')) if x

#      config[:files] << File.join(File.dirname(__FILE__),'spec',File.basename(x,'.iseq')) if x
      
#    end
#   config[:files] << 'spec/rhom_object_factory_spec'
   config[:files] << 'spec/rhom_spec'
   config[:files] << 'spec/rhoruby_spec'
   config[:files] << 'spec/rho_controller_spec'
   config[:files] << 'spec/rho_spec'
   config[:files] << 'spec/spec_helper'
   config[:files] << 'spec/webview_spec'
    
  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
end