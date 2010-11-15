#

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

def set_app_bundle_identifier(newname)
  fname = $config["build"]["iphonepath"] + "/Info.plist"
  nextline = false
  replaced = false
  buf = ""
  File.new(fname,"r").read.each_line do |line|
    if nextline and not replaced
      return if line =~ /#{newname}/
      buf << line.gsub(/<string>.*<\/string>/,"<string>#{newname}</string>")
      puts "set bundle identifier"
      replaced = true
    else
      buf << line
    end
    nextline = true if line =~ /CFBundleIdentifier/
  end
  File.open(fname,"w") { |f| f.write(buf) }
end

def set_app_url_scheme(newname)
  fname = $config["build"]["iphonepath"] + "/Info.plist"
  nextline = false
  nextnextline = false
  replaced = false
  buf = ""
  File.new(fname,"r").read.each_line do |line|
    if nextline and not replaced
      return if line =~ /#{newname}/
      buf << line.gsub(/<string>.*<\/string>/,"<string>#{newname}</string>")
      puts "set URL Scheme"
      replaced = true
    else
      if nextnextline
          nextline = true
      end
      buf << line
    end
    nextnextline = true if line =~ /CFBundleURLSchemes/
  end
  File.open(fname,"w") { |f| f.write(buf) }
end

def set_app_url_name(newname)
  fname = $config["build"]["iphonepath"] + "/Info.plist"
  nextline = false
  replaced = false
  buf = ""
  File.new(fname,"r").read.each_line do |line|
    if nextline and not replaced
      return if line =~ /#{newname}/
      buf << line.gsub(/<string>.*<\/string>/,"<string>#{newname}</string>")
      puts "set URL name"
      replaced = true
    else
      buf << line
    end
    nextline = true if line =~ /CFBundleURLName/
  end
  File.open(fname,"w") { |f| f.write(buf) }
end

def make_app_info
  fname = File.join($app_path, 'bin', 'target', 'iOS', $sdk,'app_info.txt')
  buf = ""
  urlscheme = 'rhodes'
  urlscheme = $app_config["iphone"]["BundleURLScheme"] unless $app_config["iphone"]["BundleURLScheme"].nil?
  buf << urlscheme
  File.open(fname,"w") { |f| f.write(buf) }
end

ICONS = ['icon', 'icon57', 'icon72', 'icon114']

def restore_app_icon
  puts "restore icon"
  ipath = $config["build"]["iphonepath"]
  ICONS.each do |name|
    ibak = File.join(ipath, name + '.bak')
    icon = File.join(ipath, name + '.png')
    next if !File.exists? ibak
    rm_f icon
    cp ibak, icon
    rm_f ibak
  end
end

def set_app_icon
  puts "set icon"
  ipath = $config["build"]["iphonepath"]
  begin
    ICONS.each do |name|
      ibak = File.join(ipath, name + '.bak')
      icon = File.join(ipath, name + '.png')
      appicon = File.join($app_path, 'icon', name + '.png')
      cp icon, ibak unless File.exists? ibak
      cp appicon, ipath
    end
  rescue => e
    puts "WARNING!!! Can not change icon: #{e.to_s}"
  end
end

LOADINGIMAGES = ['loading', 'loading@2x', 'loading-Portrait', 'loading-PortraitUpsideDown', 'loading-Landscape', 'loading-LadscapeLeft', 'loading-LandscapeRight']

def restore_default_images
  puts "restore_default_images"
  ipath = $config["build"]["iphonepath"]
  LOADINGIMAGES.each do |name|
    defname = name.sub('loading', 'Default')
    ibak = File.join(ipath, defname + '.bak')
    imag = File.join(ipath, defname + '.png')
    rm_f imag
    next if !File.exists? ibak
    rm_f imag
    cp ibak, imag
    rm_f ibak
  end
end

def set_default_images
  puts "set_default_images"
  ipath = $config["build"]["iphonepath"]
  begin
    LOADINGIMAGES.each do |name|
      defname = name.sub('loading', 'Default')
      ibak = File.join(ipath, defname + '.bak')
      imag = File.join(ipath, defname + '.png')
      appimage = File.join($app_path, 'app', name + '.png')
      if File.exists? imag
        cp imag, ibak unless File.exists? ibak
      end
      #bundlei = File.join($srcdir, defname + '.png')
      #cp appimage, bundlei unless !File.exist? appimage
      cp appimage, imag unless !File.exists? appimage
    end
  rescue => e
    puts "WARNING!!! Can not change default image: #{e.to_s}"
  end
end




def set_signing_identity(identity,profile,entitlements)
  fname = $config["build"]["iphonepath"] + "/rhorunner.xcodeproj/project.pbxproj"
  buf = ""
  File.new(fname,"r").read.each_line do |line|
      line.gsub!(/CODE_SIGN_ENTITLEMENTS = .*;/,"CODE_SIGN_ENTITLEMENTS = \"#{entitlements}\";")
      line.gsub!(/CODE_SIGN_IDENTITY = .*;/,"CODE_SIGN_IDENTITY = \"#{identity}\";")
      line.gsub!(/"CODE_SIGN_IDENTITY\[sdk=iphoneos\*\]" = .*;/,"\"CODE_SIGN_IDENTITY[sdk=iphoneos*]\" = \"#{identity}\";")
      if profile and profile.to_s != ""
        line.gsub!(/PROVISIONING_PROFILE = .*;/,"PROVISIONING_PROFILE = \"#{profile}\";")
        line.gsub!(/"PROVISIONING_PROFILE\[sdk=iphoneos\*\]" = .*;/,"\"PROVISIONING_PROFILE[sdk=iphoneos*]\" = \"#{profile}\";")
      end
      
      puts line if line =~ /CODE_SIGN/
      buf << line
  end
  
  File.open(fname,"w") { |f| f.write(buf) }

end

def basedir
  File.join(File.dirname(__FILE__),'..','..','..')
end

def app_expanded_path(appname)
  File.expand_path(File.join(basedir,'spec',appname))
end


namespace "config" do
  task :set_iphone_platform do
    $current_platform = "iphone"
  end

  task :iphone => [:set_iphone_platform, "config:common", "switch_app"] do
    $rubypath = "res/build-tools/RubyMac" #path to RubyMac
    iphonepath = $config["build"]["iphonepath"]
    $builddir = iphonepath + "/rbuild"
    $bindir = Jake.get_absolute(iphonepath) + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = iphonepath + "/target" 
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir =  $bindir +"/tmp"

    $devroot = '/Developer' if $devroot.nil?

    $xcodebuild = $devroot + "/usr/bin/xcodebuild"

    $homedir = ENV['HOME']
    $simdir = "#{$homedir}/Library/Application Support/iPhone Simulator/"
    $sim = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/Applications"
    $guid = `uuidgen`.strip
    $applog = File.join($homedir,$app_config["applog"]) if $app_config["applog"] 


    if $app_config["iphone"].nil?
      $signidentity = $config["env"]["iphone"]["codesignidentity"]
      $provisionprofile = $config["env"]["iphone"]["provisionprofile"]
      $entitlements = $config["env"]["iphone"]["entitlements"]
      $configuration = $config["env"]["iphone"]["configuration"]
      $sdk = $config["env"]["iphone"]["sdk"]
    else
      $signidentity = $app_config["iphone"]["codesignidentity"]
      $provisionprofile = $app_config["iphone"]["provisionprofile"]
      $entitlements = $app_config["iphone"]["entitlements"]
      $configuration = $app_config["iphone"]["configuration"]
      $sdk = $app_config["iphone"]["sdk"]
    end

    if $sdk =~ /iphonesimulator/
      sdkver = $sdk.gsub(/iphonesimulator/,"")
      $sdkroot = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" + sdkver + ".sdk"
    else
      sdkver = $sdk.gsub(/iphoneos/,"")
      $sdkroot = $devroot + "/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS" + sdkver + ".sdk"
    end

    $emulator_version = nil
    plist = File.join($sdkroot, 'System/Library/CoreServices/SystemVersion.plist')
    if File.exists? plist
      File.open(plist, 'r') do |f|
        while line = f.gets
          next unless line =~ /<string>(#{sdkver.gsub('.', '\.')}[^<]*)<\/string>/
          $emulator_version = $1
          break unless $emulator_version.nil?
        end
      end
    end

    unless File.exists? $homedir + "/.profile"
      File.open($homedir + "/.profile","w") {|f| f << "#" }
      chmod 0744, $homedir + "/.profile"
    end

    #if $app_config["iphone"] and $app_config["iphone"]["extensions"]
    #  $app_config["extensions"] += $app_config["iphone"]["extensions"] if $app_config["extensions"]
    #  $app_config["iphone"]["extensions"] = nil
    #end

    # check environment variables setted by XCode (when we executed from XCode)
    #xcode_sdk_name = ENV['SDK_NAME']
    #$sdk = xcode_sdk_name if not xcode_sdk_name.nil?
    #$xcode_sdk_dir = ENV['SDK_DIR']
    #xcode_configuration = ENV['CONFIGURATION']
    #$configuration = xcode_configuration if not xcode_configuration.nil?

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

      Rake::Task["build:iphone:extensions"].execute

      # Store hash
      File.open(File.join($srcdir, "hash"), "w") { |f| f.write(get_dir_hash($srcdir).hexdigest) }
      # Store app name
      File.open(File.join($srcdir, "name"), "w") { |f| f.write($app_config["name"]) }

    end

    task :extensions => "config:iphone" do
      ENV['RHO_PLATFORM'] = 'iphone'
      simulator = $sdk =~ /iphonesimulator/
      ENV["PLATFORM_DEVELOPER_BIN_DIR"] ||= $devroot + "/Platforms/" + ( simulator ? "iPhoneSimulator" : "iPhoneOS" ) +
        ".platform/Developer/usr/bin"
      ENV["SDKROOT"] = $sdkroot

      #ENV["SDKROOT"] = $xcode_sdk_dir if not $xcode_sdk_dir.nil?


      ENV["BUILD_DIR"] ||= $startdir + "/platform/iphone/build"
      ENV["TARGET_TEMP_DIR"] ||= $startdir + "/platform/iphone/build/rhorunner.build/#{$configuration}-" +
        ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
      ENV["TEMP_FILES_DIR"] ||= ENV["TARGET_TEMP_DIR"]

      ENV["ARCHS"] ||= simulator ? "i386" : "armv6"
      ENV["RHO_ROOT"] = $startdir

      # added by dmitrys
      ENV["XCODEBUILD"] = $xcodebuild
      ENV["CONFIGURATION"] ||= $configuration
      ENV["SDK_NAME"] ||= $sdk
 
      puts "extpaths: #{$app_config["extpaths"].inspect.to_s}"
      $stdout.flush
      $app_config["extensions"].each do |ext|
        $app_config["extpaths"].each do |p|
          extpath = File.join(p, ext, 'ext')
          next unless File.executable? File.join(extpath, 'build')

          puts Jake.run('./build', [], extpath)
          exit 1 unless $? == 0
        end
      end
    end
    
#    desc "Build rhodes"
    task :rhodes => ["config:iphone", "build:iphone:rhobundle"] do
  
      set_app_name($app_config["name"]) unless $app_config["name"].nil?
      set_app_bundle_identifier($app_config["iphone"]["BundleIdentifier"]) unless $app_config["iphone"]["BundleIdentifier"].nil?
      set_app_url_scheme($app_config["iphone"]["BundleURLScheme"]) unless $app_config["iphone"]["BundleURLScheme"].nil?
      set_app_url_name($app_config["iphone"]["BundleURLName"]) unless $app_config["iphone"]["BundleURLName"].nil?
 
            

      set_app_icon
      set_default_images

      set_signing_identity($signidentity,$provisionprofile,$entitlements.to_s) if $signidentity.to_s != ""

      chdir $config["build"]["iphonepath"]
      args = ['build', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk]

      puts Jake.run($xcodebuild,args)
      ret = $?

      chdir $startdir
      
      set_app_name("Rhodes") unless $app_config["name"].nil?
      set_app_bundle_identifier("com.rhomobile.rhodes") unless $app_config["iphone"]["BundleIdentifier"].nil?
      set_app_url_scheme("rhodes") unless $app_config["iphone"]["BundleURLScheme"].nil?
      set_app_url_name("com.rhomobile.rhodes") unless $app_config["iphone"]["BundleURLName"].nil?
      
      restore_default_images
      restore_app_icon

      unless ret == 0
        puts "Error cleaning"
        exit 1
      end

    end
    
  end
end

namespace "run" do
  namespace "iphone" do
    task :spec => ["clean:iphone",:buildsim] do

      sdkroot = $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" +
                $sdk.gsub(/iphonesimulator/,"") + ".sdk"

      old_user_home = ENV["CFFIXED_USER_HOME"]
      old_dyld_root = ENV["DYLD_ROOT_PATH"]
      old_dyld_framework = ENV["DYLD_FRAMEWORK_PATH"]
      old_iphone_simulator = ENV["IPHONE_SIMULATOR_ROOT"]

      ENV["CFFIXED_USER_HOME"] = $simrhodes
      ENV["DYLD_ROOT_PATH"] = sdkroot
      ENV["DYLD_FRAMEWORK_PATH"] = sdkroot + "/System/Library/Frameworks"
      ENV["IPHONE_SIMULATOR_ROOT"] = sdkroot

      command = '"' + $simrhodes + '/rhorunner.app/rhorunner"' + " -RegisterForSystemEvents"

      #if someone runs against the wrong app, kill after 120 seconds
      Thread.new {
        sleep 300
        `killall -9 rhorunner`
      }

      `killall -9 rhorunner`

      # Run local http server
      $iphonespec = true
      httpserver = false
      httpserver = true if File.exist? "#{$app_path}/app/spec/library/net/http/http/fixtures/http_server.rb"

      if httpserver
        require "#{$app_path}/app/spec/library/net/http/http/fixtures/http_server"
        NetHTTPSpecs.start_server
      end

      Jake.before_run_spec

      start = Time.now
      io = IO.popen(command)
      io.each do |line|
        Jake.process_spec_output(line)
      end

      Jake.process_spec_results(start)

      $stdout.flush

      NetHTTPSpecs.stop_server if httpserver

      ENV["CFFIXED_USER_HOME"] = old_user_home
      ENV["DYLD_ROOT_PATH"] = old_dyld_root
      ENV["DYLD_FRAMEWORK_PATH"] = old_dyld_framework
      ENV["IPHONE_SIMULATOR_ROOT"] = old_iphone_simulator
      exit $failed.to_i unless $dont_exit_on_failure
    end

    task :phone_spec do
      Jake.run_spec_app('iphone','phone_spec')
    end

    task :framework_spec do
      Jake.run_spec_app('iphone','framework_spec')
    end

    task :allspecs do
      rm_rf basedir + "/faillog.txt"
      $dont_exit_on_failure = true
      Rake::Task['run:iphone:phone_spec'].invoke
      Rake::Task['run:iphone:framework_spec'].invoke
      failure_output = ""
      if $failed.to_i > 0
        failure_output = ""
        failure_output += "phone_spec failures:\n\n" + File.open(app_expanded_path('phone_spec') + "/faillog.txt").read if
          File.exist?(app_expanded_path('phone_spec') + "/faillog.txt")
        failure_output += "framework_spec failures:\n\n" + File.open(app_expanded_path('framework_spec') + "/faillog.txt").read if
          File.exist?(app_expanded_path('framework_spec') + "/faillog.txt")
        chdir basedir
        File.open("faillog.txt", "w") { |io| failure_output.each {|x| io << x }  }
      end
      puts "Agg Total: #{$total}"
      puts "Agg Passed: #{$passed}"
      puts "Agg Failed: #{$failed}"
      exit $failed.to_i
    end


  end

  task :buildsim => ["config:iphone", "build:iphone:rhodes"] do
    
     unless $sdk =~ /^iphonesimulator/
       puts "SDK must be one of the iphonesimulator sdks to run in the iphone simulator"
       exit 1       
     end
     `killall "iPhone Simulator"`

     sdkver = $sdk.gsub(/^iphonesimulator/, '')
     # Workaround: sometimes sdkver could differ from emulator version.
     # Example: iPhone SDK 4.0.1. In this case sdk is still iphonesimulator4.0 but version of simulator is 4.0.1
     sdkver = $emulator_version.to_s unless $emulator_version.nil?

     elements = []
     binplist = File.join(ENV['HOME'], 'Library', 'Preferences', 'com.apple.iphonesimulator.plist')
     xmlplist = '/tmp/iphone.plist'
     if File.exists? binplist
       `plutil -convert xml1 -o #{xmlplist} #{binplist}`

       elements = []
       doc = REXML::Document.new(File.new(xmlplist))
       nextignore = false
       doc.elements.each('plist/dict/*') do |element|
         if nextignore
           nextignore = false
           next
         end
         if element.name == 'key'
           if element.text == 'currentSDKRoot' or element.text == 'SimulateDevice'
             nextignore = true
             next
           end
         end
         
         elements << element
       end
     end

     e = REXML::Element.new 'key'
     e.text = 'SimulateDevice'
     elements << e
     e = REXML::Element.new 'string'
     e.text = sdkver == '3.2' ? 'iPad' : 'iPhone'
     elements << e
     e = REXML::Element.new 'key'
     e.text = 'currentSDKRoot'
     elements << e
     e = REXML::Element.new 'string'
     e.text = $sdkroot
     elements << e

     File.open(xmlplist, 'w') do |f|
       f.puts "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
       f.puts "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
       f.puts "<plist version=\"1.0\">"
       f.puts "<dict>"
       elements.each do |e|
         f.puts "\t#{e.to_s}"
       end
       f.puts "</dict>"
       f.puts "</plist>"
     end

     `plutil -convert binary1 -o #{binplist} #{xmlplist}`

     rhorunner = $config["build"]["iphonepath"] + "/build/#{$configuration}-iphonesimulator/rhorunner.app"
     puts "rhorunner: #{rhorunner}"

     puts "our app name: #{$app_config['name']}"
     puts "simdir: #{$simdir}"

     Dir.glob(File.join($simdir, sdkver, "Applications", "*")).each do |simapppath|
       need_rm = true if File.directory? simapppath
       if File.exists?(File.join(simapppath, 'rhorunner.app', 'name'))
         name = File.read(File.join(simapppath, 'rhorunner.app', 'name'))
         puts "found app name: #{name}"
         guid = File.basename(simapppath)
         puts "found guid: #{guid}"
         if name == $app_config['name']
           $guid = guid
           need_rm = false
         end
       end
       rm_rf simapppath if need_rm
       rm_rf simapppath + ".sb" if need_rm
     end

     puts "app guid: #{$guid}"

     mkdir_p File.join($simdir, sdkver)

     simapp = File.join($simdir, sdkver, "Applications")
     simlink = File.join($simdir, sdkver, "Library", "Preferences")

     $simrhodes = File.join(simapp, $guid)

     mkdir_p File.join($simrhodes, "Documents")
     mkdir_p File.join($simrhodes, "Library", "Preferences")

     rm_rf File.join($simrhodes, 'rhorunner.app')
     cp_r rhorunner, $simrhodes
     ['com.apple.PeoplePicker.plist', '.GlobalPreferences.plist'].each do |f|
       `ln -f -s "#{simlink}/#{f}" "#{$simrhodes}/Library/Preferences/#{f}"`
     end

     `echo "#{$applog}" > "#{$simrhodes}/Documents/rhologpath.txt"`
     rholog = simapp + "/" + $guid + "/Documents/RhoLog.txt"


     simpublic = simapp + "/" + $guid + "/Documents/apps/public"
     apppublic = $app_path + "/sim-public-#{sdkver}"

     apprholog = $app_path + "/rholog-#{sdkver}.txt"
     rm_f apprholog
     rm_f apppublic
     `ln -f -s "#{simpublic}" "#{apppublic}"`
     `ln -f -s "#{rholog}" "#{apprholog}"`
     `echo > "#{rholog}"`
     f = File.new("#{simapp}/#{$guid}.sb","w")
     f << "(version 1)\n(debug deny)\n(allow default)\n"
     f.close
     
  end

  # split this off separate so running it normally is run:iphone
  # testing we will not launch emulator directly
  desc "Builds everything, launches iphone simulator"
  task :iphone => :buildsim do
     system("open \"#{$sim}/iPhone Simulator.app\"")

  end
  
  task :allspecs do
    $dont_exit_on_failure = true
    Rake::Task['run:iphone:phone_spec'].invoke
    Rake::Task['run:iphone:framework_spec'].invoke
    failure_output = ""
    if $failed.to_i > 0
      failure_output = ""
      failure_output += "phone_spec failures:\n\n" + File.open(app_expanded_path('phone_spec') + "/faillog.txt").read if
        File.exist?(app_expanded_path('phone_spec') + "/faillog.txt")
      failure_output += "framework_spec failures:\n\n" + File.open(app_expanded_path('framework_spec') + "/faillog.txt").read if
        File.exist?(app_expanded_path('framework_spec') + "/faillog.txt")
      chdir basedir
      File.open("faillog.txt", "w") { |io| failure_output.each {|x| io << x }  }
    end
    puts "Agg Total: #{$total}"
    puts "Agg Passed: #{$passed}"
    puts "Agg Failed: #{$failed}" 
    exit $failed.to_i
  end
  
end

namespace "clean" do
  desc "Clean iphone"
  task :iphone => ["clean:iphone:all"]
  namespace "iphone" do
#    desc "Clean rhodes binaries"
    task :rhodes => ["config:iphone"] do 

      app_path = File.join($app_path, 'bin', 'target', 'iOS')
      rm_rf app_path

      chdir $config["build"]["iphonepath"]
    
      args = ['clean', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk]
      puts Jake.run($xcodebuild,args)
      unless $? == 0
        puts "Error cleaning"
        exit 1
      end
      chdir $startdir
      
      chdir 'platform/iphone'
       rm_rf 'build/Debug-*'
       rm_rf 'build/Release-*'
      chdir $startdir

      found = true

      while found do
        found = false
        Find.find($simdir) do |path|
          if File.basename(path) == "rhorunner.app"
            $guid = File.basename(File.dirname(path))
            found = true
          end
        end

        if found
         Dir.glob($simdir + '*').each do |sdk|
          simapp = sdk + "/Applications"
          simrhodes = File.join(simapp,$guid)
          rm_rf simrhodes
          rm_rf simrhodes + ".sb"
         end
        end
      end
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
    task :production => ["config:iphone", "build:iphone:rhodes"] do
    
    #copy build results to app folder
    
    app_path = File.join($app_path, 'bin', 'target', 'iOS', $sdk)
    
    iphone_path = $config["build"]["iphonepath"]    
    if $sdk =~ /iphonesimulator/
       iphone_path = File.join(iphone_path, 'build', $configuration+'-iphonesimulator')
    else 
       iphone_path = File.join(iphone_path, 'build', $configuration+'-iphoneos')
    end
    appname = $app_config["name"]
    if appname == nil
       appname = 'rhorunner'
    end
    src_file = File.join(iphone_path, 'rhorunner.app')
    dst_file = File.join(app_path, appname+'.app')
    
    rm_rf dst_file
    rm_rf app_path

    mkdir_p app_path

    puts 'copy result build package to application target folder ...'    
    cp_r src_file, dst_file
    make_app_info 

    end
  end

end
