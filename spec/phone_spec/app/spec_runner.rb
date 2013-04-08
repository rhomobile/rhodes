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

    config[:files] << "spec/log_spec"

    config[:files] << "spec/rho_spec"

    config[:files] << "spec/crypt_spec"

    config[:files] << "spec/json_spec"
    config[:files] << "spec/xml_spec"
    config[:files] << "spec/rhofile_spec"
    config[:files] << "spec/asynchttp_spec"
    config[:files] << "spec/date_spec"
    config[:files] << "spec/bsearch_spec"

    config[:files] << "spec/rho_controller_spec"

    config[:files] << [ "spec/rhom_object_spec",
        [ {:schema_model=>true, :sync_model=>true},  {:schema_model=>true, :sync_model=>false},
          {:schema_model=>false, :sync_model=>true} , {:schema_model=>false, :sync_model=>false} ] ]

    # should be enabled as extension
    # config[:files] << "spec/javascriptvm_spec" if System.get_property('platform') == 'APPLE'

if System.get_property('platform') != 'WP8'
    config[:files] << "spec/contacts_spec" unless System.get_property('platform') == 'WINDOWS_DESKTOP'

    # Disable events specs on Android because emulator doesn't contain Calendar provider
    config[:files] << "spec/events_spec"  unless System.get_property('platform') == 'WINDOWS_DESKTOP' or (System.get_property('platform') == 'ANDROID' and System.get_property('is_emulator'))

    config[:files] << "spec/barcode_spec" unless System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP'
    config[:files] << "spec/mapview_spec"  unless System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP'
end

    config[:files] << "spec/bundle_update_spec" if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID' || System.get_property('platform') == 'WINDOWS' || System.get_property('platform') == 'WINDOWS_DESKTOP'

    config[:files] << "spec/nativebar_spec" if System.get_property('platform') != 'Blackberry' && System.get_property('platform') != 'WP8'
    config[:files] << "spec/navbar_spec" if System.get_property('platform') == 'APPLE' || System.get_property('platform') == 'ANDROID'

    config[:files] << "spec/xruby_spec" if defined? RHO_ME

if !(System.get_property('platform') == 'Blackberry' && (System::get_property('os_version') =~ /^6\.0/)) && System.get_property('platform') != 'WP8'
    config[:files] << "spec/uri_spec"
end

if !defined?( RHO_ME )
    config[:files] << "spec/database_spec" unless System.get_property('platform') == 'WINDOWS' && System.get_property('is_emulator')
end

  end

  def run
    MSpec.register_files config[:files]

    MSpec.process
    MSpec.exit_code
  end

end
