#
def set_app_name_android(newname)
  puts "set_app_name"
  $stdout.flush

  fname = Jake.get_absolute File.join($androidpath, "Rhodes", "res", "values", "strings.xml")
  buf = File.new(fname,"r").read.gsub(/"app_name">.*<\/string>/,"\"app_name\">#{newname}</string>")
  File.open(fname,"w") { |f| f.write(buf) }

  lowname = newname.downcase.gsub(/[^A-Za-z_0-9]/, '')

  fname = Jake.get_absolute File.join($androidpath, "Rhodes", "AndroidManifest.xml")
  buf = File.new(fname,"r").read.gsub(/package=".*"/,"package=\"com.rhomobile.#{lowname}\"")
  File.open(fname,"w") { |f| f.write(buf) }

  fname = Jake.get_absolute File.join($androidpath, "Rhodes", "src", "com", "rhomobile", "rhodes", "AndroidR.java")
  buf = File.new(fname,"r").read.gsub(/^\s*import com\.rhomobile\..*\.R;\s*$/,"\nimport com.rhomobile.#{lowname}.R;\n")
  File.open(fname,"w") { |f| f.write(buf) }
end

def generate_rjava
  Rake::Task["build:android:rjava"].execute
end

namespace "config" do
  task :android => ["config:common"] do
    $config["platform"] = "android"

    $java = $config["env"]["paths"]["java"]
    $androidsdkpath = $config["env"]["paths"]["android"]
    $androidplatform = "android-1.5"
    $avdname = "rhoAndroid15"
    $androidpath = Jake.get_absolute $config["build"]["androidpath"]
    $bindir = File.join($app_path, "bin")
    $builddir = File.join($androidpath, "build")
    $srcdir = File.join($bindir, "RhoBundle")
    $targetdir = File.join($bindir, "target")
    $excludelib = ['**/singleton.rb','**/rational.rb','**/rhoframework.rb','**/date.rb']
    $tmpdir = File.join($bindir, "tmp")
    $resourcedir = File.join($tmpdir, "resource")
    $excludeapps = "public/js/iui/**,**/jquery*"
    $libs = File.join($androidpath, "Rhodes", "libs")
    $appname = $app_config["name"]
    $appname = "Rhodes" if $appname.nil?

    if RUBY_PLATFORM =~ /(win|w)32$/
      $emulator = "cmd /c " + File.join( $androidsdkpath, "tools", "emulator.exe" )
      $bat_ext = ".bat"
      $exe_ext = ".exe"
      $path_separator = ";"
    else
      #XXX make these absolute
      $emulator = File.join( $androidsdkpath, "tools", "emulator" )
      $bat_ext = ""
      $exe_ext = ""
      $path_separator = ":"
    end

    $dx = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "dx" + $bat_ext )
    $aapt = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "aapt" + $exe_ext )
    $apkbuilder = File.join( $androidsdkpath, "tools", "apkbuilder" + $bat_ext )
    $androidbin = File.join( $androidsdkpath, "tools", "android" + $bat_ext )
    $adb = File.join( $androidsdkpath, "tools", "adb" + $exe_ext )

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
 #   desc "Generate R.java file"
    task :rjava => "config:android" do

      manifest = File.join($androidpath, "Rhodes", "AndroidManifest.xml")
      resource = Jake.get_absolute File.join($androidpath, "Rhodes", "res")
      assets = Jake.get_absolute File.join($androidpath, "Rhodes", "assets")
      rjava = Jake.get_absolute File.join($androidpath, "Rhodes", "gen", "com", "rhomobile", "rhodes")
      androidjar = File.join($androidsdkpath, "platforms", $androidplatform, "android.jar")

      mkdir_p $tmpdir
      iconbakname = $tmpdir + "/icon.png.bak"
      iconappname = $app_path + "/icon/icon.png"
      cp resource + "/drawable/icon.png",iconbakname
      cp iconappname, resource + "/drawable" if File.exists?(iconappname)

      args = ["package","-f","-M",manifest,"-S", resource,"-A", assets,"-I",androidjar,"-J", rjava  ]
      puts Jake.run($aapt,args)

      exitstatus = $?

      mv iconbakname,resource + "/drawable/icon.png" if File.exists?(iconbakname)

      unless exitstatus == 0
        puts "Error in AAPT"
        exit 1
      end

    end
#    desc "Build RhoBundle for android"
    task :rhobundle => "config:android" do
      Rake::Task["build:bundle:xruby"].execute

      cp_r $srcdir + "/apps", Jake.get_absolute($androidpath) + "/Rhodes/assets"
      cp_r $bindir + "/RhoBundle.jar", $libs
    end

#    desc "Build RubyVM for android"
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
 #   desc "Build Rhodes for android"
    task :rhodes => [:rubyvm, :rhobundle] do
      javac = $config["env"]["paths"]["java"] + "/javac" + $exe_ext

      rm_rf $tmpdir + "/Rhodes"
      mkdir_p $tmpdir + "/Rhodes"

      set_app_name_android($appname)
      generate_rjava

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
        set_app_name_android("Rhodes")
        generate_rjava
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
        set_app_name_android("Rhodes")
        generate_rjava
        puts "Error compiling java code"
        exit 1
      end

      set_app_name_android("Rhodes")
      generate_rjava

      args = ["cf","../../Rhodes.jar", "#{$all_files_mask}"]
      puts Jake.run($config["env"]["paths"]["java"] + "/jar" + $exe_ext, args, "#{$tmpdir}/Rhodes/")
      unless $? == 0
        puts "Error running jar"
        exit 1
      end
    end

    #desc "build all"
    task :all => [:rubyvm, :rhobundle, :rhodes]
  end
end

namespace "package" do
  task :android => "build:android:all" do
    puts "Running dx utility"
    args = []
    args << "--dex"
    outfile = "#{$bindir}/classes.dex"
    args << "--output=#{outfile}"
    args << "#{$bindir}/Rhodes.jar"
    args << "#{$bindir}/RubyVM.jar"
    args << "#{$bindir}/RhoBundle.jar"
    puts Jake.run($dx,args)
    unless $? == 0
      puts "Error running DX utility"
      exit 1
    end

    manifest = Jake.get_absolute $androidpath + "/Rhodes/AndroidManifest.xml"
    resource = Jake.get_absolute $androidpath + "/Rhodes/res"
    assets = Jake.get_absolute $androidpath + "/Rhodes/assets"
    androidjar = "#{$androidsdkpath}/platforms/#{$androidplatform}/android.jar"
    resourcepkg =  $bindir + "/rhodes.ap_"

    puts "Packaging Assets and Jars"

    iconbakname = $tmpdir + "/icon.png.bak"
    iconappname = $app_path + "/icon/icon.png"
    cp resource + "/drawable/icon.png",iconbakname
    cp iconappname, resource + "/drawable" if File.exists?(iconappname)
    set_app_name_android($appname)

    puts `#{$aapt} package -f -M "#{manifest}" -S "#{resource}" -A "#{assets}" -I "#{androidjar}" -F "#{resourcepkg}"`
    returnval = $?

    set_app_name_android("Rhodes")
    mv iconbakname,resource + "/drawable/icon.png" if File.exists?(iconbakname)

    unless returnval == 0
      puts "Error running AAPT"
      exit 1
    end
    
  end
end

namespace "device" do
  namespace "android" do
    desc "Build debug self signed for device"
    task :debug => "package:android" do
      dexfile =  $bindir + "/classes.dex"
      apkfile =  $targetdir + "/" + $appname + "-debug.apk"
      resourcepkg =  $bindir + "/rhodes.ap_"

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -z "#{resourcepkg}" -f "#{dexfile}"`
      unless $? == 0
        puts "Error building APK file"
        exit 1
      end

    end
    desc "Build production signed for device"
    task :production => "package:android" do
      dexfile =  $bindir + "/classes.dex"
      apkfile =  $targetdir + "/" + $appname + ".apk"
      signedapkfile =  $targetdir + "/" + $appname + "_signed.apk"
      resourcepkg =  $bindir + "/rhodes.ap_"

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
    apkfile = Jake.get_absolute $targetdir + "/" + $appname + "-debug.apk"
    puts `#{$adb} start-server`
    sleep 5

    system("#{$androidbin} create avd --name #{$avdname} --target 2 --sdcard 32M --skin HVGA")

    Thread.new { system("#{$emulator} -avd #{$avdname}") }

    sleep 10

    puts "Waiting for emulator to get started"
    $stdout.flush
    puts `#{$adb} wait-for-device`
    sleep 10

    puts "Loading package into emulator"
    theoutput = `#{$adb} install -r "#{apkfile}"`
    count = 0
    while (not theoutput.to_s.match(/Success/))  and count < 15 do
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
  desc "Clean Android"
  task :android => "clean:android:all"
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
      rm_rf $libs
    end
#    desc "clean android"
    task :all => [:assets,:files]
  end
end

