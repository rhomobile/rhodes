require 'bsearch'
require 'time'

module Rho
	module RhoEvent
    def self.find(*args)
      if args.first == :all
        start = nil
        finish = nil
        if args.length >= 2 and args[1].is_a? Hash
          params = args[1]
          start = params[:start_date]
          finish = params[:end_date]
        end
        start = MIN_TIME if start.nil? or start < MIN_TIME
        finish = MAX_TIME if finish.nil? or finish > MAX_TIME

        events = Rho::Calendar.fetch(start, finish)

        ret = {}
        events.each do |e|
          ret[e[Rho::RhoEvent::ID]] = e
        end
        ret
      else
        id = args.first.to_s
        Rho::Calendar.fetch(id)
      end
    end

    def self.create!(event)
      Rho::Calendar.save(event)
    end

    def self.destroy(id)
      Rho::Calendar.delete(id)
    end

    def self.update_attributes(event)
      Rho::Calendar.save(event)
    end

	end # module RhoEvent
end # module Rho
