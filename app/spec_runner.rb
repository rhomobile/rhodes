require 'mspec'
require 'mspec/utils/script'
require 'mspec/version'

class SpecRunner < MSpecScript
  def initialize
    super
    config[:files] = []
    
    # turn on exception backtrace
    #MSpec.backtrace = true
    
    Dir.entries(File.join(File.dirname(__FILE__),'spec')).each do |x|
      x = nil if not x =~ /.iseq/
      puts "Adding spec file: #{x}" if x
      config[:files] << File.join(File.dirname(__FILE__),'spec',File.basename(x,'.iseq')) if x
    end
  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
end