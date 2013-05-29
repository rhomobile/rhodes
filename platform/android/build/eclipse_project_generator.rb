require 'erb'

class EclipseProjectGenerator
  attr_reader :appName
  attr_reader :appPath
  attr_reader :rhodesPath
  attr_reader :androidSDKTarget
  attr_reader :virtualFolders
  attr_reader :extensions

  def initialize(appName, appPath, rhodesPath, androidTarget)
    @appName = appName
    @appPath = appPath
    @rhodesPath = rhodesPath
    @androidSDKTarget = androidTarget
    @virtualFolders = {'rhodes'=>rhodesPath}
    @extensions = []
  end

  def addVirtualFolder(path)
    @virtualFolders[File.basename(path)] = path
  end

  def addExtension(name, classpaths)
    classpaths.each do |classpath|
      @virtualFolders.each do |folderName, path|
        if classpath.start_with? path
          classpath.gsub! path, folderName
          break
        end
      end
    end
    @extensions << {:name => name, :classpaths => classpaths}
  end

  def render(erbPath)
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end