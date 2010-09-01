module Rho
  class RhoError < Exception
    def self.err_message(code)
        if code == ERR_NETWORK
            return "No se pudo establecer conexión de red."
        elsif code == ERR_REMOTESERVER
            return "El servidor ha devuelto un error."
        elsif code ==  ERR_RUNTIME
            return "La aplicación ha detectado un error y debe cerrarse."
        elsif code ==  ERR_UNEXPECTEDSERVERRESPONSE
            return "Sincronización con servidor devuelve una respuesta inesperada."
        elsif code ==  ERR_DIFFDOMAINSINSYNCSRC
            return "Todas las fuentes de sincronización debe ser de un dominio. Vuelva a instalar la aplicación."
        elsif code ==  ERR_NOSERVERRESPONSE
            return "No se pudo conectar al servidor de sincronización."
        elsif code ==  ERR_CLIENTISNOTLOGGEDIN
            return "El cliente no se encuetra logueado. La sincronización no se llevará a cabo."
        elsif code ==  ERR_CUSTOMSYNCSERVER
            return "No se pudo conectar al servidor de datos."
        elsif code ==  ERR_UNATHORIZED
            return "Ha introducido un usuario/contraseña invalido, por favor, inténtelo de nuevo."
        elsif code ==  ERR_CANCELBYUSER
            return "Operación fue cancelada por el usuario."
        elsif code ==  ERR_SYNCVERSION
            return "Conexión terminada por el servidor. Su aplicación móvil debe ser actualizada. Por favor, contacte con su administrador."
        elsif code ==  ERR_GEOLOCATION
            return "La ubicación no se pudo determinar."
        elsif code ==  ERR_NONE
            return ""
        end
        
        return "Error desconocido"
    end
    
  end # RhoError
end # Rho
