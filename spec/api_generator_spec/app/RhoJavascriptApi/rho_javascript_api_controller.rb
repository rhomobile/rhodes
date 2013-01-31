require 'rho/rhocontroller'
require 'helpers/application_helper'
require 'helpers/browser_helper'
require 'json'

class RhoJavascriptApiController < Rho::RhoController
  include ApplicationHelper
  include BrowserHelper

  LOG_TAG = 'JSAPI'
  RHO_ID_PARAM = '__rhoID'
  COMMAND_REGEXP = /^command_proxy_/

  @layout = 'RhoJavascriptApi/layout'

  def command_handler
    RhoLog.info LOG_TAG, "command_handler: #{@params}"

    data = Rho::JSON.parse(@params['data'])

    module_name = data['module']
    method_name = data['method'].gsub(/is_(.*)/, '\1?')
    method_args = data['args']

    begin
      if module_name.nil?
        err_message = "Undefined module: #{data['module']}"
        RhoLog.error LOG_TAG, err_message
        raise ArgumentError, err_message
      end

      puts "method_args : #{method_args}"

      proxy_method = "command_proxy_#{module_name.gsub(/::/, '_')}_#{method_name}"
      res = self.send(proxy_method.to_sym(), module_name, method_name.to_sym(), method_args)
    rescue Exception => e
      @response['status'] = 500
      @response['message'] = e.message
      res = ::JSON.generate({'error' => e.message, 'stack' => e.backtrace})
    end
    puts "res : #{res}"

    render :string => "#{res}"
  end

  def command_proxy_Barcode1_testMethod(module_name, method_name, method_args)
    result = '';
    method_args.each { |arg| result.concat arg.to_s }
    ::JSON.generate({'value' => result, 'type' => 'string'})
  end

  def command_proxy_Barcode1_enumerate(module_name, method_name, method_args)
    ::JSON.generate({'value' => '00001 00002 00003', 'type' => 'instance', 'class' => 'Barcode1'})
  end

  def command_proxy_Barcode1_getProps(module_name, method_name, method_args)
    ::JSON.generate({'value' => { 'resolution' => '1024x768' }, 'type' => 'object'})
  end

  def command_proxy_Barcode1_setProps(module_name, method_name, method_args)
    ::JSON.generate({'value' => { 'resolution' => method_args[0]['resolution'] }, 'type' => 'object'})
  end

end
