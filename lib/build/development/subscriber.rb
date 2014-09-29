module RhoDevelopment

  class Subscriber

    @uri
    @platform
    @name
    @application

    attr_accessor :uri
    attr_accessor :platform
    attr_accessor :name
    attr_accessor :application

    def ip
      @uri.split(':')[0]
    end

    def port
      @uri.split(':')[1]
    end

    def normalized_platform_name
      if ['apple', 'iphone'].include?(@platform.downcase)
        return 'iphone'
      end
      @platform.downcase
    end

    def has_ip(aString)
      self.ip == aString
    end

    def notify_url
      ip = Configuration::own_ip_address
      port = Configuration::webserver_port
      uri = Configuration::webserver_uri
      urlForDownload = "http://#{uri}/download/#{self.normalized_platform_name}/#{Configuration::partial_bundle_name}&server_ip=#{ip}&server_port=#{port}"
      deviceUrl = "http://#{@uri}/development/update_bundle"
      URI("#{deviceUrl}?package_url=#{urlForDownload}")
    end

    def notify
      print "Notifying #{self} ...".primary
      url = self.notify_url
      begin
        http = Net::HTTP.new(url.host, url.port)
        http.open_timeout = 5
        http.start() { |http|
          http.get(url.path + '?' + url.query)
        }
        puts 'done'.success
      rescue Errno::ECONNREFUSED,
          Net::OpenTimeout => e
        puts "failed".warning
      end
    end

    def to_s
      "#{self.class}(uri=#{@uri}, name=#{@name}, platform=#{@platform}, app=#{@application})"
    end

  end

end