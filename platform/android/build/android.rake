#
require File.dirname(__FILE__) + '/androidcommon.rb'

USE_STLPORT = true
USE_OPENSSL = false

ANDROID_API_LEVEL_TO_MARKET_VERSION = {}
ANDROID_MARKET_VERSION_TO_API_LEVEL = {}
{2 => "1.1", 3 => "1.5", 4 => "1.6", 5 => "2.0", 6 => "2.0.1", 7 => "2.1"}.each do |k,v|
  ANDROID_API_LEVEL_TO_MARKET_VERSION[k] = v
  ANDROID_MARKET_VERSION_TO_API_LEVEL[v] = k
end

def get_market_version(apilevel)
  ANDROID_API_LEVEL_TO_MARKET_VERSION[apilevel]
end

def get_api_level(version)
  ANDROID_MARKET_VERSION_TO_API_LEVEL[version]
end

# Here is place were android platform should be specified.
# For complete list of android API levels and its mapping to
# market names (such as "Android-1.5" etc) see output of
# command "android list targets"
ANDROID_API_LEVEL = 3

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

def get_boolean(arg)
  arg == 'true' or arg == 'yes' or arg == 'enabled' or arg == 'enable' or arg == '1'
end

namespace "config" do
  task :android => ["config:common"] do
    $config["platform"] = "android"

    $gapikey = $app_config["android"]["apikey"] unless $app_config["android"].nil?
    $gapikey = $config["android"]["apikey"] if $gapikey.nil? and not $config["android"].nil?
    $gapikey = '' unless $gapikey.is_a? String
    $gapikey = nil if $gapikey.empty?

    $use_geomapping = $app_config["android"]["mapping"] unless $app_config["android"].nil?
    $use_geomapping = $config["android"]["mapping"] if $use_geomapping.nil? and not $config["android"].nil?
    $use_geomapping = '' unless $use_geomapping.is_a? String
    $use_geomapping = get_boolean($use_geomapping)

    $emuversion = $app_config["android"]["version"] unless $app_config["android"].nil?
    $emuversion = $config["android"]["version"] if $emuversion.nil? and !$config["android"].nil?
    $emuversion = get_market_version(ANDROID_API_LEVEL) if $emuversion.nil?
    $emuversion = $emuversion.to_s unless $emuversion.nil?

    # Here is switch between release/debug configuration used for
    # building native libraries
    if $app_config["debug"].nil?
      $build_release = true
    else
      $build_release = !$app_config["debug"].to_i
    end

    $androidsdkpath = $config["env"]["paths"]["android"]
    unless File.exists? $androidsdkpath
      puts "Missing or invalid 'android' section in rhobuild.yml"
      exit 1
    end

    $androidndkpath = $config["env"]["paths"]["android-ndk"]
    unless File.exists? $androidndkpath
      puts "Missing or invalid 'android-ndk' section in rhobuild.yml"
      exit 1
    end

    $java = $config["env"]["paths"]["java"]
    $androidpath = Jake.get_absolute $config["build"]["androidpath"]
    $bindir = File.join($app_path, "bin")
    $rhobindir = File.join($androidpath, "bin")
    $builddir = File.join($androidpath, "build")
    $shareddir = File.join($androidpath, "..", "shared")
    $srcdir = File.join($bindir, "RhoBundle")
    $targetdir = File.join($bindir, "target")
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir = File.join($bindir, "tmp")
    $resourcedir = File.join($tmpdir, "resource")
    $libs = File.join($androidpath, "Rhodes", "libs")
    $appname = $app_config["name"]
    $appname = "Rhodes" if $appname.nil?

    if RUBY_PLATFORM =~ /(win|w)32$/
      $emulator = "cmd /c " + File.join( $androidsdkpath, "tools", "emulator.exe" )
      $bat_ext = ".bat"
      $exe_ext = ".exe"
      $path_separator = ";"
      $rubypath = "res/build-tools/RhoRuby.exe"

      # Add PATH to cygwin1.dll
      if $path_cygwin_modified.nil?
        ENV['PATH'] = Jake.get_absolute("res/build-tools") + ";" + ENV['PATH']
        path_cygwin_modified = true
      end
    else
      #XXX make these absolute
      $emulator = File.join( $androidsdkpath, "tools", "emulator" )
      $bat_ext = ""
      $exe_ext = ""
      $path_separator = ":"
      # TODO: add ruby executable for Linux
      if RUBY_PLATFORM =~ /darwin/
        $rubypath = "res/build-tools/RubyMac"
      else
        $rubypath = "res/build-tools/rubylinux"
      end
    end

    $androidplatform = "android-" + get_market_version(ANDROID_API_LEVEL)
    
    $dx = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "dx" + $bat_ext )
    $aapt = File.join( $androidsdkpath, "platforms", $androidplatform, "tools", "aapt" + $exe_ext )
    $apkbuilder = File.join( $androidsdkpath, "tools", "apkbuilder" + $bat_ext )
    $androidbin = File.join( $androidsdkpath, "tools", "android" + $bat_ext )
    $adb = File.join( $androidsdkpath, "tools", "adb" + $exe_ext )
    $zipalign = File.join( $androidsdkpath, "tools", "zipalign" + $exe_ext )
    $androidjar = File.join($androidsdkpath, "platforms", $androidplatform, "android.jar")

    $keytool = File.join( $java, "keytool" + $exe_ext )
    $jarsigner = File.join( $java, "jarsigner" + $exe_ext )
    $jarbin = File.join( $java, "jar" + $exe_ext )
    $keystoredir = File.expand_path('~') + "/.rhomobile"
    $keystore = $keystoredir + "/keystore"
    $storepass = "81719ef3a881469d96debda3112854eb"
    $keypass = $storepass

    # Detect android targets
    if $androidtargets.nil?
      $androidtargets = {}
      id = nil

      `"#{$androidbin}" list targets`.split(/\n/).each do |line|
        line.chomp!

        if line =~ /^id:\s+([0-9]+)/
          id = $1
        end

        if $use_geomapping
          if line =~ /:Google APIs:([0-9]+)/
            apilevel = $1
            $androidtargets[apilevel.to_i] = id.to_i
          end
        else
          if line =~ /^\s+API\s+level:\s+([0-9]+)$/ and not id.nil?
            apilevel = $1
            $androidtargets[apilevel.to_i] = id.to_i
          end
        end
      end
    end

    $avdname = "rhoAndroid" + $emuversion.gsub(/[^0-9]/, "")
    $avdname += "ext" if $use_geomapping
    $avdtarget = $androidtargets[get_api_level($emuversion)]

    # Detect Google API add-on path
    if $use_geomapping
      Dir.glob(File.join($androidsdkpath, 'add-ons', '*')).each do |dir|
        break unless $gapijar.nil?

        props = File.join(dir, 'manifest.ini')
        next unless File.file? props

        apilevel = -1
        File.open(props, 'r') do |f|
          while line = f.gets
            next unless line =~ /^api=([0-9]+)$/
            apilevel = $1.to_i
            break
          end
        end

        $gapijar = File.join(dir, 'libs', 'maps.jar') if apilevel == ANDROID_API_LEVEL
      end
    end

    setup_ndk($androidndkpath,ANDROID_API_LEVEL)
    
    $stlport_includes = File.join $shareddir, "stlport", "stlport"

    $native_libs = ["sqlite", "curl", "stlport", "ruby", "json", "rhocommon", "rhodb", "rholog", "rhosync", "rhomain"]
    $native_libs << "openssl" if USE_OPENSSL

    if $build_release
      $confdir = "release"
    else
      $confdir = "debug"
    end
    $objdir = {}
    $libname = {}
    $native_libs.each do |x|
      $objdir[x] = File.join($rhobindir, $confdir, "lib" + x)
      $libname[x] = File.join($rhobindir, $confdir, "lib" + x + ".a")
    end

    $extensionsdir = $bindir + "/libs/" + $confdir + "/extensions"

    if $app_config["android"] and $app_config["android"]["extensions"]
      $app_config["extensions"] += $app_config["android"]["extensions"] if $app_config["extensions"]
      $app_config["android"]["extensions"] = nil
    end

    mkdir_p $bindir if not File.exists? $bindir
    mkdir_p $rhobindir if not File.exists? $rhobindir
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
      resource = Jake.get_absolute(File.join($androidpath, "Rhodes", "res"))
      assets = Jake.get_absolute(File.join($androidpath, "Rhodes", "assets"))
      nativelibs = Jake.get_absolute(File.join($androidpath, "Rhodes", "libs"))
      rjava = Jake.get_absolute(File.join($androidpath, "Rhodes", "gen", "com", "rhomobile", "rhodes"))

      mkdir_p $tmpdir
      iconbakname = $tmpdir + "/icon.png.bak"
      iconappname = $app_path + "/icon/icon.png"
      cp resource + "/drawable/icon.png",iconbakname
      cp iconappname, resource + "/drawable" if File.exists?(iconappname)

      args = ["package", "-f", "-M", '"'+manifest+'"', "-S", '"'+resource+'"', "-A", '"'+assets+'"', "-I", '"'+$androidjar+'"', "-J", '"'+rjava+'"']
      puts Jake.run('"' + $aapt + '"',args)

      exitstatus = $?

      mv iconbakname,resource + "/drawable/icon.png" if File.exists?(iconbakname)

      unless exitstatus == 0
        puts "Error in AAPT"
        exit 1
      end

    end
#    desc "Build RhoBundle for android"
    task :rhobundle => ["config:android","build:bundle:noxruby",:extensions,:librhodes] do
#      Rake::Task["build:bundle:noxruby"].execute

      assets = File.join(Jake.get_absolute($androidpath), "Rhodes", "assets")
      rm_rf assets
      mkdir_p assets
      hash = nil
      ["apps", "db", "lib"].each do |d|
        cp_r File.join($srcdir, d), assets
        # Calculate hash of directories
        hash = get_dir_hash(File.join(assets, d), hash)
      end
      File.open(File.join(assets, "hash"), "w") { |f| f.write(hash.hexdigest) }

    File.open(File.join(assets, "name"), "w") { |f| f.write($appname) }

    end

    task :extensions => "config:android" do

      ENV["ANDROID_NDK"] = $androidndkpath
      ENV["ANDROID_API_LEVEL"] = ANDROID_API_LEVEL.to_s
      ENV["TARGET_TEMP_DIR"] = $extensionsdir
      ENV["BUILD_DIR"] ||= $startdir + "/platform/android/build"

      mkdir_p $bindir + "/libs/" + $confdir + "/extensions" unless File.exist? $bindir + "/libs/" + $confdir + "/extensions"

      $app_config["extensions"].each do |ext|
        ENV['TEMP_FILES_DIR'] = ENV["TARGET_TEMP_DIR"] + "/#{ext}"

        rhoextpath = "lib/extensions/" + ext + "/ext"
        appextpath = $app_path + "/extensions/" + ext + "/ext"
        extpath = ""

        puts appextpath
        puts rhoextpath

        if File.exists? appextpath
          extpath = appextpath
        elsif File.exists? rhoextpath
          extpath = rhoextpath
        end
        
     

        puts Jake.run('./build', [], extpath) if File.executable? File.join(extpath, 'build')
        exit 1 unless $? == 0
        
      end

    end

    task :libsqlite => "config:android" do
      srcdir = File.join($shareddir, "sqlite")
      objdir = $objdir["sqlite"]
      libname = $libname["sqlite"]

      cc_compile File.join(srcdir, "sqlite3.c"), objdir, ["-I#{srcdir}"] or exit 1
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    unless USE_OPENSSL
      task :libopenssl do
      end
    else
      task :libopenssl => "config:android" do
        srcdir = File.join($shareddir, "openssl")
        objdir = $objdir["openssl"]
        hashloc = File.join(objdir, 'hash')

        unless FileUtils.uptodate? hashloc, Dir.glob(srcdir + '/**/*')
          mkdir_p objdir + "/openssl"
          cp_r srcdir, objdir, :remove_destination => true
        end

        objdir += '/openssl'

        hash = get_dir_hash(objdir).hexdigest

        oldhash = ''
        oldhash = File.new(hashloc, 'r').read if File.file? hashloc
        if oldhash != hash
          args = []
          args << "Configure"
          args << "no-threads"
          args << "no-shared"
          args << "zlib-dynamic"
          args << "no-asm"
          # Ciphers
          args << "no-camellia"
          args << "no-cast"
          args << "no-cms"
          args << "no-dso"
          args << "no-ec"
          args << "no-ecdh"
          args << "no-ecdsa"
          args << "no-engine"
          args << "no-err"
          args << "no-fips"
          args << "no-idea"
          args << "no-jpake"
          args << "no-krb5"
          args << "no-md2"
          args << "no-md4"
          args << "no-mdc2"
          args << "no-montasm"
          args << "no-pqueue"
          args << "no-rc2"
          args << "no-rc4"
          args << "no-rc5"
          args << "no-seed"
          args << "no-store"
          args << "no-txt_db"
          # Others
          args << "no-ui"
          args << "no-engines"
          args << "no-apps"
          args << "no-tools"
          args << "no-test"
          args << "linux-elf"
          cc_run('perl', args, objdir) or exit 1

          if RUBY_PLATFORM =~ /(win|w)32$/
            afs = []
            Dir.glob(File.join(objdir, 'crypto', '*.h')).each do |f|
              next if f =~ /\/buildinf\.h$/
              afs << f
            end
            Dir.glob(File.join(objdir, 'ssl', '*.h')).each do |f|
              afs << f
            end
            afs << File.join(objdir, 'fips', 'fips.h')
            afs << File.join(objdir, 'e_os2.h')

            incpath = File.join(objdir, 'include', 'openssl')
            afs.each do |f|
              cp_r f, incpath, :remove_destination => true unless FileUtils.uptodate? File.join(incpath, File.basename(f)), f
            end
          end

          #cc_run('make', 'clean', objdir)

          args = []
          args << "CC=#{$gccbin}"
          args << "CFLAG=\"#{cc_def_args.join(' ')}\""
          args << "AR=\"#{$arbin} cr\""
          args << "RANLIB=\"#{$ranlib}\""
          args << "SHARED_LDFLAGS=\"-dynamiclib\""
          args << "build_crypto build_ssl"
          cc_run('make', args, objdir) or exit 1

          rm_f File.join(objdir, '..', ',,', 'libssl.a')
          rm_f File.join(objdir, '..', '..', 'libcrypto.a')
          cp File.join(objdir, 'libssl.a'), File.join(objdir, '..', '..')
          cp File.join(objdir, 'libcrypto.a'), File.join(objdir, '..', '..')

          hash = get_dir_hash(objdir).hexdigest
          File.open(hashloc, 'w') { |f| f.write(hash) }
        end

      end
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

      #use_openssl = false
      #rhoconf_h = File.join(srcdir, 'rhoconf.h')
      #if File.file? rhoconf_h
      #  File.open(rhoconf_h, 'r') do |f|
      #    while line = f.gets
      #      use_openssl = true if line =~ /USE_OPENSSL\s+1/
      #      break if use_openssl
      #    end
      #  end
      #end
      #if USE_OPENSSL != use_openssl
      #  ssldef = []
      #  ssldef << "#define USE_SSLEAY 1" if USE_OPENSSL
      #  ssldef << "#define USE_OPENSSL 1" if USE_OPENSSL
      #  File.open(rhoconf_h, 'w') { |f| f.write(ssldef.join("\n") + "\n") }
      #end

      args = []
      args << "-DHAVE_CONFIG_H"
      args << "-I#{srcdir}/../include"
      args << "-I#{srcdir}"
      args << "-I#{objdir}/../libopenssl/openssl/include" if USE_OPENSSL

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

      cc_ar libname, Dir.glob(objdir + "/**/*.o") + Dir.glob($extensionsdir + "/*.a") or exit 1
    end

    task :libjson => "config:android" do
      srcdir = File.join $shareddir, "json"
      objdir = $objdir["json"]
      libname = $libname["json"]
      args = []
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

      objects = []
      File.read(File.join($builddir, "libjson_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    unless USE_STLPORT
      task :libstlport do
      end
    else
      task :libstlport => "config:android" do
        if USE_STLPORT
          objdir = $objdir["stlport"]
          libname = $libname["stlport"]

          args = []
          args << "-I#{$stlport_includes}"
          args << "-DTARGET_OS=android"
          args << "-DOSNAME=android"
          args << "-DCOMPILER_NAME=gcc"
          args << "-DBUILD_OSNAME=android"
          args << "-D_REENTRANT"
          args << "-D__NEW__"
          args << "-ffunction-sections"
          args << "-fdata-sections"
          args << "-fno-rtti"
          args << "-fno-exceptions"

          File.read(File.join($builddir, "libstlport_build.files")).each do |f|
            cc_compile f, objdir, args or exit 1
          end
          cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
        end
      end
    end

    task :librholog => "config:android" do
      srcdir = File.join $shareddir, "logging"
      objdir = $objdir["rholog"]
      libname = $libname["rholog"]
      args = []
      args << "-I#{srcdir}/.."
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

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
      args << "-I#{srcdir}"
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

      File.read(File.join($builddir, "librhomain_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :librhocommon => "config:android" do
      objdir = $objdir["rhocommon"]
      libname = $libname["rhocommon"]
      args = []
      args << "-I#{$shareddir}"
      args << "-I#{$shareddir}/curl/include"
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

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
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."
      args << "-I#{srcdir}/../sqlite"
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

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
      args << "-I#{srcdir}"
      args << "-I#{srcdir}/.."
      args << "-I#{srcdir}/../sqlite"
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

      File.read(File.join($builddir, "librhosync_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libs => [:libsqlite, :libopenssl, :libcurl, :libruby, :libjson, :libstlport, :librhodb, :librhocommon, :librhomain, :librhosync, :librholog]

    task :librhodes => :libs do
      srcdir = File.join $androidpath, "Rhodes", "jni", "src"
      incdir = File.join $androidpath, "Rhodes", "jni", "include"
      objdir = File.join $rhobindir, $confdir, "librhodes"
      libname = File.join $bindir, "libs", $confdir, "librhodes.so"

      # Generate gapikey.h
      gapikey_h = File.join(incdir, 'gapikey.h')
      gapi_defined = false
      if File.file? gapikey_h
        File.open(gapikey_h, 'r') do |f|
          while line = f.gets
            gapi_defined = true if line =~ /GOOGLE_API_KEY/
            break if gapi_defined
          end
        end
      end

      if $use_geomapping != gapi_defined or not File.file? gapikey_h
        File.open(gapikey_h, 'w') do |f|
          f.puts "#ifndef RHO_GAPIKEY_H_411BFA4742CF4F2AAA3F6B411ED7514F"
          f.puts "#define RHO_GAPIKEY_H_411BFA4742CF4F2AAA3F6B411ED7514F"
          f.puts "#define GOOGLE_API_KEY \"#{$gapikey}\"" if $use_geomapping and !$gapikey.nil?
          f.puts "#endif /* RHO_GAPIKEY_H_411BFA4742CF4F2AAA3F6B411ED7514F */"
        end
      end

      args = []
      args << "-I#{srcdir}/../include"
      args << "-I#{$shareddir}"
      args << "-I#{$shareddir}/sqlite"
      args << "-I#{$shareddir}/curl/include"
      args << "-I#{$shareddir}/ruby/include"
      args << "-I#{$shareddir}/ruby/linux"
      args << "-D__SGI_STL_INTERNAL_PAIR_H" if USE_STLPORT
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

      File.read(File.join($builddir, "librhodes_build.files")).each do |f|
        cc_compile f, objdir, args or exit 1
      end

      deps = []
      $libname.each do |k,v|
        deps << v
      end

      args = []
      args << "-L#{$rhobindir}/#{$confdir}"
      args << "-L#{$bindir}/libs/#{$confdir}"
      args << "-L#{$extensionsdir}"
      args << "-lrhomain"
      args << "-lruby"
      args << "-lrhosync"
      args << "-lrhodb"
      args << "-lrholog"
      args << "-lrhocommon"
      args << "-ljson"
      args << "-lstlport" if USE_STLPORT
      args << "-lcurl"
      args << "-lrhocommon" # Need to specify twice because libcurl depends on librhocommon and vice versa
      args << "-lssl" if USE_OPENSSL
      args << "-lcrypto" if USE_OPENSSL
      args << "-lsqlite"
      args << "-llog"
      args << "-ldl"
      args << "-lz"

      stub = []
      Dir.glob($extensionsdir + "/*.a").reverse.each do |f|
        lparam = "-l" + File.basename(f).gsub(/^lib/,"").gsub(/\.a$/,"")
        args << lparam
        # Workaround for GNU ld: this way we have specified one lib multiple times
        # command line so ld's dependency mechanism will find required functions
        # independently of its position in command line
        stub.each do |s|
          args << s
        end
        stub << lparam
      end

  	  mkdir_p File.dirname(libname) unless File.directory? File.dirname(libname)
      cc_link libname, Dir.glob(objdir + "/**/*.o"), args, deps or exit 1

      destdir = File.join($androidpath, "Rhodes", "libs", "armeabi")
      mkdir_p destdir unless File.exists? destdir
      cp_r libname, destdir
      cc_run($stripbin, [File.join(destdir, File.basename(libname))])
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
      puts Jake.run('"'+javac+'"',args)
      unless $? == 0
        set_app_name_android("Rhodes")
        generate_rjava
        puts "Error compiling java code"
        exit 1
      end

      srclist = File.join($builddir, "RhodesSRC_build.files")
      unless $use_geomapping
        newsrclist = File.join($tmpdir, "RhodesSRC_build.files")
        lines = []
        File.open(srclist, "r") do |f|
          while line = f.gets
            line.chomp!
            next if line =~ /\/mapview\//
            lines << line
          end
        end
        File.open(newsrclist, "w") { |f| f.write lines.join("\n") }
        srclist = newsrclist
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
      classpath = $androidjar
      classpath += $path_separator + $gapijar unless $gapijar.nil?
      classpath += $path_separator + "#{$tmpdir}/Rhodes"
      args << '"' + classpath + '"'
      args << "@#{srclist}"
      puts Jake.run('"'+javac+'"',args)
      unless $? == 0
        set_app_name_android("Rhodes")
        generate_rjava
        puts "Error compiling java code"
        exit 1
      end

      set_app_name_android("Rhodes")
      generate_rjava

      args = ["cf","../../Rhodes.jar", "#{$all_files_mask}"]
      puts Jake.run('"'+$jarbin+'"', args, "#{$tmpdir}/Rhodes/")
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
    args << "\"--output=#{outfile}\""
    args << "\"#{$bindir}/Rhodes.jar\""
    #args << "#{$bindir}/RhoBundle.jar"
    puts Jake.run('"'+$dx+'"',args)
    unless $? == 0
      puts "Error running DX utility"
      exit 1
    end

    manifest = Jake.get_absolute $androidpath + "/Rhodes/AndroidManifest.xml"
    resource = Jake.get_absolute $androidpath + "/Rhodes/res"
    assets = Jake.get_absolute $androidpath + "/Rhodes/assets"
    resourcepkg =  $bindir + "/rhodes.ap_"

    puts "Packaging Assets and Jars"

    iconbakname = $tmpdir + "/icon.png.bak"
    iconappname = $app_path + "/icon/icon.png"
    cp resource + "/drawable/icon.png",iconbakname
    cp iconappname, resource + "/drawable" if File.exists?(iconappname)
    set_app_name_android($appname)

    args = ["package", "-f", "-M", '"'+manifest+'"', "-S", '"'+resource+'"', "-A", '"'+assets+'"', "-I", '"'+$androidjar+'"', "-F", '"'+resourcepkg+'"']
    puts Jake.run('"'+$aapt+'"', args)
    returnval = $?

    set_app_name_android("Rhodes")
    mv iconbakname,resource + "/drawable/icon.png" if File.exists?(iconbakname)

    unless returnval == 0
      puts "Error running AAPT"
      exit 1
    end

    rm_rf $bindir + "/lib"
    mkdir_p $bindir + "/lib/armeabi"
    cp_r $bindir + "/libs/" + $confdir + "/librhodes.so", $bindir + "/lib/armeabi"
    cc_run($stripbin, [$bindir + "/lib/armeabi/librhodes.so"])
    args = ["add", '"'+resourcepkg+'"', "lib/armeabi/librhodes.so"]
    puts Jake.run('"'+$aapt+'"', args, $bindir)
    err = $?
    rm_rf $bindir + "/lib"
    unless err == 0
      puts "Error running AAPT"
      exit 1
    end
  end
end

def get_app_log(appname, device)
  pkgname = 'com.rhomobile.' + appname.downcase.gsub(/[^A-Za-z_0-9]/, '')
  path = File.join('/sdcard/rhomobile', pkgname, 'RhoLog.txt')
  cc_run('adb', [device ? '-d' : '-e', 'pull', path, '.']) or return false
  puts "RhoLog.txt stored to current directory"
  return true
end

namespace "device" do
  namespace "android" do

    desc "Build debug self signed for device"
    task :debug => "package:android" do
      dexfile =  $bindir + "/classes.dex"
      apkfile =  $targetdir + "/" + $appname + "-debug.apk"
      resourcepkg =  $bindir + "/rhodes.ap_"

      puts "Building APK file"
      puts `"#{$apkbuilder}" "#{apkfile}" -z "#{resourcepkg}" -f "#{dexfile}"`
      unless $? == 0
        puts "Error building APK file"
        exit 1
      end
    end

    task :install => :debug do
      apkfile = $targetdir + "/" + $appname + "-debug.apk"
      puts "Install APK file"
      puts `"#{$adb}" -d install -r "#{apkfile}"`
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
      puts `"#{$apkbuilder}" "#{apkfile}" -u -z "#{resourcepkg}" -f "#{dexfile}"`
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
        puts Jake.run('"'+$keytool+'"', args)
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
      puts Jake.run('"'+$jarsigner+'"', args)
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

    task :getlog => "config:android" do
      get_app_log($appname, true) or exit 1
    end
  end
end

namespace "emulator" do
  namespace "android" do
    task :getlog => "config:android" do
      get_app_log($appname, false) or exit 1
    end
  end
end

namespace "run" do
  desc "build and launch emulator"
  task :android => "device:android:debug" do
    apkfile = Jake.get_absolute $targetdir + "/" + $appname + "-debug.apk"
    puts `"#{$adb}" start-server`

    createavd = "\"#{$androidbin}\" create avd --name #{$avdname} --target #{$avdtarget} --sdcard 32M --skin HVGA"
    system(createavd) unless File.directory?( File.join(ENV['HOME'], ".android", "avd", "#{$avdname}.avd" ) )

    if $use_geomapping
      avdini = File.join(ENV['HOME'], '.android', 'avd', "#{$avdname}.ini")
      avd_using_gapi = true if File.new(avdini).read =~ /:Google APIs:/
      unless avd_using_gapi
        puts "Can not use specified AVD (#{$avdname}) because of incompatibility with Google APIs. Delete it and try again."
        exit 1
      end
    end

    running = false
    `adb devices`.split("\n").each do |line|
      next unless line =~ /^emulator/
      running = true
      break
    end
    Thread.new { system("\"#{$emulator}\" -avd #{$avdname}") } unless running

    puts "Waiting for emulator to get started" unless running
    puts "Emulator is up and running" if running
    $stdout.flush
    puts `"#{$adb}" -e wait-for-device`

    puts "Loading package into emulator"
    theoutput = `"#{$adb}" -e install -r "#{apkfile}"`
    count = 0
    done = false
    while count < 15
      theoutput = `"#{$adb}" -e install -r "#{apkfile}"`
      puts theoutput.to_s
      $stdout.flush

      if theoutput.to_s.match(/Success/)
        done = true
        break
      end

      if theoutput.to_s.match(/Failure/)
        done = false
        break
      end

      puts "Failed to load (possibly because emulator not done launching)- retrying"
      $stdout.flush
      sleep 5
      count += 1
    end
    puts "Loading complete, you may now run the application" if done
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
  task :libsqlite => "config:android" do
    cc_clean "sqlite"
  end
  task :libopenssl => "config:android" do
    rm_rf $objdir['openssl'] if USE_OPENSSL
    rm_rf File.dirname($libname['openssl']) + "/libssl.a" if USE_OPENSSL
    rm_rf File.dirname($libname['openssl']) + "/libcrypto.a" if USE_OPENSSL
  end
  task :libs => ["config:android",:libopenssl] do
    $native_libs.each do |l|
      cc_clean l
    end
  end
  task :librhodes => "config:android" do
    rm_rf $rhobindir + "/" + $confdir + "/librhodes"
    rm_rf $bindir + "/libs/" + $confdir + "/librhodes.so"
  end
#  desc "clean android"
  task :all => [:assets,:librhodes,:libs,:files]
  end
end

