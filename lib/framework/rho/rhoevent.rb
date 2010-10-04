require 'bsearch'
require 'time'

module Rho
	module RhoEvent
    # These values determined experimentally on iPhone
    MIN_TIME = Time.utc(2007, 'jan', 1, 0, 0, 0)
    MAX_TIME = Time.utc(2030, 'dec', 31, 23, 59, 59)

    def self.find(*args)
      raise ::Rhom::RecordNotFound if args[0].nil? or args.length == 0			

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

    #TODO: update examples    
    # Examples of how to use select method:
    #
      # selected = Rho::RhoEvent.select('first_name' => 'David') { |x| x[1]['last_name']=='Taylor' }
    # ==> returns record(s) of the David Taylor
    #
      # selected = Rho::RhoEvent.select('first_name' => 'Kate')
    # ==> Returns all records of Kate
    #
      # selected = Rho::RhoEvent.select('last_name' => 'User') do |x|
      # 	x[1]['first_name']=='Test' and x[1]['company_name']=="rhomobile"
      # end
    # ==> returns all records of the Test User from the company rhomobile
    #
    def self.select(index, &block)
      key, value = index.keys[0], index.values[0]
      if @events.nil? or @key != key
        @key, @events = key, find(:all).to_a.sort! {|x,y| x[1][key] <=> y[1][key] }
      end
      found = @events[@events.bsearch_range {|x| x[1][key] <=> value}]
      unless found.nil? or block.nil?
        return found.select(&block)
      end
      return found
    end

	end # module RhoEvent
end # module Rho
