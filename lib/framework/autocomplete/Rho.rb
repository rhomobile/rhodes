
#
# It is auto-generated content.
# Do not do required for this file in your application.
#

module Rho
  module AsyncHttp
    def self.do_request(rest)
    end
    def self.cancel(rest)
    end
    def self.make_auth_header(args)
    end
    def self.preprocess_headers(args)
    end
    def self.check_ssl_verify_peer(args)
    end
    def self.process_result(res,callback)
    end
    def self.get(args)
    end
    def self.post(args)
    end
    def self.download_file(args)
    end
    def self.upload_file(args)
    end
  end

  module SignatureCapture
    def self.take(rest)
    end
    def self.visible(rest)
    end
    def self.capture(rest)
    end
    def self.clear(rest)
    end
  end

  class JSON
    def self.parse(req)
    end
    def self.quote_value(req)
    end
  end

  module RingtoneManager
    def self.get_all_ringtones(rest)
    end
    def self.stop(rest)
    end
    def self.play(rest)
    end
  end

  module RhoEvent
    def self.find(args)
    end
    def self.create!(event)
    end
    def self.destroy(id)
    end
    def self.update_attributes(event)
    end
    def self.get_authorization_status
    end
    def self.select(index,block)
    end
  end

  module Calendar
    def self.fetch(rest)
    end
    def self.fetch_by_id(rest)
    end
    def self.save(rest)
    end
    def self.delete(rest)
    end
    def self.calendar_get_authorization_status(rest)
    end
  end

  class RhoFSConnector
    def self.get_app_path(appname)
    end
    def self.get_base_app_path
    end
    def self.get_user_path
    end
    def self.get_app_manifest_filename
    end
    def self.get_rhoconfig_filename
    end
    def self.get_model_path(appname,modelname)
    end
    def self.get_db_fullpathname(postfix)
    end
    def self.get_blob_folder
    end
    def self.get_public_folder
    end
    def self.get_blob_path(relative_path)
    end
  end

  module NumberHelper
    def self.number_to_currency(number,options)
    end
    def self.number_with_precision(number,options)
    end
    def self.number_with_delimiter(number,options)
    end
  end

  class RhoMessages
    def self.get_message(name)
    end
  end

  class NativeTabbar
    def self.create(params)
    end
    def self.create_vertical(params)
    end
    def self.remove
    end
    def self.switch_tab(tab_index)
    end
    def self.set_tab_badge(tab_index,value)
    end
    def self.get_current_tab
    end
  end

  class NativeToolbar
    def self.create(params)
    end
    def self.remove
    end
  end

  class RhoError
    def self.err_message(code)
    end
    def code
    end
    def message
    end
    def unknown_client?(msg)
    end
  end

  class RhoApplication
    def self.get_app_path(appname)
    end
    def self.get_base_app_path
    end
    def self.get_user_path
    end
    def self.get_model_path(appname,modelname)
    end
    def self.get_blob_folder
    end
    def self.get_public_folder
    end
    def self.get_blob_path(relative_path)
    end
    def self.current_controller
    end
    def self.set_current_controller(ctrl)
    end
    def default_menu
    end
    def default_menu=(req)
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
    def on_ui_close_request
    end
    def on_ui_destroyed
    end
    def on_sync_user_changed
    end
    def on_reinstall_config_update(conflicts)
    end
    def on_migrate_source(old_version,new_src)
    end
    def set_menu(menu,back_action)
    end
    def serve(req,res)
    end
  end

  module RhoSupport
    def self.url_encode(s)
    end
    def self.binary_encode(str)
    end
    def self.binary_decode(str)
    end
    def self.url_decode(str)
    end
    def self._url_decode(str,regex)
    end
    def self.form_decode(str)
    end
    def self.parse_query_parameters(query_string)
    end
    def self.query_params(req)
    end
    def self.rhobundle_getfilename
    end
    def self.rhobundle_download(download_url,download_callback)
    end
  end

  class RHO
    def self.get_instance
    end
    def self.get_src_db(src_name)
    end
    def self.get_user_db
    end
    def self.get_db_partitions
    end
    def self.check_sources_migration(uniq_sources)
    end
    def self.check_source_migration(source)
    end
    def self.load_all_sources
    end
    def self.init_sync_source_properties(uniq_sources)
    end
    def self.processIndexes(index_param,src_name,is_unique)
    end
    def self.init_schema_sources(hash_migrate)
    end
    def self.init_schema_sources_partition(uniq_sources,hash_migrate,partition,db)
    end
    def self.make_createsql_script(name,schema_attr)
    end
    def self.current_exception
    end
    def db_partitions
    end
    def init_app
    end
    def init_nativebar
    end
    def on_config_conflicts(conflicts)
    end
    def activate_app
    end
    def deactivate_app
    end
    def ui_close_request
    end
    def ui_created
    end
    def ui_destroyed
    end
    def raise_rhoerror(errCode)
    end
    def set_app(appname,app)
    end
    def get_app(appname)
    end
    def load_models_from_file(app_manifest_filename)
    end
    def _load_models_from_file(f)
    end
    def process_model_dirs(app_manifest_filename)
    end
    def reset_db_on_sync_user_changed
    end
    def load_server_sources(data)
    end
    def load_all_sync_sources
    end
    def load_model(modelName,init_db)
    end
    def find_src_byname(db_sources,name)
    end
    def init_sources
    end
    def process_blob_attribs(source,db)
    end
    def get_start_id(db_sources,db_partition)
    end
    def init_db_sources(db,uniq_sources,db_partition,hash_migrate)
    end
    def serve(req)
    end
    def serve_hash(req)
    end
    def serve_index(index_name,req)
    end
    def serve_index_hash(index_name,req)
    end
    def make_httpdate(t)
    end
    def init_response(status,message,body)
    end
    def send_response(res)
    end
    def send_response_hash(res)
    end
    def send_error(exception,status,hash)
    end
  end

  class RhoConfig
    def self.method_missing(name,args)
    end
    def self.exists?(name)
    end
    def self.max_config_srcid
    end
    def self.show_log
    end
    def self.send_log
    end
    def self.read_log(limit)
    end
    def self.sources
    end
    def self.clean_cached_changed
    end
    def self.source_changed?(name,partition)
    end
    def self.add_source(modelname,props)
    end
    def self.add_loaded_source(modelname,new_source)
    end
    def self.generate_id
    end
  end

  module Timer
    def self.start(interval_ms,callback,callback_data)
    end
    def self.stop(callback)
    end
  end

  class RhoController
    def self.layout(name)
    end
    def self.get_layout_name
    end
    def self.renderfile(filename,req,res)
    end
    def self.cached_metadata
    end
    def self.clean_cached_metadata
    end
    def self.start_geoview_notification
    end
    def self.start_objectnotify
    end
    def self.process_rho_object(params)
    end
    def self.before(block)
    end
    def inst_render_index(filename,req,res)
    end
    def getBinding
    end
    def __get_model
    end
    def render(options)
    end
    def render_metadata(action,metadata)
    end
    def render_partial(options)
    end
    def render_partial_collection(options,partial_name)
    end
    def set_geoview_notification(callback,callback_data,timeout_sec)
    end
    def add_objectnotify(arg)
    end
    def truncate(text,length)
    end
    def click_to_call(phone,description)
    end
    def mailto(address,description)
    end
    def link_to(name,url_params,html_options,confirm)
    end
    def url_for(params)
    end
    def url_for_nativeview(params)
    end
    def menu
    end
    def menu=(req)
    end
    def rho_info(str)
    end
    def rho_error(str)
    end
    def app_info(str)
    end
    def app_error(str)
    end
    def default_action
    end
    def serve(application,object_mapping,req,res)
    end
    def xml_http_request?
    end
    def xhr?
    end
    def redirect(url_params,options)
    end
    def strip_braces(str)
    end
  end

  class RhoContact
    def self.get_authorization_status
    end
    def self.open_phonebook
    end
    def self.close_phonebook(phonebook)
    end
    def self.find(args)
    end
    def self.create!(properties,phonebook)
    end
    def self.destroy(recordId,phonebook)
    end
    def self.update_attributes(properties,phonebook)
    end
    def self.select(index,block)
    end
    def self.select_by_name(first_last_name,block)
    end
  end

  class BluetoothManager
    def self.is_bluetooth_available
    end
    def self.off_bluetooth
    end
    def self.set_device_name(name)
    end
    def self.get_device_name
    end
    def self.get_last_error
    end
    def self.create_session(role,callback_url)
    end
    def self.create_server_and_wait_for_connection(callback_url)
    end
    def self.create_client_connection_to_device(server_name,callback_url)
    end
    def self.stop_current_connection_process
    end
  end

  class BluetoothSession
    def self.set_callback(connected_device_name,session_callback_url)
    end
    def self.disconnect(connected_device_name)
    end
    def self.get_status(connected_device_name)
    end
    def self.read(connected_device_name)
    end
    def self.write(connected_device_name,data)
    end
    def self.read_string(connected_device_name)
    end
    def self.write_string(connected_device_name,data)
    end
  end

  def self.file_exist?(fname)
  end
  def self.get_app
  end
  def self.support_transitions?
  end
  def self.close_database(arg_partition)
  end
end
