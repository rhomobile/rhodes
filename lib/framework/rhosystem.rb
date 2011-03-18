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
end

module WebView
	def self.set_menu_items(disp_menu)
		#TODO: set_menu_items
	end
end

module GeoLocation
  def self.set_view_notification(geoview_callback, geoview_callback_data, geoview_timeout_sec)
	#TODO: set_view_notification
  end
end

module NativeBar
 
  def self.create_native_toolbar(type, params)
  end
 
  def self.remove_native_toolbar()
  end

end

module SyncEngine
	def self.clean_objectnotify
	#TODO: clean_objectnotify
	end
end

module Rho

class RhoConf
    #TODO: implement persistent RhoConf
	@@config = nil

	def self.set_property_by_name(name, val)
		process_rhoconfig

		puts "set_property_by_name: #{name}"

		@@config[name] = val
	end

	def self.get_property_by_name(name)
		process_rhoconfig

	    puts "get_property_by_name: #{name} : #{@@config[name]}"
		@@config[name]
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
