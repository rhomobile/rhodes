#
require 'digest/sha2'

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

def cc_deps(filename)
  # TODO: implement dependencies
  []
end

def cc_compile(filename, objdir, additional = nil)
  filename.chomp!
  objname = File.join objdir, File.basename(filename).gsub(/\.[cC]([cC]|[xXpP][xXpP])?$/, ".o")

  return true if FileUtils.uptodate? objname, [filename] + cc_deps(filename)

  mkdir_p objdir unless File.directory? objdir

  if filename =~ /\.[cC]$/
    ccbin = $gccbin
  elsif filename =~ /.[cC]([cC]|[xXpP][xXpP])$/
    ccbin = $gppbin
  end

  args = []
  args << "--sysroot"
  args << $ndksysroot
  args << "-fPIC"
  args << "-mandroid"
  args << "-Wall"
  args << "-DANDROID"
  args << "-DOS_ANDROID"
  args += additional if additional.is_a? Array and not additional.empty?
  if $build_release
    args << "-O2"
    args << "-DNDEBUG"
  else
    args << "-g"
    args << "-O0"
    args << "-D_DEBUG"
  end
  args << "-c"
  args << filename
  args << "-o"
  args << objname
  puts Jake.run(ccbin, args)
  return $? == 0
end

def cc_ar(libname, objects)
  return true if FileUtils.uptodate? libname, objects
  args = []
  args << "crs"
  args << libname
  args += objects
  puts Jake.run($arbin, args)
  return $? == 0
end

def cc_link(outname, objects, additional = nil, deps = nil)
  dependencies = objects
  dependencies += deps unless deps.nil?
  return true if FileUtils.uptodate? outname, dependencies
  args = []
  args << "-nostdlib"
  args << "-Wl,-shared,-Bsymbolic"
  args << "-Wl,--whole-archive"
  args << "-Wl,--no-whole-archive"
  args << "-Wl,--no-undefined"
  args << "#{$ndksysroot}/usr/lib/libc.so"
  args << "#{$ndksysroot}/usr/lib/libstdc++.so"
  args << "#{$ndksysroot}/usr/lib/libm.so"
  args << "-L#{$ndksysroot}/usr/lib"
  args << "-Wl,-rpath-link=#{$ndksysroot}/usr/lib"
  args << "#{$ndktools}/lib/gcc/arm-eabi/#{$ndkgccver}/interwork/libgcc.a"
  args << "-shared"
  args << "-fPIC"
  args << "-Wl,-soname,#{outname}"
  args << "-o"
  args << outname
  args += objects
  args += additional if additional.is_a? Array and not additional.empty?
  puts Jake.run($gccbin, args)
  return false unless $? == 0

  puts Jake.run($stripbin, [outname])
  return $? == 0
end

def cc_clean(name)
  [$objdir[name], $libname[name]].each do |x|
    rm_rf x if File.exists? x
  end
end

namespace "config" do
  task :android => ["config:common"] do
    $config["platform"] = "android"

    # Here is place were android platform should be specified.
    # For complete list of android API levels and its mapping to
    # market names (such as "Android-1.5" etc) see output of
    # command "android list targets"
    ANDROID_API_LEVEL = 3

    # Here is switch between release/debug configuration used for
    # building native libraries
    $build_release = true

    $androidsdkpath = $config["env"]["paths"]["android"]
    puts "Missing 'android' section in rhobuild.yml" if $androidsdkpath.nil?
    $androidndkpath = $config["env"]["paths"]["android-ndk"]
    puts "Missing 'android-ndk' section in rhobuild.yml" if $androidndkpath.nil?

    $java = $config["env"]["paths"]["java"]
    $androidpath = Jake.get_absolute $config["build"]["androidpath"]
    $bindir = File.join($app_path, "bin")
    $builddir = File.join($androidpath, "build")
    $shareddir = File.join($androidpath, "..", "shared")
    $srcdir = File.join($bindir, "RhoBundle")
    $targetdir = File.join($bindir, "target")
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir = File.join($bindir, "tmp")
    $resourcedir = File.join($tmpdir, "resource")
    $excludeapps = "public/js/iui/**,**/jquery*"
    $libs = File.join($androidpath, "Rhodes", "libs")
    $appname = $app_config["name"]
    $appname = "Rhodes" if $appname.nil?

    $androidapi = {2 => "1.1", 3 => "1.5", 4 => "1.6", 5 => "2.0"}
    $androidtargets = {2 => 1, 3 => 2, 4 => 3, 5 => 4}

    $androidplatform = "android-" + $androidapi[ANDROID_API_LEVEL]
    $avdname = "rhoAndroid" + $androidapi[ANDROID_API_LEVEL].gsub(/[^0-9]/, "")

    if RUBY_PLATFORM =~ /(win|w)32$/
      $emulator = "cmd /c " + File.join( $androidsdkpath, "tools", "emulator.exe" )
      $bat_ext = ".bat"
      $exe_ext = ".exe"
      $path_separator = ";"
      $ndkhost = "windows"
      $rubypath = "res/build-tools/RhoRuby.exe"
    else
      #XXX make these absolute
      $emulator = File.join( $androidsdkpath, "tools", "emulator" )
      $bat_ext = ""
      $exe_ext = ""
      $path_separator = ":"
      $ndkhost = `uname -s`.downcase!.chomp! + "-x86"
      # TODO: add ruby executable for Linux
      if RUBY_PLATFORM =~ /darwin/
        $rubypath = "res/build-tools/RubyMac"
      else
        $rubypath = "res/build-tools/rubylinux"
      end
    end

    $dx = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "dx" + $bat_ext )
    $aapt = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "aapt" + $exe_ext )
    $apkbuilder = File.join( $androidsdkpath, "tools", "apkbuilder" + $bat_ext )
    $androidbin = File.join( $androidsdkpath, "tools", "android" + $bat_ext )
    $adb = File.join( $androidsdkpath, "tools", "adb" + $exe_ext )
    $zipalign = File.join( $androidsdkpath, "tools", "zipalign" + $exe_ext )

    $keytool = File.join( $java, "keytool" + $exe_ext )
    $jarsigner = File.join( $java, "jarsigner" + $exe_ext )
    $jarbin = File.join( $java, "jar" + $exe_ext )
    $keystoredir = ENV['HOME'] + "/.rhomobile"
    $keystore = $keystoredir + "/keystore"
    $storepass = "81719ef3a881469d96debda3112854eb"
    $keypass = $storepass

    $ndkgccver = "4.2.1"
    $ndktools = $androidndkpath + "/build/prebuilt/#{$ndkhost}/arm-eabi-#{$ndkgccver}"
    $ndksysroot = $androidndkpath + "/build/platforms/android-#{ANDROID_API_LEVEL}/arch-arm"

    $gccbin = $ndktools + "/bin/arm-eabi-gcc" + $exe_ext
    $gppbin = $ndktools + "/bin/arm-eabi-g++" + $exe_ext
    $arbin = $ndktools + "/bin/arm-eabi-ar" + $exe_ext
    $stripbin = $ndktools + "/bin/arm-eabi-strip" + $exe_ext

    $stlport_includes = File.join $shareddir, "stlport", "stlport"

    $native_libs = ["sqlite", "curl", "stlport", "shttpd", "ruby", "json", "rhocommon", "rhodb", "rholog", "rhosync", "rhomain"]

    $objdir = {}
    $libname = {}
    $native_libs.each do |x|
      $objdir[x] = $bindir + "/libs/lib" + x
      $libname[x] = $bindir + "/libs/lib" + x + ".a"
    end

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
      nativelibs = Jake.get_absolute File.join($androidpath, "Rhodes", "libs")
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
    task :rhobundle => :librhodes do
      Rake::Task["build:bundle:noxruby"].execute

      assets = File.join(Jake.get_absolute($androidpath), "Rhodes", "assets")
      hash = ""
      ["apps", "db", "lib"].each do |d|
        cp_r File.join($srcdir, d), assets
        # Calculate hash of directories
        Dir.glob(File.join(assets, d, "**/*")) do |f|
          hash += Digest::SHA2.file(f).hexdigest if File.file? f
        end
      end
      File.open(File.join(assets, "hash"), "w") { |f| f.write(Digest::SHA2.hexdigest(hash)) }

	  File.open(File.join(assets, "name"), "w") { |f| f.write($appname) }

    end

    task :libsqlite => "config:android" do
      srcdir = File.join($shareddir, "sqlite")
      objdir = $objdir["sqlite"]
      libname = $libname["sqlite"]

      cc_compile File.join(srcdir, "sqlite3.c"), objdir, ["-I#{srcdir}"] or exit 1
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libcurl => "config:android" do
      # Steps to get curl_config.h from fresh libcurl sources:
      #export PATH=<ndkroot>/build/prebuilt/linux-x86/arm-eabi-4.2.1/bin:$PATH
      #export CC=arm-eabi-gcc
      #export CPP=arm-eabi-cpp
      #export CFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
      #export CPPFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
      #./configure --without-ssl --without-ca-bundle --without-ca-path --without-libssh2 --without-libidn --disable-ldap --disable-ldaps --host=arm-eabi

      srcdir = File.join $shareddir, "curl", "lib"
      objdir = $objdir["curl"]
      libname = $libname["curl"]
      args = []
      args << "-DHAVE_CONFIG_H"
      args << "-I#{srcdir}/../include"
      args << "-I#{srcdir}"

      File.read(File.join($builddir, "libcurl_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libruby => "config:android" do
      srcdir = File.join $shareddir, "ruby"
      objdir = $objdir["ruby"]
      libname = $libname["ruby"]
      args = []
      args << "-I#{srcdir}/include"
      args << "-I#{srcdir}/linux"
      args << "-I#{srcdir}/generated"
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."
      args << "-I#{srcdir}/../sqlite"

      File.read(File.join($builddir, "libruby_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libjson => "config:android" do
      srcdir = File.join $shareddir, "json"
      objdir = $objdir["json"]
      libname = $libname["json"]
      args = []
      args << "-D__NEW__"
      args << "-I#{$stlport_includes}"
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."

      objects = []
      File.read(File.join($builddir, "libjson_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libshttpd => "config:android" do
      srcdir = File.join $shareddir, "shttpd", "src"
      objdir = $objdir["shttpd"]
      libname = $libname["shttpd"]
      args = []
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/../.."

      File.read(File.join($builddir, "libshttpd_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libstlport => "config:android" do
      objdir = $objdir["stlport"]
      libname = $libname["stlport"]
      args = []
      args << "-C"
      args << $shareddir + "/stlport/build/lib"
      args << "-f"
      args << "android.mak"
      args << "NDK_DIR=#{$androidndkpath}"
      args << "NDK_HOST=#{$ndkhost}"
      args << "PRE_OUTPUT_DIR=#{objdir}"
      args << "release-static"
      puts Jake.run("make", args)
      unless $? == 0
        exit 1
      end
      source = File.join(objdir, "so", "libstlport.a")
      cp_r source, libname unless FileUtils.uptodate? libname, source
    end

    task :librholog => "config:android" do
      srcdir = File.join $shareddir, "logging"
      objdir = $objdir["rholog"]
      libname = $libname["rholog"]
      args = []
      args << "-D__NEW__"
      args << "-I#{$stlport_includes}"
      args << "-I#{srcdir}/.."

      File.read(File.join($builddir, "librholog_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :librhomain => "config:android" do
      srcdir = $shareddir
      objdir = $objdir["rhomain"]
      libname = $libname["rhomain"]
      args = []
      args << "-D__NEW__"
      args << "-I#{$stlport_includes}"
      args << "-I#{srcdir}"

      File.read(File.join($builddir, "librhomain_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :librhocommon => "config:android" do
      objdir = $objdir["rhocommon"]
      libname = $libname["rhocommon"]
      args = []
      args << "-D__NEW__"
      args << "-I#{$stlport_includes}"
      args << "-I#{$shareddir}"
      args << "-I#{$shareddir}/curl/include"

      objects = []
      File.read(File.join($builddir, "librhocommon_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :librhodb => "config:android" do
      srcdir = File.join $shareddir, "db"
      objdir = $objdir["rhodb"]
      libname = $libname["rhodb"]
      args = []
      args << "-D__NEW__"
      args << "-I#{$stlport_includes}"
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."
      args << "-I#{srcdir}/../sqlite"

      File.read(File.join($builddir, "librhodb_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :librhosync => "config:android" do
      srcdir = File.join $shareddir, "sync"
      objdir = $objdir["rhosync"]
      libname = $libname["rhosync"]
      args = []
      args << "-D__NEW__"
      args << "-I#{$stlport_includes}"
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."
      args << "-I#{srcdir}/../sqlite"

      File.read(File.join($builddir, "librhosync_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libs => [:libsqlite, :libcurl, :libruby, :libjson, :libshttpd, :libstlport, :librhodb, :librhocommon, :librhomain, :librhosync, :librholog]

    task :librhodes => :libs do
      srcdir = File.join $androidpath, "Rhodes", "jni", "src"
      objdir = File.join $bindir, "libs", "librhodes"
      libname = File.join $bindir, "libs", "librhodes.so"
      args = []
      args << "-I#{$stlport_includes}"
      args << "-I#{srcdir}/../include"
      args << "-I#{$shareddir}"
      args << "-I#{$shareddir}/sqlite"
      args << "-I#{$shareddir}/curl/include"
      args << "-I#{$shareddir}/ruby/include"
      args << "-I#{$shareddir}/ruby/linux"
      args << "-D__NEW__"
      args << "-D__SGI_STL_INTERNAL_PAIR_H"

      File.read(File.join($builddir, "librhodes_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end

      deps = []
      $libname.each do |k,v|
        deps << v
      end

      args = []
      args << "-L#{$bindir}/libs"
      args << "-lrhomain"
      args << "-lshttpd"
      args << "-lruby"
      args << "-lrhosync"
      args << "-lrhodb"
      args << "-lrholog"
      args << "-lrhocommon"
      args << "-ljson"
      args << "-lstlport"
      args << "-lcurl"
      args << "-lsqlite"
      args << "-ldl"
      args << "-lz"
      cc_link libname, Dir.glob(objdir + "/**/*.o"), args, deps or exit 1

      destdir = File.join($androidpath, "Rhodes", "libs", "armeabi")
      mkdir_p destdir unless File.exists? destdir
      cp_r libname, destdir
    end

 #   desc "Build Rhodes for android"
    task :rhodes => :rhobundle do
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
      #args << "#{$androidsdkpath}/platforms/#{$androidplatform}/android.jar" + $path_separator + "#{$tmpdir}/Rhodes" + $path_separator + "#{$libs}/RhoBundle.jar"
      args << "#{$androidsdkpath}/platforms/#{$androidplatform}/android.jar" + $path_separator + "#{$tmpdir}/Rhodes"
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
      puts Jake.run($jarbin, args, "#{$tmpdir}/Rhodes/")
      unless $? == 0
        puts "Error running jar"
        exit 1
      end
    end

    #desc "build all"
    task :all => [:rhobundle, :rhodes]
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
    #args << "#{$bindir}/RhoBundle.jar"
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

    args = ["package", "-f", "-M", manifest, "-S", resource, "-A", assets, "-I", androidjar, "-F", resourcepkg]
    puts Jake.run($aapt, args)
    returnval = $?

    set_app_name_android("Rhodes")
    mv iconbakname,resource + "/drawable/icon.png" if File.exists?(iconbakname)

    unless returnval == 0
      puts "Error running AAPT"
      exit 1
    end

    rm_rf $bindir + "/lib"
    mkdir_p $bindir + "/lib/armeabi"
    cp_r $bindir + "/libs/librhodes.so", $bindir + "/lib/armeabi"
    args = ["add", resourcepkg, "lib/armeabi/librhodes.so"]
    puts Jake.run($aapt, args, $bindir)
    unless $? == 0
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

    task :install => :debug do
      apkfile = $targetdir + "/" + $appname + "-debug.apk"
      puts "Install APK file"
      puts `#{$adb} install -r "#{apkfile}"`
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

      # puts "Align APK file"
      # args = []
      # args << "-c"
      # args << "-v"
      # args << "4"
      # args << '"' + apkfile + '"'
      # puts Jake.run($zipalign, args)
      # unless $? == 0
      #   puts "Error running zipalign"
      #   exit 1
      # end
    end
  end
end

namespace "run" do
  desc "build and launch emulator"
  task :android => "device:android:debug" do
    apkfile = Jake.get_absolute $targetdir + "/" + $appname + "-debug.apk"
    puts `#{$adb} start-server`
    sleep 5

    system("#{$androidbin} create avd --name #{$avdname} --target #{$androidtargets[ANDROID_API_LEVEL]} --sdcard 32M --skin HVGA")

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
      Dir.glob($androidpath + "/Rhodes/assets/**/*") do |f|
        rm f, :force => true unless f =~ /\/loading\.html$/
      end
    end
    task :files => "config:android" do
      rm_rf $targetdir
      rm_rf $bindir
      rm_rf $srcdir
      rm_rf $libs
    end
    task :libs => "config:android" do
      $native_libs.each do |l|
        cc_clean l
      end
    end
    task :librhodes => "config:android" do
      rm_rf $bindir + "/libs/librhodes"
      rm_rf $bindir + "/libs/librhodes.so"
    end
#    desc "clean android"
    task :all => [:assets,:librhodes,:libs,:files]
  end
end

