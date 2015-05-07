module Rho
    class Timer
        def self.start( interval_ms, callback, callback_data )
        puts "inside start"
        System.start_timer(interval_ms, callback, callback_data)
        end

        def self.stop( callback )
        puts "inside stop"
        System.stop_timer( callback )
        end
    end
end
