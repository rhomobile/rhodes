#Set $LISTEN_GEM_DEBUGGING to 1 or 2 for debugging
#More information on https://github.com/guard/listen
#
#$LISTEN_GEM_DEBUGGING = 2

require 'fileutils'
require 'socket'
require 'uri'
require 'net/http'
require 'json'
require 'yaml'
require 'listen'
require 'typhoeus'
require 'childprocess'
require_relative 'ExtendedString'


class LiveUpdatingConfig
=begin
  I store settings for collaborated classes
=end
  @@applicationRoot

  class << self
    attr_accessor :applicationRoot
  end

  def self.ownIPAddress
    IPSocket.getaddress(Socket.gethostname)
  end

  def self.isWebServerAliveRequest
    URI("http://#{self.webServerUri}/alive")
  end

  def self.stoppingWebServerRequest
    URI("http://#{self.webServerUri}/shutdown")
  end

  def self.webServerUri
    "#{self.ownIPAddress}:#{self.webServerPort}"
  end

  def self.webServerPort
    3000
  end

  def self.configFilename
    File.join(self.applicationRoot, 'dev-config.yml')
  end

  def self.readConfiguration
    if File.exist?(self.configFilename)
      return YAML.load_file(self.configFilename)
    end
    return {}
  end

  def self.subscribers
    subscribers = []
    config = self.readConfiguration
    unless config['devices'].nil?
      config['devices'].each { |each|
        subscriber = Subscriber.new
        subscriber.uri = each['uri']
        subscriber.platform = each['platform']
        subscriber.name = each['name']
        subscriber.application = each['application']
        subscribers << subscriber
      }
    end
    subscribers
  end

  def self.hasSubscribers
    config = self.readConfiguration
    config['devices'].nil? ? false : true
  end

  def self.storeSubscribers(anArray)
    config = self.readConfiguration
    config['devices'] = anArray
    yml = config.to_yaml
    File.open(self.configFilename, 'w') { |file| file.write yml }
  end

  def self.subscriberByIP(aString)
    self.subscribers.each { |each|
      if (each.hasIP(aString))
        return each
      end
    }
    raise "Subscriber with IP #{aString} not found"
  end

  def self.subscriberPlatforms
    (self.subscribers.collect { |each| each.normalizedPlatformName }).to_set
  end

  def self.downloadBundleName
    'bundle.zip'
  end

  def self.documentRoot
    config = self.readConfiguration
    webServerConfig = config['webserver']
    if (webServerConfig.nil? || webServerConfig['documentRoot'].nil?)
      documentRoot = nil
    else
      documentRoot = webServerConfig['documentRoot']
    end
    documentRoot
  end

  def self.documentRoot=(aString)
    config = self.readConfiguration
    puts config
    config['webserver'] = {'documentRoot' => aString}
    puts config
    yml = config.to_yaml
    File.open(self.configFilename, 'w') { |file| file.write yml }
  end


end

#TODO: create DocumentRoot on start and remove it on stop
class WebServerWrapper
=begin
  I'm create web server and configure it for serve requests from subscribers (devices)
=end
  @@webserver

  def self.start
    host = LiveUpdatingConfig::ownIPAddress
    port = LiveUpdatingConfig::webServerPort
    documentRoot = self.documentRoot
    puts 'Starting web server... '.primary
    @@webserver = WEBrick::HTTPServer.new(
        :Port => port,
        :DocumentRoot => documentRoot,
        :ServerType => WEBrick::SimpleServer,
        :BindAddress => host
    )
    self.configure

    webServerThread = Thread.new do
      @@webserver.start
    end

    begin
      trap 'INT' do
        @@webserver.shutdown
      end
    end

    webServerThread.join


  end

  def self.documentRoot
    documentRoot = LiveUpdatingConfig::documentRoot

    if (documentRoot.nil?)
      documentRoot = Dir.mktmpdir
      LiveUpdatingConfig::documentRoot = documentRoot
    else
      unless File.exist?(documentRoot)
        puts 'Path specified by setting \'webserver/documentRoot\' doesn\'t exist'.primary
        FileUtils.mkpath(documentRoot)
      end
      print 'Cleaning document root directory... '.primary
      FileUtils.rm_rf("#{documentRoot}/.", secure: true)
      puts 'done'.success
    end
    puts "Path '#{documentRoot}' will be used as web server document root".primary
    documentRoot
  end


  def self.configure
    @@webserver.mount_proc '/shutdown' do |request, response|
      response.body = 'Server was shutdown'
      response.status = 200
      response.content_length = response.body.length
      @@webserver.shutdown
    end

    @@webserver.mount_proc '/alive' do |request, response|
      response.body = 'Server is alive'
      response.status = 200
      response.content_length = response.body.length
    end

    @@webserver.mount_proc '/response_from_device' do |request, response|
      if request.query['status'] == 'need_full_update'
        subscriber = LiveUpdatingConfig::subscriberByIP(request.query["ip"])
        puts "#{subscriber} is requesting full update bundle".info
        (BuildServer.new).buildFullBundleForSubscriber(subscriber)
      end
      if request.query['status'] == 'ok'
        subscriber = LiveUpdatingConfig::subscriberByIP(request.query["ip"])
        puts "#{subscriber} applied update bundle successfully".info
      end
      if request.query['status'] == 'error'
        subscriber = LiveUpdatingConfig::subscriberByIP(request.query["ip"])
        puts "#{subscriber} got an error while updating bundle: #{request.query["status"].message}".info
      end
    end

  end

  def self.alive?
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

  def self.stop
    url = LiveUpdatingConfig::stoppingWebServerRequest
    http = Net::HTTP.new(url.host, url.port)
    http.open_timeout = 5
    http.start() { |http|
      http.get(url.path)
      puts 'Web server was shutdown'.primary
    }
  end

  def self.ensureRunning
    print 'Looking for working web server... '.primary
    unless WebServerWrapper.alive?
      puts 'failed'.warning
      process = ChildProcess.build('rake', 'dev:webserver:start')
      process.io.inherit!
      process.cwd = LiveUpdatingConfig::applicationRoot
      process.start
    else
      puts 'server is running'.success
    end
  end

end

class OneTimeUpdater
=begin
 I check source once and if it changed then I should build update bundle and notify subscribers
=end

  def run
    unless LiveUpdatingConfig::hasSubscribers
      puts 'Subscribers not found'.warning
      return
    end
    WebServerWrapper::ensureRunning


    if self.sourceChanged?
      puts 'Source code is changed'.primary
      buildServer = BuildServer.new
      buildServer.buildPartialBundlesForAllSubscribers
    else
      puts 'Source code changes are not detected'.primary
    end
  end

  def sourceChanged?
    devDir = File.join(LiveUpdatingConfig::applicationRoot, '.development')
    updatedListFilename = File.join(LiveUpdatingConfig::applicationRoot, 'upgrade_package_add_files.txt')
    removedListFilename = File.join(LiveUpdatingConfig::applicationRoot, 'upgrade_package_remove_files.txt')
    mkdir_p devDir
    result = false
    LiveUpdatingConfig::subscriberPlatforms.each { |each|
      RhoDevelopment.setup(devDir, each)
      #is_require_full_update = RhoDevelopment.is_require_full_update
      platformResult = RhoDevelopment.check_changes_from_last_build(updatedListFilename, removedListFilename)
      t = platformResult ? 'was changed' : 'don\'t changed'
      puts "Source code for platform #{each} #{t}".primary
      result = result || (platformResult.nil? ? true : platformResult)
    }
    result
  end
end


class AutoUpdater
=begin
  I check source continuously and if it changed then I should build update bundle and notify subscribers
=end

  def initialize
    @listeners = []
  end

  def run
    WebServerWrapper::ensureRunning
    @listeners.each { |each| each.start }

    begin
      sleep 1
    end while self.hasActiveListeners
  end

  def hasActiveListeners
    @listeners.any? { |each| each.processing? }
  end

  def addDirectory(aString)
    listener = Listen.to(aString, debug: true) do |modified, added, removed|
      self.onFileChanged(added, modified, removed)
    end
    @listeners << listener
  end

  def onFileChanged(addedFiles, changedFiles, removedFiles)
    puts 'Files changed...'
    puts "File added: #{addedFiles}"
    puts "File changed: #{changedFiles}"
    puts "File removed: #{removedFiles}"
    begin
      self.createDiffFiles(addedFiles, changedFiles, removedFiles)
      buildServer = BuildServer.new
      buildServer.buildPartialBundlesForAllSubscribers
    rescue => e
      puts 'Exception...'.warning
      puts e.message.warning
      puts e.backtrace.inspect
    end
  end

  def createDiffFiles(addedFiles, changedFiles, removedFiles)
    self.writeUpdatedListFile(addedFiles, changedFiles)
    self.writeRemovedListFile(removedFiles)
  end

  def writeUpdatedListFile(addedFiles, changedFiles)
    self.writeArrayToFile('upgrade_package_add_files.txt', addedFiles + changedFiles)
  end

  def writeRemovedListFile(removedFiles)
    self.writeArrayToFile('upgrade_package_remove_files.txt', removedFiles)
  end

  def writeArrayToFile(filename, anArray)
    path = File.join(LiveUpdatingConfig::applicationRoot, filename)
    File.open(path, 'w') { |file|
      anArray.each { |each| file.puts(self.relativePath(each)) }
    }
  end

  def relativePath(aString)
    first = Pathname LiveUpdatingConfig::applicationRoot
    second = Pathname aString
    second.relative_path_from first
  end

end


class BuildServer
=begin
  I build update bundles, copy them to document root of web server
  and notify subscribers about ready updates
=end

  def buildPartialBundlesForAllSubscribers
    puts "Start building partial bundles for all subscribers".primary
    builtPlatforms = []
    LiveUpdatingConfig::subscribers.each { |each|
      builtPlatforms << each.platform
      self.buildPartialBundleForSubscriber(each)
      self.copyPlatformBundleToWebServerRoot(each.platform, "upgrade_bundle_partial.zip")
    }
    puts "Partial bundles for all subscribers were built".primary
    self.notifySubscribers
  end

  def buildPartialBundleForSubscriber(aSubscriber)
    puts "Start building partial bundle for #{aSubscriber.platform}".primary
    RhoDevelopment.setup(File.join(LiveUpdatingConfig::applicationRoot, '.development'), aSubscriber.normalizedPlatformName)
    RhoDevelopment.make_partial_bundle
    puts "Partial bundle for #{aSubscriber.platform} was built".primary
  end


  def buildFullBundleForSubscriber(aSubscriber)
    puts "Start building full bundle for #{aSubscriber.platform}".primary
    RhoDevelopment.setup(File.join(LiveUpdatingConfig::applicationRoot, '.development'), aSubscriber.normalizedPlatformName)
    RhoDevelopment.make_full_bundle
    self.copyPlatformBundleToWebServerRoot(aSubscriber.platform, "upgrade_bundle.zip")
    puts "Full bundle for #{aSubscriber.platform} was built".primary
    aSubscriber.notify
  end


  def notifySubscribers
    LiveUpdatingConfig::subscribers.each { |subscriber|
      subscriber.notify
    }
    #TODO: Notify also about failed notifications
  end

  def copyPlatformBundleToWebServerRoot(platform, filename)
    from = File.join($targetdir, filename)
    to = File.join(LiveUpdatingConfig::documentRoot, 'download', platform, LiveUpdatingConfig::downloadBundleName)
    FileUtils.mkpath(File.dirname(to))
    FileUtils.cp(from, to)
    puts "Bundle #{filename} for platform #{platform} was copied to web server document root".primary
  end

end

class Subscriber
=begin
  I store information about device and can notify it about ready updates on server
=end

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

  def normalizedPlatformName
    if ['apple', 'iphone'].include?(@platform.downcase)
      return 'iphone'
    end
    @platform.downcase
  end

  def hasIP(aString)
    self.ip == aString
  end

  def notifyUrl
    ip = LiveUpdatingConfig::ownIPAddress
    port = LiveUpdatingConfig::webServerPort
    uri = LiveUpdatingConfig::webServerUri
    urlForDownload = "http://#{uri}/download/#{@platform}/#{LiveUpdatingConfig::downloadBundleName}&server_ip=#{ip}&server_port=#{port}"
    deviceUrl = "http://#{@uri}/development/update_bundle"
    URI("#{deviceUrl}?package_url=#{urlForDownload}")
  end

  def notify
    print "Notifying #{self} ...".primary
    url = self.notifyUrl
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

class DeviceFinder
=begin
  I am responsible for searching devices in local network
  and saving searching result to configuration file
=end
  def run
    print "Discovering... ".primary
    subscribers = self.discovery
    if subscribers.empty?
      puts 'no devices found'.warning
    else
      puts 'done'.success
      puts subscribers.to_s.info
      print 'Storing subscribers...'.primary
      LiveUpdatingConfig::storeSubscribers(subscribers)
      puts 'done'.success
    end
  end

  def discovery
    subscribers = []
    mask = LiveUpdatingConfig::ownIPAddress.split('.')[0, 3].join('.')
    hydra = Typhoeus::Hydra.hydra
    1.upto(254) { |each|
      url = URI("http://#{mask}.#{each}:37579/development/get_info")
      request = Typhoeus::Request.new(url)
      request.options['timeout'] = 5
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
end