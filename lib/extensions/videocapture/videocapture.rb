module VideoCapture



   def self.start
       VideoCaptureNative.start
   end

   def self.stop
       VideoCaptureNative.stop
   end

   def self.cancel
       VideoCaptureNative.cancel
   end



#duration
#destination
#name
#audioSaveEvent


class << self
      def method_missing(name, *args)
        unless name == Fixnum
          if name[name.length()-1] == '='
            VideoCaptureNative.set_property(name.to_s.chop,(args[0]!=nil) ? args[0].to_s : "") 
          else
            VideoCaptureNative.get_property(name.to_s)
          end
        end
      end



end

end

