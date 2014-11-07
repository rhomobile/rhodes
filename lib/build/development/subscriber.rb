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
      if ['ANDROID'].include?(@platform.downcase)
        return 'android'
      end
      @platform.downcase
    end

    def has_ip(aString)
      self.ip == aString
    end

    def notify_url(filename)
      query = "package_url=#{Configuration::webserver_uri}/download/#{self.normalized_platform_name}/#{filename}&server_ip=#{Configuration::webserver_uri.host}&server_port=#{Configuration::webserver_uri.port}"
      url = "http://#{@uri}/development/update_bundle"
      URI("#{url}?#{query}")
    end

    def notify(anUrl)
      print "Notifying #{self} ...".primary
      begin
        http = Net::HTTP.new(anUrl.host, anUrl.port)
        http.open_timeout = 5
        http.start() { |http|
          http.get(anUrl.path + '?' + anUrl.query)
        }
        puts 'done'.success
      rescue *Configuration::handledNetworkExceptions => e
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