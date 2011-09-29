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

#require 'strscan'
load_assembly 'IronRuby.Libraries', 'IronRuby.StandardLibrary.StringScanner'

#module System
#	def self.get_locale
#		"en"
#	end

#	def self.get_property(name)
#		return "WP7" if name == "platform"
#		return "7.0" if name == "os_version"
#		return "us" if name == "country"
#		""
#	end

#	def self.open_url(url)
		#TODO: open_url
#	end

#	def self.app_installed?(app_name)
#		return false
#	end
#end

module GeoLocation
  def self.set_view_notification(geoview_callback, geoview_callback_data, geoview_timeout_sec)
	#TODO: set_view_notification
  end
end


module Alert
	def self.show_status( title, msg, hide)
		#TODO: show_status
	end
end

module NavBar
	def self.create( arg )
		#TODO: create
	end
	
	def self.remove( )
		#TODO: remove
	end

	def self.started( )
		#TODO: started
		
		false
	end
	
end

module Rho

class RhoConf
    #TODO: implement persistent RhoConf
	@@config = nil

	def self.set_property_by_name(name, val)
		process_rhoconfig

		#puts "set_property_by_name: #{name}"

		@@config[name] = val
	end

	def self.get_property_by_name(name)
		process_rhoconfig

	    #puts "get_property_by_name: #{name} : #{@@config[name]}"
		@@config[name]
	end

	def self.is_property_exists(name)
		@@config.has_key?(name)	
	end

	def self.process_rhoconfig
	  return if @@config

	  @@config = {}
      begin
        File.open(Rho::RhoFSConnector.get_rhoconfig_filename) do |f|
          f.each do |line|
              # Skip empty or commented out lines
		      line.strip!

              #next if line =~ /^\s*(#|$)/
		      next if line.start_with?('#') || line.start_with?('$')

              parts = line.chomp.split('=', 2)
              key = parts[0]
              value = nil
              
              if !value
                value = parts[1] if parts[1]
              end
                
              if key and value
			    key.strip!
			    value.strip!

                val = value.gsub(/\'|\"/,'')
                val = val == 'nil' ? nil : val

                puts "rhoconfig: #{key} => #{val}"
                @@config[key] = val if key # allow nil value
              end  
          end      
        end
      rescue Exception => e
        puts "Error opening rhoconfig.txt: #{e}, using defaults."
      end
    end

	def self.read_log(size)
	    res = ""
		(1..size).each do |i|
			res += 'a'
		end

		res
	end
end

end

class RhoLog
    def initialize
	end

	def write(msg)
		puts msg
	end
	def print(msg)
		puts msg
	end
	def info(cat, msg)
		puts cat + ":" + msg
	end
	def error(cat, msg)
		puts "ERROR:" + cat + ":" + msg
	end
	
	def debug(cat, msg)
		puts "DEBUG:" + cat + ":" + msg
	end
	
	def flush
	end
	def fileno
		0
	end
	def to_i
		0
	end
end
