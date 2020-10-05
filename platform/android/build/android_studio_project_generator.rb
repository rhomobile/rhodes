require 'erb'

class AndroidStudioProjectGenerator
  attr_accessor :rhoRoot
  attr_accessor :appincdir
  attr_accessor :buildMode
  attr_accessor :extLibs
  attr_accessor :externalDeps
  attr_accessor :targetArch
  attr_accessor :compileSdkVersion
  attr_accessor :buildToolsVersion
  attr_accessor :applicationId
  attr_accessor :minSdkVersion
  attr_accessor :targetSdkVersion
  attr_accessor :versionCode
  attr_accessor :versionName

  def render_app_gradle(erbPath)
    @versionCode = @versionName.to_i
    #@compileSdkVersion = @targetSdkVersion

    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end

  def build(target)

    #set intermediate dir
    #set output dir

    #collect app bundle
    #collect rho core native sources and libs             #from existing config file
    #collect rho core resources and assets
    #collect core manifest ( or use app-overrided one )

    #! generate rho native sources ( for compile-time prefs )
    #! generate rho java sources ( for compile-time prefs )

    #each do:
    #collect extensions info ( make AARs from each ext? ):  # pass extensions info object from caller.
    #  native sources
    #  java sources
    #  assets
    #  resources
    #  compiled c libs
    #  compiled jars
    #  maven dependencies
    #  collect manifest changes                             #use standard toolchain ( aapt? )
    #  collect java compile settings                        #pass from caller as object, refactor to move compiler settings to config file ( .yml or? )
    #  collect c compile flags, includes, libs dir etc      #pass from caller as object, refactor to move compiler settings to config file ( .yml or? )

    #  collect studio info: gradle version etc

    #  generate cmakelists
    #  combine resources
    #  combine bundle and extra assets

    #  use Android toolchain to generate manifest
    #  generate gradle from template

  end
end