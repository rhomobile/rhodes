#
require File.dirname(__FILE__) + '/androidcommon.rb'
require 'pathname'

USE_STLPORT = true
USE_TRACES = false

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

ANDROID_PERMISSIONS = {
  'camera' => 'CAMERA',
  'gps' => 'ACCESS_FINE_LOCATION',
  'network_state' => 'ACCESS_NETWORK_STATE',
  'phone' => ['CALL_PHONE', 'READ_PHONE_STATE'],
  'pim' => ['READ_CONTACTS', 'WRITE_CONTACTS'],
  'vibrate' => 'VIBRATE'
}

def get_sources(name)
  File.read(File.join($builddir, name + '_build.files')).split("\n")
end

def set_app_name_android(newname)
  puts "set_app_name"
  $stdout.flush

  rm_rf $appres
  cp_r $rhores, $appres

  iconappname = File.join($app_path, "icon", "icon.png")
  iconresname = File.join($appres, "drawable", "icon.png")
  rm_f iconresname
  cp iconappname, iconresname

  rhostrings = File.join($rhores, "values", "strings.xml")
  appstrings = File.join($appres, "values", "strings.xml")
  doc = REXML::Document.new(File.new(rhostrings))
  doc.elements["resources/string[@name='app_name']"].text = newname
  File.open(appstrings, "w") { |f| doc.write f }

  version = {'major' => 0, 'minor' => 0, 'patch' => 0}
  if $app_config["version"]
    if $app_config["version"] =~ /^(\d+)$/
      version["major"] = $1.to_i
    elsif $app_config["version"] =~ /^(\d+)\.(\d+)$/
      version["major"] = $1.to_i
      version["minor"] = $2.to_i
    elsif $app_config["version"] =~ /^(\d+)\.(\d+)\.(\d+)$/
      version["major"] = $1.to_i
      version["minor"] = $2.to_i
      version["patch"] = $3.to_i
    end
  end
  
  version = version["major"]*10000 + version["minor"]*100 + version["patch"]

  doc = REXML::Document.new(File.new($rhomanifest))
  doc.root.attributes['package'] = $app_package_name
  if version > 0
    doc.root.attributes['android:versionCode'] = version.to_s
    doc.root.attributes['android:versionName'] = $app_config["version"]
  end
  doc.elements.delete "manifest/application/uses-library[@android:name='com.google.android.maps']" unless $use_geomapping

  caps = ['INTERNET']
  $app_config["capabilities"].each do |cap|
    cap = ANDROID_PERMISSIONS[cap]
    next if cap.nil?
    cap = [cap] if cap.is_a? String
    cap = [] unless cap.is_a? Array
    caps += cap
  end
  caps.uniq!

  manifest = doc.elements["manifest"]
  manifest.elements.each('uses-permission') { |e| manifest.delete e }
  caps.each do |cap|
    element = REXML::Element.new('uses-permission')
    element.add_attribute('android:name', "android.permission.#{cap}")
    manifest.add element
  end

  File.open($appmanifest, "w") { |f| doc.write f, 2 }

  buf = File.new($rho_android_r,"r").read.gsub(/^\s*import com\.rhomobile\..*\.R;\s*$/,"\nimport #{$app_package_name}.R;\n")
  File.open($app_android_r,"w") { |f| f.write(buf) }
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
    $use_geomapping = 'false' if $use_geomapping.nil?
    $use_geomapping = get_boolean($use_geomapping.to_s)

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
    $app_package_name = "com.rhomobile." + $appname.downcase.gsub(/[^A-Za-z_0-9]/, '')

    $rhomanifest = File.join $androidpath, "Rhodes", "AndroidManifest.xml"
    $appmanifest = File.join $tmpdir, "AndroidManifest.xml"

    $rhores = File.join $androidpath, "Rhodes", "res"
    $appres = File.join $tmpdir, "res"

    $appincdir = File.join $tmpdir, "include"

    $rho_android_r = File.join $androidpath, "Rhodes", "src", "com", "rhomobile", "rhodes", "AndroidR.java"
    $app_android_r = File.join $tmpdir, "AndroidR.java"
    $app_rjava_dir = File.join $tmpdir
    $app_native_libs_java = File.join $tmpdir, "NativeLibraries.java"

    if RUBY_PLATFORM =~ /(win|w)32$/
      $emulator = #"cmd /c " + 
        File.join( $androidsdkpath, "tools", "emulator.exe" )
      $bat_ext = ".bat"
      $exe_ext = ".exe"
      $path_separator = ";"
      $rubypath = "res/build-tools/RhoRuby.exe"

      # Add PATH to cygwin1.dll
      ENV['CYGWIN'] = 'nodosfilewarning'
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

    puts "+++ Looking for platform..." if USE_TRACES
    Dir.glob(File.join($androidsdkpath, "platforms", "*")).each do |platform|
      props = File.join(platform, "source.properties")
      unless File.file? props
        puts "+++ WARNING! No source.properties found in #{platform}" if USE_TRACES
        next
      end

      apilevel = -1
      marketversion = nil
      File.open(props, "r") do |f|
        while line = f.gets
          apilevel = $1.to_i if line =~ /^\s*AndroidVersion\.ApiLevel\s*=\s*([0-9]+)\s*$/
          marketversion = $1 if line =~ /^\s*Platform\.Version\s*=\s*([^\s]*)\s*$/
        end
      end

      puts "+++ API LEVEL of #{platform}: #{apilevel}" if USE_TRACES

      $androidplatform = File.basename(platform) if apilevel == ANDROID_API_LEVEL
    end

    if $androidplatform.nil?
      ajar = File.join($androidsdkpath, 'platforms', 'android-' + ANDROID_API_LEVEL_TO_MARKET_VERSION[ANDROID_API_LEVEL], 'android.jar')
      $androidplatform = 'android-' + ANDROID_API_LEVEL_TO_MARKET_VERSION[ANDROID_API_LEVEL] if File.file?(ajar)
    end

    if $androidplatform.nil?
      puts "+++ No required platform (API level #{ANDROID_API_LEVEL}) found, can't proceed"
      exit 1
    else
      puts "+++ Platform found: #{$androidplatform}" if USE_TRACES
    end
    $stdout.flush
    
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
      puts "+++ Looking for Google APIs add-on..." if USE_TRACES
      Dir.glob(File.join($androidsdkpath, 'add-ons', '*')).each do |dir|
        break unless $gapijar.nil?

        props = File.join(dir, 'manifest.ini')
        if !File.file? props
          puts "+++ WARNING: no manifest.ini found in #{dir}" if USE_TRACES
          next
        end

        apilevel = -1
        File.open(props, 'r') do |f|
          while line = f.gets
            next unless line =~ /^api=([0-9]+)$/
            apilevel = $1.to_i
            break
          end
        end

        puts "+++ API LEVEL of #{dir}: #{apilevel}" if USE_TRACES

        $gapijar = File.join(dir, 'libs', 'maps.jar') if apilevel == ANDROID_API_LEVEL
      end
      if $gapijar.nil?
        puts "+++ No Google APIs add-on found (which is required because 'mapping' enabled in build.yml)"
        exit 1
      else
        puts "+++ Google APIs add-on found: #{$gapijar}" if USE_TRACES
      end
    end

    setup_ndk($androidndkpath,ANDROID_API_LEVEL)
    
    $stlport_includes = File.join $shareddir, "stlport", "stlport"

    $native_libs = ["sqlite", "curl", "stlport", "ruby", "json", "rhocommon", "rhodb", "rholog", "rhosync", "rhomain"]

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

    $app_config["extensions"] = [] if $app_config["extensions"].nil?
    $app_config["extensions"] = [] unless $app_config["extensions"].is_a? Array
    if $app_config["android"] and $app_config["android"]["extensions"]
      $app_config["extensions"] += $app_config["android"]["extensions"]
      $app_config["android"]["extensions"] = nil
    end

    $app_config["capabilities"] = [] if $app_config["capabilities"].nil?
    $app_config["capabilities"] = [] unless $app_config["capabilities"].is_a? Array
    if $app_config["android"] and $app_config["android"]["capabilities"]
      $app_config["capabilities"] += $app_config["android"]["capabilities"]
      $app_config["android"]["capabilities"] = nil
    end
    $app_config["capabilities"].map! { |cap| cap.is_a?(String) ? cap : nil }.delete_if { |cap| cap.nil? }

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

      manifest = $appmanifest
      resource = $appres
      assets = Jake.get_absolute(File.join($androidpath, "Rhodes", "assets"))
      nativelibs = Jake.get_absolute(File.join($androidpath, "Rhodes", "libs"))
      #rjava = Jake.get_absolute(File.join($androidpath, "Rhodes", "gen", "com", "rhomobile", "rhodes"))

      args = ["package", "-f", "-M", manifest, "-S", resource, "-A", assets, "-I", $androidjar, "-J", $app_rjava_dir]
      puts Jake.run($aapt, args)

      exitstatus = $?
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

    task :extensions => :genconfig do

      ENV['RHO_PLATFORM'] = 'android'
      ENV["ANDROID_NDK"] = $androidndkpath
      ENV["ANDROID_API_LEVEL"] = ANDROID_API_LEVEL.to_s
      ENV["TARGET_TEMP_DIR"] = $extensionsdir
      ENV["RHO_ROOT"] = $startdir
      ENV["BUILD_DIR"] ||= $startdir + "/platform/android/build"
      ENV["RHO_INC"] = $appincdir

      mkdir_p $extensionsdir unless File.directory? $extensionsdir

      $app_config["extensions"].each do |ext|
        $app_config["extpaths"].each do |p|
          extpath = File.join(p, ext, 'ext')
          if RUBY_PLATFORM =~ /(win|w)32$/
            next unless File.exists? File.join(extpath, 'build.bat')
          else
            next unless File.executable? File.join(extpath, 'build')
          end

          ENV['TEMP_FILES_DIR'] = File.join(ENV["TARGET_TEMP_DIR"], ext)

          if RUBY_PLATFORM =~ /(win|w)32$/
            puts Jake.run('build.bat', [], extpath)
          else
            puts Jake.run('./build', [], extpath)
          end
          exit 1 unless $? == 0
        end
      end

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

      get_sources('libcurl').each do |f|
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

      get_sources('libruby').each do |f|
        cc_compile f, objdir, args or exit 1
      end

      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
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
      get_sources('libjson').each do |f|
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

          get_sources('libstlport').each do |f|
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

      get_sources('librholog').each do |f|
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

      get_sources('librhomain').each do |f|
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
      get_sources('librhocommon').each do |f|
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

      get_sources('librhodb').each do |f|
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

      get_sources('librhosync').each do |f|
        cc_compile f, objdir, args or exit 1
      end
      cc_ar libname, Dir.glob(objdir + "/**/*.o") or exit 1
    end

    task :libs => [:libsqlite, :libcurl, :libruby, :libjson, :libstlport, :librhodb, :librhocommon, :librhomain, :librhosync, :librholog]

    task :genconfig => "config:android" do
      mkdir_p $appincdir unless File.directory? $appincdir

      # Generate genconfig.h
      genconfig_h = File.join($appincdir, 'genconfig.h')

      gapi_already_enabled = false
      caps_already_enabled = {}
      ANDROID_PERMISSIONS.keys.each do |k|
        caps_already_enabled[k] = false
      end
      if File.file? genconfig_h
        File.open(genconfig_h, 'r') do |f|
          while line = f.gets
            if line =~ /^\s*#\s*define\s+RHO_GOOGLE_API_KEY\s+"[^"]*"\s*$/
              gapi_already_enabled = true
            else
              ANDROID_PERMISSIONS.keys.each do |k|
                re = /^\s*#\s*define\s+RHO_CAP_#{k.upcase}_ENABLED\s+true\s*$/
                caps_already_enabled[k] = true if line =~ re
              end
            end
          end
        end
      end

      regenerate = false
      regenerate = true unless File.file? genconfig_h
      regenerate = $use_geomapping != gapi_already_enabled unless regenerate

      caps_enabled = {}
      ANDROID_PERMISSIONS.keys.each do |k|
        caps_enabled[k] = $app_config["capabilities"].index(k) != nil
        regenerate = true if caps_enabled[k] != caps_already_enabled[k]
      end

      if regenerate
        puts "Need to regenerate genconfig.h"
        $stdout.flush
        File.open(genconfig_h, 'w') do |f|
          f.puts "#ifndef RHO_GENCONFIG_H_411BFA4742CF4F2AAA3F6B411ED7514F"
          f.puts "#define RHO_GENCONFIG_H_411BFA4742CF4F2AAA3F6B411ED7514F"
          f.puts ""
          f.puts "#define RHO_GOOGLE_API_KEY \"#{$gapikey}\"" if $use_geomapping and !$gapikey.nil?
          caps_enabled.each do |k,v|
            f.puts "#define RHO_CAP_#{k.upcase}_ENABLED #{v ? "true" : "false"}"
          end
          f.puts ""
          f.puts "#endif /* RHO_GENCONFIG_H_411BFA4742CF4F2AAA3F6B411ED7514F */"
        end
      else
        puts "No need to regenerate genconfig.h"
        $stdout.flush
      end

      # Generate rhocaps.inc
      rhocaps_inc = File.join($appincdir, 'rhocaps.inc')
      caps_already_defined = []
      if File.exists? rhocaps_inc
        File.open(rhocaps_inc, 'r') do |f|
          while line = f.gets
            next unless line =~ /^\s*RHO_DEFINE_CAP\s*\(\s*([A-Z_]*)\s*\)\s*\s*$/
            caps_already_defined << $1.downcase
          end
        end
      end

      if caps_already_defined.sort.uniq != ANDROID_PERMISSIONS.keys.sort.uniq
        puts "Need to regenerate rhocaps.inc"
        $stdout.flush
        File.open(rhocaps_inc, 'w') do |f|
          ANDROID_PERMISSIONS.keys.each do |k|
            f.puts "RHO_DEFINE_CAP(#{k.upcase})"
          end
        end
      else
        puts "No need to regenerate rhocaps.inc"
        $stdout.flush
      end
    end

    task :gen_java_ext => "config:android" do
      File.open($app_native_libs_java, "w") do |f|
        f.puts "package com.rhomobile.rhodes;"
        f.puts "public class NativeLibraries {"
        f.puts "  public static void load() {"
        f.puts "    // Load native .so libraries"
        Dir.glob($extensionsdir + "/lib*.so").reverse.each do |lib|
          libname = File.basename(lib).gsub(/^lib/, '').gsub(/\.so$/, '')
          f.puts "    System.loadLibrary(\"#{libname}\");"
        end
        f.puts "    // Load native implementation of rhodes"
        f.puts "    System.loadLibrary(\"rhodes\");"
        f.puts "  }"
        f.puts "};"
      end
    end

    task :gensources => [:genconfig, :gen_java_ext]

    task :librhodes => [:libs, :gensources] do
      srcdir = File.join $androidpath, "Rhodes", "jni", "src"
      objdir = File.join $bindir, "libs", $confdir, "librhodes"
      libname = File.join $bindir, "libs", $confdir, "librhodes.so"

      args = []
      args << "-I#{$appincdir}"
      args << "-I#{srcdir}/../include"
      args << "-I#{$shareddir}"
      args << "-I#{$shareddir}/sqlite"
      args << "-I#{$shareddir}/curl/include"
      args << "-I#{$shareddir}/ruby/include"
      args << "-I#{$shareddir}/ruby/linux"
      args << "-D__SGI_STL_INTERNAL_PAIR_H" if USE_STLPORT
      args << "-D__NEW__" if USE_STLPORT
      args << "-I#{$stlport_includes}" if USE_STLPORT

      get_sources('librhodes').each do |f|
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

      rlibs = []
      rlibs << "rhomain"
      rlibs << "ruby"
      rlibs << "rhosync"
      rlibs << "rhodb"
      rlibs << "rholog"
      rlibs << "rhocommon"
      rlibs << "json"
      rlibs << "stlport" if USE_STLPORT
      rlibs << "curl"
      rlibs << "sqlite"
      rlibs << "log"
      rlibs << "dl"
      rlibs << "z"

      rlibs.map! { |x| "-l#{x}" }

      args += rlibs

      extlibs = Dir.glob($extensionsdir + "/lib*.a") + Dir.glob($extensionsdir + "/lib*.so")
      stub = []
      extlibs.reverse.each do |f|
        lparam = "-l" + File.basename(f).gsub(/^lib/,"").gsub(/\.(a|so)$/,"")
        args << lparam
        # Workaround for GNU ld: this way we have specified one lib multiple times
        # command line so ld's dependency mechanism will find required functions
        # independently of its position in command line
        stub.each do |s|
          args << s
        end
        stub << lparam
      end

      args += rlibs

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
      args << $tmpdir + '/Rhodes'
      args << "-source"
      args << "1.6"
      args << "-target"
      args << "1.6"
      args << "-nowarn"
      args << "-encoding"
      args << "latin1"
      args << File.join($app_rjava_dir, "R.java")
      puts Jake.run(javac, args)
      unless $? == 0
        puts "Error compiling java code"
        exit 1
      end

      srclist = File.join($builddir, "RhodesSRC_build.files")
      newsrclist = File.join($tmpdir, "RhodesSRC_build.files")
      lines = []
      File.open(srclist, "r") do |f|
        while line = f.gets
          line.chomp!
          next if line =~ /\/AndroidR\.java\s*$/
          next if !$use_geomapping and line =~ /\/mapview\//
          lines << line
        end
      end
      lines << $app_android_r
      lines << $app_native_libs_java
      if File.exists? File.join($extensionsdir, "ext_build.files")
        File.open(File.join($extensionsdir, "ext_build.files")) do |f|
          while line = f.gets
            lines << line
          end
        end
      end
      File.open(newsrclist, "w") { |f| f.write lines.join("\n") }
      srclist = newsrclist

      args = []
      args << "-g"
      args << "-d"
      args << $tmpdir + '/Rhodes'
      args << "-source"
      args << "1.6"
      args << "-target"
      args << "1.6"
      args << "-nowarn"
      args << "-classpath"
      classpath = $androidjar
      classpath += $path_separator + $gapijar unless $gapijar.nil?
      classpath += $path_separator + "#{$tmpdir}/Rhodes"
      args << classpath
      args << "@#{srclist}"
      puts Jake.run(javac, args)
      unless $? == 0
        puts "Error compiling java code"
        exit 1
      end

      files = []
      Dir.glob(File.join($tmpdir, "Rhodes", "*")).each do |f|
        relpath = Pathname.new(f).relative_path_from(Pathname.new(File.join($tmpdir, "Rhodes"))).to_s
        files << relpath
      end
      unless files.empty?
        args = ["cf", "../../Rhodes.jar"]
        args += files
        puts Jake.run($jarbin, args, File.join($tmpdir, "Rhodes"))
        unless $? == 0
          puts "Error running jar"
          exit 1
        end
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
    args << "--output=#{$bindir}/classes.dex"
    args << "#{$bindir}/Rhodes.jar"
    puts Jake.run($dx, args)
    unless $? == 0
      puts "Error running DX utility"
      exit 1
    end

    manifest = $appmanifest
    resource = $appres
    assets = Jake.get_absolute $androidpath + "/Rhodes/assets"
    resourcepkg =  $bindir + "/rhodes.ap_"

    puts "Packaging Assets and Jars"

    set_app_name_android($appname)

    args = ["package", "-f", "-M", manifest, "-S", resource, "-A", assets, "-I", $androidjar, "-F", resourcepkg]
    puts Jake.run($aapt, args)
    unless $? == 0
      puts "Error running AAPT (1)"
      exit 1
    end

    # Workaround: manually add files starting with '_' because aapt silently ignore such files when creating package
    rm_rf File.join($tmpdir, "assets")
    cp_r assets, $tmpdir
    Dir.glob(File.join($tmpdir, "assets/**/*")).each do |f|
      next unless File.basename(f) =~ /^_/
      relpath = Pathname.new(f).relative_path_from(Pathname.new($tmpdir)).to_s
      puts "Add #{relpath} to #{resourcepkg}..."
      args = ["add", resourcepkg, relpath]
      puts Jake.run($aapt, args, $tmpdir)
      unless $? == 0
        puts "Error running AAPT (2)"
        exit 1
      end
    end

    # Add native librhodes.so
    rm_rf File.join($tmpdir, "lib")
    mkdir_p File.join($tmpdir, "lib/armeabi")
    cp_r File.join($bindir, "libs", $confdir, "librhodes.so"), File.join($tmpdir, "lib/armeabi")
    # Add extensions .so libraries
    Dir.glob($extensionsdir + "/lib*.so").each do |lib|
      cp_r lib, File.join($tmpdir, "lib/armeabi")
    end
    args = ["add", resourcepkg]
    # Strip them all to decrease size
    Dir.glob($tmpdir + "/lib/armeabi/lib*.so").each do |lib|
      cc_run($stripbin, [lib])
      args << "lib/armeabi/#{File.basename(lib)}"
    end
    puts Jake.run($aapt, args, $tmpdir)
    err = $?
    rm_rf $tmpdir + "/lib"
    unless err == 0
      puts "Error running AAPT (3)"
      exit 1
    end
  end
end

def get_app_log(appname, device)
  pkgname = 'com.rhomobile.' + appname.downcase.gsub(/[^A-Za-z_0-9]/, '')
  path = File.join('/sdcard/rhomobile', pkgname, 'RhoLog.txt')
  cc_run($adb, [device ? '-d' : '-e', 'pull', path, $app_path]) or return false
  puts "RhoLog.txt stored to " + $app_path
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
      Jake.run($apkbuilder, [apkfile, "-z", resourcepkg, "-f", dexfile])
      unless $? == 0
        puts "Error building APK file"
        exit 1
      end
    end

    task :install => :debug do
      apkfile = $targetdir + "/" + $appname + "-debug.apk"
      puts "Install APK file"
      Jake.run($adb, ["-d", "install", "-r", apkfile])
      unless $? == 0
        puts "Error installing APK file"
        exit 1
      end
      puts "Install complete"
    end

    desc "Build production signed for device"
    task :production => "package:android" do
      dexfile =  $bindir + "/classes.dex"
      apkfile =  $targetdir + "/" + $appname + ".apk"
      signedapkfile =  $targetdir + "/" + $appname + "_signed.apk"
      resourcepkg =  $bindir + "/rhodes.ap_"

      puts "Building APK file"
      Jake.run($apkbuilder, [apkfile, "-u", "-z", resourcepkg, "-f", dexfile])
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
        args << $keystore
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
      args << $keystore
      args << "-storepass"
      args << $storepass
      args << "-signedjar"
      args << signedapkfile
      args << apkfile
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


def is_emulator_running
  `#{$adb} devices`.split("\n")[1..-1].each do |line|
    return true if line =~ /^emulator/
  end
  return false
end

def is_device_running
  `#{$adb} devices`.split("\n")[1..-1].each do |line|
    return true if line !~ /^emulator/
  end
  return false
end

namespace "run" do
  namespace "android" do
    task :emulator => "device:android:debug" do
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

      running = is_emulator_running
      Thread.new { system("\"#{$emulator}\" -avd #{$avdname}") } unless running

      puts "Waiting for emulator to get started" unless running
      puts "Emulator is up and running" if running
      $stdout.flush
      puts `"#{$adb}" -e wait-for-device`

      puts "Loading package into emulator"
      count = 0
      done = false
      while count < 20
        f = Jake.run2($adb, ["-e", "install", "-r", apkfile], {:nowait => true})
        theoutput = ""
        while c = f.getc
          $stdout.putc c
          $stdout.flush
          theoutput << c
        end
        f.close

        if theoutput.to_s.match(/Success/)
          done = true
          break
        end

        puts "Failed to load (possibly because emulator not done launching)- retrying"
        $stdout.flush
        sleep 1
        count += 1
      end
      puts "Loading complete, you may now run the application" if done
    end

    desc "build and install on device"
    task :device => "device:android:install" do
    end
  end

  desc "build and launch emulator"
  task :android => "run:android:emulator" do
  end
end

namespace "uninstall" do
  def do_uninstall(flag)
    args = []
    args << flag
    args << "uninstall"
    args << $app_package_name
    Jake.run($adb, args)
    unless $? == 0
      puts "Error uninstalling application"
      exit 1
    end

    args = []
    args << flag
    args << "shell"
    args << "rm"
    args << "-r"
    args << "/sdcard/rhomobile/#{$app_package_name}"
    Jake.run($adb, args)
    unless $? == 0
      puts "Error removing files from SD card"
      exit 1
    end

    puts "Application uninstalled successfully"
  end

  namespace "android" do
    task :emulator => "config:android" do
      unless is_emulator_running
        puts "WARNING!!! Emulator is not up and running"
        exit 1
      end
      do_uninstall('-e')
    end

    desc "uninstall from device"
    task :device => "config:android" do
      unless is_device_running
        puts "WARNING!!! Device is not connected"
        exit 1
      end
      do_uninstall('-d')
    end
  end

  desc "uninstall from emulator"
  task :android => "uninstall:android:emulator" do
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
  task :libs => ["config:android"] do
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

