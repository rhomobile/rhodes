def set_app_name(newname)
  fname = $config["build"]["iphonepath"] + "/Info.plist"
  nextline = false
  replaced = false
  buf = ""
  File.new(fname,"r").read.each_line do |line|
    if nextline and not replaced
      return if line =~ /#{newname}/
      buf << line.gsub(/<string>.*<\/string>/,"<string>#{newname}</string>")
      puts "set name"
      replaced = true
    else
      buf << line
    end

    nextline = true if line =~ /CFBundleDisplayName/
    
  end
  
  File.open(fname,"w") { |f| f.write(buf) }

end

def set_signing_identity(identity,entitlements)
  fname = $config["build"]["iphonepath"] + "/rhorunner.xcodeproj/project.pbxproj"
  buf = ""
  File.new(fname,"r").read.each_line do |line|
      line.gsub!(/CODE_SIGN_ENTITLEMENTS = .*;/,"CODE_SIGN_ENTITLEMENTS = \"#{entitlements}\";")
      line.gsub!(/CODE_SIGN_IDENTITY = .*;/,"CODE_SIGN_IDENTITY = \"#{identity}\";")
      line.gsub!(/"CODE_SIGN_IDENTITY\[sdk=iphoneos\*\]" = .*;/,"\"CODE_SIGN_IDENTITY[sdk=iphoneos*]\" = \"#{identity}\";")
      
      puts line if line =~ /CODE_SIGN/
      buf << line
  end
  
  File.open(fname,"w") { |f| f.write(buf) }

end


namespace "config" do
  task :iphone => ["config:common", "switch_app"] do
    $config["platform"] = "iphone"
    $rubypath = "res/build-tools/RubyMac" #path to RubyMac
    iphonepath = $config["build"]["iphonepath"]
    $builddir = iphonepath + "/rbuild"
    $bindir = Jake.get_absolute(iphonepath) + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = iphonepath + "/target" 
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir =  $bindir +"/tmp"

    $homedir = `echo ~`.to_s.strip
    $simapp="#{$homedir}/Library/Application Support/iPhone Simulator/User/Applications"
    $simlink="#{$homedir}/Library/Application Support/iPhone Simulator/User/Library/Preferences"
    $sim="/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications"
    $guid="364FFCAF-C71D-4543-B293-9058E31CFFEE"
    $applog = File.join($homedir,$app_config["applog"]) if $app_config["applog"] 

    if $app_config["iphone"].nil?
      $signidentity = $config["env"]["iphone"]["codesignidentity"]
      $entitlements = $config["env"]["iphone"]["entitlements"]
      $configuration = $config["env"]["iphone"]["configuration"]
      $sdk = $config["env"]["iphone"]["sdk"]
    else
      $signidentity = $app_config["iphone"]["codesignidentity"]
      $entitlements = $app_config["iphone"]["entitlements"]
      $configuration = $app_config["iphone"]["configuration"]
      $sdk = $app_config["iphone"]["sdk"]
    end

    unless File.exists? $homedir + "/.profile"
      File.open($homedir + "/.profile") {|f| f << "#" }
    end
  end
end

namespace "build" do
  namespace "iphone" do
#    desc "Build iphone rhobundle"
    task :rhobundle => ["config:iphone"] do
      chdir 'platform/iphone'
      rm_rf 'bin'
      rm_rf 'build/Debug-*'
      rm_rf 'build/Release-*'
      
      chdir $startdir

      Rake::Task["build:bundle:noxruby"].execute

    end
    
#    desc "Build rhodes"
    task :rhodes => ["config:iphone", "build:iphone:rhobundle"] do
  
      set_app_name($app_config["name"]) unless $app_config["name"].nil?
      cp $app_path + "/icon/icon.png", $config["build"]["iphonepath"]

      set_signing_identity($signidentity,$entitlements.to_s) if $signidentity.to_s != ""

      chdir $config["build"]["iphonepath"]
      args = ['build', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk]

      puts Jake.run("xcodebuild",args)
      unless $? == 0
        puts "Error cleaning"
        exit 1
      end
      chdir $startdir

    end
    
  end
end

namespace "run" do
  desc "Builds everything, launches iphone simulator"
  task :iphone => ["config:iphone", "build:iphone:rhodes"] do
    
     unless $sdk =~ /^iphonesimulator/
       puts "SDK must be one of the iphonesimulator sdks to run in the iphone simulator"
       exit 1       
     end
     `killall "iPhone Simulator"`
     
     rhorunner = $config["build"]["iphonepath"] + "/build/#{$configuration}-iphonesimulator/rhorunner.app"

     Find.find($simapp) do |path| 
       if File.basename(path) == "rhorunner.app"
         $guid = File.basename(File.dirname(path))
       end
     end
    
     simrhodes = File.join($simapp,$guid)
     rm_rf simrhodes
   
     mkdir_p File.join(simrhodes,"Documents")
     mkdir_p File.join(simrhodes,"Library","Preferences")
     
     puts `cp -R -p "#{rhorunner}" "#{simrhodes}"`
     puts `ln -f -s "#{$simlink}/com.apple.PeoplePicker.plist" "#{simrhodes}/Library/Preferences/com.apple.PeoplePicker.plist"`
     puts `ln -f -s "#{$simlink}/.GlobalPreferences.plist" "#{simrhodes}/Library/Preferences/.GlobalPreferences.plist"`

     puts `echo "#{$applog}" > "#{simrhodes}/Documents/rhologpath.txt"`
     
     f = File.new("#{$simapp}/#{$guid}.sb","w")
     f << "(version 1)\n(debug deny)\n(allow default)\n"
     f.close
     
     
     system("open \"#{$sim}/iPhone Simulator.app\"")
  end
end

namespace "clean" do
  desc "Clean iphone"
  task :iphone => ["clean:iphone:all"]
  namespace "iphone" do
#    desc "Clean rhodes binaries"
    task :rhodes => ["config:iphone"] do 
      chdir $config["build"]["iphonepath"]
    
      args = ['clean', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk]
      puts Jake.run("xcodebuild",args)
      unless $? == 0
        puts "Error cleaning"
        exit 1
      end
      chdir $startdir
      
      chdir 'platform/iphone'
       rm_rf 'build/Debug-*'
       rm_rf 'build/Release-*'
      chdir $startdir
    
    end
    
#    desc "Clean rhobundle"
    task :rhobundle => ["config:iphone"] do
      rm_rf $bindir
    end

    task :all => ["clean:iphone:rhodes", "clean:iphone:rhobundle"]
  end
end

namespace "device" do
  namespace "iphone" do
    desc "Builds and signs iphone for production"
    task :production => ["config:iphone", "build:iphone:rhodes"]
  end

end