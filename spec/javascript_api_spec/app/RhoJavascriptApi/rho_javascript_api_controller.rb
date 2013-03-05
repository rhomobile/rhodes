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

    # data['method'] contain it as 'Rho.ModuleName.methodName' or 'Rho:ModuleName.methodName'
    id = data['id']
    module_name = data['method'][0 .. data['method'].rindex('.') - 1].gsub(/\:/, '.')
    method_name = data['method'][data['method'].rindex('.') + 1 .. -1].gsub(/is_(.*)/, '\1?')
    method_args = data['args']

    begin
      if module_name.nil?
        err_message = "Undefined module: #{data['module']}"
        RhoLog.error LOG_TAG, err_message
        raise ArgumentError, err_message
      end

      puts "method_args : #{method_args}"

      proxy_method = "command_proxy_#{module_name.gsub(/::|:|\./, '_')}_#{method_name}"
      res = self.send(proxy_method.to_sym(), module_name, method_name.to_sym(), method_args)
      jsonrpc = { 'jsonrpc' => '2.0', 'id' => id, 'result' => res }
    rescue Exception => e
      @response['status'] = 500
      @response['message'] = e.message
      res = {'code' => -1, 'message' => e.message, 'stack' => e.backtrace}
      jsonrpc = { 'jsonrpc' => '2.0', 'id' => id, 'error' => res }
    end

    jsonrpc = ::JSON.generate(jsonrpc)
    render :string => "#{jsonrpc}"
  end

  def command_proxy_Rho_Barcode_testMethod(module_name, method_name, method_args)
    result = '';
    method_args.each { |arg| result.concat arg.to_s }
    {'value' => result, 'type' => 'string'}
  end

  def command_proxy_Rho_Barcode_getDefaultID(module_name, method_name, method_args)
    {'value' => '00001', 'type' => 'instance', 'class' => 'Rho.Barcode'}
  end

  def command_proxy_Rho_Barcode_enumerate(module_name, method_name, method_args)
    {'value' => '00001 00002 00003', 'type' => 'instance', 'class' => 'Rho.Barcode'}
  end

  def command_proxy_Rho_Barcode_getProperties(module_name, method_name, method_args)
    {'value' => { 'resolution' => '1024x768' }, 'type' => 'object'}
  end

  def command_proxy_Rho_Barcode_setProperties(module_name, method_name, method_args)
    {'value' => { 'resolution' => method_args[0]['resolution'] }, 'type' => 'object'}
  end

end
