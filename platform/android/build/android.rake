#
def set_app_name(newname)
  fname = Jake.get_absolute $androidpath + "/Rhodes/res/values/strings.xml"
  buf = File.new(fname,"r").read.gsub(/"app_name">.*<\/string>/,"\"app_name\">#{newname}</string>")
  File.open(fname,"w") { |f| f.write(buf) }

end

namespace "config" do
  task :android => ["config:common"] do
    $java = $config["env"]["paths"]["java"]
    $androidsdkpath = $config["env"]["paths"]["android"]
    $androidplatform = "android-1.1"
    $androidpath = $config["build"]["androidpath"]
    $bindir = $androidpath + "/bin"
    $builddir = $androidpath + "/build"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = $androidpath + "/target"
    $excludelib = ['**/singleton.rb','**/rational.rb','**/rhoframework.rb','**/date.rb']
    $tmpdir =  $bindir +"/tmp"
    $resourcedir = $tmpdir + "/resource"
    $excludeapps = "public/js/iui/**,**/jquery*"
    $libs = $androidpath + "/Rhodes/libs"

    if RUBY_PLATFORM =~ /(win|w)32$/
      $dx = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "dx.bat" )
      $aapt = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "aapt.exe" )
      $apkbuilder = File.join( $androidsdkpath, "tools", "apkbuilder.bat" )
      $androidbin = File.join( $androidsdkpath, "tools", "android.bat" )
      $emulator = "cmd /c " + File.join( $androidsdkpath, "tools", "emulator.exe" )
      $adb = File.join( $androidsdkpath, "tools", "adb.exe" )
      $exe_ext = ".exe"
      $path_separator = ";"
    else
      #XXX make these absolute
      $dx = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "dx" )
      $aapt = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "aapt" )
      $apkbuilder = File.join( $androidsdkpath, "tools", "apkbuilder" )
      $androidbin = File.join( $androidsdkpath, "tools", "android" )
      $emulator = File.join( $androidsdkpath, "tools", "emulator" )
      $adb = File.join( $androidsdkpath, "tools", "adb" )
      $exe_ext = ""
      $path_separator = ":"
    end

    $keytool = File.join( $java, "keytool" + $exe_ext )
    $jarsigner = File.join( $java, "jarsigner" + $exe_ext )
    $keystoredir = ENV['HOME'] + "/.rhomobile"
    $keystore = $keystoredir + "/keystore"
    $storepass = "81719ef3a881469d96debda3112854eb"
    $keypass = $storepass

    mkdir_p $bindir if not File.exists? $bindir
    mkdir_p $targetdir if not File.exists? $targetdir
    mkdir_p $srcdir if not File.exists? $srcdir
    mkdir_p $libs if not File.exists? $libs


  end
end


namespace "build" do
  namespace "android" do
    desc "Generate R.java file"
    task :rjava => "config:android" do

      manifest = Jake.get_absolute $androidpath + "/Rhodes/AndroidManifest.xml"
      resource = Jake.get_absolute $androidpath + "/Rhodes/res"
      assets = Jake.get_absolute $androidpath + "/Rhodes/assets"
      rjava = Jake.get_absolute $androidpath + "/Rhodes/gen/com/rhomobile/rhodes"
      androidjar = $androidsdkpath + "/platforms/" + $androidplatform + "/android.jar"

      mkdir_p $tmpdir
      cp resource + "/drawable/icon.png",$tmpdir + "/icon.png.bak"
      cp $config["env"]["app"] + "/icon/icon.png", resource + "/drawable"
      set_app_name($config["env"]["appname"]) unless $config["env"]["appname"].nil?

      args = ["package","-f","-M",manifest,"-S", resource,"-A", assets,"-I",androidjar,"-J", rjava  ]
      puts Jake.run($aapt,args)

      set_app_name("Rhodes") unless $config["env"]["appname"].nil?
      mv $tmpdir + "/icon.png.bak",resource + "/drawable/icon.png"

      unless $? == 0
        puts "Error in AAPT"
        exit 1
      end

    end
    desc "Build RhoBundle for android"
    task :rhobundle => "config:android" do
      Rake::Task["build:bundle:xruby"].execute

      cp_r $srcdir + "/apps", $androidpath + "/Rhodes/assets"
      cp_r $bindir + "/RhoBundle.jar", $libs

      Rake::Task["build:android:rjava"].execute
    end

    desc "Build RubyVM for android"
    task :rubyvm => "config:android" do
      javac = $config["env"]["paths"]["java"] + "/javac" + $exe_ext
      cp_r "platform/shared/rubyJVM", $bindir

      rm_rf $tmpdir + "/RubyVM"
      mkdir_p $tmpdir + "/RubyVM"
      
      args = []
      args << "-g"
      args << "-d"
      args << '"' +$tmpdir + '/RubyVM"'
      args << "-source"
      args << "1.6"
      args << "-target"
      args << "1.6"
      args << "-nowarn"
      args << "-encoding"
      args << "latin1"
      args << "@#{$builddir}/RubyVM_build.files"
      puts Jake.run(javac,args)
      unless $? == 0
        puts "Error compiling java code"
        exit 1
      end

      args = ["cf","../../RubyVM.jar", "#{$all_files_mask}"]
      puts Jake.run($config["env"]["paths"]["java"] + "/jar" + $exe_ext, args, "#{$tmpdir}/RubyVM/")
      unless $? == 0
        puts "Error running jar"
        exit 1
      end

      cp_r $bindir + "/RubyVM.jar", $libs

    end
    desc "Build Rhodes for android"
    task :rhodes => [:rubyvm, :rhobundle] do
      javac = $config["env"]["paths"]["java"] + "/javac" + $exe_ext

      rm_rf $tmpdir + "/Rhodes"
      mkdir_p $tmpdir + "/Rhodes"

      args = []
      args << "-g"
      args << "-d"
      args << '"' +$tmpdir + '/Rhodes"'
      args << "-source"
      args << "1.6"
      args << "-target"
      args << "1.6"
      args << "-nowarn"
      args << "-encoding"
      args << "latin1"
      args << "@#{$builddir}/RhodesGEN_build.files"
      puts Jake.run(javac,args)
      unless $? == 0
        puts "Error compiling java code"
        exit 1
      end

      args = []
      args << "-g"
      args << "-d"
      args << '"' +$tmpdir + '/Rhodes"'
      args << "-source"
      args << "1.6"
      args << "-target"
      args << "1.6"
      args << "-nowarn"
      args << "-classpath"
      args << "#{$androidsdkpath}/platforms/#{$androidplatform}/android.jar" + $path_separator + "#{$tmpdir}/Rhodes" + $path_separator + "#{$libs}/RubyVM.jar" + $path_separator + "#{$libs}/RhoBundle.jar"
      args << "@#{$builddir}/RhodesSRC_build.files"
      puts Jake.run(javac,args)
      unless $? == 0
        puts "Error compiling java code"
        exit 1
      end




      args = ["cf","../../Rhodes.jar", "#{$all_files_mask}"]
      puts Jake.run($config["env"]["paths"]["java"] + "/jar" + $exe_ext, args, "#{$tmpdir}/Rhodes/")
      unless $? == 0
        puts "Error running jar"
        exit 1
      end
    end

    desc "build all"
    task :all => [:rubyvm, :rhobundle, :rhodes]
  end
end

namespace "package" do
  task :android => "build:android:all" do
    puts "Running dx utility"
    args = []
    args << "--dex"
    outfile = Jake.get_absolute("#{$bindir}/classes.dex")
    args << "--output=#{outfile}"
    args << Jake.get_absolute("#{$bindir}/Rhodes.jar")
    args << Jake.get_absolute("#{$bindir}/RubyVM.jar")
    args << Jake.get_absolute("#{$bindir}/RhoBundle.jar")
    puts Jake.run($dx,args)
    unless $? == 0
      puts "Error running DX utility"
      exit 1
    end

    manifest = Jake.get_absolute $androidpath + "/Rhodes/AndroidManifest.xml"
    resource = Jake.get_absolute $androidpath + "/Rhodes/res"
    assets = Jake.get_absolute $androidpath + "/Rhodes/assets"
    androidjar = "#{$androidsdkpath}/platforms/#{$androidplatform}/android.jar"
    resourcepkg = Jake.get_absolute $bindir + "/rhodes.ap_"

    puts "Packaging Assets and Jars"

    cp resource + "/drawable/icon.png",$tmpdir + "/icon.png.bak"
    cp $config["env"]["app"] + "/icon/icon.png", resource + "/drawable"
    set_app_name($config["env"]["appname"]) unless $config["env"]["appname"].nil?

    puts `#{$aapt} package -f -M "#{manifest}" -S "#{resource}" -A "#{assets}" -I "#{androidjar}" -F "#{resourcepkg}"`
    returnval = $?

    set_app_name("Rhodes") unless $config["env"]["appname"].nil?
    mv $tmpdir + "/icon.png.bak",resource + "/drawable/icon.png"

    unless returnval == 0
      puts "Error running AAPT"
      exit 1
    end
    
  end
end

namespace "device" do
  namespace "android" do
    desc "build self signed for debug"
    task :debug => "package:android" do
      dexfile = Jake.get_absolute $bindir + "/classes.dex"
      apkfile = Jake.get_absolute $targetdir + "/Rhodes-debug.apk"
      resourcepkg = Jake.get_absolute $bindir + "/rhodes.ap_"

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -z "#{resourcepkg}" -f "#{dexfile}"`
      unless $? == 0
        puts "Error building APK file"
        exit 1
      end

    end
    desc "build signed for production"
    task :production => "package:android" do
      dexfile = Jake.get_absolute $bindir + "/classes.dex"
      apkfile = Jake.get_absolute $targetdir + "/Rhodes.apk"
      signedapkfile = Jake.get_absolute $targetdir + "/Rhodes_signed.apk"
      resourcepkg = Jake.get_absolute $bindir + "/rhodes.ap_"

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -u -z "#{resourcepkg}" -f "#{dexfile}"`
      unless $? == 0
        puts "Error building APK file"
        exit 1
      end

      if not File.exists? $keystore
        puts "Generating private keystore..."
        mkdir_p $keystoredir

        args = []
        args << "-genkey"
        args << "-alias"
        args << "rhomobile.keystore"
        args << "-keyalg"
        args << "RSA"
        args << "-validity"
        args << "20000"
        args << "-keystore"
        args << '"' + $keystore + '"'
        args << "-storepass"
        args << $storepass
        args << "-keypass"
        args << $keypass
        puts Jake.run($keytool, args)
        unless $? == 0
          puts "Error generating keystore file"
          exit 1
        end
      end

      puts "Signing APK file"
      args = []
      args << "-verbose"
      args << "-keystore"
      args << '"' + $keystore + '"'
      args << "-storepass"
      args << $storepass
      args << "-signedjar"
      args << '"' + signedapkfile + '"'
      args << '"' + apkfile + '"'
      args << "rhomobile.keystore"
      puts Jake.run($jarsigner, args)
      unless $? == 0
        puts "Error running jarsigner"
        exit 1
      end
    end
  end
end

namespace "run" do
  desc "build and launch emulator"
  task :android => "device:android:debug" do
    apkfile = Jake.get_absolute $targetdir + "/Rhodes-debug.apk"
    puts `#{$adb} start-server`
    sleep 5

    system("#{$androidbin} create avd --name rhoAndroid11 --target 1 --sdcard 32M --skin HVGA")

    Thread.new { system("#{$emulator} -avd rhoAndroid11") }

    sleep 10

    puts "Waiting for emulator to get started"
    $stdout.flush
    puts `#{$adb} wait-for-device`
    sleep 10

    puts "Loading package into emulator"
    theoutput = `#{$adb} install -r "#{apkfile}"`
    count = 0
    while (theoutput.to_s.match(/Error Type/) or theoutput.to_s.match(/Fail/))  and count < 15 do
      puts "Failed to load (possibly because emulator not done launching)- retrying"
      $stdout.flush
      sleep 5
      count += 1
      theoutput = `#{$adb} install -r "#{apkfile}"`
    end
    puts "Loading complete, you may now run the application"
  end
end

namespace "clean" do
  namespace "android" do
    task :assets => "config:android" do
      Dir.glob($androidpath + "/Rhodes/assets/apps/**/*") do |f|
        rm_rf f unless f =~ /\/loading.html$/
      end
    end
    task :files => "config:android" do
      rm_rf $targetdir
      rm_rf $bindir
      rm_rf $srcdir
    end
    desc "clean android"
    task :all => [:assets,:files]
  end
end

namespace "prebuild" do
  desc "Build binaries for anroid to be inserted into gem"
  task :android => "build:android:all" do
    prebuilt = "rhodes/rhodes-build/res/prebuilt/android"

    if File.exists? $bindir + "/RubyVM.jar" and File.exists? $bindir + "/Rhodes.jar"
      rm_rf prebuilt if File.exists? prebuilt
      mkdir_p prebuilt 

      cp $androidpath + "/Rhodes/assets/apps/loading.html", prebuilt
      cp $androidpath + "/Rhodes/AndroidManifest.xml", prebuilt

      cp_r $androidpath + "/Rhodes/res", prebuilt

      mkdir_p prebuilt + "/src/com/rhomobile/rhodes"

      cp_r $androidpath + "/Rhodes/src/com/rhomobile/rhodes/AndroidR.java", prebuilt + "/src/com/rhomobile/rhodes"
    
      mkdir_p prebuilt + "/classes"

      Jake.unjar($bindir + "/RubyVM.jar", prebuilt + "/classes")
      Jake.unjar($bindir + "/Rhodes.jar", prebuilt + "/classes")
      rm_rf prebuilt + "/classes/META-INF"
    end
  end
end
