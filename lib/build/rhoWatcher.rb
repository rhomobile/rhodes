#Set $LISTEN_GEM_DEBUGGING to 1 or 2 for debugging
#More information on https://github.com/guard/listen
#
$LISTEN_GEM_DEBUGGING = 1

require 'pathname'
require 'listen'

class RhoDevice
  def initialize(anUri, aString)
    @uri = anUri
    @platform = aString
  end

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

  def buildTask
    "build:#{@platform}:upgrade_package_partial"
  end
end

class RhoWatcher
  def initialize
    @devices = Array.new
    @listeners = Array.new
    @serverRoot = Dir.mktmpdir
  end

  def addDevice(aRhoDevice)
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
    puts 'Create diff files...'
    self.writeUpdatedListFile(addedFiles, changedFiles)
    self.writeRemovedListFile(removedFiles)
  end




  def createBundles
    puts "Building..."
    #TODO Platform can occur more than once. It need build only once for each platform
    @devices.each { |each|
      Rake::Task[each.buildTask].invoke
      from = File.join($targetdir, "upgrade_bundle_partial.zip")
      to = File.join(@serverRoot, each.platform, self.downloadedBundleName)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
    }
  end

  def sendNotificationsToDevices
    @devices.each { |each|
      #TODO Create method urlForUpdate in RhoDevice with "http://#{each.uri}/development/update_bundle" ?
      url = URI("http://#{each.uri}/development/update_bundle?http://#{@serverUri.host}:#{@serverUri.port}/#{each.platform}/#{self.downloadedBundleName}")
      puts "Send to #{url}"
      begin
        http = Net::HTTP.new(url.host, url.port)
        http.open_timeout = 5
        http.start() { |http|
          http.get(url.path)
        }
      rescue  Errno::ECONNREFUSED,
              Net::OpenTimeout  => e
        #TODO may be it is necessary remove device from list?
        puts "device #{each.uri} is not accessible"
      end
    }
  end

  def run
    webServer = WEBrick::HTTPServer.new :BindAddress => @serverUri.host, :Port => @serverUri.port, :DocumentRoot => @serverRoot
    webServer.mount @serverRoot, WEBrick::HTTPServlet::FileHandler, './'

    webServerThread = Thread.new do
      webServer.start
    end
    puts 'Web server started'
    @listeners.each { |each| each.start }
    puts 'File system listeners started'

    trap 'INT' do
      webServer.shutdown
    end

    webServerThread.join
  end


end