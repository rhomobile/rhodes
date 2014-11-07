require 'yaml'

module RhoDevelopment

  class Configuration

    @@application_root

    class << self
      attr_accessor :application_root
    end

    def self.handledNetworkExceptions
      [Errno::ECONNREFUSED, Errno::EHOSTDOWN, Errno::EHOSTUNREACH, Net.const_defined?(:OpenTimeout) ? Net::OpenTimeout : Timeout::Error]
    end

    def self.own_ip_addresses
      Socket.ip_address_list.select { |each| each.ipv4? and !each.ipv4_loopback? and !each.ipv4_multicast? }.map { |each| each.ip_address }.uniq
    end

    def self.own_ip_address
      self.own_ip_addresses.first
    end

    def self.webserver_alive_request
      URI.join(self.webserver_uri, 'alive')
    end

    def self.shut_down_webserver_request
      URI.join(self.webserver_uri, 'shutdown')
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

    def self.webserver_port
      3000
    end


    def self.config_filename
      File.join(self.application_root, 'dev-config.yml')
    end

    def self.read_configuration
      if File.exist?(self.config_filename)
        YAML.load_file(self.config_filename)
      end
      {}
    end

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
          subscribers << subscriber
        }
      end
      subscribers
    end

    def self.has_subscribers?
      config = self.read_configuration
      config['devices'].nil? ? false : true
    end

    def self.store_subscribers(anArray)
      config = self.read_configuration
      config['devices'] = anArray
      yml = config.to_yaml
      File.open(self.config_filename, 'w') { |file| file.write yml }
    end

    def self.subscriber_by_ip(aString)
      self.subscribers.each { |each|
        if each.has_ip(aString)
          return each
        end
      }
      raise "Subscriber with IP #{aString} not found"
    end

    def self.subscriber_platforms
      (self.subscribers.collect { |each| each.normalized_platform_name }).to_set
    end

    def self.subscribers_by_platform(platform_string)
      (self.subscribers.select { |each| (each.normalized_platform_name) == platform_string || (each.platform == platform_string) })
    end

    def self.partial_bundle_name
      'upgrade_bundle_partial.zip'
    end

    def self.full_bundle_name
      'upgrade_bundle.zip'
    end

    def self.document_root=(aString)
      config = self.read_configuration
      config['webserver'] = {'documentRoot' => aString}
      yml = config.to_yaml
      File.open(self.config_filename, 'w') { |file| file.write yml }
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


  end

end