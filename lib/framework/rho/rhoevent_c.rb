require 'bsearch'
require 'time'

module Rho
	module RhoEvent
    def self.find(*args)
      if args.first == :all
        params = {}
        if args.length >= 2 and args[1].is_a? Hash
          args[1].each do |k,v|
            params[k.to_s] = v
          end
        end

        params['start_date'] = params['start_date'].to_time if !params['start_date'].nil? and !params['start_date'].is_a?(Time)
        params['end_date'] = params['end_date'].to_time if !params['end_date'].nil? and !params['start_date'].is_a?(Time)
        
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
      event['reminder'] = event['reminder'].to_i if event['reminder'] != nil
      new_id = Rho::Calendar.save(event)
      event['id'] = new_id
      return event
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
