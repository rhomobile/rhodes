
#USE_TRACES = Rake.application.options.trace

$basepath = File.dirname(__FILE__)
$rootpath = File.join $basepath, '..', '..'

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

require File.join($basepath, 'android_sdk.rb')
require File.join($rootpath, 'lib/build/jake.rb')

$rootpath = Jake.get_absolute $rootpath
$sharedpath = File.join $rootpath, 'platform', 'shared'
CONFIG = 'debug'
BUILDPATH = File.join('build', CONFIG)
LIBS = {'curl' => File.join(BUILDPATH, 'libcurl.a'),
        'sqlite' => File.join(BUILDPATH, 'libsqlite.a')}
LIBFN = FileList.new File.join(BUILDPATH, 'libcurl.a'), File.join(BUILDPATH, 'libsqlite.a')

$buildargs = {
              # Steps to ge-DHAVE_CONFIG_Ht curl_config.h from fresh libcurl sources:
              #export PATH=<ndkroot>/build/prebuilt/linux-x86/arm-eabi-4.2.1/bin:$PATH
              #export CC=arm-eabi-gcc
              #export CPP=arm-eabi-cpp
              #export CFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
              #export CPPFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
              #./configure --without-ssl --without-ca-bundle --without-ca-path --without-libssh2 --without-libidn --disable-ldap --disable-ldaps --host=arm-eabi
              "curl" => ["-DHAVE_CONFIG_H",
                           "-I#{File.join($sharedpath, "curl", "include")}",
                           "-I#{File.join($sharedpath, "curl")}",
                           "-I#{$sharedpath}"],
              "sqlite" => ["-I#{File.join($sharedpath, "sqlite")}",
                           "-I#{$sharedpath}"]
             }

#task :default => ["android:config"]

LIBS.each do |name, filename|
  puts "lib: #{name} => #{filename}"
end

SRC = FileList.new
OBJ = FileList.new
CLEAN = FileList.new

SRC.include get_sources("libcurl")
OBJ.include get_objects(get_sources("libcurl"), File.join(BUILDPATH, "libcurl"))

SRC.include get_sources("libsqlite")
OBJ.include get_objects(get_sources("libsqlite"), File.join(BUILDPATH, "libsqlite"))

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

    task :rhobuildyml do
      rhobuildyml = File.join $rootpath, 'rhobuild.yml'
      rhobuildyml = ENV["RHOBUILD"] unless ENV["RHOBUILD"].nil?

      $rhoconfig = Jake.config(File.open(rhobuildyml))
      $rhoconfig["platform"] = 'android'
    end

    task :buildyml do
      buildyml = File.join $basepath, 'build.yml'
      $appconfig = Jake.config(File.open(buildyml))
    end

    task :sdk => [:buildyml, :rhobuildyml] do
      $androidsdkpath = $rhoconfig["env"]["paths"]["android"]
      sdk_level = setup_sdk $androidsdkpath, $min_api_level

      $androidndkpath = $rhoconfig["env"]["paths"]["android-ndk"]
      setup_ndk $androidndkpath, sdk_level
    end

  end # namespace "config"

  task :config => "config:sdk" do
    build_tree BUILDPATH
    
  end # task :config

  namespace "build" do

    task :libraries => ["android:config", BUILDPATH, :libcurl, :libsqlite]

    task :libsqlite => [File.join(BUILDPATH, 'libsqlite'), LIBS['sqlite'] ]

    task :libcurl => [File.join(BUILDPATH, 'libcurl'), LIBS['curl'] ]

  end # namespace "build"

  task :clean do
    CLEAN.each do |filename|
      `rm -rf #{filename}`
    end
  end
end # namespace "android"

    directory 'build'
  
    directory BUILDPATH
    file BUILDPATH => ["android:config", 'build']
    
    directory File.join(BUILDPATH, 'libcurl')
    file File.join(BUILDPATH, 'libcurl') => BUILDPATH

    directory File.join(BUILDPATH, 'libsqlite')
    file File.join(BUILDPATH, 'libsqlite') => BUILDPATH

    def lib_objects(libfile)
      lib = File.basename(libfile).gsub(/\.a$/, "")
      sources = get_sources lib

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


