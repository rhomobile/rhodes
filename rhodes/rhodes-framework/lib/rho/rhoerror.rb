module Rho
  class RhoError < Exception
    ERR_NONE = 0
    ERR_NETWORK = 1
    ERR_REMOTESERVER = 2
    ERR_RUNTIME = 3
    
    attr_reader :code
    
    def initialize(errCode)
        @code = errCode
    end
    
    def message
        if code == ERR_NETWORK
            return "Could not establish network connection"
        elif code == ERR_REMOTESERVER
            return "Server return error"
        elif code ==  ERR_RUNTIME
            return "Internal error"
        end
        
        return "Unknown error"
    end
    
  end # RhoError
end # Rho
