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

    def notify_url(filename)
      server_ip = Configuration::own_ip_address
      server_port = Configuration::webserver_port
      host = Configuration::webserver_uri
      platform = self.normalized_platform_name
      query = "package_url=#{host}/download/#{platform}/#{filename}&server_ip=#{server_ip}&server_port=#{server_port}"
      url = "http://#{@uri}/development/update_bundle"
      URI("#{url}?#{query}")
    end

    def notifyUrl(anUrl)
      print "Notifying #{self} ...".primary
      begin
        http = Net::HTTP.new(anUrl.host, anUrl.port)
        http.open_timeout = 5
        http.start() { |http|
          http.get(anUrl.path + '?' + anUrl.query)
        }
        puts 'done'.success
      rescue Errno::ECONNREFUSED,
          Net::OpenTimeout => e
        puts 'failed'.warning
      end
    end

    def partial_notify
      self.notify(self.notify_url(Configuration::partial_bundle_name))
    end

    def full_notify
      self.notify(self.notify_url(Configuration::full_bundle_name))
    end


    def to_s
      "#{self.class}(uri=#{@uri}, name=#{@name}, platform=#{@platform}, app=#{@application})"
    end

  end

end