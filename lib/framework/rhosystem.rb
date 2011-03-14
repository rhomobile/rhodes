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
end

module Kernel   
	def __rhoGetCurrentDir
		"/rho"
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
