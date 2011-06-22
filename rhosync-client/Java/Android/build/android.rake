
#USE_TRACES = Rake.application.options.trace
USE_OWN_STLPORT = false


$current_platform = 'android'
$min_api_level = 4

if RUBY_PLATFORM =~ /(win|w)32$/
  $all_files_mask = "*.*"
  $rubypath = "res/build-tools/RhoRuby.exe"
  $bat_ext = ".bat"
  $exe_ext = ".exe"
  $path_separator = ";"
else
  $all_files_mask = "*"
  $bat_ext = ""
  $exe_ext = ""
  $path_separator = ":"
  if RUBY_PLATFORM =~ /darwin/
    $rubypath = "res/build-tools/RubyMac"
  else
    $rubypath = "res/build-tools/rubylinux"
  end
end
$basepath = File.expand_path File.dirname(__FILE__)

require File.join($basepath, 'android_sdk.rb')

CONFIG = 'debug'
$buildroot = $appconfig['android']["build_path"]
BUILDPATH = File.join($buildroot, CONFIG)

$buildargs = {
              # Steps to ge-DHAVE_CONFIG_Ht curl_config.h from fresh libcurl sources:
              #export PATH=<ndkroot>/build/prebuilt/linux-x86/arm-eabi-4.2.1/bin:$PATH
              #export CC=arm-eabi-gcc
              #export CPP=arm-eabi-cpp
              #export CFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
              #export CPPFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
              #./configure --without-ssl --without-ca-bundle --without-ca-path --without-libssh2 --without-libidn --disable-ldap --disable-ldaps --host=arm-eabi
              "curl" =>   ["-DHAVE_CONFIG_H",
                           "-I#{File.join($sharedpath, "curl", "include")}",
                           "-I#{File.join($sharedpath, "curl")}",
                           "-I#{$sharedpath}"],
              "sqlite" => ["-I#{File.join($sharedpath, "sqlite")}",
                           "-I#{$sharedpath}"],
              "json" =>   ["-I#{File.join($sharedpath, "json")}",
                           "-I#{$sharedpath}"]
             }

#task :default => ["android:config"]

SRC = FileList.new
OBJ = FileList.new
LIBFN = FileList.new
CLEAN = FileList.new

LIBS = Hash.new

LIBS['curl'] = File.join(BUILDPATH, 'libcurl.a')
LIBS['sqlite'] = File.join(BUILDPATH, 'libsqlite.a')
LIBS['json'] = File.join(BUILDPATH, 'libjson.a')

CPPLIBS = ['json']

LIBS.each do |name, filename|
  sources = get_sources("lib#{name}")
  SRC.include sources
  OBJ.include get_objects(sources, File.join(BUILDPATH, "lib#{name}"))
  LIBFN.include filename
end

CLEAN.include OBJ
CLEAN.include LIBFN

namespace "android" do
  namespace "config" do

    def build_tree(buildpath)
      $obj_src = Hash.new
      $obj_lib = Hash.new
      LIBS.each do |name, filename|
        libsources = get_sources "lib#{name}"
        libsources.each do |src|
          objpath = get_object src, File.join(buildpath, "lib#{name}")
          $obj_src[objpath] = src
          $obj_lib[objpath] = name

          if USE_TRACES          
            puts "#{objpath} => #{src}"
            puts "#{objpath} => #{name}"
            puts ""
          end
        end
      end
    end

    def build_args()
      CPPLIBS.each do |lib|
        unless $std_includes.nil?
          puts "Use stl for #{lib}" if USE_TRACES
          $buildargs[lib] << "-I#{$std_includes}"
        end
        if USE_OWN_STLPORT
          puts "Use own stlport for #{lib}" if USE_TRACES
          $buildargs[lib] << "-D__NEW__"
          $buildargs[lib] << "-I#{$stlport_includes}"
        end
      end
    end

    task :sdk do # => ["config:buildyml", "config:rhobuildyml"] do
      $androidsdkpath = $rhoconfig["env"]["paths"]["android"]
      sdk_level = setup_sdk $androidsdkpath, $min_api_level

      $androidndkpath = $rhoconfig["env"]["paths"]["android-ndk"]
      setup_ndk $androidndkpath, sdk_level
    end

    task :stdlib => [:sdk] do
      $std_includes = File.join $androidndkpath, "sources", "cxx-stl", "stlport", "stlport"
      unless File.directory? $std_includes
        $stlport_includes = File.join $sharedpath, "stlport", "stlport"
        USE_OWN_STLPORT = true
      end
    end

  end # namespace "config"

  task :config => ["config:sdk", "config:stdlib"] do
    build_tree BUILDPATH
    build_args
  end # task :config

  namespace "build" do

    task :libraries => ["android:config", BUILDPATH, :libcurl, :libsqlite]

    task :libsqlite => [File.join(BUILDPATH, 'libsqlite'), LIBS['sqlite'] ]

    task :libcurl => [File.join(BUILDPATH, 'libcurl'), LIBS['curl'] ]

    task :libjson => [File.join(BUILDPATH, 'libjson'), LIBS['json'] ]

  end # namespace "build"

  task :clean do
    CLEAN.each do |filename|
      `rm -rf #{filename}`
    end
  end
end # namespace "android"

    directory BUILDPATH
    file BUILDPATH => ["android:config"]
    
    directory File.join(BUILDPATH, 'libcurl')
    file File.join(BUILDPATH, 'libcurl') => BUILDPATH

    directory File.join(BUILDPATH, 'libsqlite')
    file File.join(BUILDPATH, 'libsqlite') => BUILDPATH

    directory File.join(BUILDPATH, 'libjson')
    file File.join(BUILDPATH, 'libjson') => BUILDPATH

    def lib_objects(libfile)
      lib = File.basename(libfile).gsub(/\.a$/, "")

      sources = get_sources lib
      if USE_TRACES
        puts "#{lib} sources:"
        sources.each do |src|
            puts src
        end
      end

      objpath = File.join(BUILDPATH, lib)

      objects = get_objects sources, objpath
      objects
    end

    def obj_source(objfile)
      src = SRC.to_a().find { |src| (File.basename(src) + ".o") == File.basename(objfile) }
      src
    end

    rule '.a' => [ lambda { |libfile| lib_objects(libfile) } ] do |t|
      lib = t.name.gsub(/.*\/lib/, "").gsub(/\.a$/, "")
      puts "#{lib}: #{t.name}" if USE_TRACES

      objpath = File.join(BUILDPATH, "lib#{lib}")
      sources = get_sources "lib#{lib}"
      objects = get_objects sources, objpath

      cc_ar(t.name, objects) or exit 1
    end

    rule '.o' => lambda { |objfile| obj_source(objfile) } do |t|
      lib = $obj_lib[t.name]
      cc_compile(t.source, File.dirname(t.name), $buildargs[lib]) or exit 1
    end


