require 'bsearch'
require 'time'

module Rho
	module RhoEvent
    def self.find(*args)
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

    def self.create!(properties)
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

    def self.destroy(recordId)
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

    def self.update_attributes(properties)
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

	end # module RhoEvent
end # module Rho
