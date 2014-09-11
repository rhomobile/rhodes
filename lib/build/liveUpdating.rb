require 'socket'
require 'uri'
require 'net/http'
require 'json'
require 'yaml'
require 'typhoeus'
require_relative 'ExtendedString'


class LiveUpdatingConfig
  @@applicationRoot

  def self.applicationRoot= (aString)
    @@aplicationRoot = aString
  end

  def self.applicationRoot
    @@aplicationRoot
  end

  def self.ownIPAddress
    IPSocket.getaddress(Socket.gethostname)
  end

  def self.isWebServerAliveRequest
    #TODO Remove hard code
    URI("http://#{self.webServerUri}/alive")
  end

  def self.stoppingWebServerRequest
    #TODO Remove hard code
    URI("#{self.webServerUri}/quit")
  end

  def self.webServerUri
    "#{self.ownIPAddress}:#{self.webServerPort}"
  end

  def self.webServerPort
    3000
  end

  def self.documentRoot
    #TODO Remove hard code
    '/Users/mva/Temp'
  end

  def self.configFilename
    File.join(self.applicationRoot, 'dev-config.yml')
  end

  def self.subscribers
    subscribers = []
    if File.exist?(self.configFilename)
      config = YAML.load_file(self.configFilename)
      config['devices'].each { |each|
        subscriber = Subscriber.new
        subscriber.uri = "#{each['uri']}"
        subscriber.platform = each['platform']
        subscriber.name = each['name']
        subscriber.application = each['application']
        subscribers << subscriber
      }
    else
      puts "Devices configuration file #{configFilename} not found".warning
    end
    subscribers
  end

  def self.subscriberPlatforms
    (self.subscribers.collect { |each| each.normalizedPlatformName }).to_set
  end

  def self.downloadBundleName
    'bundle.zip'
  end

end

class WebServerWrapper
  @@webServer

  def self.startServer
    host = LiveUpdatingConfig::ownIPAddress
    port = LiveUpdatingConfig::webServerPort
    documentRoot = LiveUpdatingConfig::documentRoot
    #@@webServer = WEBrick::HTTPServer.new :BindAddress => host, :Port => port, :DocumentRoot => documentRoot
    @@webServer = WEBrick::HTTPServer.new(
        :Port => port,
        :DocumentRoot => documentRoot,
        #:ServerType => WEBrick::Daemon,
        :ServerType => WEBrick::SimpleServer,
        :BindAddress => host
    )
    self.configure
    @@webServer.start
  end

  def self.configure
    @@webServer.mount_proc '/quit' do |request, response|
      response.body = "Server is shot down"
      response.status = 200
      response.content_length = response.body.length
      @@webServer.shutdown
    end

    @@webServer.mount_proc '/alive' do |request, response|
      response.body = "Server is alive"
      response.status = 200
      response.content_length = response.body.length
    end
  end

  def self.isAlive
    result = true
    url = LiveUpdatingConfig::isWebServerAliveRequest
    http = Net::HTTP.new(url.host, url.port)
    http.open_timeout = 5
    begin
      http.start() { |http|
        http.get(url.path)
      }
    rescue Errno::ECONNREFUSED,
        Net::OpenTimeout => e
      result = false
    end
    result
  end

  def self.shotdown
    url = LiveUpdatingConfig::stoppingWebServerRequest
    http = Net::HTTP.new(url.host, url.port)
    http.open_timeout = 5
    http.start() { |http|
      http.get(url.path)
      puts 'Web server is shotdown'.primary
    }
  end

  def self.ensureRunning
    if !WebServerWrapper.isAlive
      puts "Web server is not started".warning
      WebServerWrapper::startServer
      puts "Starting web server".primary
    else
      puts "Web server already started".primary
    end
  end

end

class OneTimeServer


  def run
    WebServerWrapper::ensureRunning
    if self.sourceChanged?
      puts "Source code is changed".primary
      self.buildPartialBundles
      self.notifySubscribers
      puts "Subscribers are notified".primary
    else
      puts "Source code changes are not detected".primary
    end
  end

  def sourceChanged?
    result = false
    devDir = File.join(LiveUpdatingConfig::applicationRoot, '.development')
    updatedListFilename = File.join(LiveUpdatingConfig::applicationRoot, 'upgrade_package_add_files.txt')
    removedListFilename = File.join(LiveUpdatingConfig::applicationRoot, 'upgrade_package_remove_files.txt')
    mkdir_p devDir
    LiveUpdatingConfig::subscriberPlatforms.each { |each|
      RhoDevelopment.setup(devDir, each)
      is_require_full_update = RhoDevelopment.is_require_full_update
      result = RhoDevelopment.check_changes_from_last_build(updatedListFilename, removedListFilename)
    }
    puts "check_changes_from_last_build = #{result}".warning
    result = true if result.nil?
    result
  end

  def buildPartialBundles
    puts "Building".primary

    builtPlatforms = []
    LiveUpdatingConfig::subscribers.each { |each|
      puts "#{each.platform} will built".primary
      builtPlatforms << each.platform
      Rake::Task[each.buildTask].invoke
      from = File.join($targetdir, "upgrade_bundle_partial.zip")
      to = File.join(LiveUpdatingConfig::documentRoot, 'download', each.platform, LiveUpdatingConfig::downloadBundleName)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
      puts "Bundle for platform #{each.platform} was built and put into #{to}".primary
    }
  end

  def notifySubscribers
    LiveUpdatingConfig::subscribers.each { |subscriber|
      self.notifySubscriber(subscriber)
    }
  end

  def notifySubscriber(aSubscriber)
    url = updateUrlForSubscriber(aSubscriber)
    puts "Send to #{aSubscriber}  request #{url}".primary
    begin
      http = Net::HTTP.new(url.host, url.port)
      http.open_timeout = 5
      http.start() { |http|
        http.get(url.path + '?' + url.query)
      }
    rescue Errno::ECONNREFUSED,
        Net::OpenTimeout => e
      puts "#{aSubscriber} is not accessible".warning
    end
  end

  def updateUrlForSubscriber(aSubscriber)
    ip = LiveUpdatingConfig::ownIPAddress
    port = LiveUpdatingConfig::webServerPort
    uri = LiveUpdatingConfig::webServerUri
    urlForDownload = "http://#{uri}/download/#{aSubscriber.platform}/#{LiveUpdatingConfig::downloadBundleName}&server_ip=#{ip}&server_port=#{port}"
    deviceUrl = "http://#{aSubscriber.uri}/development/update_bundle"
    URI("#{deviceUrl}?package_url=#{urlForDownload}")
  end

end

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
    @uri
  end

  def port
    @uri
  end

  def normalizedPlatformName
    if ['apple', 'iphone'].include?(@platform.downcase)
      return 'iphone'
    end
    @platform
  end

  def buildTask
    "build:#{self.normalizedPlatformName}:upgrade_package_partial"
  end

  def to_s
    "#{self.class}(uri=#{@uri}, name=#{@name}, platform=#{@platform}, app=#{@application})"
  end

  def to_hash
    hash = {}
    instance_variables.each { |var| hash[var.to_s.delete("@")] = instance_variable_get(var) }
    hash
  end

end

class DeviceFinder
  def initialize(appDir)
    @applicationDirectory = appDir
  end

  def ownIP
    LiveUpdatingConfig::ownIPAddress
  end

  def run
    puts "Start discovering...".primary
    subscribers = self.discovery
    if subscribers.empty?
      puts 'No devices found'.primary
    else
      puts subscribers.to_s.info
      self.saveSubscribers(subscribers)
    end
  end

  def discovery
    subscribers = []
    mask = self.ownIP.split('.')[0, 3].join('.')
    hydra = Typhoeus::Hydra.hydra
    1.upto(254) { |each|
      url = URI("http://#{mask}.#{each}:37579/development/get_info")
      request = Typhoeus::Request.new(url)
      request.options['timeout']= 1
      request.on_complete do |response|
        if response.code == 200
          data = JSON.parse(response.body)
          subscriber = {}
          subscriber['uri'] = "#{data['ip']}:#{data['port']}"
          subscriber['name'] = data['deviceFriendlyName']
          subscriber['platform'] = data['platform']
          subscriber['application'] = data['applicationName']
          subscribers << subscriber
        end
      end
      hydra.queue request
    }
    hydra.run
    return subscribers
  end

  def saveSubscribers(anArray)
    filename = File.join(@applicationDirectory, 'dev-config.yml')
    if File.exist?(filename)
      config = YAML.load_file(filename)
    else
      config = {"devices" => []}
    end
    config['devices'] = anArray
    yml = config.to_yaml
    File.open(filename, 'w') { |file| file.write yml }
  end

end