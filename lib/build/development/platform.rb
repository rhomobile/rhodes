module RhoDevelopment
  class Platform
    def self.windows?
      return /mswin|mingw32|windows/i === RbConfig::CONFIG['host_os']
    end

    def self.osx?
      return /darwin/i === RbConfig::CONFIG['host_os']
    end
  end
end