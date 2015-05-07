require 'yaml'

module RhoDevelopment

  class Configuration

    @@application_root

    class << self
      attr_accessor :application_root
    end

    def self.handledNetworkExceptions
      exceptions = [Errno::ECONNREFUSED, Errno::EHOSTDOWN, Errno::EHOSTUNREACH]
      if Net.const_defined?(:OpenTimeout)
        exceptions << Net::ReadTimeout
      end

      if Net.const_defined?(:OpenTimeout)
        exceptions << Net::OpenTimeout
      else
        exceptions << Timeout::Error
      end
      return exceptions
      #[Errno::ECONNREFUSED, Errno::EHOSTDOWN, Errno::EHOSTUNREACH, Net::ReadTimeout, Net.const_defined?(:OpenTimeout) ? Net::OpenTimeout : Timeout::Error]
    end


    def self.development_directory
      File.join(self.application_root, '.development')
    end

    def self.own_ip_address
      Network::available_addresses.first
    end

    def self.webserver_alive_request
      URI.join(self.webserver_uri, 'alive')
    end

    def self.shut_down_webserver_request
      URI.join(self.webserver_uri, 'shutdown')
    end

    def self.auto_update_pid_request
      URI.join(self.webserver_uri, 'auto_update_pid')
    end

    def self.webserver_uri
      URI("http://#{self.webserver_ip}:#{self.webserver_port}")
    end

    def self.webserver_ip
      config = self.read_configuration
      web_server_config = config['webserver']
      if web_server_config.nil? || web_server_config['ip'].nil?
        self.own_ip_address
      else
        web_server_config['ip']
      end
    end

    def self.auto_refresh?
      config = self.read_configuration
      config['refresh'].nil? ? false : config['refresh'] != false && config['refresh'] != 0
    end

    def self.webserver_port
      3000
    end

    def self.config_filename
      File.join(self.application_root, 'dev-config.yml')
    end

    def self.read_configuration
      config = {}
      if File.exist?(self.config_filename)
        config = YAML.load_file(self.config_filename)
      end
      config
    end

    # Returns all subscribers in dev-config.yml
    # @return [Subscribers] array of subscribers
    def self.subscribers
      subscribers = []
      config = self.read_configuration
      unless config['devices'].nil?
        config['devices'].each { |each|
          subscriber = RhoDevelopment::Subscriber.new
          subscriber.uri = each['uri']
          subscriber.platform = each['platform']
          subscriber.name = each['name']
          subscriber.application = each['application']
          (each['enabled'] == true || each['enabled'] == 1) ? subscriber.beEnabled : subscriber.beDisabled
          subscribers << subscriber
        }
      end
      subscribers
    end

    # Returns all subscribers in dev-config.yml
    # @return [Subscribers] array of subscribers
    def self.enabled_subscribers
      self.subscribers.select { | each | each.enabled?}
    end

    def self.has_subscribers?
      self.subscribers.empty? == false
    end

    def self.has_enabled_subscribers?
      self.enabled_subscribers.empty? == false
    end

    def self.store_subscribers(anArray)
      config = self.read_configuration
      config['devices'] = anArray
      self.save_to_file(config)
    end

    def self.subscriber_by_ip(aString)
      self.subscribers.each { |each|
        if each.has_ip(aString)
          return each
        end
      }
      raise "Subscriber with IP #{aString} not found"
    end

    def self.enabled_subscriber_platforms
      (self.enabled_subscribers.collect { |each| each.normalized_platform_name }).to_set
    end

    def self.enabled_subscribers_by_platform(platform_string)
      (self.enabled_subscribers.select { |each| (each.normalized_platform_name) == platform_string || (each.platform == platform_string) })
    end

    def self.partial_bundle_name
       return 'upgrade_bundle_partial.zip'
    end

    def self.full_bundle_name
      return 'upgrade_bundle.zip'
    end

    def self.next_filename_for_downloading
      config = self.read_configuration
      key = (config['last_bundle_key'].nil?) ? 0 : config['last_bundle_key']
      key = key + 1
      config['last_bundle_key'] = key
      self.save_to_file(config)
      return "bundle_#{key}.zip"
    end

    def self.last_filename_for_downloading
      config = self.read_configuration
      if config['last_bundle_key'].nil?
        raise "The last bundle key not found at #{self.config_filename}"
      end
      key = config['last_bundle_key']
      return "bundle_#{key}.zip"
    end


    def self.document_root=(aString)
      config = self.read_configuration
      config['webserver'] = {'documentRoot' => aString}
      self.save_to_file(config)
    end

    def self.document_root
      config = self.read_configuration
      web_server_config = config['webserver']
      if web_server_config.nil? || web_server_config['documentRoot'].nil?
        document_root = Dir.mktmpdir
        self.document_root=document_root
      else
        document_root = web_server_config['documentRoot']
      end
      FileUtils.mkpath(document_root) unless File.exist?(document_root)
      document_root
    end

    def self.save_to_file(config)
      yml = config.to_yaml
      File.open(self.config_filename, 'w') { |file| file.write yml }
    end

  end

end