require 'bsearch'

module Rho
	class RhoEvent
		class << self
			def find(*args)
                raise ::Rhom::RecordNotFound if args[0].nil? or args.length == 0			
                
				pb = Calendar::openCalendar
				if pb.nil?
					puts "Can't open calendar"
					return nil
				elsif args.first == :all
				
                    if args.length > 1
                        records = Calendar::findCalendarEvents(pb, args[1])
                    else
    					records = Calendar::getallCalendarEvents(pb)
    				end
    					
					Calendar::closeCalendar(pb)
					return records
				else
					record = Calendar::getCalendarEvent(pb,args.first)
					Calendar::closeCalendar(pb)
					return record
				end
			end

			def create!(properties)
				pb = Calendar::openCalendar
				unless pb.nil?
					record = Calendar::createEvent(pb)
					if record.nil?
						puts "Can't find event " + properties['id']
					else
						properties.each do |key,value|
							Calendar::setEventValue(record,key,value)
						end
						Calendar::addEvent(pb,record)
					end
					Calendar::closeCalendar(pb)
				end
			end

			def destroy(recordId)
				pb = Calendar::openCalendar
				unless pb.nil?
					record = Calendar::openCalendarEvent(pb,recordId)
					if record.nil?
						puts "Can't find event " + recordId
					else
						Calendar::deleteEvent(pb,record)
					end
					Calendar::closeCalendar(pb)
				end
			end

			def update_attributes(properties)
				pb = Calendar::openCalendar
				unless pb.nil?
					record = Calendar::openCalendarEvent(pb,properties['id'])
					if record.nil?
						puts "Can't find event " + properties['id']
					else
						properties.each do |key,value|
							Calendar::setEventValue(record,key,value)
						end
						Calendar::saveEvent(pb,record)
					end
					Calendar::closeCalendar(pb)
				end
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
			def select(index, &block)
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

		end #<< self
	end # class RhoEvent
end # module Rho