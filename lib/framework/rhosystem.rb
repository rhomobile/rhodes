#require 'strscan'
load_assembly 'IronRuby.Libraries', 'IronRuby.StandardLibrary.StringScanner'

module System
	def self.get_locale
		"en"
	end

	def self.get_property(name)
		return "WP7" if name == "platform"
		return "7.0" if name == "os_version"
		return "us" if name == "country"
		""
	end

	def self.open_url(url)
		#TODO: open_url
	end

	def self.app_installed?(app_name)
		return false
	end
end

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
        File.open(Rho::RhoFSConnector.get_rhoconfig_filename).each do |line|
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
	def flush
	end
	def fileno
		0
	end
	def to_i
		0
	end
end
