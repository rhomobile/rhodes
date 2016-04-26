require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
require 'json'

module Rho::WebSql

  class Transaction
    def initialize(db)
      @db = db
      @db.start_transaction
    end

    def commit
      @db.commit
    end

    def rollback
      @db.rollback
    end

    def execute_sql(sql, vars)
      @db.execute(sql, false, vars)
    end
  end

  class Database
    def initialize(name)
      @db = SQLite3::Database.new(name, '')
    end

    def transaction
      Transaction.new(@db)
    end
  end


  def self.open_database(name, version = '1.0', descr = '', size = 1024*1024)
    Database.new(name)
  end

end



class RhoJavascriptApiController < Rho::RhoController
  include ApplicationHelper
  include BrowserHelper

  LOG_TAG = 'JSAPI'
  RHO_ID_PARAM = '__rhoID'
  COMMAND_REGEXP = /^command_proxy_/
  CALLBACK_REGEXP = Regexp.new("^callback_handler")

  def store_object(id, obj)
    $object_map = {} unless $object_map
    $object_map[id] = obj
  end

  def get_object(id)
    return nil unless $object_map
    return $object_map[id]
  end

  def remove_object(id)
    return unless $object_map
    $object_map.delete(id)
  end

  def get_module(module_name)
    module_obj = Object
    module_name.split('::').each do |submod_name|
      unless module_obj.nil?
        if module_obj.const_defined?(submod_name.to_sym())
          module_obj = module_obj.const_get(submod_name.to_sym())
        else
          module_obj = nil
        end
      end
    end
    return module_obj
  end

  def callback_handler(callback_name)
    id = callback_name.to_s.gsub(CALLBACK_REGEXP, '')
    js_params = ::JSON.generate(@params)
    RhoLog.info LOG_TAG, "callback PROXY, id: #{id}, js_params: #{js_params}, @request: #{@request}"

    #TODO: define what should be passed to js callback - json or url encoded string ?

    tab = get_object(id)
    if tab
      RhoLog.error LOG_TAG, "callback PROXY js call: Rho.pre4_callback_handler('#{id}', #{js_params});"
      WebView.execute_js("Rho.pre4_callback_handler('#{id}', #{js_params});" , tab)
    else
      RhoLog.error LOG_TAG, "unknown js callback id: #{id}, ids are: #{$object_map.keys.join(', ')}"
    end
  end

  def command_handler
    RhoLog.info LOG_TAG, "command_handler: #{@params}"

    data = Rho::JSON.parse(@params['data'])

    module_name = data['module']
    method_name = data['method'].gsub(/is_(.*)/, '\1?')
    method_args = data['args']
    callback_index = data['callback_index'] ? data['callback_index'].to_i() : nil

    begin
      module_object = get_module(module_name)

      if module_object.nil?
        err_message = "Undefined module: #{data['module']}"
        RhoLog.error LOG_TAG, err_message
        raise ArgumentError, err_message
      end

      if callback_index
        if method_args[callback_index] && method_args[callback_index].length() > 0
          callback_id = method_args[callback_index].gsub(/^.*?\/callback_/, 'callback_').gsub(CALLBACK_REGEXP, '')
          RhoLog.info LOG_TAG, "command_handler, callback_id: #{callback_id}"
          store_object(callback_id, WebView.active_tab())
        end
      end

      puts "method_args : #{method_args}"

      proxy_method = "command_proxy_#{module_name.gsub(/::/, '_')}_#{method_name}"
      res = self.send(proxy_method.to_sym(), module_object, method_name.to_sym(), method_args)
    rescue Exception => e
      @response['status'] = 500
      @response['message'] = e.message
      res = ::JSON.generate({'error' => e.message, 'stack' => e.backtrace})
    end
    puts "res : #{res}"

    render :string => "#{res}"
  end

  def respond_to?(name)
    RhoLog.info LOG_TAG, "respond_to?, name: #{name.to_s}"
    return true if name.to_s =~ COMMAND_REGEXP or name.to_s =~ CALLBACK_REGEXP
    super
  end

  def method_missing(name, *args)
    RhoLog.info LOG_TAG, "method_missing, name: #{name.to_s}"
    RhoLog.info LOG_TAG, "method_missing, match to callback: #{name.to_s =~ CALLBACK_REGEXP}"
    RhoLog.info LOG_TAG, "method_missing, match to command: #{name.to_s =~ COMMAND_REGEXP}"
    if name.to_s =~ CALLBACK_REGEXP
      callback_handler(name)
    elsif name.to_s =~ COMMAND_REGEXP
      default_command_proxy(*args)
    else
      super
    end
  end

  def time_to_json(value)
    ::JSON.generate({
        'value' => value.to_s,
        'time' => {
            'year'      => value.year,
            'month'     => value.month,
            'day'       => value.day,
            'wday'      => value.wday,
            'yday'      => value.yday,
            'hour'      => value.hour,
            'min'       => value.min,
            'sec'       => value.sec,
            'sec_epoch' => value.strftime('%s'),
            'zone'      => value.zone
        },
        'type'   => value.class.name.downcase
    })
  end

  def filestat_to_json(value)
    ::JSON.generate({
        'value' => {
            'ino'      => value.ino,
            'mode'     => value.mode,
            'nlink'       => value.nlink,
            'uid'      => value.uid,
            'gid'      => value.gid,
            'rdev'      => value.rdev,
            'size'       => value.size,
            'blksize'       => value.blksize,
            'blocks' => value.blocks
        },
        'type'   => value.class.name.downcase
    })
  end

  def default_command_proxy(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "DEFAULT PROXY, #{module_object.to_s}.#{method_name.to_s}, args: #{method_args}"
    res = module_object.send(method_name, *method_args)
    res = case res
        when Symbol, String, Integer, Fixnum, Bignum, Float, Rational, TrueClass, FalseClass then
          ::JSON.generate({'value' => res.to_s, 'type' => res.class.name.downcase})
        when Time then
          time_to_json(res)
        when File::Stat then
          filestat_to_json(res)
        when NilClass then
          ::JSON.generate({'value' => nil, 'type' => 'null'})
        else
          ::JSON.generate(res)
    end
  end

  def object_instance_command_proxy(method_name, method_args)
    RhoLog.info LOG_TAG, "Instance PROXY for method: #{method_name.to_s}, args: #{method_args}"

    id = method_args[0]
    args = method_args[1..-1]

    obj = get_object(id)
    raise ArgumentError, "Invalid object id: #{id}, $obj_map: #{$object_map}, obj: #{obj}" unless obj
    RhoLog.info LOG_TAG, "#{obj.class.name} instance.#{method_name.to_s} PROXY: args: #{args}"

    obj.send(method_name, *args)
  end

  def object_instance_command_proxy_json(method_name, method_args)
    res = object_instance_command_proxy(method_name, method_args)
    ::JSON.generate({'value' => res, 'type' => res.class.name.downcase})
  end

  def command_proxy_File_utime(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "File.utime PROXY"
    args = [
        Time.at(Integer(method_args[0])),
        Time.at(Integer(method_args[1])),
        method_args[2]
    ]
    res = module_object.send(method_name, *args)
    store_object(res.object_id, res)
    ::JSON.generate({'value' => res.object_id, 'type' => res.class.name.downcase})
  end

  def command_proxy_File_open(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "File.open PROXY"
    res = module_object.send(method_name, *method_args)
    store_object(res.object_id, res)
    ::JSON.generate({RHO_ID_PARAM => res.object_id, 'type' => res.class.name.downcase})
  end

  def command_proxy_File_getc(module_object, method_name, method_args)
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_File_ungetc(module_object, method_name, method_args)
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_File_gets(module_object, method_name, method_args)
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_File_putc(module_object, method_name, method_args)
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_File_puts(module_object, method_name, method_args)
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_File_flush(module_object, method_name, method_args)
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_File_close(module_object, method_name, method_args)
    res = object_instance_command_proxy_json(method_name, method_args)
    remove_object(method_args[0])
    return res
  end


  def command_proxy_Rho_AsyncHttp_get(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AsyncHttp.get PROXY"
    args = {
        :url            => method_args[0],
        :headers        => method_args[1],
        :callback       => method_args[2],
        :callback_param => method_args[3]
    }
    default_command_proxy(module_object, method_name, [args])
  end

  def command_proxy_Rho_AsyncHttp_post(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AsyncHttp.post PROXY"
    args = {
        :url            => method_args[0],
        :headers        => method_args[1],
        :body           => method_args[2],
        :callback       => method_args[3],
        :callback_param => method_args[4]
    }
    default_command_proxy(module_object, method_name, [args])
  end

  def command_proxy_Rho_AsyncHttp_download_file(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AsyncHttp.download_file PROXY"
    args = {
        :url            => method_args[0],
        :headers        => method_args[1],
        :filename       => method_args[2],
        :callback       => method_args[3],
        :callback_param => method_args[4]
    }
    default_command_proxy(module_object, method_name, [args])
  end

  def command_proxy_Rho_AsyncHttp_upload_file(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AsyncHttp.upload_file PROXY"
    args = {
        :url            => method_args[0],
        :headers        => method_args[1],
        :filename       => method_args[2],
        :body           => method_args[3],
        :callback       => method_args[4],
        :callback_param => method_args[5]
    }
    default_command_proxy(module_object, method_name, [args])
  end


  def command_proxy_Rho_WebSql_open_database(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "Rho::WebSql.open_database PROXY"
    res = module_object.send(method_name, *method_args)
    store_object(res.object_id, res)
    ::JSON.generate({RHO_ID_PARAM => res.object_id, 'type' => res.class.name.downcase})
  end

  def command_proxy_Rho_WebSql_transaction(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "Rho::WebSql.transaction PROXY"
    res = object_instance_command_proxy(method_name, method_args)
    store_object(res.object_id, res)
    ::JSON.generate({RHO_ID_PARAM => res.object_id, 'type' => res.class.name.downcase})
  end

  def command_proxy_Rho_WebSql_execute_sql(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "Rho::WebSql.execute_sql PROXY"
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_Rho_WebSql_commit(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "Rho::WebSql.commit PROXY"
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_Rho_WebSql_rollback(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "Rho::WebSql.rollback PROXY"
    object_instance_command_proxy_json(method_name, method_args)
  end

  def command_proxy_AudioCapture_set_duration(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AudioCapture.set_duration PROXY"
    AudioCapture.duration = Integer(method_args[0])
    ::JSON.generate({'value' => nil, 'type' => 'null'})
  end

  def command_proxy_AudioCapture_set_destination(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AudioCapture.set_destination PROXY"
    AudioCapture.destination = method_args[0]
    ::JSON.generate({'value' => nil, 'type' => 'null'})
  end

  def command_proxy_AudioCapture_set_name(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AudioCapture.set_name PROXY"
    AudioCapture.name = method_args[0]
    ::JSON.generate({'value' => nil, 'type' => 'null'})
  end

  def command_proxy_AudioCapture_set_audio_save_event(module_object, method_name, method_args)
    RhoLog.info LOG_TAG, "AudioCapture.set_audio_save_event PROXY"
    AudioCapture.audioSaveEvent = method_args[0]
    ::JSON.generate({'value' => nil, 'type' => 'null'})
  end

end
