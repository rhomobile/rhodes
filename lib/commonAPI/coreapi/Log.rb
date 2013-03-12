module RhoLog

    def self.info(cat, msg)
        Rho::Log.info(msg, cat)
    end

    def self.error(cat, msg)
        Rho::Log.error(msg, cat)
    end

end