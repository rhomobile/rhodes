# Simple rakefile that loads subdirectory 'rhodes' Rakefile
# run "rake -T" to see list of available tasks

desc "Get versions"
task :get_version do
  bbver = "unknown"
  iphonever = "unknown"
  #symver = "unknown"
  wmver = "unknown"
  androidver = "unknown"
  

  File.open("platform/bb/build/build.yml","r") do |f|
    file = f.read
    if file.match(/version: (\d+\.\d+\.\d+)/)
      bbver = $1
    end
  end

  File.open("platform/iphone/Info.plist","r") do |f|
    file = f.read
    if file.match(/CFBundleVersion<\/key>\s+<string>(\d+\.\d+\.*\d*)<\/string>/)
      iphonever =  $1
    end
  end

  # File.open("platform/symbian/build/release.properties","r") do |f|
  #     file = f.read
  #     major = ""
  #     minor = ""
  #     build = ""
  # 
  #     if file.match(/release\.major=(\d+)/)
  #       major =  $1
  #     end
  #     if file.match(/release\.minor=(\d+)/)
  #       minor =  $1
  #     end
  #     if file.match(/build\.number=(\d+)/)
  #       build =  $1
  #     end
  # 
  #     symver = major + "." + minor + "." + build
  #   end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") do |f|
    file = f.read
    if file.match(/versionName="(\d+\.\d+\.*\d*)"/)
      androidver =  $1
    end
  end

  rhodesver = "unknown"
  rbuildver = "unknown"
  rframever = "unknown"
  rgenver = "unknown"

  File.open("rhodes/rhodes/lib/rhodes.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rhodesver =  $1
    end
  end

  File.open("rhodes/rhodes-build/lib/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rbuildver =  $1
    end
  end

  File.open("rhodes/rhodes-framework/lib/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rframever =  $1
    end
  end

  File.open("rhodes/rhodes-generator/lib/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rgenver =  $1
    end
  end
  

  puts "Versions:"
  puts "  Blackberry:       " + bbver
  puts "  iPhone:           " + iphonever
  #puts "  Symbian:          " + symver
  puts "  WinMo:            " + wmver
  puts "  Android:          " + androidver
  puts "  Rhodes:           " + rhodesver
  puts "  Rhodes Build:     " + rbuildver
  puts "  Rhodes Framework: " + rframever
  puts "  Rhodes Generator: " + rgenver
end

desc "Set version"
task :set_version, [:version] do |t,args|
  throw "You must pass in version" if args.version.nil?
  ver = args.version.split(/\./)
  major = ver[0]
  minor = ver[1]
  build = ver[2]
  
  throw "Invalid version format. Must be in the format of: major.minor.build" if major.nil? or minor.nil? or build.nil?

  verstring = major+"."+minor+"."+build
  origfile = ""

  File.open("platform/bb/build/build.yml","r") { |f| origfile = f.read }
  File.open("platform/bb/build/build.yml","w") do |f| 
    f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  end
  
  File.open("rhodes/rhodes-generator/generators/templates/application/build.yml","r") { |f| origfile = f.read }
  File.open("rhodes/rhodes-generator/generators/templates/application/build.yml","w") do |f| 
    f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  end

  File.open("platform/iphone/Info.plist","r") { |f| origfile = f.read }
  File.open("platform/iphone/Info.plist","w") do |f| 
    f.write origfile.gsub(/CFBundleVersion<\/key>(\s+)<string>(\d+\.\d+\.*\d*)<\/string>/, "CFBundleVersion</key>\n  <string>#{verstring}</string>")
  end

  # File.open("platform/symbian/build/release.properties","r") { |f| origfile = f.read }
  # File.open("platform/symbian/build/release.properties","w") do |f|
  #   origfile.gsub!(/release\.major=(\d+)/,"release.major=#{major}")
  #   origfile.gsub!(/release\.minor=(\d+)/,"release.minor=#{minor}")
  #   origfile.gsub!(/build\.number=(\d+)/,"build.number=#{build}")
  #   f.write origfile
  # end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") { |f| origfile = f.read }
  File.open("platform/android/Rhodes/AndroidManifest.xml","w") do |f|
    origfile.match(/versionCode="(\d+)"/)
    vercode = $1.to_i + 1
    origfile.gsub!(/versionCode="(\d+)"/,"versionCode=\"#{vercode}\"")
    origfile.gsub!(/versionName="(\d+\.\d+\.*\d*)"/,"versionName=\"#{verstring}\"")

    f.write origfile
  end

  ["rhodes/rhodes/lib/rhodes.rb",
   "rhodes/rhodes-build/lib/version.rb",
   "rhodes/rhodes-framework/lib/version.rb",
   "rhodes/rhodes-framework/lib/rhodes.rb",
   "rhodes/rhodes-generator/lib/version.rb"].each do |versionfile|
  
    File.open(versionfile,"r") { |f| origfile = f.read }
    File.open(versionfile,"w") do |f|
      origfile.gsub!(/VERSION = '(\d+\.\d+\.*\d*)'/, "VERSION = '#{verstring}'")
      origfile.gsub!(/DBVERSION = '(\d+\.\d+\.*\d*)'/, "DBVERSION = '#{verstring}'")
      f.write origfile
    end
  end
  Rake::Task[:get_version].invoke  
end

desc "Update prebuild binaries on windows"
task :prebuild_win do
  basedir = pwd
  if RUBY_PLATFORM =~ /(win|w)32$/
    rake = "cmd.exe /c rake"
    ant = "ant.bat"
  else
    rake = "rake"
    ant = "ant"
  end

  chdir 'platform/bb/build'
  puts `#{rake} clean`
  puts `#{rake} build:all`

  throw "blackberry rhodes.jar missing" if not File.exists? '../preverified/rhodes.jar'
  throw "blackberry RubyVM.jar missing" if not File.exists? '../preverified/RubyVM.jar'

  cp "MANIFEST.MF", "../../../rhodes/rhodes-build/res/prebuilt/bb"
  cp "rhodesApp.alx", "../../../rhodes/rhodes-build/res/prebuilt/bb"
  cp "../preverified/rhodes.jar", "../../../rhodes/rhodes-build/res/prebuilt/bb" 
  cp "../preverified/RubyVM.jar", "../../../rhodes/rhodes-build/res/prebuilt/bb" 

  chdir basedir
  chdir 'platform/wm/build'

  zip = File.join(basedir,'rhodes/rhodes-build/res/7z.exe')

  puts `#{rake} compile`

  chdir '../bin'
  throw "windows build missing" if not File.exists? 'Windows Mobile 6 Professional SDK (ARMV4I)'
  
  puts `#{zip} a -mx=9 -r wm6.7z "Windows Mobile 6 Professional SDK (ARMV4I)"`
  throw "windows zip missing" if not File.exists? 'wm6.7z'
  
  cp "wm6.7z", "../../../rhodes/rhodes-build/res/prebuilt/wm"

  # chdir basedir
  #   chdir "platform/symbian/build"
  #   filecontents = ""
  #   File.open("build.properties","r") { |f| filecontents = f.read }
  #   File.open("build.properties","w") do |f|
  #     filecontents.gsub!(/build\.target=[A-Z ]+/,"build.target=GCCE UREL")
  #     f.write filecontents
  #   end
  #   if filecontents.match(/S60_3rd_FP1=(.+)/)
  #     epoc32 = $1
  #   else
  #     epoc32 = "\\Symbian\\9.2\\S60_3rd_FP1\\"
  #   end
  # 
  #   epoc32.gsub!(/\\\\/,"\\")  
  # 
  #   puts `#{ant} build-prebuilt  -DSDK=S60_3rd_FP1`
  #   puts "Looking for: " + File.join(epoc32,"Epoc32\\release\\gcce\\urel\\rhodes.exe")
  #   throw "symbian rhodes.exe missing" if not File.exists?(File.join(epoc32,"Epoc32\\release\\gcce\\urel\\rhodes.exe"))
  # 
  #   prebuilt = "../../../rhodes/rhodes-build/res/prebuilt/symbian/"
  # 
  #   rm_rf prebuilt + "Epoc32"
  # 
  #   mkdir_p prebuilt + "Epoc32/data/z/private/10003a3f"
  #   mkdir_p prebuilt + "Epoc32/data/z/resource"
  #   mkdir_p prebuilt + "Epoc32/data/z/system"
  #   mkdir_p prebuilt + "Epoc32/release/gcce/urel"
  # 
  #   cp_r epoc32 + "Epoc32\\data\\z\\private\\10003a3f\\apps\\", prebuilt + "Epoc32/data/z/private/10003a3f/", :verbose => true
  #   cp_r epoc32 + "Epoc32\\data\\z\\resource\\apps\\", prebuilt + "Epoc32/data/z/resource/"
  #   cp_r epoc32 + "Epoc32\\data\\z\\system\\data\\", prebuilt + "Epoc32/data/z/system/"
  #   cp_r epoc32 + "Epoc32\\release\\gcce\\urel\\rhodes.exe", prebuilt + "Epoc32/release/gcce/urel/"
  #   
  # 
  # 
  #   File.open("build.properties","w") do |f|
  #     filecontents.gsub!(/build\.target=[A-Z ]+/,"build.target=WINSCW UDEB")
  #     f.write filecontents
  #   end
  # 
  #   
  #   
  #   puts `#{ant} build-prebuilt  -DSDK=S60_3rd_FP1`
  #   puts "Looking for: " + File.join(epoc32,"Epoc32\\release\\winscw\\deb\\rhodes.exe")
  #   throw "symbian rhodes.exe missing" if not File.exists?(File.join(epoc32,"Epoc32\\release\\winscw\\udeb\\rhodes.exe"))
  # 
  #   mkdir_p prebuilt + "Epoc32/winscw/c/Data/Rho"
  #   mkdir_p prebuilt + "Epoc32/release/winscw/udeb"
  # 
  #   cp_r epoc32 + "Epoc32\\winscw\\c\\Data\\Rho\\rhologpath.txt", prebuilt + "Epoc32/winscw/c/data/Rho"
  #   cp_r epoc32 + "Epoc32\\release\\winscw\\udeb\\rhodes.exe", prebuilt + "Epoc32/release/winscw/udeb/"

###### build android pre-built binaries ######

  chdir basedir

  require 'rhodes/rhodes-build/lib/jake.rb'

  prebuilt = "rhodes/rhodes-build/res/prebuilt/android/"

  chdir 'platform/android/build'

  config = Jake.config(File.open('build.yml'))

  android_sdk = File.join( config["env"]["paths"]["android_sdk"], "platforms", "android-1.1")

  eclipse_home = config["env"]["paths"]["eclipse_home"]
  javac_home = config["env"]["paths"]["javac_home"]

  puts "Compile RhoBundle, required by Rhodes"
  chdir basedir
  chdir 'platform/android/RhoBundle'    
  puts `#{ant} clean`  
  puts `#{ant} -Djavac.home="#{javac_home}"`

  puts "Compile Rhodes"
  chdir basedir
  chdir 'platform/android/Rhodes'
  
  puts `#{ant} clean`  
  puts `#{ant} build -DECLIPSE_HOME="#{eclipse_home}" -DANDROID_SDK="#{android_sdk}"`
  
  chdir basedir
  chdir 'platform/android'

  rm_rf File.join( basedir, prebuilt)
  mkdir_p File.join( basedir, prebuilt )

  puts "copy classes"
  mkdir_p File.join( basedir, prebuilt, 'classes' )
  cp_r File.join( basedir, 'platform', 'android', 'Rhodes', 'bin', 'com' ), File.join( basedir, prebuilt, 'classes' )

  cp_r File.join( basedir, 'platform', 'android', 'RubyJVM', 'bin', 'com' ), File.join( basedir, prebuilt, 'classes' )
  cp_r File.join( basedir, 'platform', 'android', 'RubyJVM', 'bin', 'j2me' ), File.join( basedir, prebuilt, 'classes' )
  cp_r File.join( basedir, 'platform', 'android', 'RubyJVM', 'bin', 'j2mex' ), File.join( basedir, prebuilt, 'classes' )
  cp_r File.join( basedir, 'platform', 'android', 'RubyJVM', 'bin', 'javolution' ), File.join( basedir, prebuilt, 'classes' )
  cp_r File.join( basedir, 'platform', 'android', 'RubyJVM', 'bin', 'org' ), File.join( basedir, prebuilt, 'classes' )

  puts "copy res folder"
  mkdir_p File.join( basedir, prebuilt, 'res' )
  cp_r File.join( basedir, 'platform', 'android', 'Rhodes', 'res' ), File.join( basedir, prebuilt )

  puts "copy manifest"
  cp File.join( basedir, 'platform', 'android', 'Rhodes', 'AndroidManifest.xml' ), File.join( basedir, prebuilt )

  puts "copy loading.html"
  cp File.join( basedir, 'platform', 'android', 'Rhodes', 'assets', 'apps', 'loading.html' ), File.join( basedir, prebuilt )

end

desc "Update prebuild binaries on mac"
task :prebuild_mac do
  basedir = pwd
  rake = "rake"
  ant = "ant"
  prebuilt = "../../../rhodes/rhodes-build/res/prebuilt/iphone/"

  chdir 'platform/iphone/rbuild'
  puts `#{ant} clean`
  puts `#{ant} runapp`
  
  throw "cant find rhorunner.app!" if not File.exists? "../build/Debug-iphonesimulator/rhorunner.app"
  
  
  rm_rf prebuilt + "sim/rhorunner.app"
  cp_r  "../build/Debug-iphonesimulator/rhorunner.app", prebuilt + "sim/"
  
  rm_rf prebuilt + "sim/rhorunner.app/apps"
  rm_rf prebuilt + "sim/rhorunner.app/lib"
  

  chdir basedir
end
