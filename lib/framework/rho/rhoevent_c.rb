require 'bsearch'
require 'time'

module Rho
	module RhoEvent
    def self.find(*args)
      if args.first == :all
        params = {}
        params = args[1] if args.length >= 2 and args[1].is_a? Hash

        params['start_date'] = MIN_TIME if params['start_date'].nil? or params['start_date'] < MIN_TIME
        params['end_date'] = MAX_TIME if params['end_date'].nil? or params['end_date'] > MAX_TIME
        
        params['include_repeating'] = false if ( params['start_date'] == MIN_TIME || params['end_date'] == MAX_TIME )
        
        puts "Rho::Calendar.fetch(params) : #{params}"
        events = Rho::Calendar.fetch(params)
        #puts "events : #{events}"
        #ret = {}
        #events.each do |e|
        #  ret[e[Rho::RhoEvent::ID]] = e
        #end
        #ret
        events
      else
        id = args.first.to_s
        id = $1 if id.is_a?(String) and id =~ /^{(.*)}$/
        Rho::Calendar.fetch_by_id(id)
      end
    end

    def self.create!(event)
      event['id'] = nil #indicate that event should be created
      Rho::Calendar.save(event)
    end

    def self.destroy(id)
      id = $1 if id.is_a?(String) and id =~ /^{(.*)}$/
      Rho::Calendar.delete(id)
    end

    def self.update_attributes(event)
      puts "update_attributes: #{event}"
      Rho::Calendar.save(event)
    end

	end # module RhoEvent
end # module Rho
