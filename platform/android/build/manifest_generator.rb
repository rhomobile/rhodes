require 'erb'

class ManifestGenerator
  attr_reader :javaPackageName
  attr_reader :appPackageName
  attr_accessor :versionName
  attr_accessor :versionCode
  attr_accessor :installLocation
  attr_accessor :minSdkVer
  attr_accessor :targetSdkVer
  attr_accessor :maxSdkVer
  attr_accessor :permissions
  attr_accessor :usesPermissions
  attr_accessor :usesFeatures
  attr_accessor :usesLibraries
  attr_accessor :screenOrientation
  attr_accessor :debuggable
  attr_accessor :allowBackup
  attr_accessor :rhodesActivityIntentFilters
  attr_accessor :hardwareAcceleration
  attr_accessor :apikey
  
  attr_accessor :manifestManifestAdds
  attr_accessor :applicationManifestAdds
  
  def initialize(javaPackage, appPackage, hidden, permissions, features = {})
    @javaPackageName = javaPackage
    @appPackageName = appPackage
    @permissions = {}
    @usesPermissions = permissions
    @usesFeatures = features
    @usesLibraries = {}
    if permissions.index('CAMERA')
      cameraFeatures = {'android.hardware.camera'=>false, 'android.hardware.camera.autofocus'=>false, 'android.hardware.camera.front'=>false, 'android.hardware.camera.flash'=>false}
      @usesFeatures.merge! cameraFeatures
    end
    @rhodesActivityIntentFilters = []
    @rhodesActivityIntentFilters << {:act=>'android.intent.action.VIEW', :cat=>['android.intent.category.DEFAULT', 'android.intent.category.BROWSABLE']}
    unless hidden
      @rhodesActivityIntentFilters << {:act=>'android.intent.action.MAIN', :cat=>['android.intent.category.DEFAULT', 'android.intent.category.LAUNCHER']}
    end
    @screenOrientation = 'unspecified'
    @debuggable = 'false'
    @allowBackup = 'true'
    @hardwareAcceleration = nil
    @manifestManifestAdds = []
    @applicationManifestAdds = []
    @apikey = ''
  end
  
  def addGoogleMaps
    @usesLibraries['com.google.android.maps'] = true 
  end
  
  def addGooglePush(erbPath)
    @permissions["#{@appPackageName}.permission.C2D_MESSAGE"] = 'signature'
    @usesPermissions << "#{@appPackageName}.permission.C2D_MESSAGE"
    @usesPermissions << 'com.google.android.c2dm.permission.RECEIVE'
    @applicationManifestAdds << erbPath
  end
    
  
  
  def addUriParams(scheme, host, pathPrefix = nil)
    @rhodesActivityIntentFilters.each do |filter|
      if filter[:act] == 'android.intent.action.VIEW' && filter[:cat] == ['android.intent.category.DEFAULT', 'android.intent.category.BROWSABLE']
        data = {'android:scheme'=>scheme,'android:host'=>host}
        data['android:pathPrefix'] = pathPrefix if pathPrefix
        filter[:data] = []
        filter[:data] << data
      end
    end
  end
  
  def makeIntentFilter(filterHash)
    filter = REXML::Element.new 'intent-filter'
    filter.add_element('action', 'android:name'=>filterHash[:act]) if filterHash[:act]
    if filterHash[:cat]
      filterHash[:cat].each do |cat|
        filter.add_element('category', 'android:name'=>cat)
      end      
    end
    if filterHash[:data]
      filterHash[:data].each do |data|
        filter.add_element('data', data)
      end
    end
    filter.to_s
  end
  
  def render(erbPath)
    puts "<<<<<<<<<<<<< render #{erbPath}"  
  
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
  
end
