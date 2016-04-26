module Rho
  class Instrumentation
    
    @@android_keycodes = {:tab => 61,
      :enter => 66,
      :escape => 111,
      :volume_down => 25}
      
    @@windows_touchevents = {:touch_down => 0,
      :touch_up => 1,
      :touch_move => 2}
  	
    @@window_keycodes = {:tab =>  9,
      :enter => 13,
  	:escape => 27,
  	:volume_down => 118}
  	
  	@@android_touchevents = {:touch_down => 0,
      :touch_up => 1,
      :touch_move => 2}
  	
    def self.get_keycode key
        if System::get_property('platform') == 'ANDROID'
          return get_android_keycode key
        else
          return get_windows_keycode key
        end
    end   
    
    def self.get_android_keycode key
      return @@android_keycodes[key]
    end
    
    def self.get_windows_keycode key
      return @@window_keycodes[key]
    end
    
    def self.get_toucheventcode touch_event
      if System::get_property('platform') == 'ANDROID'
          return get_android_toucheventcode touch_event
      else
          return get_windows_toucheventcode touch_event
      end
    end
    
    def self.get_android_toucheventcode touch_event
      return @@android_touchevents[touch_event]
    end
    
    def self.get_windows_toucheventcode touch_event
      return @@windows_touchevents[touch_event]
    end
  
  class InstrumentationStringHelper
  
     def self.process_string(str)
         return Rho::Instrumentation::native_process_string(str)
     end
        
  end
  
  end

end
