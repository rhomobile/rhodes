require 'rhospec'

class SpecRunner < MSpecScript
  def initialize
     config[:files] = []

     config[:files] << "spec/simpleonlystatic_spec"
     config[:files] << "spec/megamodule_spec"

  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
  
end
