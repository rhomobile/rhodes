#make alias for older File.exist? method
unless File.respond_to?(:exists?)
  class File
    def self.exists?(file)
      File.exist?(file)
    end
  end
end

#make alias for older Dir.exist? method
unless Dir.respond_to?(:exists?)
  class Dir
    def self.exists?(dir)
      Dir.exist?(dir)
    end
  end
end
