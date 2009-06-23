namespace "config" do
  task :android => ["config:common"] do
    $java = $config["env"]["paths"]["java"]
    $androidpath = $config["build"]["androidpath"]
    $bindir = $androidpath + "/bin"
    $builddir = $androidpath + "/build"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = $androidpath + "/target"
    $excludelib = ['**/singleton.rb','**/rational.rb','**/rhoframework.rb','**/date.rb']
    $tmpdir =  $bindir +"/tmp"
    $excludeapps = "public/js/iui/**,**/jquery*"
    $libs = $androidpath + "/Rhodes/libs"

    if RUBY_PLATFORM =~ /(win|w)32$/
      $dx = File.join( $config["env"]["paths"]["android"], "platforms", "android-1.1", "tools", "dx.bat" )
      $aapt = File.join( $config["env"]["paths"]["android"], "platforms", "android-1.1", "tools", "aapt.exe" )
      $apkbuilder = File.join( $config["env"]["paths"]["android"], "tools", "apkbuilder.bat" )
      $emulator = "cmd /c " + File.join( $config["env"]["paths"]["android"], "tools", "emulator.exe" )
      $adb = File.join( $config["env"]["paths"]["android"], "tools", "adb.exe" )

    else
      #XXX make these absolute
      $dx = "dx"
      $aapt = "aapt"
      $apkbuilder = "apkbuilder"
      $emulator = "emulator"
      $adb = "adb"
    end

    mkdir_p $bindir if not File.exists? $bindir
    mkdir_p $targetdir if not File.exists? $targetdir
    mkdir_p $srcdir if not File.exists? $srcdir
    mkdir_p $libs if not File.exists? $libs

  end
end


namespace "build" do
  namespace "android" do
    desc "Build RhoBundle for android"
    task :rhobundle => "config:android" do
      Rake::Task["build:bundle:xruby"].execute

      cp_r $srcdir + "/apps", $androidpath + "/Rhodes/assets"
      cp_r $bindir + "/RhoBundle.jar", $libs
      
    end

    desc "Build RubyVM for android"
    task :rubyvm => "config:android" do
      javac = $config["env"]["paths"]["java"] + "/javac.exe"
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
      args << "@#{$builddir}/RubyVM_build.files"
      puts Jake.run(javac,args)

      args = ["cf","../../RubyVM.jar", "#{$all_files_mask}"]
      puts Jake.run("jar",args,"#{$tmpdir}/RubyVM/")

      cp_r $bindir + "/RubyVM.jar", $libs

    end
    desc "Build Rhodes for android"
    task :rhodes => [:rubyvm, :rhobundle] do
      javac = $config["env"]["paths"]["java"] + "/javac.exe"

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
      args << "@#{$builddir}/RhodesGEN_build.files"
      puts Jake.run(javac,args)

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
      args << "#{$config["env"]["paths"]["android"]}/platforms/android-1.1/android.jar;#{$tmpdir}/Rhodes;#{$libs}/RubyVM.jar;#{$libs}/RhoBundle.jar"
      args << "@#{$builddir}/RhodesSRC_build.files"
      puts Jake.run(javac,args)




      args = ["cf","../../Rhodes.jar", "#{$all_files_mask}"]
      puts Jake.run("jar",args,"#{$tmpdir}/Rhodes/")
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

    manifest = Jake.get_absolute $androidpath + "/Rhodes/AndroidManifest.xml"
    resource = Jake.get_absolute $androidpath + "/Rhodes/res"
    assets = Jake.get_absolute $androidpath + "/Rhodes/assets"
    androidjar = $config["env"]["paths"]["android"] + "/platforms/android-1.1/android.jar"
    resourcepkg = Jake.get_absolute $bindir + "/rhodes.ap_"

    puts "Packaging Assets and Jars"
    puts `#{$aapt} package -f -M "#{manifest}" -S "#{resource}" -A "#{assets}" -I "#{androidjar}" -F "#{resourcepkg}"`
    
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

    end
    desc "build unsigned for production"
    task :production => "package:android" do
      dexfile = Jake.get_absolute $bindir + "/classes.dex"
      apkfile = Jake.get_absolute $targetdir + "/Rhodes.apk"
      resourcepkg = Jake.get_absolute $bindir + "/rhodes.ap_"

      puts "Building APK file"
      puts `#{$apkbuilder} "#{apkfile}" -u -z "#{resourcepkg}" -f "#{dexfile}"`

    end
  end
end

namespace "run" do
  desc "build and launch emulator"
  task :android => "device:android:debug" do
      apkfile = Jake.get_absolute $targetdir + "/Rhodes-debug.apk"
      puts `#{$adb} start-server`
      sleep 5

      system("#{$config["env"]["paths"]["android"]}/tools/android.bat create avd --name rhoAndroid11 --target 1 --sdcard 32M --skin HVGA")

      Thread.new { system("#{$emulator} -avd rhoAndroid11") }

      sleep 10

      puts "Waiting for emulator to get started"
      $stdout.flush
      puts `#{$adb} wait-for-device`
      sleep 60

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