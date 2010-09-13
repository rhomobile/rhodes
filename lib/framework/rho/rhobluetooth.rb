
module Rho
 
class BluetoothManager
 
  OK = 'OK'
  CANCEL = 'CANCEL'
  ERROR = 'ERROR'
 
  ROLE_SERVER = 'ROLE_SERVER'
  ROLE_CLIENT = 'ROLE_CLIENT'

  def self.is_bluetooth_available
    # return true/false
    return RhoBluetooth.is_bluetooth_available()
  end
 
  def self.off_bluetooth
    RhoBluetooth.off_bluetooth()
  end
 
  def self.set_device_name(name)
    RhoBluetooth.set_device_name(name)
  end
 
  def self.get_device_name
    return RhoBluetooth.get_device_name()
  end
 
  def self.get_last_error
    #return OK/ERROR/...
    return RhoBluetooth.get_last_error()
  end
 
  def self.create_session(role, callback_url)
    # return OK/ERROR
    #
    # in callback
    # status - OK/ERROR/CANCEL
    # connected_device_name
    puts 'BluetoothManager.rb::create_session()'
    RhoBluetooth.create_session(role, callback_url)
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
    RhoBluetooth.session_set_callback(connected_device_name, session_callback_url)
    return OK
  end  
 
  def self.disconnect(connected_device_name)
    # return OK/ERROR
    RhoBluetooth.session_disconnect(connected_device_name)
    return OK
  end
 
  def self.get_status(connected_device_name)
    # return -1 if error, 0 if empty or 0< if data waiting for read 
    return RhoBluetooth.session_get_status(connected_device_name)
  end
 
  def self.read(connected_device_name)
    # return array of bytes
    return RhoBluetooth.session_read_data(connected_device_name)
  end
 
  def self.write(connected_device_name, data)
    # array of bytes
    RhoBluetooth.session_write_data(connected_device_name, data)
  end
 
  def self.read_string(connected_device_name)
    # return string
    return RhoBluetooth.session_read_string(connected_device_name)
  end
 
  def self.write_string(connected_device_name, data)
    # return OK/ERROR
    RhoBluetooth.session_write_string(connected_device_name, data)
    return OK
  end
 
end
 
 
end
