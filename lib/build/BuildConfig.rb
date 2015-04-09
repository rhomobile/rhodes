class BuildConfig
  @@app_config = nil
  @@config = nil
  @@shared_config = nil

  class << self

    def parse_path(path)
      path.split('/').map{|c| c.strip}.reject { |c| c.empty? }
    end

    def find_elem(config, path, default = nil)
      if path.kind_of?(String)
        loc_path = parse_path(path)
      else
        loc_path = path.dup
      end

      result = nil

      if !config.nil?
        result = config
        until result.nil? || loc_path.empty?
          section = loc_path.shift
          result = section.nil? ? nil : result[section]
        end
      end

      result.nil? ? default : result
    end

    def get_key(key_path, default = nil)
      if key_path.kind_of?(String)
        key_sections = parse_path(key_path)
      end

      result = nil

      [@@app_config, $app_config, @@config, $config, @@shared_config, $shared_config].compact.each do |config|
        unless config.nil?
          result = find_elem(config, key_sections, nil)

          break unless result.nil?
        end
      end

      puts "Could not get config #{key_path}" if result.nil?

      result.nil? ? default : result
    end

    attr_accessor :app_config, :config, :shared_config
  end


end