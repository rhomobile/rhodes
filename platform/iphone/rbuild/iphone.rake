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

def run_spec_app(appname)
  rhobuildyml = File.join(basedir,'rhobuild.yml')
  rhobuild = YAML::load_file(rhobuildyml)
  rhobuild['env']['app'] = app_expanded_path(appname)
  File.open(rhobuildyml,'w') {|f| f.write rhobuild.to_yaml}
  $app_path = File.expand_path(File.join(basedir,'spec',appname))
  $app_config = Jake.config(File.open(File.join($app_path, "build.yml")))
  $config = Jake.config(File.open(rhobuildyml,'r'))
  Rake::Task.tasks.each { |t| t.reenable }
  Rake::Task['run:iphonespec'].invoke
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

    unless File.exists? $homedir + "/.profile"
      File.open($homedir + "/.profile","w") {|f| f << "#" }
      chmod 0744, $homedir + "/.profile"
    end

    if $app_config["iphone"] and $app_config["iphone"]["extensions"]
      $app_config["extensions"] += $app_config["iphone"]["extensions"] if $app_config["extensions"]
      $app_config["iphone"]["extensions"] = nil
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

      if simulator
        ENV["SDKROOT"] ||= $devroot + "/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator" +
              $sdk.gsub(/iphonesimulator/,"") + ".sdk"
      else
        ENV["SDKROOT"] ||= $devroot + "/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS" +
              $sdk.gsub(/iphoneos/,"") + ".sdk"
      end
      ENV["BUILD_DIR"] ||= $startdir + "/platform/iphone/build"
      ENV["TARGET_TEMP_DIR"] ||= $startdir + "/platform/iphone/build/rhorunner.build/#{$configuration}-" +
        ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
      ENV["TEMP_FILES_DIR"] ||= ENV["TARGET_TEMP_DIR"]

      ENV["ARCHS"] ||= simulator ? "i386" : "armv6"
      ENV["RHO_ROOT"] = $startdir

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
      ipath = $config["build"]["iphonepath"]
      cp File.join(ipath, 'icon.png'), File.join(ipath, 'icon.png.bak') unless File.exists? File.join(ipath, 'icon.png.bak')
      cp $app_path + "/icon/icon.png", ipath

      set_signing_identity($signidentity,$provisionprofile,$entitlements.to_s) if $signidentity.to_s != ""

      chdir $config["build"]["iphonepath"]
      args = ['build', '-target', 'rhorunner', '-configuration', $configuration, '-sdk', $sdk]

      puts Jake.run($xcodebuild,args)
      ret = $?

      chdir $startdir
      set_app_name("Rhodes") unless $app_config["name"].nil?
      rm_f File.join(ipath, 'icon.png')
      cp File.join(ipath, 'icon.png.bak'), File.join(ipath, 'icon.png')
      rm_f File.join(ipath, 'icon.png.bak')

      unless ret == 0
        puts "Error cleaning"
        exit 1
      end

    end
    
  end
end

namespace "run" do
  task :buildsim => ["config:iphone", "build:iphone:rhodes"] do
    
     unless $sdk =~ /^iphonesimulator/
       puts "SDK must be one of the iphonesimulator sdks to run in the iphone simulator"
       exit 1       
     end
     `killall "iPhone Simulator"`

     puts "sdk: #{$sdk.inspect.to_s}"
     sdkver = $sdk.gsub(/^iphonesimulator/, '')
     
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
    Rake::Task['run:phone_spec'].invoke
    Rake::Task['run:framework_spec'].invoke    
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
  
  task :phone_spec do
    run_spec_app('phone_spec')
  end
  
  task :framework_spec do
    run_spec_app('framework_spec')
  end

  task :iphonespec => ["clean:iphone",:buildsim] do

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

    total = failed = passed = 1

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

    faillog = []
    getdump = false
    start = Time.now
    io = IO.popen(command)
    io.each { |line|

      puts line if line =~ /\| - it/ or line =~ /\| describe/

      if getdump
        if line =~ /^I/
          getdump = false
        else
          faillog << line
        end
      end

      if line =~ /\*\*\*Failed:\s+(.*)/
        failed = $1
        `killall -9 rhorunner`
      elsif line =~ /\*\*\*Total:\s+(.*)/
        total = $1
      elsif line =~ /\*\*\*Passed:\s+(.*)/
        passed = $1
      end

      if line =~ /\| FAIL:/
        faillog << line.gsub(/I.*APP\|/,"\n\n***")
        getdump = true
      end
    }
    finish = Time.now


    NetHTTPSpecs.stop_server if httpserver

    rm_rf $app_path + "/faillog.txt"
    File.open($app_path + "/faillog.txt", "w") { |io| faillog.each {|x| io << x }  } if failed.to_i > 0
    
    $total ||= 0
    $passed ||= 0
    $failed ||= 0
    
    $total += total.to_i
    $passed += passed.to_i
    $failed += failed.to_i

    puts "************************"
    puts "\n\n"
    puts "Tests completed in #{finish - start} seconds"
    puts "Total: #{total}"
    puts "Passed: #{passed}"
    puts "Failed: #{failed}"
    puts "\n"
    puts "Failures stored in faillog.txt" if failed.to_i > 0
        
    ENV["CFFIXED_USER_HOME"] = old_user_home
    ENV["DYLD_ROOT_PATH"] = old_dyld_root
    ENV["DYLD_FRAMEWORK_PATH"] = old_dyld_framework
    ENV["IPHONE_SIMULATOR_ROOT"] = old_iphone_simulator
    exit $failed.to_i unless $dont_exit_on_failure
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
    task :production => ["config:iphone", "build:iphone:rhodes"]
  end

end
