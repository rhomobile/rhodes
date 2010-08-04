module Rho
  class RhoError < Exception
    ERR_NONE = 0
    ERR_NETWORK = 1
    ERR_REMOTESERVER = 2
    ERR_RUNTIME = 3
    ERR_UNEXPECTEDSERVERRESPONSE = 4
    ERR_DIFFDOMAINSINSYNCSRC = 5
    ERR_NOSERVERRESPONSE = 6
    ERR_CLIENTISNOTLOGGEDIN = 7
    ERR_CUSTOMSYNCSERVER = 8
    ERR_UNATHORIZED = 9
    ERR_CANCELBYUSER = 10
    ERR_SYNCVERSION = 11
    ERR_GEOLOCATION = 12
    
    attr_reader :code
    
    def initialize(err_code)
        @code = err_code
    end
    
    def message
        ::Rho::RhoError.err_message(code)        
    end
    
    def unknown_client?(msg)
       code ==  ERR_REMOTESERVER && msg && msg.downcase == 'unknown client'
    end
    
  end # RhoError
end # Rho

require 'rholang/localization_simplified'
LocalizationSimplified.requre_loc('rholang/rhoerror_',false)