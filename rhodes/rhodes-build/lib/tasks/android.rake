require File.join(File.dirname(__FILE__),'..','jake.rb')

namespace "config" do
  task :android => :common do

    $deploydir = File.join($basedir,'deploy','android')
    $excludelib = ['**/singleton.rb','**/TestServe.rb','**/rhoframework.rb','**/date.rb']

    if RUBY_PLATFORM =~ /(win|w)32$/
      $dx = "dx.bat"
      $aapt = "aapt.exe"
      $apkbuilder = "apkbuilder.bat"
      $rhoruby = "RhoRuby.exe"
      $emulator = "cmd /c emulator.exe"
      $adb = "adb.exe"
    else
      $dx = "dx"
      $aapt = "aapt"
      $apkbuilder = "apkbuilder"
      $rhoruby = "RubyMac"
      $emulator = "emulator"
      $adb = "adb"
    end

  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "bundle" do
  task :android =>  ["config:android", "loadframework", "makedirs"] do
    android = $config["env"]["paths"]["android"]

    rm_rf $srcdir
    mkdir_p $srcdir
    mkdir_p File.join($srcdir,'apps')

    compileERB = File.join($compileERBbase,'bb.rb')
    rubypath =  File.join($res,$rhoruby)
    xruby =  File.join($res,'xruby-0.3.3.jar')

    dest = $srcdir 
    chdir $rhodeslib
    Dir.glob("*").each { |f| 
      src = f
      cp_r src,dest
    }
    chdir dest
    Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
    Dir.glob("**/erb.rb").each {|f| rm f}
    Dir.glob("**/find.rb").each {|f| rm f}
    $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

    chdir $basedir
#throw "ME"
    cp_r 'app',File.join($srcdir,'apps')
    cp_r 'public', File.join($srcdir,'apps')
    cp   'rhoconfig.txt', File.join($srcdir,'apps')
    #cp   'index.erb', File.join($srcdir,'apps')
    #cp   'layout.erb', File.join($srcdir,'apps')
    #cp   'loading.html', File.join($srcdir,'apps')
    cp   $appmanifest, $srcdir
    puts `#{rubypath} -R#{$rhodeslib} #{$srcdir}/createAppManifest.rb` 
    rm   File.join($srcdir,'createAppManifest.rb')
    cp   compileERB, $srcdir
    puts `#{rubypath} -R#{$rhodeslib} #{$srcdir}/bb.rb` 

    chdir $bindir
    puts `java -jar #{xruby} -c RhoBundle`
    chdir $srcdir
    Dir.glob("**/*.rb") { |f| rm f }
    Dir.glob("**/*.erb") { |f| rm f }
    puts `jar uf ../RhoBundle.jar apps/*.*`
    mkdir_p "../assets"
    cp_r "apps","../assets"
    cp File.join($prebuilt,"android","loading.html"), "../assets/apps"
    
    chdir $basedir
    
    rm_rf $targetdir
    mkdir_p $targetdir

    chdir $bindir
  
    classes = File.join($prebuilt,"android","classes")
    rhobundle = File.join($bindir,"RhoBundle.jar")
    dexfile = File.join($bindir,"classes.dex")

    puts "Running dx utility"
    puts `#{$dx} --dex "--output=#{dexfile}" #{classes} #{rhobundle}`

    manifest = File.join($prebuilt,"android","AndroidManifest.xml")
    resource = File.join($prebuilt,"android","res")
    assets = File.join($bindir,"assets")
    androidjar = File.join(android,"android.jar")
    resourcepkg = File.join($bindir,"rhodes.ap_")

    puts "Packaging Assets and Jars"
    puts `#{$aapt} package -f -M "#{manifest}" -S "#{resource}" -A "#{assets}" -I "#{androidjar}" -F "#{resourcepkg}"`


    chdir $basedir
  end
end

namespace "device" do
  desc "Create downloadable app for android"
  task :android => "bundle:android" do
      chdir $bindir

      dexfile = File.join($bindir,"classes.dex")
      apkfile = File.join($targetdir,"Rhodes.apk")
      resourcepkg = File.join($bindir,"rhodes.ap_")

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -u -z "#{resourcepkg}" -f "#{dexfile}"`
      chdir $basedir
  end
end

namespace "run" do
  namespace "android" do
    desc "Run app in Android Sim"
    task :app  => ["bundle:android"] do

      chdir $bindir

      dexfile = File.join($bindir,"classes.dex")
      apkfile = File.join($targetdir,"Rhodes-debug.apk")
      resourcepkg = File.join($bindir,"rhodes.ap_")

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -z "#{resourcepkg}" -f "#{dexfile}"`
      chdir $basedir

      puts `#{$adb} start-server`
      sleep 5

      system("android create avd --name rhoAndroid11 --target 1 --sdcard 32M --skin HVGA")

      Thread.new { system("#{$emulator} -avd rhoAndroid11") }

      sleep 10
      
      puts "Waiting for emulator to get started"
      $stdout.flush
      puts `#{$adb} wait-for-device`
      sleep 60
      apkfile = File.join($targetdir,"Rhodes-debug.apk")    

      puts "Loading package into emulator"
      theoutput = `#{$adb} install -r "#{apkfile}"`
      count = 0
      while theoutput.to_s.match(/Error Type/) and count < 15 do
        puts "Failed to load (possibly because emulator not done launching)- retrying"
        $stdout.flush
        sleep 5
        count += 1
        theoutput = `#{$adb} install -r "#{apkfile}"`
      end
      puts "Loading complete, you may now run the application"      
      
    end
  end
end

namespace "check" do
  task :android => "config:android" do
    errors = Array.new

  



    begin
      android = $config["env"]["paths"]["android"]
    rescue
      puts " - Error parsing build.yml make sure you have all of the required fields (see generated build.yml)"
      errors << "invalid build.yml"
    end

    if RUBY_PLATFORM =~ /(win|w)32$/
      curr = pwd
      if android.downcase[0] != curr.downcase[0]
        puts " - Error due to android sdk issues, your app folder must be on the same drive as your sdk folder"
        errors << "must be on the same drive as android sdk"
      end
    end

    if not FileTest.exists? android
      puts " - android sdk not found. Make sure you have the correct path to the android sdk in your build.yml file "
      errors << "android sdk missing"
    end


    begin
      blah = `#{$dx} 2>&1`
    rescue
      puts " - dx is not on the path. Make sure you have the android sdk tools folder in your path"
      errors << "dx not on path"
    end

    begin
      blah = `#{$aapt} 2>&1`
    rescue
      puts " - aapt is not on the path. Make sure you have the android sdk tools folder in your path"
      errors << "aapt not on path"
    end

    begin
      blah = `#{$apkbuilder} 2>&1`
    rescue
      puts " - apkbuilder is not on the path. Make sure you have the android sdk tools folder in your path"
      errors << "apkbuilder not on path"
    end


    begin
      blah = `jar 2>&1`
    rescue
      puts " - jar is not on the path. Make sure you have the java sdk bin folder in your path"
      errors << "jar not on path"
    end

    begin
      blah = `java 2>&1`
    rescue
      puts " - java is not on the path. Make sure you have the java sdk bin folder in your path"
      errors << "java not on path"
    end

    puts "\nANDROID: " + android

    if errors.size > 0
      puts "\nFound the following errors for windows mobile: "
      errors.each { |error| puts "\t" + error.to_s }
    else
      puts "Android config appears valid"
    end

  end
end
