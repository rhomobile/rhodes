class BuildConfig
  @@app_config = nil
  @@config = nil
  @@shared_config = nil

  class << self
    def get_key(key_path, default = nil)
      result = nil

      key_sections = key_path.split('/').reject { |c| c.empty? }

      puts 'getting_key ' + key_sections.join(' ')

      [@@app_config, $app_config, @@config, $config, @@shared_config, $shared_config].compact.each do |config|
        if !config.nil?
          curr = config
          key_sections.each_with_index do |section, i|
            if !curr[section].nil?
              curr = curr[section]
            else
              break
            end
            if (i == key_sections.length-1) && !curr.nil?
              result = curr
            end
          end
          break if !result.nil?
        end
      end

      if result.nil?
        result = default
      end

      result
    end

    attr_accessor :app_config, :config, :shared_config
  end


end