module Rho
  class RhoError < Exception
    ERR_NONE = 0
    ERR_NETWORK = 1
    ERR_REMOTESERVER = 2
    ERR_RUNTIME = 3
    ERR_UNEXPECTEDSERVERRESPONSE = 4
    
    attr_reader :code
    
    def initialize(errCode)
        @code = errCode
    end
    
    def message
        if code == ERR_NETWORK
            return "Could not establish network connection"
        elsif code == ERR_REMOTESERVER
            return "Server returned an error"
        elsif code ==  ERR_RUNTIME
            return "Internal error"
        elsif code ==  ERR_UNEXPECTEDSERVERRESPONSE
            return "Unexpected server response"
        end
        
        return "Unknown error"
    end
    
  end # RhoError
end # Rho
