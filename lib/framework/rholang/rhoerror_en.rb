module Rho
  class RhoError < Exception
    def self.err_message(code)
        if code == ERR_NETWORK
            return "Could not establish network connection."
        elsif code == ERR_REMOTESERVER
            return "Server returned an error."
        elsif code ==  ERR_RUNTIME
            return "Application has encountered an error and must close."
        elsif code ==  ERR_UNEXPECTEDSERVERRESPONSE
            return "Sync server returned an unexpected response."
        elsif code ==  ERR_DIFFDOMAINSINSYNCSRC
            return "All sync sources should be from one domain.Re-install the application."
        elsif code ==  ERR_NOSERVERRESPONSE
            return "Could not connect to sync server."
        elsif code ==  ERR_CLIENTISNOTLOGGEDIN
            return "Client is not logged in. No sync will be performed."
        elsif code ==  ERR_CUSTOMSYNCSERVER
            return "Could not connect to data server."
        elsif code ==  ERR_UNATHORIZED
            return "You entered an invalid login/password, please try again."
        elsif code ==  ERR_CANCELBYUSER
            return "Operation was cancelled by user."
        elsif code ==  ERR_NONE
            return ""
        end
        
        return "Unknown error"
    end
    
  end # RhoError
end # Rho
