require 'rhospec'

class SpecRunner < MSpecScript
  def initialize
    config[:files] = []

    config[:files] << "spec/string/end_with_spec"
    config[:files] << "spec/string/start_with_spec"
    config[:files] << "spec/string/replace_spec"
    config[:files] << "spec/string/split_spec"
    config[:files] << "spec/string/strip_spec"
    config[:files] << "spec/string/rstrip_spec"
    config[:files] << "spec/string/lstrip_spec"
    config[:files] << "spec/string/slice_spec"
    config[:files] << "spec/array/pack_spec"
    
    config[:files] << "spec/rho_spec"
if !defined?(RHO_WP7)
    config[:files] << "spec/asynchttp_spec"
    config[:files] << "spec/crypt_spec"
    config[:files] << "spec/json_spec"
    config[:files] << "spec/xml_spec"
    config[:files] << "spec/rhofile_spec"
end
    config[:files] << "spec/date_spec"
    config[:files] << "spec/bsearch_spec"

    config[:files] << "spec/rho_controller_spec"
    config[:files] << [ "spec/rhom_object_spec",
        [ {:schema_model=>true, :sync_model=>true},  {:schema_model=>true, :sync_model=>false},
          {:schema_model=>false, :sync_model=>true} , {:schema_model=>false, :sync_model=>false} ] ]

if !defined?(RHO_WP7)
    config[:files] << "spec/contacts_spec" unless System.get_property('device_name') == 'Win32'

    # Disable events specs on Android because emulator doesn't contain Calendar provider
    config[:files] << "spec/events_spec"  unless System.get_property('device_name') == 'Win32' or (System.get_property('platform') == 'ANDROID' and System.get_property('is_emulator'))

    config[:files] << "spec/barcode_spec" unless System.get_property('device_name') == 'Win32'            
    config[:files] << "spec/mapview_spec"  unless System.get_property('platform') == 'WINDOWS'    
end
    config[:files] << "spec/nativebar_spec" if System.get_property('platform') != 'Blackberry'
    config[:files] << "spec/navbar_spec" if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'

    config[:files] << "spec/xruby_spec" if defined? RHO_ME

    config[:files] << [ "spec/syncengine_spec", [ {:schema_model=>true }, {:schema_model=>false } ] ]

if !defined?(RHO_WP7)
    config[:files] << "spec/blobsync_spec"
    config[:files] << "spec/bulksync_spec"
end

  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end
  
end
