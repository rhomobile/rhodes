require 'serialport.so'

class SerialPort
   private_class_method(:create)

   # Creates a serial port object.
   #
   # <tt>port</tt> may be a port number
   # or the file name of a defice.
   # The number is portable; so 0 is mapped to "COM1" on Windows,
   # "/dev/ttyS0" on Linux, "/dev/cuaa0" on Mac OS X, etc.
   #
   # <tt>params</tt> can be used to configure the serial port.
   # See SerialPort#set_modem_params for details
   def SerialPort::new(port, *params)
      sp = create(port)
      begin
         sp.set_modem_params(*params)
      rescue
         sp.close
         raise
      end
      return sp
   end

   # This behaves like SerialPort#new, except that you can pass a block
   # to which the new serial port object will be passed. In this case
   # the connection is automaticaly closed when the block has finished.
   def SerialPort::open(port, *params)
      sp = create(port)
      begin
         sp.set_modem_params(*params)
      rescue
         sp.close
         raise
      end
      if (block_given?)
        begin
           yield sp
        ensure             
           sp.close
        end
        return nil
      end
      return sp
   end
end
