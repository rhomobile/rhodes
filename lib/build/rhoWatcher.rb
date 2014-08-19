require 'listen'
require 'zip'

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
    @pathes = Array.new
    @listener = nil
  end

  def addDevice(aRhoDevice)
    @devices << aRhoDevice
  end

  def addDirectory(path)
    @pathes << path
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

  def onFileChanged(addedFiles, changedFiles, removedFiles)
    puts "on file changed"
    self.createDiffFiles(addedFiles, changedFiles, removedFiles)
    self.createBundles
    self.sendNotificationsToDevices
  end

  def stop
    #  @listener.stop
  end

  def createDiffFiles(addedFiles, changedFiles, removedFiles)
    puts "create diff files"
    File.open(@applicationRoot + "/upgrade_package_add_files.txt", "w") { |file| addedFiles.each { |each| file.puts(each) }
    changedFiles.each { |each| file.puts(each) }
    }
    File.open(@applicationRoot + "/upgrade_package_remove_files.txt", "w") { |file| removedFiles.each { |each| file.puts(each) } }
  end

  def createBundles
    puts "create builds"
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
    puts "send notifications to devices"
    @devices.each { |each| uri = URI.parse('http://' + each.uri + '/system/update_bundle?http://' + @serverUri + "/" + each.platform + '/bundle.zip')
    Net::HTTP.get_response(uri) }
  end


  def run
=begin
    @listener = Listen.to(*@pathes) do |modified, added, removed|
    self.onFileChanged(added, modified, removed)
  end
  @listener.start
=end
    self.onFileChanged([], [], [])
  end


end