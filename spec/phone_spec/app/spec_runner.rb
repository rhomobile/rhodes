require 'rhospec'

class SpecRunner < MSpecScript
  def initialize
    config[:files] = []
    
    config[:files] << "spec/asynchttp_spec"
    config[:files] << "spec/json_spec"
    config[:files] << "spec/xml_spec"
    config[:files] << "spec/rhofile_spec"
    config[:files] << "spec/date_spec"
    config[:files] << "spec/bsearch_spec"
    config[:files] << "spec/rho_spec"
    config[:files] << "spec/rho_controller_spec"

    config[:files] << [ "spec/rhom_object_spec",
        [ {:schema_model=>true, :sync_model=>true},  {:schema_model=>true, :sync_model=>false},
          {:schema_model=>false, :sync_model=>true}, {:schema_model=>false, :sync_model=>true} ] ]

    config[:files] << "spec/contacts_spec" unless System.get_property('device_name') == 'Win32'
    config[:files] << "spec/barcode_spec" unless System.get_property('device_name') == 'Win32'            
    config[:files] << "spec/mapview_spec"  unless System.get_property('platform') == 'WINDOWS'    
    config[:files] << "spec/nativebar_spec" if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'
    config[:files] << "spec/navbar_spec" if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'

    config[:files] << "spec/xruby_spec" if defined? RHO_ME

    config[:files] << "spec/syncengine_spec"
    config[:files] << "spec/blobsync_spec"
    #config[:files] << "spec/bulksync_spec"

  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
  
end
