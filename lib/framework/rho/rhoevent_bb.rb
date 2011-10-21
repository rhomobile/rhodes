#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------

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
          args[1][:start_date] = args[1][:start_date].to_time unless args[1][:start_date].nil?
          args[1][:end_date] = args[1][:end_date].to_time unless args[1][:end_date].nil?

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
      return_event = nil
      unless pb.nil?
        record = Calendar::createEvent(pb)
        if record.nil?
          puts "Can't find event " + properties['id']
        else
          properties.each do |key,value|
            if key == 'start_date' or key == 'end_date'
              value = Time.parse(value) if value.is_a? String
            end
            if key == 'reminder' and value.is_a?(String) and value.empty?
              next
            end
            Calendar::setEventValue(record,key,value)
          end
          return_event = Calendar::addEvent(pb,record)
        end
        Calendar::closeCalendar(pb)
      end
      return return_event
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
