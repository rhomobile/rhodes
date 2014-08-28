#Set $LISTEN_GEM_DEBUGGING to 1 or 2 for debugging
#More information on https://github.com/guard/listen
#
$LISTEN_GEM_DEBUGGING = 1

require 'pathname'
require 'listen'

class RhoDevice
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
    "RhoDevice(uri=#{@uri}, name=#{@name}, platform=#{@platform}, appName=#{@applicationName})"
  end
end

class RhoWatcher
  def initialize
    @devices = Array.new
    @listeners = Array.new
    @serverRoot = Dir.mktmpdir
  end

  def addDevice(aRhoDevice)
    #TODO It need to prevent register device app twice. What does means "twice"?
    @devices << aRhoDevice
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
    self.sendNotificationsToDevices
    puts 'All devices have been notified'
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
    #TODO Platform can occur more than once. It need build only once for each platform
    @devices.each { |each|
      Rake::Task[each.buildTask].invoke
      from = File.join($targetdir, "upgrade_bundle_partial.zip")
      to = File.join(@serverRoot, 'download', each.platform, self.downloadedBundleName)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
    }
  end

  def sendNotificationsToDevices
    @devices.each { |each|
      #TODO Create method urlForUpdate in RhoDevice with "http://#{each.uri}/development/update_bundle" ?
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
        #TODO may be it is necessary remove device from list?
        puts "#{each} is not accessible".warning
      end
    }
  end

  def run
    webServer = WEBrick::HTTPServer.new :BindAddress => @serverUri.host, :Port => @serverUri.port, :DocumentRoot => @serverRoot
    webServer.mount File.join(@serverRoot, 'download'), WEBrick::HTTPServlet::FileHandler, './download/'

    webServer.mount_proc '/register' do |request, response|
      requiredKeys = ['ip', 'port', 'deviceName', 'appName', 'platform']
      puts "Trying to register device from #{request.remote_ip}".primary
      #TODO It need to prevent from empty data also
      if ((request.query.keys - requiredKeys).length != (request.query.keys.length - requiredKeys.length))
        puts "Invalid register request".alarm
        response.body = "Invalid request"
        response.status = 400
        response.content_length = response.body.length
      else
        device = RhoDevice.new
        device.uri = "#{request.query['ip'].to_s}:#{request.query['port'].to_s}"
        device.name = request.query['deviceName'].to_s
        device.platform = request.query['platform'].to_s
        device.applicationName = request.query['appName'].to_s
        self.addDevice(device)
        puts "Device #{device} registered successfully".primary
        response.body = "Device registered"
        response.status = 200
        response.content_length = response.body.length
      end
    end

    webServerThread = Thread.new do
      webServer.start
    end
    puts 'Web server started'.primary
    @listeners.each { |each| each.start }
    puts 'File system listeners started'.primary

    trap 'INT' do
      webServer.shutdown
    end

    webServerThread.join
  end


end