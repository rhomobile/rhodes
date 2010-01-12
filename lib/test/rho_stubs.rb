 module System
   def get_locale
    'en'
  end
 end
 include System

 class RhoLog
   def error(cat,msg)
     puts cat + "|" + msg
   end
   def info(cat,msg)
     puts cat + "|" + msg
   end
 end

 class SyncEngine
   @@mutex = ::Mutex.new()
   def self.lock_sync_mutex
     @@mutex.lock
   end
   def self.unlock_sync_mutex
     @@mutex.unlock
   end
 end

 class RhoConf
   @@conf = {}
   def self.set_property_by_name(name,value)
     @@conf[name] = value
   end
 end

module Rho
   class RhoFSConnector
     def self.__rhoGetCurrentDir
       Dir.pwd
     end
   end
 end

