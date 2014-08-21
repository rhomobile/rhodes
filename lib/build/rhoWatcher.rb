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

end

class RhoWatcher
  def initialize
    @devices = Array.new
    @directories = Array.new
    @serverRoot = Dir.mktmpdir
  end

  def addDevice(aRhoDevice)
    @devices << aRhoDevice
  end

  def addDirectory(aString)
    @directories << aString
  end

  def serverUri=(uri)
    @serverUri = uri
  end

  def serverUri
    @serverUri
  end

  def serverRoot=(aString)
    @serverRoot = aString
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

  def startWebServer
    puts "Create web server..."
    @webServer = WEBrick::HTTPServer.new :BindAddress => @serverUri.host, :Port => @serverUri.port, :DocumentRoot => @serverRoot
    @webServer.mount @serverRoot, WEBrick::HTTPServlet::FileHandler, './'

    @webServerThread = Thread.new do
      puts "Starting web server..."
      @webServer.start
    end
  end

  def onFileChanged(addedFiles, changedFiles, removedFiles)
    puts "On file changed..."
    self.createDiffFiles(addedFiles, changedFiles, removedFiles)
    self.createBundles
    self.sendNotificationsToDevices
  end



  def createDiffFiles(addedFiles, changedFiles, removedFiles)
    puts "Create diff files..."
    File.open(@applicationRoot + "/upgrade_package_add_files.txt", "w") { |file| addedFiles.each { |each| file.puts(each) }
    changedFiles.each { |each| file.puts(each) }
    }
    File.open(@applicationRoot + "/upgrade_package_remove_files.txt", "w") { |file| removedFiles.each { |each| file.puts(each) } }
  end

  def createBundles
    puts "Build bundles..."
    @devices.each { |each|
      taskName = "build:#{each.platform}:upgrade_package_partial"
      Rake::Task[taskName].invoke
      from = File.join($targetdir, "/upgrade_bundle_partial.zip")
      to = File.join(@serverRoot, each.platform, 'upgrade_bundle_partial.zip')
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
    }
  end

  def sendNotificationsToDevices
    puts "Send notifications to devices..."
    @devices.each { |each| uri = URI.parse('http://' + each.uri + '/system/update_bundle?http://' + @serverUri + "/" + each.platform + '/bundle.zip')
    Net::HTTP.get_response(uri) }
  end


  def run
    self.startWebServer
    puts "Create listener..."
    puts @directories
    @listener = Listen.to(*@directories) do |modified, added, removed|
      self.onFileChanged(added, modified, removed)
    end
    @listener.start

    trap 'INT' do
      self.stop
    end

    @webServerThread.join
  end

  def stop
    @listener.stop
    @webServer.shutdown
  end

end