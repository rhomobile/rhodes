module RhoDevelopment

  class Subscriber

    @uri
    @platform
    @name
    @application
    @enabled

    attr_accessor :uri
    attr_accessor :platform
    attr_accessor :name
    attr_accessor :application

    def initialize
      self.beEnabled
    end

    def ip
      @uri.split(':')[0]
    end

    def port
      @uri.split(':')[1]
    end

    def enabled?
      @enabled
    end

    def beEnabled
      @enabled = true
    end

    def beDisabled
      @enabled = false
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
      url = URI("http://#{@uri}/development/update_bundle")
      params = []
      params << "package_url=http://#{Configuration::webserver_ip}:#{Configuration::webserver_port}/download/#{self.normalized_platform_name}/#{filename}"
      params << "server_ip=#{Configuration::webserver_ip}"
      params << "server_port=#{Configuration::webserver_port}"
      if Configuration::auto_refresh? then
        params << 'refresh_after_update=true'
      end
      query = params.join('&')
      url.query = query
      #puts url.to_s.warning
      return url
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
      "#{self.class}(uri=#{@uri}, name=#{@name}, platform=#{@platform}, app=#{@application}, enabled=#{@enabled})"
    end

  end

end