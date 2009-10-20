require 'find'
require 'erb'

chdir File.dirname(__FILE__)

require 'lib/build/jake.rb'

load 'platform/bb/build/bb.rake'
load 'platform/android/build/android.rake'
load 'platform/iphone/rbuild/iphone.rake'
load 'platform/wm/build/wm.rake'


namespace "config" do
  task :common do
    $startdir = File.dirname(__FILE__)
    buildyml = 'rhobuild.yml'

    buildyml = ENV["RHOBUILD"] unless ENV["RHOBUILD"].nil?
    $config = Jake.config(File.open(buildyml))
    if RUBY_PLATFORM =~ /(win|w)32$/
      $all_files_mask = "*.*"
    else
      $all_files_mask = "*"
    end
    if $app_path.nil? #if we are called from the rakefile directly, this wont be set
      #load the apps path and config
      $app_path = $config["env"]["app"]
      $app_config = YAML::load_file($app_path + "/build.yml")
    end
  end
end

def copy_assets(asset)
  
  dest = File.join($srcdir,'apps/public')
  
  cp_r asset + "/.", dest, :remove_destination => true 
  
end


def common_bundle_start(startdir, dest)
  app = $app_path
  rhodeslib = "lib/framework"

  rm_rf $srcdir
  mkdir_p $srcdir
  mkdir_p dest if not File.exists? dest
  mkdir_p File.join($srcdir,'apps')


  
  chdir rhodeslib
  Dir.glob("*").each { |f|
    src = f
    cp_r src,dest
  }
  chdir dest
  Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
  Dir.glob("**/erb.rb").each {|f| rm f}
  Dir.glob("**/find.rb").each {|f| rm f}
  $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

  chdir startdir
  #throw "ME"
  cp_r app + '/app',File.join($srcdir,'apps')
  cp_r app + '/public', File.join($srcdir,'apps')
  cp   app + '/rhoconfig.txt', File.join($srcdir,'apps')

  copy_assets($assetfolder) if ($assetfolder and File.exists? $assetfolder)
end

def create_manifest
  dir = File.join($srcdir, 'apps')
  fname = "config.rb"
  fappManifest = File.new( File.join(dir,'app_manifest.txt'), "w")

  Find.find(dir) do |path|
    if File.basename(path) == fname

      relPath = path[dir.length+1, File.dirname(path).length-1]   #relative path
      relPath = relPath[0, relPath.length-3] #remove .rb extension
      fappManifest.puts( relPath )

    end
  end

  fappManifest.close()
  
end
  
namespace "build" do
  namespace "bundle" do
    task :xruby do
      #needs $config, $srcdir, $excludelib, $bindir
      app = $config["env"]["app"]
      startdir = pwd
      dest = startdir + "/" + $srcdir
      xruby =  File.dirname(__FILE__) + '/res/build-tools/xruby-0.3.3.jar'

      common_bundle_start(startdir,dest)

      if not $config["excludedirs"].nil?
        if $config["excludedirs"].has_key?($config["platform"])
          chdir File.join($srcdir, 'apps')

          excl = $config["excludedirs"][$config["platform"]]
          excl.each do |mask|
            Dir.glob(mask).each {|f| rm_rf f}
          end
        end
      end
      chdir startdir
      
      #create manifest
      create_manifest
      
      #"compile ERB"
      ext = ".erb"
      Find.find($srcdir) do |path|
        if File.extname(path) == ext
          rbText = ERB.new( IO.read(path) ).src
          newName = File.basename(path).sub('.erb','_erb.rb')
          fName = File.join(File.dirname(path), newName)
          frb = File.new(fName, "w")
          frb.write( rbText )
          frb.close()
        end
      end


      chdir $bindir
      puts `java -jar "#{xruby}" -v -c RhoBundle 2>&1`
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end
      chdir startdir
      chdir $srcdir
  
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
      
      puts `jar uf ../RhoBundle.jar apps/#{$all_files_mask}`
      unless $? == 0
        puts "Error creating Rhobundle.jar"
        exit 1
      end
      chdir startdir
    end

    task :noxruby do
      app = $config["env"]["app"]
      rhodeslib = "lib/framework"
      compileERB = "lib/build/compileERB/default.rb"
      compileRB = "lib/build/compileRB/compileRB.rb"
      startdir = pwd
      dest = startdir + "/" + $srcdir + "/lib"      

      common_bundle_start(startdir,dest)

      create_manifest
      
      cp   compileERB, $srcdir
      puts "Running default.rb"
      puts `#{$rubypath} -R#{rhodeslib} #{$srcdir}/default.rb` 
      unless $? == 0
        puts "Error interpreting erb code"
        exit 1
      end

      rm "#{$srcdir}/default.rb"

      cp   compileRB, $srcdir
      puts "Running compileRB"
      puts `#{$rubypath} -R#{rhodeslib} #{$srcdir}/compileRB.rb` 
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end

      chdir $srcdir
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
  
      chdir startdir

      cp_r "res/build-toos/db", $srcdir 
    end
  end
end


# Simple rakefile that loads subdirectory 'rhodes' Rakefile
# run "rake -T" to see list of available tasks

desc "Get versions"
task :get_version do
  bbver = "unknown"
  iphonever = "unknown"
  #symver = "unknown"
  wmver = "unknown"
  androidver = "unknown"
  

  File.open("rhobuild.yml","r") do |f|
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

  File.open("lib/build/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rbuildver =  $1
    end
  end

  File.open("lib/framework/version.rb","r") do |f|
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

  File.open("rhobuild.yml","r") { |f| origfile = f.read }
  File.open("rhobuild.yml","w") do |f| 
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
    "lib/build/version.rb",
    "lib/framework/version.rb",
    "lib/framework/rhodes.rb",
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

namespace "prebuild" do
  desc "Prebuild binaries for WM gems"
  task :wm do
    basedir = pwd
    if RUBY_PLATFORM =~ /(win|w)32$/
      rake = "cmd.exe /c rake"
      ant = "ant.bat"
    else
      rake = "rake"
      ant = "ant"
    end

    chdir 'platform/wm/build'

    zip = File.join(basedir,'rhodes/rhodes-build/res/7z.exe')

    puts `#{rake} compile`
      unless $? == 0
        puts "Error compiling WM code"
        exit 1
      end
    chdir '../bin'
    throw "windows build missing" if not File.exists? 'Windows Mobile 6 Professional SDK (ARMV4I)'
  
    puts `#{zip} a -mx=9 -r wm6.7z "Windows Mobile 6 Professional SDK (ARMV4I)"`
    throw "windows zip missing" if not File.exists? 'wm6.7z'
  
    cp "wm6.7z", "../../../rhodes/rhodes-build/res/prebuilt/wm"

    chdir basedir

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


  end

  desc "Prebuild iPhone binaries for gems"
  task :iphone do
    basedir = pwd
    rake = "rake"
    ant = "ant"
    prebuilt = "../../../rhodes/rhodes-build/res/prebuilt/iphone/"

    chdir 'platform/iphone/rbuild'
	# Simulator
    puts `#{ant} clean -Diphone.sdk=iphonesimulator3.0 -Diphone.config=Debug`
    unless $? == 0
      puts "Error cleaning iphone"
      exit 1
    end
    puts `#{ant} buildapp -Diphone.sdk=iphonesimulator3.0 -Diphone.config=Debug`

    throw "cant find rhorunner.app!" if not File.exists? "../build/Debug-iphonesimulator/rhorunner.app"

	mkdir_p prebuilt + "sim"
    rm_rf prebuilt + "sim/rhorunner.app"
    cp_r  "../build/Debug-iphonesimulator/rhorunner.app", prebuilt + "sim/"

    rm_rf prebuilt + "sim/rhorunner.app/apps"
    rm_rf prebuilt + "sim/rhorunner.app/lib"
    chdir basedir
	
	# Device
    #chdir 'platform/iphone/rbuild'	
    #puts `#{ant} clean -Diphone.sdk=iphoneos2.2.1 -Diphone.config=Release`
    #puts `#{ant} buildapp -Diphone.sdk=iphoneos2.2.1 -Diphone.config=Release`

    #throw "cant find rhorunner.app!" if not File.exists? "../build/Release-iphoneos/rhorunner.app"

	#mkdir_p prebuilt + "device"
    #rm_rf prebuilt + "device/rhorunner.app"
    #cp_r  "../build/Release-iphoneos/rhorunner.app", prebuilt + "device/"

    #rm_rf prebuilt + "device/rhorunner.app/apps"
    #rm_rf prebuilt + "device/rhorunner.app/lib"

    #chdir basedir
  end

end

namespace "buildall" do
  namespace "bb" do
    desc "Build all jdk versions for blackberry"
    task :production => "config:common" do
      $config["env"]["paths"].each do |k,v|
        if k.to_s =~ /^4/
          puts "BUILDING VERSION: #{k}"
          $config["env"]["bbver"] = k
          Jake.reconfig($config)
 
          #reset all tasks used for building
          Rake::Task["config:bb"].reenable
          Rake::Task["build:bb:rhobundle"].reenable
          Rake::Task["build:bb:rhodes"].reenable
          Rake::Task["build:bb:rubyvm"].reenable
          Rake::Task["device:bb:dev"].reenable
          Rake::Task["device:bb:production"].reenable
          Rake::Task["device:bb:rhobundle"].reenable
          Rake::Task["package:bb:dev"].reenable
          Rake::Task["package:bb:production"].reenable
          Rake::Task["package:bb:rhobundle"].reenable
          Rake::Task["package:bb:rhodes"].reenable
          Rake::Task["package:bb:rubyvm"].reenable
          Rake::Task["device:bb:production"].reenable
          Rake::Task["clean:bb:preverified"].reenable

          Rake::Task["clean:bb:preverified"].invoke
          Rake::Task["device:bb:production"].invoke
        end
      end

    end
  end
end

