 
#
# It is auto-generated content.
# Do not do required for this file in your application.
#
 
module Rho
  module AsyncHttp
    def self.do_request
    end    
    def self.cancel
    end    
    def self.make_auth_header
    end    
    def self.preprocess_headers
    end    
    def self.check_ssl_verify_peer
    end    
    def self.process_result
    end    
    def self.get
    end    
    def self.post
    end    
    def self.download_file
    end    
    def self.upload_file
    end    
  end

  module SignatureCapture
    def self.take
    end    
    def self.visible
    end    
    def self.capture
    end    
    def self.clear
    end    
  end

  class JSON
    def self.parse
    end    
    def self.quote_value
    end    
  end

  module RingtoneManager
    def self.get_all_ringtones
    end    
    def self.stop
    end    
    def self.play
    end    
  end

  module RhoEvent
    def self.find
    end    
    def self.create!
    end    
    def self.destroy
    end    
    def self.update_attributes
    end    
    def self.select
    end    
  end

  module Calendar
    def self.fetch
    end    
    def self.fetch_by_id
    end    
    def self.save
    end    
    def self.delete
    end    
  end

  class RhoFSConnector
    def self.get_app_path
    end    
    def self.get_base_app_path
    end    
    def self.get_user_path
    end    
    def self.get_app_manifest_filename
    end    
    def self.get_rhoconfig_filename
    end    
    def self.get_model_path
    end    
    def self.get_db_fullpathname
    end    
    def self.get_blob_folder
    end    
    def self.get_public_folder
    end    
    def self.get_blob_path
    end    
  end

  module NumberHelper
    def self.number_to_currency
    end    
    def self.number_with_precision
    end    
    def self.number_with_delimiter
    end    
  end

  class RhoMessages
    def self.get_message
    end    
  end

  class NativeTabbar
    def self.create
    end    
    def self.create_vertical
    end    
    def self.remove
    end    
    def self.switch_tab
    end    
    def self.set_tab_badge
    end    
    def self.get_current_tab
    end    
  end

  class NativeToolbar
    def self.create
    end    
    def self.remove
    end    
  end

  class RhoError
    def self.err_message
    end    
    def code
    end    
    def message
    end    
    def unknown_client?
    end    
  end

  class RhoApplication
    def self.get_app_path
    end    
    def self.get_base_app_path
    end    
    def self.get_user_path
    end    
    def self.get_model_path
    end    
    def self.get_blob_folder
    end    
    def self.get_public_folder
    end    
    def self.get_blob_path
    end    
    def self.current_controller
    end    
    def self.set_current_controller
    end    
    def default_menu
    end    
    def default_menu=
    end    
    def init_nativebar
    end    
    def initialized?
    end    
    def on_activate_app
    end    
    def on_deactivate_app
    end    
    def on_ui_created
    end    
    def on_ui_destroyed
    end    
    def on_sync_user_changed
    end    
    def on_reinstall_config_update
    end    
    def on_migrate_source
    end    
    def set_menu
    end    
    def serve
    end    
  end

  module RhoSupport
    def self.url_encode
    end    
    def self.binary_encode
    end    
    def self.binary_decode
    end    
    def self.url_decode
    end    
    def self._url_decode
    end    
    def self.form_decode
    end    
    def self.parse_query_parameters
    end    
    def self.query_params
    end    
    def self.rhobundle_getfilename
    end    
    def self.rhobundle_download
    end    
  end

  class RHO
    def self.get_instance
    end    
    def self.get_src_db
    end    
    def self.get_user_db
    end    
    def self.get_db_partitions
    end    
    def self.check_sources_migration
    end    
    def self.check_source_migration
    end    
    def self.load_all_sources
    end    
    def self.init_sync_source_properties
    end    
    def self.processIndexes
    end    
    def self.init_schema_sources
    end    
    def self.init_schema_sources_partition
    end    
    def self.make_createsql_script
    end    
    def self.current_exception
    end    
    def db_partitions
    end    
    def init_app
    end    
    def init_nativebar
    end    
    def on_config_conflicts
    end    
    def activate_app
    end    
    def deactivate_app
    end    
    def ui_created
    end    
    def ui_destroyed
    end    
    def raise_rhoerror
    end    
    def set_app
    end    
    def get_app
    end    
    def load_models_from_file
    end    
    def _load_models_from_file
    end    
    def process_model_dirs
    end    
    def reset_db_on_sync_user_changed
    end    
    def load_server_sources
    end    
    def load_all_sync_sources
    end    
    def load_model
    end    
    def find_src_byname
    end    
    def init_sources
    end    
    def process_blob_attribs
    end    
    def get_start_id
    end    
    def init_db_sources
    end    
    def serve
    end    
    def serve_hash
    end    
    def serve_index
    end    
    def serve_index_hash
    end    
    def make_httpdate
    end    
    def init_response
    end    
    def send_response
    end    
    def send_response_hash
    end    
    def send_error
    end    
  end

  class RhoConfig
    def self.method_missing
    end    
    def self.exists?
    end    
    def self.max_config_srcid
    end    
    def self.show_log
    end    
    def self.send_log
    end    
    def self.read_log
    end    
    def self.sources
    end    
    def self.clean_cached_changed
    end    
    def self.source_changed?
    end    
    def self.add_source
    end    
    def self.add_loaded_source
    end    
    def self.generate_id
    end    
  end

  module Timer
    def self.start
    end    
    def self.stop
    end    
  end

  class RhoController
    def self.layout
    end    
    def self.get_layout_name
    end    
    def self.renderfile
    end    
    def self.cached_metadata
    end    
    def self.clean_cached_metadata
    end    
    def self.start_geoview_notification
    end    
    def self.start_objectnotify
    end    
    def self.process_rho_object
    end    
    def self.before
    end    
    def inst_render_index
    end    
    def getBinding
    end    
    def __get_model
    end    
    def render
    end    
    def render_metadata
    end    
    def render_partial
    end    
    def render_partial_collection
    end    
    def set_geoview_notification
    end    
    def add_objectnotify
    end    
    def truncate
    end    
    def click_to_call
    end    
    def mailto
    end    
    def link_to
    end    
    def url_for
    end    
    def url_for_nativeview
    end    
    def menu
    end    
    def menu=
    end    
    def rho_info
    end    
    def rho_error
    end    
    def app_info
    end    
    def app_error
    end    
    def default_action
    end    
    def serve
    end    
    def xml_http_request?
    end    
    def xhr?
    end    
    def redirect
    end    
    def strip_braces
    end    
  end

  class RhoContact
    def self.open_phonebook
    end    
    def self.close_phonebook
    end    
    def self.find
    end    
    def self.create!
    end    
    def self.destroy
    end    
    def self.update_attributes
    end    
    def self.select
    end    
    def self.select_by_name
    end    
  end

  class BluetoothManager
    def self.is_bluetooth_available
    end    
    def self.off_bluetooth
    end    
    def self.set_device_name
    end    
    def self.get_device_name
    end    
    def self.get_last_error
    end    
    def self.create_session
    end    
    def self.create_server_and_wait_for_connection
    end    
    def self.create_client_connection_to_device
    end    
    def self.stop_current_connection_process
    end    
  end

  class BluetoothSession
    def self.set_callback
    end    
    def self.disconnect
    end    
    def self.get_status
    end    
    def self.read
    end    
    def self.write
    end    
    def self.read_string
    end    
    def self.write_string
    end    
  end

  def self.file_exist?
  end  
  def self.get_app
  end  
  def self.support_transitions?
  end  
  def self.close_database
  end  
end

