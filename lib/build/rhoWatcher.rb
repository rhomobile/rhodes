#Set $LISTEN_GEM_DEBUGGING to 1 or 2 for debugging
#More information on https://github.com/guard/listen
#
$LISTEN_GEM_DEBUGGING = 1


require 'pathname'
require 'listen'
require_relative 'ExtendedString'

class RhoWatcherSubscriber
  def uri=(anUri)
    @uri = anUri
  end

  def uri
    @uri
  end

  def platform=(aString)
    @platform = aString
  end

  def platform
    @platform
  end

  def name=(aString)
    @name = aString
  end

  def name
    @name
  end

  def applicationName=(aString)
    @applicationName = aString
  end

  def applicationName
    @applicationName
  end

  def buildTask
    "build:#{@platform}:upgrade_package_partial"
  end

  def to_s
    "RhoWatcherSubscriber(uri=#{@uri}, name=#{@name}, platform=#{@platform}, appName=#{@applicationName})"
  end

end

class RhoWatcher
  def initialize
    @subscribers = Array.new
    @listeners = Array.new
    @serverRoot = Dir.mktmpdir
  end

  def addSubscriber(aSubscriber)
    @subscribers << aSubscriber
  end

  def addDirectory(aString)
    listener = Listen.to(aString) do |modified, added, removed|
      self.onFileChanged(added, modified, removed)
    end
    @listeners << listener
  end

  def serverUri=(anUri)
    @serverUri = anUri
  end

  def serverUri
    @serverUri
  end

  def serverRoot
    @serverRoot
  end

  def applicationRoot=(aString)
    @applicationRoot = aString
  end

  def applicationRoot
    @applicationRoot
  end

  def downloadedBundleName
    'bundle.zip'
  end

  def hasSubscriberWithUri(aString)
    (@subscribers.index { |each| each.uri == aString }) != nil
  end

  def removeSubsriberWithUri(aString)
    @subscribers.delete_if { |each| each.uri == aString }
  end

  def writeUpdatedListFile(addedFiles, changedFiles)
    self.writeArrayToFile('upgrade_package_add_files.txt', addedFiles + changedFiles)
  end

  def writeRemovedListFile(removedFiles)
    self.writeArrayToFile('upgrade_package_remove_files.txt', removedFiles)
  end

  def writeArrayToFile(filename, anArray)
    File.open(File.join(@applicationRoot, filename), 'w') { |file|
      anArray.each { |each| file.puts(self.relativePath(each)) }
    }
  end


  def onFileChanged(addedFiles, changedFiles, removedFiles)
    puts 'Files changed...'
    self.createDiffFiles(addedFiles, changedFiles, removedFiles)
    self.createBundles
    self.notifySubscribers
    puts 'Subscribers have been notified'
  end

  def relativePath(aString)
    first = Pathname @applicationRoot
    second = Pathname aString
    second.relative_path_from first
  end


  def createDiffFiles(addedFiles, changedFiles, removedFiles)
    puts 'Create diff files'.primary
    self.writeUpdatedListFile(addedFiles, changedFiles)
    self.writeRemovedListFile(removedFiles)
  end


  def createBundles
    puts "Building".primary

    buildedPlatforms = []
    @subscribers.each { |each|

      if !buildedPlatforms.include?(each.platform)
        puts "#{each.platform} will build"
        buildedPlatforms << each.platform
        Rake::Task[each.buildTask].invoke
        from = File.join($targetdir, "upgrade_bundle_partial.zip")
        to = File.join(@serverRoot, 'download', each.platform, self.downloadedBundleName)
        FileUtils.mkpath(File.dirname(to))
        FileUtils.cp(from, to)
      else
        puts "#{each.platform} already built"
      end
    }
  end

  def notifySubscribers
    @subscribers.each { |each|
      #TODO Create method urlForUpdate in Subscriber with "http://#{each.uri}/development/update_bundle" ?
      url = URI("http://#{each.uri}/development/update_bundle?http://#{@serverUri.host}:#{@serverUri.port}/#{each.platform}/#{self.downloadedBundleName}")
      puts "Send to #{each}".primary
      begin
        http = Net::HTTP.new(url.host, url.port)
        http.open_timeout = 5
        http.start() { |http|
          http.get(url.path)
        }
      rescue Errno::ECONNREFUSED,
          Net::OpenTimeout => e
        #TODO may be it is necessary remove subscriber from list?
        puts "#{each} is not accessible".warning
      end
    }
  end

  def run
    webServer = WEBrick::HTTPServer.new :BindAddress => @serverUri.host, :Port => @serverUri.port, :DocumentRoot => @serverRoot
    self.configureWebServer(webServer)

    webServerThread = Thread.new do
      webServer.start
    end
    puts 'WebServer started'.primary
    @listeners.each { |each| each.start }
    puts 'FileSystem listeners started'.primary

    trap 'INT' do
      webServer.shutdown
    end

    webServerThread.join

  end

  def configureWebServer(webServer)
    webServer.mount File.join(@serverRoot, 'download'), WEBrick::HTTPServlet::FileHandler, './download/'

    webServer.mount_proc '/register' do |request, response|
      puts "Trying to register subscriber from #{request.remote_ip}".primary
      begin
        self.onRegisterRequest(request, response)
      rescue => e
        puts "Internal Server Error #{e.message}".alarm
        response.body = 'Internal Server Error'
        response.status = 500
        response.content_length = response.body.length
      end
    end

    webServer.mount_proc '/unregister' do |request, response|
      puts "Trying to unregister subscriber from #{request.remote_ip}".primary
      begin
        self.onUnregisterRequest(request, response)
      rescue => e
        puts "Internal Server Error #{e.message}".alarm
        response.body = 'Internal Server Error'
        response.status = 500
        response.content_length = response.body.length
      end
    end
  end

  def onUnregisterRequest(request, response)
    requiredKeys = ['ip', 'port']
    if ((request.query.keys - requiredKeys).length != (request.query.keys.length - requiredKeys.length))
      puts "Invalid unregister request".alarm
      response.body = "Invalid request"
      response.status = 400
      response.content_length = response.body.length
      return
    end

    uri = "#{request.query['ip'].to_s}:#{request.query['port'].to_s}"

    if !hasSubscriberWithUri(uri)
      puts "Subscriber with uri #{uri} is not registered".warning
      response.body = "Subscriber is not registered"
      response.status = 200
      response.content_length = response.body.length
      return
    end

    self.removeSubsriberWithUri(uri)
    puts "Subscriber with uri #{uri} unregistered".primary
    response.body = "Subscriber unregistered"
    response.status = 200
    response.content_length = response.body.length
  end


  def onRegisterRequest(request, response)

    requiredKeys = ['ip', 'port', 'deviceName', 'appName', 'platform']
    #TODO It need to prevent from empty data also
    if ((request.query.keys - requiredKeys).length != (request.query.keys.length - requiredKeys.length))
      puts "Invalid registration request".alarm
      response.body = "Invalid request"
      response.status = 400
      response.content_length = response.body.length
      return
    end

    uri = "#{request.query['ip'].to_s}:#{request.query['port'].to_s}"

    if hasSubscriberWithUri(uri)
      puts "Subscriber with uri #{uri} already registered".warning
      response.body = "Subscriber already registered"
      response.status = 200
      response.content_length = response.body.length
      return
    end

    subscriber = RhoWatcherSubscriber.new
    subscriber.uri = uri
    subscriber.name = request.query['deviceName'].to_s
    subscriber.platform = request.query['platform'].to_s
    subscriber.applicationName = request.query['appName'].to_s

    self.addSubscriber(subscriber)
    puts "#{subscriber} registered successfully".primary
    response.body = "Subscriber registered"
    response.status = 200
    response.content_length = response.body.length
  end

end