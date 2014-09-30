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
      server_ip = Configuration::own_ip_address
      server_port = Configuration::webserver_port
      host = Configuration::webserver_uri
      platform = self.normalized_platform_name
      filename = Configuration::partial_bundle_name
      query = "package_url=#{host}/download/#{platform}/#{filename}&server_ip=#{server_ip}&server_port=#{server_port}"
      url = "http://#{@uri}/development/update_bundle"
      URI("#{url}?#{query}")
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