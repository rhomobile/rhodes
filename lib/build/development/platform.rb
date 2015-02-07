module RhoDevelopment
  class Platform
    def self.windows?
      return /mswin|mingw32|windows/i === RbConfig::CONFIG['host_os']
    end

    def self.osx?
      return /darwin/i === RbConfig::CONFIG['host_os']
    end

    def self.terminate_process(pid)
      if self.windows?
        system "taskkill /PID #{pid}"
      elsif self.osx?
        Process.kill('SIGTERM', pid)
      else
        raise "Unknown platform #{RbConfig::CONFIG['host_os']}"
      end
    end
  end
end