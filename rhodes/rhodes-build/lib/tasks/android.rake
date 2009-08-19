#
require File.join(File.dirname(__FILE__),'..','jake.rb')

def freplace( fname, pattern, str )
  f = File.open( fname )
  strings = f.read
  f.close

  strings.gsub!( pattern, str )

  f = File.new( fname, "w" )
  f.print strings
  f.close
end

namespace "config" do
  task :android => :common do

    if RUBY_PLATFORM =~ /(win|w)32$/
      $all_files_mask = "*.*"
      $exe_ext = ".exe"
      $bat_ext = ".bat"
    else
      $all_files_mask = "*"
      $exe_ext = ""
      $bat_ext = ""
    end

    $tmpdir = File.join( $bindir, "tmp" )

    $deploydir = File.join($basedir,'deploy','android')
    $excludelib = ['**/singleton.rb','**/rational.rb','**/TestServe.rb','**/rhoframework.rb','**/date.rb']

    $java = $config["env"]["paths"]["java"]
    $keytool = File.join( $java, "keytool" + $exe_ext )
    $jarsigner = File.join( $java, "jarsigner" + $exe_ext )

    $androidsdk = $config["env"]["paths"]["android"]
    $androidplatform = "android-1.1"

    $dx = File.join( $androidsdk, "platforms", $androidplatform, "tools", "dx" + $bat_ext )
    $aapt = File.join( $androidsdk, "platforms", $androidplatform, "tools", "aapt" + $exe_ext )
    $apkbuilder = File.join( $androidsdk, "tools", "apkbuilder" + $bat_ext )
    $adb = File.join( $androidsdk, "tools", "adb" + $exe_ext )
    $emulator = File.join( $androidsdk, "tools", "emulator" + $exe_ext )
    $androidjar = File.join( $androidsdk, "platforms", $androidplatform, "android.jar" )

    $keystoredir = ENV['HOME'] + "/.rhomobile"
    $keystore = $keystoredir + "/keystore"
    $storepass = "81719ef3a881469d96debda3112854eb"
    $keypass = $storepass

    if $config["env"].has_key? "application-name"
      $appname = $config["env"]["application-name"]
    else
      $appname = "Rhodes"
    end

    $appid = "com.rhomobile.rhodes." + $appname

  end
end

task :loadframework do
  require 'rhodes-framework'
  puts $rhodeslib
end

namespace "resources" do
  task :android => ["config:android", "loadframework", "makedirs"] do
    android = $config["env"]["paths"]["android"]

    rm_rf $tmpdir
    cp_r File.join( $prebuilt, "android" ), $tmpdir

    manifest = File.join( $tmpdir, "AndroidManifest.xml" )
    res = File.join( $tmpdir, "res" )
    classes = File.join( $tmpdir, "classes" )
    androidr = File.join( $tmpdir, "src", "com", "rhomobile", "rhodes", "AndroidR.java" )
    genr = File.join( $tmpdir, "R.java" )

    # Modify app_name
    freplace( File.join( res, "values", "strings.xml" ), /<string name="app_name">[^>]*<\/string>/, "<string name=\"app_name\">#{$appname}</string>" )
    # Modify package name
    freplace( manifest, /package="[^"]*"/, "package=\"#{$appid}\"" )
    # Generate new R.java and compile it
    puts `#{$aapt} package -M #{manifest} -S #{res} -I #{$androidjar} -J #{$tmpdir}`
    puts `javac -d #{classes} #{genr}`
    # Modify AndroidR import section and compile it
    freplace( androidr, /import com\.rhomobile\..*\.R;/, "import #{$appid}.R;" )
    puts `javac -cp #{classes} -d #{classes} #{androidr}`
  end
end

namespace "bundle" do
  task :android =>  ["config:android", "resources:android", "loadframework", "makedirs"] do
    android = $config["env"]["paths"]["android"]

    rm_rf $srcdir
    mkdir_p $srcdir
    mkdir_p File.join($srcdir,'apps')

    compileERB = File.join($compileERBbase,'bb.rb')
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
    puts `ruby "#{$srcdir}/createAppManifest.rb"`
    rm   File.join($srcdir,'createAppManifest.rb')
    cp   compileERB, $srcdir
    puts `ruby "#{$srcdir}/bb.rb"`
    
    chdir $bindir
    puts `java -jar #{xruby} -c RhoBundle`
    chdir $srcdir
    Dir.glob("**/*.rb") { |f| rm f }
    Dir.glob("**/*.erb") { |f| rm f }
    puts `jar uf ../RhoBundle.jar apps/#{$all_files_mask}`
    mkdir_p "../assets"
    cp_r "apps","../assets"
    cp File.join($prebuilt,"android","loading.html"), "../assets/apps"
    
    chdir $basedir
    
    rm_rf $targetdir
    mkdir_p $targetdir

    chdir $bindir
  
    classes = File.join($tmpdir,"classes")
    rhobundle = File.join($bindir,"RhoBundle.jar")
    dexfile = File.join($bindir,"classes.dex")

    puts "Running dx utility"
    puts `#{$dx} --dex "--output=#{dexfile}" "#{classes}" "#{rhobundle}"`

    manifest = File.join($tmpdir,"AndroidManifest.xml")
    resource = File.join($tmpdir,"res")
    assets = File.join($bindir,"assets")
    resourcepkg = File.join($bindir,"rhodes.ap_")

    puts "Packaging Assets and Jars"
    puts `#{$aapt} package -f -M "#{manifest}" -S "#{resource}" -A "#{assets}" -I "#{$androidjar}" -F "#{resourcepkg}"`


    chdir $basedir
  end
end

namespace "device" do
  desc "Create downloadable signed app for android"
  task :android => "bundle:android" do
      chdir $bindir

      dexfile = File.join($bindir,"classes.dex")
      apkfile = File.join($targetdir,$appname + ".apk")
      signedapkfile = File.join($targetdir,$appname + "_signed.apk")
      resourcepkg = File.join($bindir,"rhodes.ap_")

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -u -z "#{resourcepkg}" -f "#{dexfile}"`

      if not File.exists? $keystore
        puts "Generating private keystore..."
        mkdir_p $keystoredir

        puts `#{$keytool} -genkey -alias rhomobile.keystore -keyalg RSA -validity 20000 -keystore "#{$keystore}" -storepass #{$storepass} -keypass #{$keypass}`
      end

      puts "Signing APK file"
      puts `#{$jarsigner} -verbose -keystore "#{$keystore}" -storepass #{$storepass} -signedjar "#{signedapkfile}" "#{apkfile}" rhomobile.keystore`

      chdir $basedir
  end
end

namespace "run" do
  namespace "android" do
    desc "Run app in Android Sim"
    task :app  => ["bundle:android"] do

      chdir $bindir

      dexfile = File.join($bindir,"classes.dex")
      apkfile = File.join($targetdir,$appname + "-debug.apk")
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
      sleep 10
      apkfile = File.join($targetdir,$appname + "-debug.apk")    

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
