#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------

module Rho

class BluetoothApiChecker



    def self.bt_is_new_api
      return (System::get_property('platform') == "ANDROID" or System::get_property('platform') == "APPLE")
      #return (System::get_property('platform') == "APPLE")
    end

end

class BluetoothManager

  OK = 'OK'
  CANCEL = 'CANCEL'
  ERROR = 'ERROR'
  NOT_FOUND = 'NOT_FOUND'

  ROLE_SERVER = 'ROLE_SERVER'
  ROLE_CLIENT = 'ROLE_CLIENT'

  def self.is_bluetooth_available
    # return true/false
    if Rho::BluetoothApiChecker.bt_is_new_api
        return Rho::BluetoothConnection.isAvailable
    else
        return RhoBluetooth.is_bluetooth_available()
    end
  end

  def self.off_bluetooth
      if Rho::BluetoothApiChecker.bt_is_new_api
          Rho::BluetoothConnection.disableBluetooth
      else
          RhoBluetooth.off_bluetooth()
      end
  end

  def self.set_device_name(name)
      if Rho::BluetoothApiChecker.bt_is_new_api
          Rho::BluetoothConnection.deviceName = name
      else
          RhoBluetooth.set_device_name(name)
      end
  end

  def self.get_device_name
      if Rho::BluetoothApiChecker.bt_is_new_api
          return Rho::BluetoothConnection.deviceName
      else
          return RhoBluetooth.get_device_name()
      end
  end

  def self.get_last_error
    #return OK/ERROR/...
    if Rho::BluetoothApiChecker.bt_is_new_api
        return Rho::BluetoothConnection.lastError
    else
        return RhoBluetooth.get_last_error()
    end
  end

  # show UI for select other device to connect
  # make BluetoothSession with selected device
  def self.create_session(role, callback_url)
    # return OK/ERROR
    #
    # in callback
    # status - OK/ERROR/CANCEL
    # connected_device_name
    puts 'BluetoothManager.rb::create_session()'
    if Rho::BluetoothApiChecker.bt_is_new_api
        Rho::BluetoothConnection.createConnection(role, callback_url)
    else
        RhoBluetooth.create_session(role, callback_url)
    end
    return OK
  end

  # make current device discoverable for other
  # wait for client connection from other device
  # after connect BluetoothSession maked
  def self.create_server_and_wait_for_connection(callback_url)
    # return OK/ERROR
    #
    # in callback
    # status - OK/ERROR/CANCEL
    # connected_device_name
    puts 'BluetoothManager.rb::create_server_and_wait_for_connection()'
    accept_any_device = 1
    if Rho::BluetoothApiChecker.bt_is_new_api
        Rho::BluetoothConnection.createServerConnection(callback_url)
    else
        RhoBluetooth.create_custom_server_session('', callback_url, accept_any_device)
    end

    return OK
  end

  # Enumerate near BT devices and check its name for equal with server_name
  # If server_name device founded - make client connection with it
  # and make BluetoothSession
  def self.create_client_connection_to_device(server_name, callback_url)
    # return OK/ERROR
    #
    # in callback
    # status - OK/ERROR/CANCEL
    # connected_device_name
    puts 'BluetoothManager.rb::create_client_connection_to_device()'
    if Rho::BluetoothApiChecker.bt_is_new_api
        Rho::BluetoothConnection.createClientConnection(server_name, callback_url)
    else
        RhoBluetooth.create_custom_client_session(server_name, callback_url)
    end
    return OK
  end

  # stop current connection process started by
  # (before BluetoothSession was created = callabck was executed!):
  # create_server_and_wait_for_connection()
  # or
  # create_client_connection_to_device()
  def self.stop_current_connection_process
    # return OK/ERROR
    puts 'BluetoothManager.rb::stop_current_custom_connections()'
    if Rho::BluetoothApiChecker.bt_is_new_api
        Rho::BluetoothConnection.stopCurrentConnectionProcess
    else
        RhoBluetooth.stop_current_connection_process
    end
    return OK
  end

end


class BluetoothSession

  OK = 'OK'
  CANCEL = 'CANCEL'
  ERROR = 'ERROR'

  SESSION_INPUT_DATA_RECEIVED = 'SESSION_INPUT_DATA_RECEIVED'
  SESSION_DISCONNECT = 'SESSION_DISCONNECT'

  def self.set_callback(connected_device_name, session_callback_url)
    # return OK/ERROR
    #
    # in callback
    # connected_device_name
    # event_type - SESSION_INPUT_DATA_RECEIVED/ERROR/SESSION_DISCONNECT
    puts 'BluetoothSession.rb::set_callback()'
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        connection.setCallback(session_callback_url)
    else
        RhoBluetooth.session_set_callback(connected_device_name, session_callback_url)
    end
    return OK
  end

  def self.disconnect(connected_device_name)
    # return OK/ERROR
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        connection.disconnect
    else
        RhoBluetooth.session_disconnect(connected_device_name)
    end
    return OK
  end

  def self.get_status(connected_device_name)
    # return -1 if error, 0 if empty or 0< if data waiting for read
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        return connection.status
    else
        return RhoBluetooth.session_get_status(connected_device_name)
    end
  end

  def self.read(connected_device_name)
    # return array of bytes
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        return connection.readData
    else
        return RhoBluetooth.session_read_data(connected_device_name)
    end
  end

  def self.write(connected_device_name, data)
    # array of bytes
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        connection.writeData(data)
    else
        RhoBluetooth.session_write_data(connected_device_name, data)
    end
  end

  def self.read_string(connected_device_name)
    # return string
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        return connection.readString
    else
        return RhoBluetooth.session_read_string(connected_device_name)
    end
  end

  def self.write_string(connected_device_name, data)
    # return OK/ERROR
    if Rho::BluetoothApiChecker.bt_is_new_api
        connection = Rho::BluetoothConnection.getConnectionByID(connected_device_name)
        connection.writeString(data)
    else
        RhoBluetooth.session_write_string(connected_device_name, data)
    end
    return OK
  end

end


end
