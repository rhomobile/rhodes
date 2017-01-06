require 'erb'

class AndroidStudioProjectGenerator
  attr_accessor :rhoRoot
  attr_accessor :compileSdkVersion
  attr_accessor :buildToolsVersion
  attr_accessor :applicationId
  attr_accessor :minSdkVersion
  attr_accessor :targetSdkVersion
  attr_accessor :versionCode
  attr_accessor :versionName

  def render_app_gradle(erbPath)
    @versionCode = @versionName.to_i
    @compileSdkVersion = @targetSdkVersion

    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end