module AudioCapture



   def self.start
       AudioCaptureNative.start
   end

   def self.stop
       AudioCaptureNative.stop
   end

   def self.cancel
       AudioCaptureNative.cancel
   end



#duration
#destination
#name
#audioSaveEvent


class << self
      def method_missing(name, *args)
        unless name == Fixnum
          if name[name.length()-1] == '='
            AudioCaptureNative.set_property(name.to_s.chop,(args[0]!=nil) ? args[0].to_s : "") 
          else
            AudioCaptureNative.get_property(name.to_s)
          end
        end
      end



end

end



