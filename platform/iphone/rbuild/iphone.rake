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
  task :iphone => ["config:common"] do
    $config["platform"] = "iphone"
    $rubypath = "res/build-tools/RubyMac" #path to RubyMac
    iphonepath = $config["build"]["iphonepath"]
    $builddir = iphonepath + "/rbuild"
    $bindir = iphonepath + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = iphonepath + "/target" 
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir =  $bindir +"/tmp"

    $homedir = `echo ~`.to_s.strip
    $simapp="#{$homedir}/Library/Application Support/iPhone Simulator/User/Applications"
    $simlink="#{$homedir}/Library/Application Support/iPhone Simulator/User/Library/Preferences"
    $sim="/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications"
    $guid="364FFCAF-C71D-4543-B293-9058E31CFFEE"
    $applog = File.join($homedir,$app_config["applog"])


  end
end

namespace "build" do
  namespace "iphone" do
    desc "Build iphone rhobundle"
    task :rhobundle => ["config:iphone"] do
      chdir 'platform/iphone'
      rm_rf 'bin'
      rm_rf 'build/Debug-*'
      rm_rf 'build/Release-*'
      
      chdir $startdir

      Rake::Task["build:bundle:noxruby"].execute

    end
    
    desc "Build rhodes"
    task :rhodes => ["config:iphone", "build:iphone:rhobundle"] do
  
      set_app_name($app_config["name"]) unless $app_config["name"].nil?
      cp $app_path + "/icon/icon.png", $config["build"]["iphonepath"]

      set_signing_identity($config["env"]["iphone"]["codesignidentity"],$config["env"]["iphone"]["entitlements"].to_s) if $config["env"]["iphone"]["codesignidentity"].to_s != ""

      chdir $config["build"]["iphonepath"]
      args = ['build', '-target', 'rhorunner', '-configuration', $config["env"]["iphone"]["configuration"], '-sdk', $config["env"]["iphone"]["sdk"]]

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
  desc "Builds and launches app in simulator"
  task :iphone => ["config:iphone", "build:iphone:rhodes"] do
    
     unless $config["env"]["iphone"]["sdk"] =~ /^iphonesimulator/
       puts "SDK must be one of the iphonesimulator sdks"
       exit 1       
     end
     `killall "iPhone Simulator"`
     
     rhorunner = $config["build"]["iphonepath"] + "/build/#{$config["env"]["iphone"]["configuration"]}-iphonesimulator/rhorunner.app"

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
  namespace "iphone" do
    desc "Clean rhodes binaries"
    task :rhodes => ["config:iphone"] do 
      chdir $config["build"]["iphonepath"]
    
      args = ['clean', '-target', 'rhorunner', '-configuration', $config["env"]["iphone"]["configuration"], '-sdk', $config["env"]["iphone"]["sdk"]]
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
    
    desc "Clean rhobundle"
    task :rhobundle => ["config:iphone"] do
      rm_rf $bindir
    end
  end
end
