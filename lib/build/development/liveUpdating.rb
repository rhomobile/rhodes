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
require_relative '../ExtendedString'


module RhoDevelopment

=begin
  class LiveUpdatingConfig

    @@applicationRoot

    class << self
      attr_accessor :applicationRoot
    end

    def self.own_ip_address
      #IPSocket.getaddress(Socket.gethostname) - it don't work on virtual machine with a lot of network interfaces

      Socket.ip_address_list.select { |each| each.ipv4? and !each.ipv4_loopback? and !each.ipv4_multicast? }.map { |each| each.ip_address }.uniq

    end

    def self.webserver_alive_Request
      URI("http://#{self.webserver_uri}/alive")
    end

    def self.stopping_webserver_request
      URI("http://#{self.webserver_uri}/shutdown")
    end

    def self.webserver_uri
      "#{self.own_ip_address}:#{self.webserver_port}"
    end

    def self.webserver_port
      3000
    end

    def self.config_filename
      File.join(self.applicationRoot, 'dev-config.yml')
    end

    def self.read_configuration
      if File.exist?(self.config_filename)
        return YAML.load_file(self.config_filename)
      end
      return {}
    end

    def self.subscribers
      subscribers = []
      config = self.read_configuration
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
        if (each.has_ip(aString))
          return each
        end
      }
      raise "Subscriber with IP #{aString} not found"
    end

    def self.subscriber_platforms
      (self.subscribers.collect { |each| each.normalized_platform_name }).to_set
    end

    def self.download_bundle_name
      'bundle.zip'
    end

    def self.document_root
      config = self.read_configuration
      webServerConfig = config['webserver']
      if (webServerConfig.nil? || webServerConfig['documentRoot'].nil?)
        documentRoot = nil
      else
        documentRoot = webServerConfig['documentRoot']
      end
      documentRoot
    end

    def self.document_root=(aString)
      config = self.read_configuration
      puts config
      config['webserver'] = {'documentRoot' => aString}
      puts config
      yml = config.to_yaml
      File.open(self.config_filename, 'w') { |file| file.write yml }
    end


  end
=end

=begin
#TODO: create DocumentRoot on start and remove it on stop
  class WebServerWrapper
    @@webserver

    def self.start
      host = LiveUpdatingConfig::own_ip_address
      port = LiveUpdatingConfig::webserver_port
      documentRoot = self.document_root
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

    def self.document_root
      documentRoot = LiveUpdatingConfig::document_root

      if (documentRoot.nil?)
        documentRoot = Dir.mktmpdir
        LiveUpdatingConfig::document_root = documentRoot
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
          subscriber = LiveUpdatingConfig::subscriber_by_ip(request.query["ip"])
          puts "#{subscriber} is requesting full update bundle".info
          (BuildServer.new).build_full_bundle_for_subscriber(subscriber)
        end
        if request.query['status'] == 'ok'
          subscriber = LiveUpdatingConfig::subscriber_by_ip(request.query["ip"])
          puts "#{subscriber} applied update bundle successfully".info
        end
        if request.query['status'] == 'error'
          subscriber = LiveUpdatingConfig::subscriber_by_ip(request.query["ip"])
          puts "#{subscriber} got an error while updating bundle: #{request.query["status"].message}".info
        end
      end

    end

    def self.alive?
      result = true
      url = LiveUpdatingConfig::webserver_alive_Request
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
      url = LiveUpdatingConfig::stopping_webserver_request
      http = Net::HTTP.new(url.host, url.port)
      http.open_timeout = 5
      http.start() { |http|
        http.get(url.path)
        puts 'Web server was shutdown'.primary
      }
    end

    def self.ensure_running
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
=end

=begin
  class OneTimeUpdater
    def run
      unless LiveUpdatingConfig::has_subscribers?
        puts 'Subscribers not found'.warning
        return
      end
      WebServerWrapper::ensure_running


      if self.source_changed?
        puts 'Source code is changed'.primary
        buildServer = BuildServer.new
        buildServer.build_partial_bundles_for_all_subscribers
      else
        puts 'Source code changes are not detected'.primary
      end
    end

    def source_changed?
      devDir = File.join(LiveUpdatingConfig::applicationRoot, '.development')
      updatedListFilename = File.join(LiveUpdatingConfig::applicationRoot, 'upgrade_package_add_files.txt')
      removedListFilename = File.join(LiveUpdatingConfig::applicationRoot, 'upgrade_package_remove_files.txt')
      mkdir_p devDir
      result = false
      LiveUpdatingConfig::subscriber_platforms.each { |each|
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
=end


  class AutoUpdater
=begin
  I check source continuously and if it changed then I should build update bundle and notify subscribers
=end

    def initialize
      @listeners = []
    end

    def run
      WebServerWrapper::ensure_running
      @listeners.each { |each| each.start }

      begin
        sleep 1
      end while self.has_active_listeners
    end

    def has_active_listeners
      @listeners.any? { |each| each.processing? }
    end

    def add_directory(aString)
      listener = Listen.to(aString, debug: true) do |modified, added, removed|
        self.on_file_changed(added, modified, removed)
      end

      @listeners << listener
    end

    def on_file_changed(addedFiles, changedFiles, removedFiles)
      puts 'Files changed...'
      puts "File added: #{addedFiles}"
      puts "File changed: #{changedFiles}"
      puts "File removed: #{removedFiles}"
      begin
        self.create_diff_files(addedFiles, changedFiles, removedFiles)
        buildServer = BuildServer.new
        buildServer.build_partial_bundles_for_all_subscribers
      rescue => e
        puts 'Exception...'.warning
        puts e.message.warning
        puts e.backtrace.inspect
      end
    end

    def create_diff_files(addedFiles, changedFiles, removedFiles)
      self.write_list_of_updated_files(addedFiles, changedFiles)
      self.write_lis_of_removed_files(removedFiles)
    end

    def write_list_of_updated_files(addedFiles, changedFiles)
      self.write_array_to_file('upgrade_package_add_files.txt', addedFiles + changedFiles)
    end

    def write_lis_of_removed_files(removedFiles)
      self.write_array_to_file('upgrade_package_remove_files.txt', removedFiles)
    end

    def write_array_to_file(filename, anArray)
      path = File.join(LiveUpdatingConfig::applicationRoot, filename)
      File.open(path, 'w') { |file|
        anArray.each { |each| file.puts(self.relative_path(each)) }
      }
    end

    def relative_path(aString)
      first = Pathname LiveUpdatingConfig::applicationRoot
      second = Pathname aString
      second.relative_path_from first
    end

  end


=begin
  class BuildServer
    def build_partial_bundles_for_all_subscribers
      puts "Start building partial bundles for all subscribers".primary
      builtPlatforms = []
      Configuration::subscribers.each { |each|
        builtPlatforms << each.platform
        self.build_partial_bundle_for_subscriber(each)
        self.copy_platform_bundle_to_webserver_root(each.platform, "upgrade_bundle_partial.zip")
      }
      puts "Partial bundles for all subscribers were built".primary
    end

    def build_partial_bundle_for_subscriber(aSubscriber)
      puts "Start building partial bundle for #{aSubscriber.platform}".primary
      RhoDevelopment.setup(File.join(Configuration::applicationRoot, '.development'), aSubscriber.normalized_platform_name)
      RhoDevelopment.make_partial_bundle
      puts "Partial bundle for #{aSubscriber.platform} was built".primary
    end


    def build_full_bundle_for_subscriber(aSubscriber)
      puts "Start building full bundle for #{aSubscriber.platform}".primary
      RhoDevelopment.setup(File.join(Configuration::applicationRoot, '.development'), aSubscriber.normalized_platform_name)
      RhoDevelopment.make_full_bundle
      self.copy_platform_bundle_to_webserver_root(aSubscriber.platform, "upgrade_bundle.zip")
      puts "Full bundle for #{aSubscriber.platform} was built".primary
    end

    def build_full_bundle_for_all_subscribers
      Configuration::subscribers.each { |subscriber|
        self.build_full_bundle_for_subscriber(subscriber)
      }
    end

    def copy_platform_bundle_to_webserver_root(platform, filename)
      from = File.join($targetdir, filename)
      to = File.join(Configuration::document_root, 'download', platform, Configuration::download_bundle_name)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
      puts "Bundle #{filename} for platform #{platform} was copied to web server document root".primary
    end

  end
=end



end
