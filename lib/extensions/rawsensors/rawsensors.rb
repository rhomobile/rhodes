class RawSensors

  ACCELEROMETER = 'accelerometer'
  MAGNETOMETER = 'magnetometer'




   def self.is_available(sensor)
       return RawSensorsNative.is_available(sensor)
   end 

   def self.getSensorData
       RawSensorsNative.self.getSensorData
   end




class << self
      def method_missing(name, *args)
        unless name == Fixnum
          if name[name.length()-1] == '='
            RawSensorsNative.set_property(name.to_s.chop,(args[0]!=nil) ? args[0].to_s : "") 
          else
            RawSensorsNative.get_property(name.to_s)
          end
        end
      end



end

end

