module Rho
  class RhoError < Exception
    def self.err_message(code)
        if code == ERR_NETWORK
            return "Netzwerkverbindung konnte nicht hergestellt werden."
        elsif code == ERR_REMOTESERVER
            return "Server gab Fehler zurück."
        elsif code ==  ERR_RUNTIME
            return "Die Applikation hat einen Fehler verursacht und muss beendet werden."
        elsif code ==  ERR_UNEXPECTEDSERVERRESPONSE
            return "Synchronisationsserver antwortete unerwartet."
        elsif code ==  ERR_DIFFDOMAINSINSYNCSRC
            return "Alle Synchronisationsquellen sollten von der gleichen Domain stammen.Installiere die Applikation erneut."
        elsif code ==  ERR_NOSERVERRESPONSE
            return "Verbindung zum Synchronisationsserver konnte nicht hergestellt werden."
        elsif code ==  ERR_CLIENTISNOTLOGGEDIN
            return "Client ist nicht eingeloggt. Keine Synchronisation wird durchgeführt."
        elsif code ==  ERR_CUSTOMSYNCSERVER
            return "Verbindung zum Datenserver konnte nicht hergestellt werden."
        elsif code ==  ERR_UNATHORIZED
            return "Eingabe eines falschen Benutzernamens/Passwortes, bitte versuchen Sie es erneut."
        elsif code ==  ERR_CANCELBYUSER
            return "Operation wurde durch den User abgebrochen."
        elsif code ==  ERR_SYNCVERSION
            return "Verbindung durch Server beendet. Ihr mobile Applikation muss aktualisiert werden. Bitte kontaktieren sie Ihren Administrator."
        elsif code ==  ERR_GEOLOCATION
            return "Ortsangabe konnte nicht bestimmt werden."
        elsif code ==  ERR_NONE
            return ""
        end
        
        return "Unbekannter Fehler"
    end
    
  end # RhoError
end # Rho
