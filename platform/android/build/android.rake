#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2011 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#------------------------------------------------------------------------

require File.dirname(__FILE__) + '/androidcommon.rb'
require File.dirname(__FILE__) + '/android_tools.rb'
require File.dirname(__FILE__) + '/apk_builder.rb'
require File.dirname(__FILE__) + '/aapt2_helper.rb'
require File.dirname(__FILE__) + '/maven_deps_extractor.rb'
require File.dirname(__FILE__) + '/manifest_generator.rb'
require File.dirname(__FILE__) + '/eclipse_project_generator.rb'
require File.dirname(__FILE__) + '/android_studio_project_generator.rb'
require File.dirname(__FILE__) + '/android_debug.rb'
require File.dirname(__FILE__) + '/../../../lib/build/BuildConfig'

require_relative 'dex_builder'
require_relative 'build_tools_finder'
require_relative 'ndkwrapper'

load File.dirname(__FILE__) + '/android-repack.rake'
require 'pathname'
require 'tempfile'
require 'zip'

include FileUtils

USE_OWN_STLPORT = false
$rhodes_as_lib = false

def get_market_version(apilevel)
  AndroidTools.get_market_version(apilevel)
end

def get_api_level(version)
  AndroidTools.get_api_level(version)
end

JAVA_PACKAGE_NAME = 'com.rhomobile.rhodes'

# Here is place were android platform should be specified.
# For complete list of android API levels and its mapping to
# market names (such as "Android-1.5" etc) see output of
# command "android list targets"
ANDROID_MIN_SDK_LEVEL = 26 #21 is the minimum API that supports arm64
ANDROID_SDK_LEVEL = 32

ANDROID_PERMISSIONS = {
    'audio' => ['RECORD_AUDIO', 'MODIFY_AUDIO_SETTINGS'],
    'camera' => 'CAMERA',
    'gps' => ['ACCESS_FINE_LOCATION', 'ACCESS_COARSE_LOCATION'],
    'network_state' => 'ACCESS_NETWORK_STATE',
    'phone' => ['CALL_PHONE', 'READ_PHONE_STATE'],
    'pim' => ['READ_CONTACTS', 'WRITE_CONTACTS', 'GET_ACCOUNTS'],
    'record_audio' => 'RECORD_AUDIO',
    'vibrate' => 'VIBRATE',
    'bluetooth' => ['BLUETOOTH_ADMIN', 'BLUETOOTH'],
    'calendar' => ['READ_CALENDAR', 'WRITE_CALENDAR'],
    'sdcard' => 'WRITE_EXTERNAL_STORAGE',
    'read_sdcard' => 'READ_EXTERNAL_STORAGE',
    'push' => nil,
    'shared_runtime' => nil,
    'hardware_acceleration' => nil,
    'install_packages' => 'REQUEST_INSTALL_PACKAGES'
}

ANDROID_CAPS_ALWAYS_ENABLED = ['network_state']

def set_app_icon_android

  #if $rhodes_as_lib 
  #  puts "Skip building copy resources (not needed for aar library)"
  #  return
  #end

  res_path = File.join($app_path, 'resources', 'android', 'res')
  if File.exists? res_path
    # NEW resource - just copy it to res folder -->>
    puts "add resources from application'sresource folder [#{res_path}] to [#{$tmpdir}]"
    cp_r res_path, $tmpdir

  else
    # NEW resources not found - use old mechanism

    iconappbase = File.join $app_path, 'icon', 'icon'

    {'drawable' => '',
     'drawable-ldpi' => '36',
     'drawable-mdpi' => '48',
     'drawable-hdpi' => '72',
     'drawable-xhdpi' => '96',
     'drawable-xxhdpi' => '144',
     'drawable-xxxhdpi' => '192',
     'mipmap' => '',
     'mipmap-ldpi' => '36',
     'mipmap-mdpi' => '48',
     'mipmap-hdpi' => '72',
     'mipmap-xhdpi' => '96',
     'mipmap-xxhdpi' => '144',
     'mipmap-xxxhdpi' => '192'
    }.each do |folder, size|
      drawable = File.join $appres, folder
      iconresname = File.join(drawable, "icon.png")

      iconapppath = iconappbase + size + '.png'

      if File.exists?(iconapppath) or File.exists?(iconresname)
        iconapppath = iconappbase + '.png' unless File.exists? iconapppath
        rm_f iconresname
        mkdir_p drawable
        cp iconapppath, iconresname if File.exist? drawable
      end
    end

    {'drawable' => '',
     'drawable-ldpi' => '18',
     'drawable-mdpi' => '24',
     'drawable-hdpi' => '36',
     'drawable-xhdpi' => '48',
     'drawable-xxhdpi' => '72',
     'drawable-xxxhdpi' => '96'
    }.each do |folder, size|
      drawable = File.join $appres, folder
      iconresname = File.join(drawable, "ic_notification.png")

      iconapppath = iconappbase + size + '.png'

      if File.exists?(iconapppath) or File.exists?(iconresname)
        iconapppath = iconappbase + '.png' unless File.exists? iconapppath
        rm_f iconresname
        mkdir_p drawable
        cp iconapppath, iconresname if File.exist? drawable
      end
    end
  end
end

def set_app_name_android(newname)
  puts "set_app_name"
  $stdout.flush

  rm_rf $appres
  cp_r $rhores, $appres

  buildToolsVerMajor = $build_tools_ver.split('.')[0].to_i
  canRenderNetworkSecurityConfig = (($found_api_level.to_i >= 24) && (buildToolsVerMajor>=24))
  rm_rf File.join($appres, 'xml', 'network_security_config.xml') if(!canRenderNetworkSecurityConfig)

  rhostrings = File.join($rhores, "values", "strings.xml")
  appstrings = File.join($appres, "values", "strings.xml")
  doc = REXML::Document.new(File.new(rhostrings))
  doc.elements["resources/string[@name='app_name']"].text = newname
  File.open(appstrings, "w") { |f| doc.write f }
end

def set_app_name_android_post(newname)
  puts "set_app_name_post"

  Dir.glob(File.join($appres, '**', 'strings.xml')).each do |res|
    doc = REXML::Document.new(File.new(res))
    doc.elements["resources/string[@name='app_name']"].text = newname
    rm_rf res
    File.open(res, "w") { |f| doc.write f }
  end
end


def get_boolean(arg)
  arg == 'true' or arg == 'yes' or arg == 'enabled' or arg == 'enable' or arg == '1'
end

namespace 'debug' do
  namespace 'android' do

    debug_port = ""
    device_type = ""

    if !$app_config.nil? && !$app_config["android"].nil?
      debug_port = $app_config["android"]["debug_port"]
      device_type = $app_config["android"]["target_debug"]
    end

    task :gdbserver => ['config:android'] do
      debugger = AndroidDebug.new($app_package_name, $app_path, debug_port, 7777, device_type)
      debugger.StartGdbServer
    end

    task :gdb => ['config:android'] do
      debugger = AndroidDebug.new($app_package_name, $app_path, debug_port, 7777, device_type)
      gdb_path = File.join($androidndkpath, "prebuilt", "windows-x86_64", "bin", "gdb")
      debugger.StartGdb(gdb_path)
    end

    task :jdb => ['config:android'] do
      debugger = AndroidDebug.new($app_package_name, $app_path, debug_port, 7777, device_type)
      debugger.StartJdb
    end

    task :appdebug => ['config:android'] do
      debugger = AndroidDebug.new($app_package_name, $app_path, debug_port, 7777, device_type)
      debugger.StartAppOnDebug
    end

    task :run_and_debug => ['config:android'] do
      gdb_path = File.join($androidndkpath, "prebuilt", "windows-x86_64", "bin", "gdb")
      debugger = AndroidDebug.new($app_package_name, $app_path, debug_port, 7777, device_type)
      debugger.StartAppOnDebug
      debugger.StartGdbServer
      debugger.StartGdb(gdb_path)
    end

    task :attach => ['config:android'] do
      gdb_path = File.join($androidndkpath, "prebuilt", "windows-x86_64", "bin", "gdb")
      debugger = AndroidDebug.new($app_package_name, $app_path, debug_port, 7777, device_type)
      debugger.StartGdbServer
      debugger.StartGdb(gdb_path)
    end

  end
end


namespace 'project' do
  namespace 'android' do
    task :eclipse => ['config:android', 'config:android:extensions','build:android:manifest'] do
      #options = [ 'create', 'project',
      #    '--path', $projectpath,
      #    '--target', $androidtargets[$found_api_level][:id],
      #    '--package', $app_package_name,
      #    '--activity', 'RhodesActivity'
      #]
      #Jake.run($androidbin, options)
      project_template_path = File.join 'res','generators','templates','project','android'
      project_erb_path = File.join project_template_path,'project.erb'
      classpath_erb_path = File.join project_template_path,'classpath.erb'
      project_prop_erb_path = File.join project_template_path,'project.properties.erb'
      manifest_path = File.join $tmpdir,'AndroidManifest.xml'
      project_path = File.join $app_path,'project','android'
      project_file_path = File.join project_path,'.project'
      classpath_file_path = File.join project_path,'.classpath'
      project_prop_file_path = File.join project_path,'project.properties'
      manifest_file_path = File.join project_path,'AndroidManifest.xml'

      rhodes_path = File.absolute_path '.'
      generator = EclipseProjectGenerator.new $appname, $app_path, rhodes_path, $androidtargets[$found_api_level][:name]

      $app_config["extpaths"].each do |extpath|
        next if extpath.start_with? rhodes_path
        generator.addVirtualFolder extpath
      end

      $ext_android_additional_sources.each do |extpath, list|
        classpaths = []
        ext = File.basename(extpath)
        puts "Adding '#{ext}' extension java sources: #{list}"
        File.open(list, "r") do |f|
          while line = f.gets
            line.chomp!
            next if line.empty?

            src = File.join(extpath, line)
            if src =~ /(.*\/src\/).*/
              src = $1
              unless classpaths.index(src)
                puts "Add classpath: #{src}"
                classpaths << src
              end
            end
          end
        end
        generator.addExtension(ext, classpaths) unless classpaths.empty?
      end

      mkdir_p project_path

      project_buf = generator.render project_erb_path
      File.open(project_file_path, "w") { |f| f.write project_buf }

      classpath_buf = generator.render classpath_erb_path
      File.open(classpath_file_path, "w") { |f| f.write classpath_buf }

      project_prop_buf = generator.render project_prop_erb_path
      File.open(project_prop_file_path, "w") { |f| f.write project_prop_buf }

      cp_r File.join(project_template_path,'externalToolBuilders'), File.join(project_path,'.externalToolBuilders') unless File.exists? File.join(project_path,'.externalToolBuilders')
      cp File.join(project_template_path,'gensources.xml'), project_path unless File.exists? File.join(project_path,'gensources.xml')
      cp File.join(project_template_path,'eclipsebundle.xml'), project_path unless File.exists? File.join(project_path,'eclipsebundle.xml')

      cp manifest_path, project_path

    end

    task :studio => ['config:android', 'config:android:extensions','build:android:manifest'] do

      project_template_path = File.join 'res','generators','templates','project','android_studio_project'

      main_gradle_script = File.join( project_template_path, 'build.gradle' )
      gradle_properties = File.join( project_template_path, 'gradle.properties' )
      gradlew = File.join( project_template_path, 'gradlew' )
      gradlew_bat = File.join( project_template_path, 'gradlew.bat' )
      settings_gradle = File.join( project_template_path, 'settings.gradle' )
      app_gradle_template = File.join( project_template_path, 'app', 'build.gradle.erb' )
      project_path = File.join $app_path,'project','android_studio'
      project_app_path = File.join $app_path,'project','android_studio', 'app'

      cmake_template_path = File.join( project_template_path, 'app', 'CMakeLists.txt.erb' )

      cmake_template_ruby = File.join( $shareddir, 'ruby', 'CMakeLists.txt.erb')
      source_template_ruby = File.join('platform', 'android', 'build', 'libruby_build.files')

      cmake_template_curl = File.join( $shareddir, 'curl', 'CMakeLists.txt.erb')
      source_template_curl = File.join('platform', 'android', 'build', 'libcurl_build.files')

      cmake_template_db = File.join( $shareddir, 'db', 'CMakeLists.txt.erb')
      source_template_db = File.join('platform', 'android', 'build', 'librhodb_build.files')

      cmake_template_common = File.join( $shareddir, 'common', 'CMakeLists.txt.erb')
      source_template_common = File.join('platform', 'android', 'build', 'librhocommon_build.files')
      source_template_rhomain = File.join('platform', 'android', 'build', 'librhomain_build.files')

      cmake_template_logging = File.join( $shareddir, 'logging', 'CMakeLists.txt.erb')
      source_template_logging = File.join('platform', 'android', 'build', 'librholog_build.files')

      cmake_template_sync = File.join( $shareddir, 'sync', 'CMakeLists.txt.erb')
      source_template_sync = File.join('platform', 'android', 'build', 'librhosync_build.files')

      cmake_template_json = File.join( $shareddir, 'json.new', 'CMakeLists.txt.erb')
      source_template_json = File.join('platform', 'android', 'build', 'libjson_build.files')

      cmake_template_sqlite = File.join( $shareddir, 'sqlite', 'CMakeLists.txt.erb')
      source_template_sqlite = File.join('platform', 'android', 'build', 'libsqlite_build.files')

      cmake_template_rhodes = File.join('platform', 'android', 'build', 'CMakeLists.txt.erb')
      source_template_rhodes = File.join('platform', 'android', 'build', 'librhodes_build.files')

      $abis = $app_config['android']['abis'] if $app_config["android"]
      $abis = ['arm'] unless $abis
      abi = $abis[0]

      if abi == 'arm'
        realabi = 'armeabi'
        studio_realabi = 'armeabi-v7a'
      else
        realabi = abi
        studio_realabi = abi
      end

      external_string = ''
      ext_libs = ''
      extlibs = Dir.glob(File.join($app_builddir, 'extensions', '**', realabi, 'lib*.a'))
      extlibs.each do |lib|

        extname = File.basename(lib).gsub('lib', '').gsub('.a', '')
        #TODO support ${ANDROID_ABI} in cmake template
        if extname == "openssl.so"
           external_string += ("add_library(#{extname} STATIC IMPORTED GLOBAL)\n" +
              "set_property(TARGET #{extname} PROPERTY IMPORTED_LOCATION #{lib})\n")
        end

        if extname == "coreapi"
          next
        end
        ext_libs += ("#{extname}\n")
      end
      ext_libs += 'coreapi'

      extensions_deps = ''
      $ext_android_build_scripts.each do |ext, builddata|
        ext_native_files = File.join(builddata[2], 'ext', 'platform', 'android', 'ext_native.files')
        ext_native_files2 = File.join(builddata[0], 'ext_native.files')
        ext_native_final = (File.exists? ext_native_files) ? ext_native_files : ext_native_files2
        if(File.exists? ext_native_final)
          mkdir_p File.join(project_app_path, ext)        
          extensions_deps += "add_subdirectory(./#{ext})\n"
        end
      end

      rhodes_path = File.absolute_path '.'

      generator = AndroidStudioProjectGenerator.new
      generator.rhoRoot = rhodes_path
      generator.buildToolsVersion = $build_tools_ver
      generator.applicationId = $app_package_name
      generator.targetSdkVersion = $found_api_level
      generator.minSdkVersion = $found_api_level
      generator.appincdir = $appincdir
      generator.buildMode = $debug ? 'Debug' : 'Release'
      generator.externalDeps = external_string
      generator.extLibs = ext_libs
      generator.targetArch = studio_realabi
      generator.extensionsDeps = extensions_deps

      generator.compileSdkVersion = $found_api_level
      generator.versionName = $app_config["version"]

      mkdir_p File.join(project_path,'app')
      mkdir_p File.join(project_app_path,'curl')
      mkdir_p File.join(project_app_path,'rhodb')
      mkdir_p File.join(project_app_path,'rhocommon')
      mkdir_p File.join(project_app_path,'rhosync')
      mkdir_p File.join(project_app_path,'sqlite')
      mkdir_p File.join(project_app_path,'ruby')
      mkdir_p File.join(project_app_path,'json')
      mkdir_p File.join(project_app_path,'rholog')
      mkdir_p File.join(project_app_path,'rhodes')

      app_gradle_path = File.join( project_path, 'app', 'build.gradle')
      cmake_path_main = File.join( project_path, 'app', 'CMakeLists.txt')
      cmake_path_ruby = File.join( project_path, 'app', 'ruby', 'CMakeLists.txt')
      cmake_path_curl = File.join( project_path, 'app', 'curl', 'CMakeLists.txt')
      cmake_path_rhodb = File.join( project_path, 'app', 'rhodb', 'CMakeLists.txt')
      cmake_path_rhocommon = File.join( project_path, 'app', 'rhocommon', 'CMakeLists.txt')
      cmake_path_rhosync = File.join( project_path, 'app', 'rhosync', 'CMakeLists.txt')
      cmake_path_sqlite = File.join( project_path, 'app', 'sqlite', 'CMakeLists.txt')
      cmake_path_json = File.join( project_path, 'app', 'json', 'CMakeLists.txt')
      cmake_path_rholog = File.join( project_path, 'app', 'rholog', 'CMakeLists.txt')
      cmake_path_rhodes = File.join( project_path, 'app', 'rhodes', 'CMakeLists.txt')

      File.open( cmake_path_main, 'w' ) { |f| f.write generator.render_app_gradle( cmake_template_path ) }
      File.open( cmake_path_ruby, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_ruby, source_template_ruby ) }
      File.open( cmake_path_curl, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_curl, source_template_curl ) }
      File.open( cmake_path_rhodb, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_db, source_template_db ) }
      File.open( cmake_path_rhocommon, 'w' ) { |f| f.write generator.render_app_gradle_ex2( cmake_template_common, source_template_common, source_template_rhomain ) }
      File.open( cmake_path_rhosync, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_sync, source_template_sync ) }
      File.open( cmake_path_sqlite, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_sqlite, source_template_sqlite ) }
      File.open( cmake_path_json, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_json, source_template_json ) }
      File.open( cmake_path_rholog, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_logging, source_template_logging ) }
      File.open( cmake_path_rhodes, 'w' ) { |f| f.write generator.render_app_gradle_ex( cmake_template_rhodes,  source_template_rhodes) }

      cmake_template_extension = File.join('platform', 'android', 'build', 'CMakeListsExts.txt.erb')
      $ext_android_build_scripts.each do |ext, builddata|
        ext_native_files = File.join(builddata[2], 'ext', 'platform', 'android', 'ext_native.files')
        ext_native_files2 = File.join(builddata[0], 'ext_native.files')
        ext_native_final = (File.exists? ext_native_files) ? ext_native_files : ext_native_files2
        if(File.exists? ext_native_final)
          cmake_path_ext = File.join( project_path, 'app', ext, 'CMakeLists.txt')
          generator.extdir = builddata[2]
          generator.extName = ext
          File.open( cmake_path_ext, 'w' ) { |f| f.write generator.render_cmake_extension( cmake_template_extension, ext_native_final ) }
        end
      end

      $ext_android_additional_sources.each do |extpath, list|
        
      end
      File.open( app_gradle_path, 'w' ) { |f| f.write generator.render_app_gradle( app_gradle_template ) }


      cp main_gradle_script,  project_path
      cp gradle_properties,   project_path
      cp gradlew,             project_path
      cp gradlew_bat,         project_path
      cp settings_gradle,     project_path
    end
  end
end

# Finds file in a given locations
# file_name should contain required file
# path_array could be
# - String with location,
# - Array of strings of locations,
# - Array of arrays of directories - will be joined using File.join.

def find_file(file_name, path_array)
  result = nil
  lookup = []

  unless path_array.empty?
    search_paths = path_array

    if path_array.first.kind_of? String
      search_paths = [path_array]
    end

    search_paths.each do |elem|
      full_path = (elem.kind_of?(String)) ? elem : File.join(elem)

      files = Dir.glob(File.join(full_path, file_name))

      unless files.empty?
        result = files.sort.last
        break
      else
        lookup << full_path
      end
    end
  end

  if result.nil?
    fail "Could not find file #{file_name} at #{lookup.join(', ')}"
  end

  result
end

def setup_ext_env( extpath, extname )
  env = {}
  env['RHO_PLATFORM'] = 'android'
  env["RHO_APP_DIR"] = $app_path
  env["ANDROID_SDK"] = $androidsdkpath
  env["ANDROID_NDK"] = $androidndkpath
  env["ANDROID_API_LEVEL"] = $min_sdk_level.to_s
  env["RHO_ROOT"] = $startdir
  env["BUILD_DIR"] ||= $startdir + "/platform/android/build"
  env["RHO_INC"] = $appincdir
  env["RHO_RES"] = $appres
  env["RHO_ANDROID_TMP_DIR"] = $tmpdir
  env["RHO_DEBUG"] = $debug.to_s
  env['SOURCEPATH'] = extpath
  sourcelist = Dir.glob(File.join(extpath,'**','android','ext_native.files'))
  env['SOURCELIST'] = sourcelist.size == 1 ? sourcelist.first : File.join(extpath,'ext_native.files')
  env["TARGET_TEMP_DIR"] = File.join($app_builddir, 'extensions', extname)
  env['TARGETPATH'] = File.join($app_builddir, 'extensions', extname)
  env['TARGETLIB'] = "lib#{extname}.a"
  env['TEMP_FILES_DIR'] = File.join($tmpdir, extname)

  env
end

def get_android_major_minor(version)
  major, minor, patch = version.split('.').map { |z| z.to_i }
  major = 0 if major.nil?
  minor = 0 if minor.nil?
  patch = 0 if patch.nil?

  $logger.debug( "Splitting version #{version}, result is: #{major}.#{minor}.#{patch}" )

  return major,minor,patch
end

def get_case_insensetive_property(property)
  $app_config["android"].each do |x|
    if (x[0].downcase == property.downcase)
      return x[1]
    end
  end
  return nil
end

def select_aapt2(build_tools_path)

  aapt2 = nil

  buildToolsVerMajor = $build_tools_ver&.split('.')[0].to_i
  if ( buildToolsVerMajor >= 34 )
    aapt2 = File.join(build_tools_path,'aapt2'+$exe_ext)
  else
    platform_dir = ''
    
    if OS.windows?
      platform_dir = 'windows'
    elsif OS.mac?
      platform_dir = 'osx'
    elsif OS.linux?
      platform_dir = 'linux'
    else
      raise "unknown host OS"
    end  

    aapt2 = File.join($startdir, "res", "build-tools", "aapt2", platform_dir , 'aapt2'+$exe_ext)
  end

  $logger.debug( "AAPT2 path is: #{aapt2}")
  return aapt2
end


def init_aapt2_helper( build_tools_path )
  helper = Aapt2Helper.instance

  helper.logger = $logger
  helper.res_dir = $appres
  helper.output_path = $targetdir
  helper.dex_path = File.join( $bindir, 'classes.dex' )
  helper.apk_path = File.join( $bindir, 'rhodes.ap_')
  #builder.res_dirs << AndroidTools::MavenDepsExtractor.instance.res_dirs

  helper.build_dir = $tmpdir
  helper.androidjar = $androidjar
  helper.manifest = $appmanifest

  helper.aapt2 = select_aapt2(build_tools_path)
  helper.bundletool = $bundletool
  helper.javabin = File.join( $java, 'java' + $exe_ext )
  helper.rjava_dir = $app_rjava_dir
  helper.maven_deps = AndroidTools::MavenDepsExtractor.instance
  helper.assets_dir = $appassets

  helper.no_compress_exts = $no_compression

  helper.init
end

namespace "config" do

  namespace "android" do
    task :enable_custom_manifest => 'config:load' do
      
      unless $app_config['android']&.[]('manifest_template')

        default_manifest_file = File.join $startdir, 'res', 'generators', 'templates', 'application', 'AndroidManifest.erb'
        
        custom_manifest_file = File.join($app_path,'AndroidManifest.erb')
        
        cp default_manifest_file, custom_manifest_file

        $app_config['android']['manifest_template'] = 'AndroidManifest.erb'

        buildyml = File.join( $app_path, 'build.yml' )
        cp buildyml,buildyml+'.bak'

        File.open(buildyml, 'w') {|f| f.write $app_config.to_yaml }

      end
    end
  end

  task :set_android_platform do
    $current_platform = "android"
  end

  task :android => :set_android_platform do
    print_timestamp('config:android START')

    Rake::Task["config:common"].invoke

    $java = $config["env"]["paths"]["java"]

    $skip_so_build = false
    $neon_root = nil
    $neon_root = $config["env"]["paths"]["neon"] unless $config["env"]["paths"].nil?
    if !($app_config["paths"].nil? or $app_config["paths"]["neon"].nil?)
      $neon_root = $app_config["paths"]["neon"]
    end

    if !$skip_checking_Android_SDK
      $androidsdkpath = $config["env"]["paths"]["android"]
      unless File.exists? $androidsdkpath
        puts "Missing or invalid 'android' section in rhobuild.yml: '#{$androidsdkpath}'"
        exit 1
      end

      $androidndkpath = $config["env"]["paths"]["android-ndk"]
      unless File.exists? $androidndkpath
        puts "Missing or invalid 'android-ndk' section in rhobuild.yml: '#{$androidndkpath}'"
        exit 1
      end

      errfmt = "WARNING!!! Path to Android %s contain spaces! It will not work because of the Google toolchain restrictions. Move it to another location and reconfigure rhodes."
      if $androidndkpath =~ /\s/
        puts(errfmt % "NDK")
        exit 1
      end
    end

    $min_sdk_level = get_case_insensetive_property("minSDK") unless $app_config["android"].nil?
    $min_sdk_level = get_case_insensetive_property("minSDK") if $min_sdk_level.nil? and not $config["android"].nil?
    $min_sdk_level = get_case_insensetive_property("minSdkVer") if $min_sdk_level.nil?
    $min_sdk_level = $min_sdk_level.to_i unless $min_sdk_level.nil?
    $min_sdk_level = ANDROID_MIN_SDK_LEVEL if $min_sdk_level.nil?

    $build_bundle = false
    $build_bundle = get_case_insensetive_property("build_bundle").to_i == 1 unless $app_config["android"].nil?
    
    $target_sdk_level = get_case_insensetive_property("targetSdk") unless $app_config["android"].nil?
    $target_sdk_level = get_case_insensetive_property("targetSdk") if $target_sdk_level.nil? and not $config["android"].nil?
    $target_sdk_level = get_case_insensetive_property("targetSdkVer") if $target_sdk_level.nil?
    $target_sdk_level = $target_sdk_level.to_i unless $target_sdk_level.nil?

    $target_sdk_level = ANDROID_SDK_LEVEL if $target_sdk_level.nil?

    $max_sdk_level = $app_config["android"]["maxSDK"] unless $app_config["android"].nil?

    if !$skip_checking_Android_SDK
      $androidplatform = AndroidTools.fill_api_levels $androidsdkpath
      if $androidplatform == nil
        puts "No Android platform found at SDK path: '#{$androidsdkpath}'"
        exit 1
      end

      android_api_levels = AndroidTools.get_installed_api_levels
      android_api_levels.sort!

      puts "Installed versions: "
      puts AndroidTools.get_installed_market_versions
      
      $found_api_level = android_api_levels.last
      $major_version = 0
      $minor_version = 0
      $patch_version = 0

      #If user has mentioned version under android, then select that particular api level.
      if $app_config["android"]["version"]
        apilevel = AndroidTools.get_api_level $app_config["android"]["version"]
        version = $app_config["android"]["version"]
        $major_version, $minor_version, $patch_version = get_android_major_minor(version)

        market_version = AndroidTools.get_market_version($target_sdk_level)
        if market_version.nil?
          apilevel = android_api_levels.last
        else
          apilevel = $target_sdk_level
        end

        if(!apilevel)
          apilevel = AndroidTools.get_api_level ($app_config["android"]["version"] + ".0")
        end
        if(!apilevel)
          apilevel = AndroidTools.get_api_level ($app_config["android"]["version"][0...-2])
        end
        if(apilevel)
          $androidplatform = AndroidTools.get_platform apilevel
          $found_api_level = apilevel
          puts "Found api level #{apilevel}"
        else
          puts "No Android platform found of version #{$app_config['android']['version']}. Picking the latest one Android #{AndroidTools.get_market_version $found_api_level} available in machine"
        end
      else
        $major_version, $minor_version, $patch_version = 
          get_android_major_minor( AndroidTools.get_market_version($found_api_level) )
      end    
    end

    $logger.info("Set up build API Level: #{$found_api_level}, version: #{$major_version}.#{$minor_version}")

    $gapikey = $app_config["android"]["apikey"] unless $app_config["android"].nil?
    $gapikey = $config["android"]["apikey"] if $gapikey.nil? and not $config["android"].nil?
    $gapikey = '' unless $gapikey.is_a? String
    $gapikey = nil if $gapikey.empty?

    $android_orientation = $app_config["android"]["orientation"] unless $app_config["android"].nil?

    use_geomapping = $app_config["android"]["mapping"] unless $app_config["android"].nil?
    use_geomapping = $config["android"]["mapping"] if use_geomapping.nil? and not $config["android"].nil?
    use_geomapping = 'false' if use_geomapping.nil?
    use_geomapping = get_boolean(use_geomapping.to_s)

    $use_google_addon_api = false
    $use_google_addon_api = true if use_geomapping

    #Additionally $use_google_addon_api set to true if PUSH capability is enabled

    $config_xml = $app_config["android"]["rhoelements"]["config"] if $app_config["android"]["rhoelements"] if $app_config["android"]
    if $config_xml
      $config_xml = File.expand_path $config_xml, $app_path
      puts "Custom config.xml path: #{$config_xml}"
    end

    $logger.debug "Use Google addon API (1): #{$use_google_addon_api}"    

    $uri_scheme = $app_config["android"]["URIScheme"] unless $app_config["android"].nil?
    $uri_scheme = "http" if $uri_scheme.nil?

    $uri_host = $app_config["android"]["URIHost"] unless $app_config["android"].nil?

    $androidpath = Jake.get_absolute $config["build"]["androidpath"]
    $bindir = File.join($app_path, "bin")
    $rhobindir = File.join($androidpath, "bin")
    $builddir = File.join($androidpath, "build")
    $shareddir = File.join($androidpath, "..", "shared")
    $coreapidir = File.join($androidpath, "..", "..", "lib", "commonAPI", "coreapi", "ext", "shared")
    $commonapidir = File.join($androidpath, "..", "..", "lib", "commonAPI")
    $targetdir = File.join($bindir, 'target', 'android')
    $projectpath = File.join($app_path, 'project', 'android')
    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
    $tmpdir = File.join($bindir, "tmp")
    $srcdir = File.join $tmpdir,'assets' #File.join($bindir, "RhoBundle")

    $rhomanifesterb = File.join($app_path, $app_config['android']['manifest_template']) unless $app_config["android"].nil? || $app_config['android']['manifest_template'].nil?
    #$rhomanifesterb = File.join $androidpath, "Rhodes", "AndroidManifest.xml.erb" if $rhomanifesterb.nil?
    $rhomanifesterb = File.join $startdir, 'res', 'generators', 'templates', 'application', 'AndroidManifest.erb' if $rhomanifesterb.nil?

    $appmanifest = File.join $tmpdir, "AndroidManifest.xml"

    $rhores = File.join $androidpath, 'Rhodes','res'
    $appres = File.join $tmpdir,'res'
    $appres_flats = File.join $tmpdir,'flats' #do we really need a global var for that?
    $appassets = $srcdir
    $applibs = File.join $tmpdir,'lib'

    $appincdir = File.join $tmpdir, "include"

    $rho_java_gen_dir = File.join $tmpdir,'gen','com','rhomobile','rhodes'

    $app_rjava_dir = $rho_java_gen_dir
    $app_native_libs_java = File.join $rho_java_gen_dir,'NativeLibraries.java'
    $app_capabilities_java = File.join $rho_java_gen_dir,'Capabilities.java'
    $app_push_java = File.join $rho_java_gen_dir,'Push.java'
    $app_startup_listeners_java = File.join $rho_java_gen_dir,'extmanager','RhodesStartupListeners.java'

    if RUBY_PLATFORM =~ /(win|w)32$/
      $bat_ext = ".bat"
      $exe_ext = ".exe"
      $path_separator = ";"

      # Add PATH to cygwin1.dll
      ENV['CYGWIN'] = 'nodosfilewarning'
      if $path_cygwin_modified.nil?
        ENV['PATH'] = Jake.get_absolute("res/build-tools") + ";" + ENV['PATH']
        path_cygwin_modified = true
      end
    else
      #XXX make these absolute
      $bat_ext = ""
      $exe_ext = ""
      $path_separator = ":"
      # TODO: add ruby executable for Linux
    end

    build_tools_ver = nil

    if !$skip_checking_Android_SDK

      AndroidTools::MavenDepsExtractor.instance.set_logger($logger)
      AndroidTools::MavenDepsExtractor.instance.set_temp_dir($tmpdir)
      AndroidTools::MavenDepsExtractor.instance.set_java_home($java)

      toolsver = $app_config['android']['buildtools'] if ($app_config['android'] and $app_config['android']['buildtools'])
      build_tools_path = AndroidTools::findBuildToolsPath(toolsver,$androidsdkpath,$androidplatform)

      $logger.info "Detected Android build tools at #{build_tools_path}"

      $aapt2 = File.join(build_tools_path, "aapt2#{$exe_ext}")

      $aapt = File.join(build_tools_path, "aapt#{$exe_ext}")
      $aapt = File.join($androidsdkpath, "platform-tools", "aapt" + $exe_ext) unless File.exists? $aapt

      AndroidTools::DexBuilder.instance.logger = $logger
      AndroidTools::DexBuilder.instance.sdk_path = $androidsdkpath
      AndroidTools::DexBuilder.instance.build_tools_path = build_tools_path
      AndroidTools::DexBuilder.instance.androidplatform = $androidplatform
      AndroidTools::DexBuilder.instance.javabin = File.join($java, 'java'+$exe_ext)

      $bundletool = File.join($startdir, "res", "build-tools", "bundletool.jar")

      $androidbin = File.join($androidsdkpath, "tools", "android" + $bat_ext)
      $adb = find_file( "adb" + $exe_ext,
                        [
                            [$androidsdkpath, "tools"],
                            [$androidsdkpath, "platform-tools"]
                        ]
      )

      $zipalign = find_file( "zipalign" + $exe_ext,
                             [
                                 [$androidsdkpath, "tools"],
                                 [build_tools_path],
                                 [$androidsdkpath,'build-tools','*']
                             ]
      )

      $androidjar = File.join($androidsdkpath, "platforms", $androidplatform, "android.jar")

      $keytool = File.join($java, "keytool" + $exe_ext)
      $jarsigner = File.join($java, "jarsigner" + $exe_ext)
      $jarbin = File.join($java, "jar" + $exe_ext)
      $javabin = File.join( $java, 'java' + $exe_ext )

      $keystore = nil
      $keystore = $app_config["android"]["production"]["certificate"] if !$app_config["android"].nil? and !$app_config["android"]["production"].nil?
      $keystore = $config["android"]["production"]["certificate"] if $keystore.nil? and !$config["android"].nil? and !$config["android"]["production"].nil?
      $keystore = File.expand_path($keystore, $app_path) unless $keystore.nil?
      $keystore = File.expand_path(File.join(ENV['HOME'], ".rhomobile", "keystore")) if $keystore.nil?

      $storepass = nil
      $storepass = $app_config["android"]["production"]["password"] if !$app_config["android"].nil? and !$app_config["android"]["production"].nil?
      $storepass = $config["android"]["production"]["password"] if $storepass.nil? and !$config["android"].nil? and !$config["android"]["production"].nil?
      $storepass = "81719ef3a881469d96debda3112854eb" if $storepass.nil?
      $keypass = $storepass

      $storealias = nil
      $storealias = $app_config["android"]["production"]["alias"] if !$app_config["android"].nil? and !$app_config["android"]["production"].nil?
      $storealias = $config["android"]["production"]["alias"] if $storealias.nil? and !$config["android"].nil? and !$config["android"]["production"].nil?
      $storealias = "rhomobile.keystore" if $storealias.nil?

      $build_tools_ver = File.split( build_tools_path )[1]

      AndroidTools.logger = $logger
      AndroidTools.jarsigner = $jarsigner
      AndroidTools.zipalign = $zipalign
      AndroidTools.keytool = $keytool
      AndroidTools.jarbin = $jarbin
      AndroidTools.apksigner = File.join(build_tools_path,'apksigner'+$bat_ext)
    end

    $app_config["capabilities"] += ANDROID_CAPS_ALWAYS_ENABLED
    $app_config["capabilities"].map! { |cap| cap.is_a?(String) ? cap : nil }.delete_if { |cap| cap.nil? }
    $use_google_addon_api = true unless $app_config["capabilities"].index("push").nil?

    $logger.debug "Use Google addon API (2): #{$use_google_addon_api}"    

    $appname = $app_config["name"]
    $appname = "Rhodes" if $appname.nil?
    $vendor = $app_config["vendor"]
    $vendor = 'rhomobile' unless $vendor
    $vendor = $vendor.gsub(/^[^A-Za-z]/, '_').gsub(/[^A-Za-z0-9]/, '_').gsub(/_+/, '_').downcase
    $app_package_name = $app_config["android"] ? $app_config["android"]["package_name"] : nil
    $app_package_name = "com.#{$vendor}." + $appname.downcase.gsub(/[^A-Za-z_0-9]/, '') unless $app_package_name
    $app_package_name.gsub!(/\.[\d]/, "._")

    puts "$vendor = #{$vendor}"
    puts "$app_package_name = #{$app_package_name}"

    if $uri_host.nil?
      $uri_host = 'rhomobile.com'
      $uri_path_prefix = "/#{$app_package_name}"
    end

    $no_compression = ['html','htm','js','css']
    $no_compression = $app_config['android']['no_compression'] if $app_config['android'] and $app_config['android']['no_compression']

    $applog_path = nil
    $applog_file = $app_config["applog"]
    $applog_file = "rholog.txt" if $applog_file.nil?

    if !$applog_file.nil?
      $applog_path = File.join($app_path, $applog_file)
    end

    if !$skip_checking_Android_SDK
      if $min_sdk_level > $found_api_level
        raise "Latest installed Android platform '#{$androidplatform}' does not meet minSdk '#{$min_sdk_level}' requirement"
      end
    end

    if !$skip_checking_Android_SDK
      # Detect Google API add-on path
      if $use_google_addon_api
        $logger.debug "Looking for Google API SDK add-on..."
        $google_classpath = AndroidTools::get_addon_classpath('Google APIs', $found_api_level)
      end

      #setup_ndk($androidndkpath, $found_api_level, 'arm')
      $abis = $app_config['android']['abis'] if $app_config["android"]
      $abis = ['arm'] unless $abis

      #build config
      core_build_cfg = {}

      begin
        core_build_cfg = YAML.load_file(File.join( $builddir, 'config.yml'))
      rescue Exception => e
        $logger.warn "Can't read core build config: #{e.inspect}"
      end

      if core_build_cfg['maven_deps']
        core_build_cfg['maven_deps'].each { |d|
          AndroidTools::MavenDepsExtractor.instance.add_dependency( d )      
        }
      end

      $java_version = 7
      if ($app_config["android"] && $app_config["android"]["java_version"])
        $java_version = $app_config["android"]["java_version"]
      end
    end
    
    unless $debug
      $confdir = "release"
    else
      $confdir = "debug"
    end

    $app_builddir = File.join($bindir, 'target', 'android', $confdir)

    $push_sender = nil
    $push_sender = $config["android"]["push"]["sender"] if !$config["android"].nil? and !$config["android"]["push"].nil?
    $push_sender = $app_config["android"]["push"]["sender"] if !$app_config["android"].nil? and !$app_config["android"]["push"].nil?
    $push_sender = "support@rhomobile.com" if $push_sender.nil?

    $push_notifications = nil
    $push_notifications = $app_config["android"]["push"]["notifications"] if !$app_config["android"].nil? and !$app_config["android"]["push"].nil?
    $push_notifications = "none" if $push_notifications.nil?
    $push_notifications = $push_notifications

    if !$skip_checking_Android_SDK
      # Detect android targets
      $androidtargets = {}
      id = nil
      apilevel = nil
      target_name = nil

      $logger.debug "Parsing 'list target' output"

      `"#{$androidbin}" list target`.split(/\n/).each do |line|
        line.chomp!

        $logger.debug "parsing line: #{line}"

        if line =~ /^id:\s+([0-9]+)\s+or\s+\"(.*)\"/
          id = $1
          target_name = $2

          $logger.debug "found target. ID: #{id}; name: #{target_name}"

          if $use_google_addon_api
            $logger.debug "checking if target is applcable for Google API"
            if line =~ /Google Inc\.:Google APIs:([0-9]+)/
              apilevel = $1.to_i
              $androidtargets[apilevel] = {:id => id.to_i, :name => target_name}
              $logger.debug "adding target #{id} for Google API"
            end
          end
        end

        unless $use_google_addon_api
          $logger.debug "checking if target is applcable for non-Google API"
          if line =~ /^\s+API\s+level:\s+([0-9]+)$/
            apilevel = $1.to_i
            $androidtargets[apilevel] = {:id => id.to_i, :name => target_name}
            $logger.debug "adding target #{id} for non-Google API"
          end
        end

        if apilevel && $androidtargets[apilevel][:id] == id.to_i
          if line =~ /^\s+(Tag\/)?ABIs\s*:\s+(.*)/
            $androidtargets[apilevel][:abis] = []
            $2.split(/,\s*/).each do |abi|
              $androidtargets[apilevel][:abis] << abi
            end
            $logger.debug "added target abis: #{pp $androidtargets[apilevel][:abis].inspect}"
          end
        end
      end

      $logger.debug "Android targets:\n#{pp $androidtargets.inspect}"

    end

    mkdir_p $bindir if not File.exists? $bindir
    mkdir_p $rhobindir if not File.exists? $rhobindir
    mkdir_p $targetdir if not File.exists? $targetdir
    mkdir_p $srcdir if not File.exists? $srcdir

    init_aapt2_helper build_tools_path

    print_timestamp('config:android FINISH')

  end #task 'config:android'

  namespace 'android' do

    # 'config:android:app_config' task is invoked directly by common Rakefile
    # just after build config has been read and before processing extensions
    task :app_config do
      if $app_config['capabilities'].index('push')
        $app_config['extensions'] << 'gcm-push' unless $app_config['extensions'].index('gcm-push') || $app_config['extensions'].index('fcm-push')
      end

      if !$app_config['android'].nil? && !$app_config['android']['abis'].nil? && ($app_config['android']['abis'].index('x86') || $app_config['android']['abis'].index('mips'))
        $app_config['extensions'].delete('rhoelements')
      end

      $file_map_name = "rho.dat"
    end

    task :extensions => ['config:android', 'build:bundle:noxruby'] do

      print_timestamp('android:extensions START')

      $ext_android_rhodes_activity_listener = []
      $ext_android_additional_sources = {}
      $ext_android_extra_classpath = {}
      $ext_android_additional_lib = []
      $ext_android_build_scripts = {}
      $ext_android_manifest_changes = {}
      $ext_android_adds = {}
      $ext_android_library_deps = {}
      $ext_android_platform_lib = []

      preload_java_classes = []

      $app_extensions_list.each do |ext, extpath|
        next if extpath.nil?

        # com.android.ndk.thirdparty:openssl:1.1.1 openssl.so and openssl.so - obsolete
        if ext == 'openssl.so'
          next
        end

        puts "#{extpath} is configuring..."
        extyml = File.join(extpath, "ext.yml")
        if File.file? extyml
          puts "#{extyml} is processing..."

          print_timestamp(' extension "'+ext+'" processing START')


          extconf = Jake.config(File.open(extyml))
          extconf_android = extconf['android']
          exttype = 'build'
          exttype = extconf_android['exttype'] if extconf_android and extconf_android['exttype']
          targetpath = File.join $app_builddir, 'extensions', ext
          addspath = File.join targetpath,'adds'
          prebuiltpath = nil
          if exttype == 'prebuilt'
            prebuiltpath = Dir.glob(File.join(extpath,'ext','**', 'android'))
            if prebuiltpath.count == 1
              prebuiltpath = prebuiltpath.first
            else
              raise "android:exttype is 'prebuilt' but prebuilt path is not found #{prebuiltpath.inspect}"
            end
          elsif extconf_android['prebuilts']
            prebuiltpath = File.join( extpath, extconf_android['prebuilts'] )            
          end

          android_listener = extconf["android_rhodes_activity_listener"]
          android_listener = extconf_android['rhodes_listener'] if android_listener.nil? and extconf_android
          $ext_android_rhodes_activity_listener << android_listener unless android_listener.nil?

          manifest_changes = extconf["android_manifest_changes"]
          manifest_changes = extconf_android['manifest_changes'] if manifest_changes.nil? and extconf_android

          prebuild_rake_task = extconf_android['prebuild_rake_task'] if extconf_android

          if manifest_changes
            manifest_changes = [manifest_changes] unless manifest_changes.is_a? Array
            manifest_changes.map! { |path| File.join(extpath,path) }
          else
            if prebuiltpath
              manifest_changes = []

              path = File.join(prebuiltpath, 'adds', 'AndroidManifest.rb')
              manifest_changes << path if File.file? path

              templates = Dir.glob File.join(prebuiltpath, 'adds', '*.erb')
              manifest_changes += templates

              if templates.empty?
                path = File.join(prebuiltpath, 'adds', 'AndroidManifest.xml')
                manifest_changes << path if File.file? path
              end
            end
          end
          if manifest_changes
            $ext_android_manifest_changes[ext] = manifest_changes
          end

          resource_addons = extconf["android_resources_addons"]
          resource_addons = extconf_android['adds'] if resource_addons.nil? and extconf_android
          if resource_addons
            resource_addons = File.join(extpath, resource_addons)
          else
            if prebuiltpath
              resource_addons = File.join(prebuiltpath, 'adds')
              resource_addons = nil unless File.directory? resource_addons
            end
          end

          if resource_addons
            $ext_android_adds[ext] = resource_addons
          end

          library_deps = extconf_android['library_deps'] if extconf_android
          if library_deps
            if library_deps.is_a? Array
              library_deps.each do |dep|
                deppath = File.join($androidsdkpath, dep)
                $ext_android_library_deps[AndroidTools.read_manifest_package(deppath)] = deppath
              end
            end
          end

          maven_deps = extconf_android['maven_deps'] if extconf_android
          if maven_deps
            if maven_deps.is_a? Array
              maven_deps.each do |dep|
                AndroidTools::MavenDepsExtractor.instance.add_dependency( ext, dep )
              end
            end
          end

          resource_packages = extconf_android['resource_packages'] if extconf_android
          if resource_packages
            if resource_packages.is_a? Array
              resource_packages.each do |package|
                $ext_android_library_deps[package] = ""
              end
            end
          end

          additional_sources = extconf["android_additional_sources_list"]
          additional_sources = extconf_android['source_list'] if additional_sources.nil? and extconf_android
          unless additional_sources.nil?
            ext_sources_list = File.join(extpath, additional_sources)

            if File.exists? ext_sources_list
              $ext_android_additional_sources[extpath] = ext_sources_list
            else
              raise "Extension java source list is missed: #{ext_sources_list}"
            end
          else
            puts "No additional java sources for '#{ext}'"
          end

          sdk_addons = extconf_android['sdk_addons'] if extconf_android
          unless sdk_addons.nil?
            ext_classpath = nil
            sdk_addons.each do |addon_pat|
              if ext_classpath
                ext_classpath += $path_separator + AndroidTools::get_addon_classpath(addon_pat)
              else
                ext_classpath = AndroidTools::get_addon_classpath(addon_pat)
              end
            end
            $ext_android_extra_classpath[ext] = ext_classpath
          end

          # there is no 'additional_libs' param in android section moreover
          # place libraries into android adds folder
          android_additional_lib = extconf["android_additional_lib"]
          if android_additional_lib != nil
            android_additional_lib.each do |lib|
              $ext_android_additional_lib << File.join(extpath, lib)
            end
          end

          android_platform_libs = nil
          android_platform_libs = extconf_android['platform_libraries'] if extconf_android
          if android_platform_libs
            android_platform_libs.each { |lib| $ext_android_platform_lib << lib }
          end

          if prebuiltpath
            mkdir_p targetpath
            Dir.glob(File.join(prebuiltpath,'**','lib*.a')).each do |lib|
              arch = File.basename(File.dirname(lib))
              libdirpath = File.join(targetpath,arch)
              mkdir_p libdirpath unless File.directory?(libdirpath)
              Jake.copyIfNeeded lib, libdirpath
            end
            Dir.glob(File.join(prebuiltpath, '*.jar')).each do |lib|
              Jake.copyIfNeeded lib, targetpath
            end
            libdirpath = nil
            Dir.glob(File.join(prebuiltpath, '**', 'lib*.so')).each do |lib|
              next if lib =~ /adds/
              arch = File.basename(File.dirname(lib))

              if lib =~ /noautoload/
                libdirpath = File.join(targetpath,'noautoload','lib',arch)
              else
                libdirpath = File.join(targetpath,'lib',arch)
              end
              mkdir_p libdirpath unless File.directory? libdirpath
              Jake.copyIfNeeded lib, libdirpath
            end
          end

          puts "#{extyml} is processed"
          print_timestamp(' extension "'+ext+'" processing FINISH')

        end

        if exttype == 'rakefile'
          rakepath = Dir.glob File.join(extpath,'**','android','Rakefile')
          if rakepath.size == 1
            rakepath = File.dirname(rakepath.first)
          else
            rakepath = extpath
          end
          $ext_android_build_scripts[ext] = [rakepath, 'rake', extpath]

          if prebuild_rake_task
            args = [ prebuild_rake_task ]
            args << '--trace' if Rake.application.options.trace
            cc_run( 'rake', args, rakepath, true, setup_ext_env( extpath, ext ) ) or raise "Extension prebuild failed: #{extpath}"
          end

        elsif exttype != 'prebuilt'
          build_script = File.join(extpath,'ext','build'+$bat_ext)
          if File.exists? build_script
            if RUBY_PLATFORM =~ /(win|w)32$/
              $ext_android_build_scripts[ext] = [File.join(extpath,'ext'),'build.bat']
            else
              $ext_android_build_scripts[ext] = [File.join(extpath,'ext'), File.join('.', 'build' + $bat_ext)]
            end
          end
        end

        preload_java_classes.push(extconf_android["preload_java_classes"]) if (extconf_android && extconf_android["preload_java_classes"])

        puts "#{extpath} is configured"
      end # $app_extensions_list.each

      mkdir_p(File.join( $tmpdir, "include" ) )
      File.open( File.join( $tmpdir, "include", "rhojava_extra.inc" ), "w" ) { |f|
        preload_java_classes.each { |cls| f.puts( "RHODES_DEFINE_JAVA_CLASS(#{cls[0]}, \"#{cls[1]}\")") }
      }

      puts "Extensions' java source lists: #{$ext_android_additional_sources.inspect}"

      AndroidTools::MavenDepsExtractor.instance.extract_all


      print_timestamp('android:extensions FINISH')

    end #task :extensions

    task :emulator => "config:android" do
      $emuversion = $app_config["android"]["version"] unless $app_config["android"].nil?
      $emuversion = $config["android"]["version"] if $emuversion.nil? and !$config["android"].nil?

      if RUBY_PLATFORM =~ /(win|w)32$/
        $emulator = #"cmd /c " +
            File.join($androidsdkpath, "tools", "emulator.exe")
      else
        $emulator = File.join($androidsdkpath, "tools", "emulator")
      end

      $emuversion = AndroidTools.get_market_version($min_sdk_level) if $emuversion.nil?
      $emuversion = AndroidTools.get_market_version(AndroidTools.get_installed_api_levels[-1]) if $emuversion.nil? #last chance

      if $emuversion.nil?
        raise "Wrong Android emulator version: #{$emuversion}. Android SDK target API is not installed"
      end

      $logger.info "Configuring emulator for version: #{$emuversion}"

      $emuversion = $emuversion.to_s

      $appavdname = $app_config["android"]["emulator"] if $app_config["android"] != nil && $app_config["android"].length > 0
      $appavdname = $config["android"]["emulator"] if $appavdname.nil? and !$config["android"].nil? and $config["android"].length > 0

    end # task 'config:android:emulator'

    task :device => "config:android" do
    end
  end #namespace 'config:android'
end


namespace "build" do
  namespace "android" do

    task :rhodeslib_bundle, [:target_path]  => ["config:set_android_platform", "config:common"] do |t, args|
      print_timestamp('build:android:rhodeslib_bundle START')
        target_path = args[:target_path]

        $skip_build_rhodes_main = true
        $skip_build_extensions = true
        $skip_build_xmls = true
        $use_prebuild_data = true

        Rake::Task['config:android'].invoke

        appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
        appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

        Rake::Task['build:android:rhobundle'].invoke

        mkdir_p target_path if not File.exists? target_path

        source_RhoBundle = $appassets

        rm_rf File.join(target_path, "apps") if File.exists? File.join(target_path, "apps")
        rm_rf File.join(target_path, "db") if File.exists? File.join(target_path, "db")
        rm_rf File.join(target_path, "lib") if File.exists? File.join(target_path, "lib")
        rm_rf File.join(target_path, "hash") if File.exists? File.join(target_path, "hash")
        rm_rf File.join(target_path, "name") if File.exists? File.join(target_path, "name")
        rm_rf File.join(target_path, "rho.dat") if File.exists? File.join(target_path, "rho.dat")

        cp_r File.join(source_RhoBundle, "apps"), target_path
        cp_r File.join(source_RhoBundle, "db"), target_path
        cp_r File.join(source_RhoBundle, "lib"), target_path
        cp_r File.join(source_RhoBundle, "hash"), target_path
        cp_r File.join(source_RhoBundle, "name"), target_path
        cp_r File.join(source_RhoBundle, "rho.dat"), target_path

      print_timestamp('build:android:rhodeslib_bundle FINISH')
    end

    task :rhodeslib_lib, [:target_path] => [:rhobundle] do |t, args|
      print_timestamp('build:android:rhodeslib_lib START')
      $rhodes_as_lib = true
      Rake::Task["package:android"].invoke
      target_path = args[:target_path]
      mkdir_p target_path if not File.exists? target_path
      cp_r File.join($bindir, "#{$appname}-#{$app_config["version"]}.aar"), target_path
      print_timestamp('build:android:rhodeslib_lib FINISH')
    end


    desc "Build RhoBundle for android"
    task :rhobundle => ["config:android"] do
      print_timestamp('build:android:rhobundle START')

      $srcdir = $appassets
      rm_rf File.join($srcdir)
      mkdir $srcdir

      if !$skip_build_extensions
        Rake::Task["build:android:extensions"].invoke
      end

      Rake::Task["build:bundle:noxruby"].invoke

      hash = nil
      ["apps", "db", "lib"].each do |d|
        # Calculate hash of directories
        hash = get_dir_hash(File.join($srcdir, d), hash)
      end

      File.open(File.join($srcdir, "hash"), "w") { |f| f.write(hash.hexdigest) }
      File.open(File.join($srcdir, "name"), "w") { |f| f.write($appname) }

      rm_rf File.join($srcdir, "apps", "rhofilelist.txt")
      Jake.build_file_map(File.join($srcdir, "apps"), "rhofilelist.txt")

      print_timestamp('build:android:rhobundle FINISH')
    end

    desc "Build RhoBundle for Eclipse project"
    task :eclipsebundle => "build:android:rhobundle" do
      eclipse_assets = File.join(Jake.get_absolute($androidpath), "Rhodes", "assets")
      rm_rf eclipse_assets
      cp_r $appassets, eclipse_assets, :preserve => true
    end

    desc 'Building native extensions'
    task :extensions => ["config:android:extensions", :genconfig] do
      print_timestamp('build:android:extensions START')

      Rake::Task["build:bundle:noxruby"].invoke

      ENV['RHO_PLATFORM'] = 'android'
      ENV["RHO_APP_DIR"] = $app_path
      ENV["ANDROID_SDK"] = $androidsdkpath
      ENV["ANDROID_NDK"] = $androidndkpath
      ENV["ANDROID_API_LEVEL"] = $min_sdk_level.to_s
      ENV["RHO_ROOT"] = $startdir
      ENV["BUILD_DIR"] ||= $startdir + "/platform/android/build"
      ENV["RHO_INC"] = $appincdir
      ENV["RHO_RES"] = $appres
      ENV["RHO_ANDROID_TMP_DIR"] = $tmpdir
      ENV["NEON_ROOT"] = $neon_root unless $neon_root.nil?
      ENV["CONFIG_XML"] = $config_xml unless $config_xml.nil?
      ENV["RHO_DEBUG"] = $debug.to_s
      if (not $app_config["android"].nil?) and (not $app_config["android"]["fcm"].nil?)
        ENV["FCM_MPBILESDK_APP_ID"] = $app_config["android"]["fcm"]["mobilesdk_app_id"]
        ENV["FCM_PROJECT_NUMBER"] = $app_config["android"]["fcm"]["project_number"]
        ENV["FCM_CURRENT_KEY"] = $app_config["android"]["fcm"]["current_key"]
        ENV["FCM_PROJECT_ID"] = $app_config["android"]["fcm"]["project_id"]
      end
      
      ENV["BARCODE_ENGINE"] = 'google' #by default

      unless $app_config['android']&.[]('barcode_engine').nil?
        engine = $app_config['android']['barcode_engine']
        case engine
        when 'google'
          ENV["BARCODE_ENGINE"] = 'google'
        when 'zxing'
          ENV["BARCODE_ENGINE"] = 'zxing'
        when 'google'
          ENV["BARCODE_ENGINE"] = 'google'
        else
          raise 'Only supported barcode engines are: google(default) and zxing'
        end
      end
      
      $ext_android_build_scripts.each do |ext, builddata|
        # com.android.ndk.thirdparty:openssl:1.1.1 extension openssl.so - obsolete
        if ext == 'openssl.so'
          next
        end

        puts "Building #{ext}: #{builddata.inspect}"

        ENV['SOURCEPATH'] = builddata[0]
        sourcelist = Dir.glob(File.join(builddata[0],'**','android','ext_native.files'))
        ENV['SOURCELIST'] = sourcelist.size == 1 ? sourcelist.first : File.join(builddata[0],'ext_native.files')
        ENV["TARGET_TEMP_DIR"] = File.join($app_builddir, 'extensions', ext)
        ENV['TARGETPATH'] = File.join($app_builddir, 'extensions', ext)
        ENV['TARGETLIB'] = "lib#{ext}.a"
        ENV['TEMP_FILES_DIR'] = File.join($tmpdir, ext)
        buildargs = ['-Iext/platform/android/generated/jni',
                     '-Iext/shared',
                     '-Iext/shared/include',
                     "-I\"#{$startdir}/platform/android/Rhodes/jni/include\"",
                     "-I\"#{$startdir}/platform/android/Rhodes/jni/include/rhodes/details\"",
                     "-I\"#{$startdir}/platform/shared/ruby/include\"",
                     "-I\"#{$startdir}/platform/shared\"",
                     "-I\"#{$startdir}/platform/shared/common\"",
                     "-I\"#{$startdir}/platform/shared/api_generator\"",
                     "-I\"#{$commonapidir}/coreapi/ext/shared\"",
                     "-I\"#{$appincdir}\"",
                     "-I\"#{$startdir}/platform/shared/ruby\"",
                     "-I\"#{$startdir}/platform/shared/ruby/android\"",
                     "-I\"#{$startdir}/platform/shared/ruby/generated\""]
        ENV['BUILDARGS'] = buildargs.join(' ')

        mkdir_p ENV["TARGET_TEMP_DIR"] unless File.directory? ENV["TARGET_TEMP_DIR"]
        mkdir_p ENV["TEMP_FILES_DIR"] unless File.directory? ENV["TEMP_FILES_DIR"]

        $abis.each do |abi|
          puts "Build extension (#{abi}): #{ext}"
          if RUBY_PLATFORM =~ /(win|w)32$/ || (builddata[1] == 'rake')

            args = []
            if builddata[1] == 'rake'
              args << "arch:#{abi}"
              args << "-m"
              if !File.file?(File.join(builddata[0],'Rakefile'))
                args << "-f"
                args << "\"#{File.join($startdir,'platform','android','build','Rakefile')}\""
              end
              args << '--trace' if Rake.application.options.trace
            end

            
            #puts "builddata: " + builddata[1] + " args: " + args.to_s + " builddata0: " + builddata[0].to_s
            #puts ENV.to_s
            cc_run(builddata[1], args, builddata[0], false, nil, Rake.application.options.trace) or raise "Extension build failed: #{builddata[0]}"
          else
            currentdir = Dir.pwd()
            Dir.chdir builddata[0]
            sh %{$SHELL #{builddata[1]} #{abi}}
            result = $?
            Dir.chdir currentdir
            raise "Cannot build #{builddata[0]}" unless result.success?
          end
          puts "Extension build completed"
        end
      end

      $ext_android_manifest_changes.each do |ext, manifest_changes|
        addspath = File.join($app_builddir, 'extensions', ext, 'adds')
        mkdir_p addspath
        manifest_changes.each do |path|
          if File.extname(path) == '.xml'
            cp path, File.join(addspath, 'AndroidManifest.xml')
          else
            if File.extname(path) == '.rb'
              cp path, File.join(addspath, 'AndroidManifest.rb')
            else
              if File.extname(path) == '.erb'
                cp path, addspath
              else
                raise "Wrong AndroidManifest patch file: #{path}"
              end
            end
          end
        end
      end

      $ext_android_adds.each do |ext, path|
        addspath = File.join($app_builddir, 'extensions', ext, 'adds')
        mkdir_p addspath
        Dir.glob(File.join(path, '*')).each do |add|
          cp_r add, addspath if File.directory? add
        end
      end
      $ext_android_library_deps.each do |package, path|
        next if path.nil? or path == ''

        puts "Library resources: #{path}"

        res = File.join path, 'res'
        assets = File.join path, 'assets'
        packagepath = File.join $app_builddir,'extensions',package
        addspath = File.join packagepath,'adds'

        Dir.glob(File.join(res,'**','*.*')) do |p|
          unless File.directory? p
            rel_path = p.gsub path, ''
            target = File.join addspath, rel_path
            if File.basename(File.dirname(rel_path)) =~ /^values/
              target = File.join addspath, File.dirname(rel_path), "#{package}.#{File.basename(rel_path)}"
            end
            mkdir_p File.dirname(target) unless File.exists?(File.dirname(target))
            Jake.copyIfNeeded p, target
          end
        end
        #  cp_r res, addspath if File.directory? res
        cp_r assets, addspath if File.directory? assets

        Dir.glob(File.join(path,'**','*.jar')) do |jar|
          mkdir_p packagepath
          cp jar, packagepath
        end
      end
      print_timestamp('build:android:extensions FINISH')
    end #task :extensions

    class BuildContext
      attr_accessor :libname
      attr_accessor :sourcelist
      attr_accessor :extra_copmiler_args
    end

    class LibBuilder     

      attr_accessor :context

      @@results = {}

      def self.results
        @@results
      end

      def self.results=v
        @@results = v
      end

      def initialize
        @context = BuildContext.new
        @context.extra_copmiler_args = []
      end

      def build
        objdir = File.join($tmpdir, @context.libname)
        libdir = File.join($app_builddir, @context.libname)
        mkdir_p libdir unless File.directory? libdir

        args = []

        args.concat @context.extra_copmiler_args        

        ENV['SOURCEPATH'] = File.join($androidpath,'..','..')
        ENV['SOURCELIST'] = @context.sourcelist
        ENV["ANDROID_NDK"] = $androidndkpath
        ENV["ANDROID_API_LEVEL"] = $min_sdk_level.to_s
        ENV["RHO_ROOT"] = $startdir
        ENV["RHO_INC"] = $appincdir
        ENV["TARGETPATH"] = libdir
        ENV['TARGETLIB'] = "lib#{@context.libname}.a"
        ENV['TEMP_FILES_DIR'] = objdir
        ENV['BUILDARGS'] = args.join(' ')

        $abis.each { |abi|
          args = ['-f', "\"#{File.join( $builddir,'Rakefile')}\"", "arch:#{abi}"]
          args << '-m'
          args << '--trace' if Rake.application.options.trace
          cc_run('rake', args, nil, false, nil, Rake.application.options.trace) or raise "Build failed: #{@context.libname}"
        }

        LibBuilder.results[@context.libname] = libdir
      end
    end

    task :libsqlite => "config:android" do
      print_timestamp('build:android:libsqlite START')

      b = LibBuilder.new
      b.context.libname = 'sqlite'
      b.context.sourcelist = File.join($builddir, 'libsqlite_build.files')

      srcdir = File.join($shareddir, "sqlite")
      b.context.extra_copmiler_args = [
        "-I\"#{srcdir}\"",
        "-I\"#{$shareddir}\""
      ]
      b.build

      print_timestamp('build:android:libsqlite FINISH')
    end

    task :libcurl => "config:android" do
      print_timestamp('build:android:libcurl START')
      # Steps to get curl_config.h from fresh libcurl sources:
      #export PATH=<ndkroot>/build/prebuilt/linux-x86/arm-eabi-4.2.1/bin:$PATH
      #export CC=arm-eabi-gcc
      #export CPP=arm-eabi-cpp
      #export CFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
      #export CPPFLAGS="--sysroot <ndkroot>/build/platforms/android-3/arch-arm -fPIC -mandroid -DANDROID -DOS_ANDROID"
      #./configure --without-ssl --without-ca-bundle --without-ca-path --without-libssh2 --without-libidn --disable-ldap --disable-ldaps --host=arm-eabi

      b = LibBuilder.new      
      b.context.libname = 'curl'
      b.context.sourcelist = File.join($builddir, 'libcurl_build.files')

      srcdir = File.join $shareddir, "curl", "lib"
      b.context.extra_copmiler_args = [
        "-DHAVE_CONFIG_H",
        "-I\"#{srcdir}/../include\"",
        "-I\"#{srcdir}\"",
        "-I\"#{srcdir}/vtls\"",
        "-I\"#{srcdir}/vauth\"",
        "-I\"#{$shareddir}\"",
        "-I\"#{$shareddir}/../../lib/extensions/openssl.so/ext/sources/include\""
      ]
      b.build

      print_timestamp('build:android:libcurl FINISH')
    end

    task :libruby => "config:android" do
      print_timestamp('build:android:libruby START')

      b = LibBuilder.new      
      b.context.libname = 'ruby'
      b.context.sourcelist = File.join($builddir, 'libruby_build.files')

      srcdir = File.join($shareddir, "ruby")
      b.context.extra_copmiler_args = [   
        "-DRUBY_EXPORT",
        "-Wno-uninitialized",
        "-Wno-missing-field-initializers",
        '-Wno-shadow',
        "-I\"#{srcdir}/include\"",
        "-I\"#{srcdir}/android\"",
        "-I\"#{srcdir}/generated\"",
        "-I\"#{srcdir}\"",
        "-I\"#{srcdir}/..\""
#        "-I\"#{b.context.srcdir}/../sqlite\""    
      ]
      b.build

      print_timestamp('build:android:libruby FINISH')
    end

    task :libjson => "config:android" do
      print_timestamp('build:android:libjson START')

      b = LibBuilder.new      
      b.context.libname = 'json'
      b.context.sourcelist = File.join($builddir, 'libjson_build.files')

      srcdir = File.join($shareddir, "json")
      b.context.extra_copmiler_args = [   
        "-I\"#{srcdir}\"",
        "-I\"#{srcdir}/..\""
      ]
      b.build

      print_timestamp('build:android:libjson FINISH')
    end

    task :librholog => "config:android" do
      print_timestamp('build:android:librholog START')

      b = LibBuilder.new      
      b.context.libname = 'rholog'
      b.context.sourcelist = File.join($builddir, 'librholog_build.files')

      srcdir = File.join($shareddir, "logging")
      b.context.extra_copmiler_args = [   
        "-I\"#{srcdir}/..\""
      ]
      b.build

      print_timestamp('build:android:librholog FINISH')
    end

    task :librhomain => "config:android" do
      print_timestamp('build:android:librhomain START')

      b = LibBuilder.new
      b.context.libname = 'rhomain'
      b.context.sourcelist = File.join($builddir, 'librhomain_build.files')
      b.context.extra_copmiler_args = [   
        "-I\"#{$shareddir}\"",
        "-I\"#{$commonapidir}\""
      ]
      b.build

      print_timestamp('build:android:librhomain FINISH')
    end

    task :librhocommon => "config:android" do
      print_timestamp('build:android:librhocommon START')

      b = LibBuilder.new
      b.context.libname = 'rhocommon'
      b.context.sourcelist = File.join($builddir, 'librhocommon_build.files')
      b.context.extra_copmiler_args = [   
        "-I\"#{$shareddir}\"",
        "-I\"#{$shareddir}/curl/include\"",
        "-I\"#{$shareddir}/ruby/include\"",
        "-I\"#{$shareddir}/ruby/android\"",
        "-I\"#{$shareddir}/../../lib/extensions/openssl.so/ext/sources/include\""
      ]
      b.build

      print_timestamp('build:android:librhocommon FINISH')
    end

    task :librhodb => "config:android" do
      print_timestamp('build:android:librhodb START')

      b = LibBuilder.new      
      b.context.libname = 'rhodb'
      b.context.sourcelist = File.join($builddir, 'librhodb_build.files')

      srcdir = File.join($shareddir, "db")
      b.context.extra_copmiler_args = [   
        "-I\"#{srcdir}\"",
        "-I\"#{srcdir}/..\"",
        "-I\"#{srcdir}/../sqlite\"",
        "-I\"#{$shareddir}/ruby/include\"",
        "-I\"#{$shareddir}/ruby/android\""
      ]
      b.build

      print_timestamp('build:android:librhodb FINISH')
    end

    task :librhosync => "config:android" do
      print_timestamp('build:android:librhosync START')

      b = LibBuilder.new      
      b.context.libname = 'rhosync'
      b.context.sourcelist = File.join($builddir, 'librhosync_build.files')

      srcdir = File.join($shareddir, "sync")
      b.context.extra_copmiler_args = [
        "-I\"#{srcdir}\"",   
        "-I\"#{srcdir}/..\"",
        "-I\"#{srcdir}/../sqlite\""
      ]
      b.build      

      print_timestamp('build:android:librhosync FINISH')
    end

    task :libs => [
      :libsqlite, 
      :libcurl, 
      :libruby, 
      :libjson, 
      :librhodb, 
      :librhocommon, 
      :librhomain, 
      :librhosync, 
      :librholog
    ]

    task :genconfig => "config:android" do
      print_timestamp('build:android:genconfig START')
      mkdir_p $appincdir unless File.directory? $appincdir

      # Generate genconfig.h
      genconfig_h = File.join($appincdir, 'genconfig.h')

      gapi_already_enabled = false
      caps_already_enabled = {}
      #ANDROID_PERMISSIONS.keys.each do |k|
      #  caps_already_enabled[k] = false
      #end
      if File.file? genconfig_h
        File.open(genconfig_h, 'r') do |f|
          while line = f.gets
            if line =~ /^\s*#\s*define\s+RHO_GOOGLE_API_KEY\s+"[^"]*"\s*$/
              gapi_already_enabled = true
            else
              ANDROID_PERMISSIONS.keys.each do |k|
                if line =~ /^\s*#\s*define\s+RHO_CAP_#{k.upcase}_ENABLED\s+(.*)\s*$/
                  value = $1.strip
                  if value == 'true'
                    caps_already_enabled[k] = true
                  elsif value == 'false'
                    caps_already_enabled[k] = false
                  else
                    raise "Unknown value for the RHO_CAP_#{k.upcase}_ENABLED: #{value}"
                  end
                end
              end
            end
          end
        end
      end

      regenerate = false
      regenerate = true unless File.file? genconfig_h
      regenerate = $gapikey.nil? ^ gapi_already_enabled unless regenerate

      caps_enabled = {}
      ANDROID_PERMISSIONS.keys.each do |k|
        caps_enabled[k] = $app_config["capabilities"].index(k) != nil
        regenerate = true if caps_already_enabled[k].nil? or caps_enabled[k] != caps_already_enabled[k]
      end

      puts caps_enabled.inspect

      if regenerate
        puts "Need to regenerate genconfig.h"
        $stdout.flush
        File.open(genconfig_h, 'w') do |f|
          f.puts "#ifndef RHO_GENCONFIG_H_411BFA4742CF4F2AAA3F6B411ED7514F"
          f.puts "#define RHO_GENCONFIG_H_411BFA4742CF4F2AAA3F6B411ED7514F"
          f.puts ""
          f.puts "#define RHO_GOOGLE_API_KEY \"#{$gapikey}\"" if $gapikey
          caps_enabled.each do |k, v|
            f.puts "#define RHO_CAP_#{k.upcase}_ENABLED #{v ? "true" : "false"}"
          end
          f.puts ""
          f.puts "#endif /* RHO_GENCONFIG_H_411BFA4742CF4F2AAA3F6B411ED7514F */"
        end
      else
        puts "No need to regenerate genconfig.h"
        $stdout.flush
      end
      print_timestamp('build:android:genconfig FINISH')
    end

    def clear_flats(path)
      Dir.glob(File.join(path,'**','*.flat')) do |filepath|
        rm_rf filepath
      end
    end

    task :librhodes => [:libs, :extensions, :genconfig] do
      print_timestamp('build:android:librhodes START')
      if $skip_so_build
        puts "Skip building rhodes shared library"
        next
      end
      srcdir = File.join $androidpath, "Rhodes", "jni", "src"
      openssl_include = File.join AndroidTools::MavenDepsExtractor.instance.prefab_dirs[0], "modules", "crypto", "include"

      # maven dependency com.android.ndk.thirdparty:openssl:1.1.1
      openssl_crypto_lib = File.join AndroidTools::MavenDepsExtractor.instance.prefab_dirs[0], "modules", "crypto", "libs"
      openssl_ssl_lib = File.join AndroidTools::MavenDepsExtractor.instance.prefab_dirs[0], "modules", "ssl", "libs"
      abi_prefab_target_dirs = { "arm" => "android.armeabi-v7a", "aarch64" => "android.arm64-v8a", "x86" => "android.x86", "armeabi-v7a" => "android.armeabi-v7a", "x86_64" => "android.x86_64" }

      args = []
      args << "-I\"#{$appincdir}\""
      args << "-I\"#{srcdir}/../include\""
      args << "-I\"#{srcdir}/../include/rhodes/details\""
      args << "-I\"#{$shareddir}\""
      args << "-I\"#{$shareddir}/common\""
      args << "-I\"#{$shareddir}/api_generator\""
      args << "-I\"#{$shareddir}/sqlite\""
      args << "-I\"#{$shareddir}/curl/include\""
      args << "-I\"#{$shareddir}/ruby/include\""
      args << "-I\"#{$shareddir}/ruby/android\""
      # maven dependency com.android.ndk.thirdparty:openssl:1.1.1
      args << "-I\"#{openssl_include}\""
      args << "-I\"#{$coreapidir}\""
      args << "-I\"#{File.join($tmpdir,"include")}\""

      ENV['SOURCEPATH'] = File.join($androidpath,'..','..')
      ENV['SOURCELIST'] = File.join($builddir, 'librhodes_build.files')
      ENV["ANDROID_NDK"] = $androidndkpath
      ENV["ANDROID_API_LEVEL"] = $min_sdk_level.to_s
      ENV["RHO_ROOT"] = $startdir
      ENV["RHO_INC"] = $appincdir
      ENV["TARGETPATH"] = File.join $app_builddir, 'librhodes', 'lib'
      ENV['SHAREDLIB'] = 'librhodes.so'
      ENV['TEMP_FILES_DIR'] = File.join $tmpdir, 'librhodes'
      ENV['BUILDARGS'] = args.join(' ')

      $abis.each do |abi|

        args = []

        rlibs = []
        rlibs << "android"
        rlibs << "log"
        rlibs << "dl"
        rlibs << "z"
        rlibs << "stdc++"

        # maven dependency com.android.ndk.thirdparty:openssl:1.1.1
        rlibs << "crypto"
        rlibs << "ssl"

        rlibs.map! { |x| "-l#{x}" }

        realabi = abi
        realabi = 'armeabi' if abi == 'arm'

        extlibs = Dir.glob(File.join($app_builddir,'extensions','**',realabi,'lib*.a')) # + Dir.glob($app_builddir + "/**/lib*.so")       

        extlibs.each do |lib|
          args << "-L\"#{File.dirname(lib)}\""
        end

        # maven dependency com.android.ndk.thirdparty:openssl:1.1.1
        args << "-L\"#{File.join(openssl_crypto_lib, abi_prefab_target_dirs[abi])}\""
        args << "-L\"#{File.join(openssl_ssl_lib, abi_prefab_target_dirs[abi])}\""
       
        deps = []
        libs = []

        LibBuilder.results.each do |name, libdir|
          deps << File.join( libdir, realabi, "lib#{name}.a" )
          libs << name
          args << "-L\"#{libdir}/#{realabi}\""
        end
        libs.map! { |x| "-l#{x}" }

        if extlibs && extlibs.size > 0
          args << '-Wl,--whole-archive'
          extlibs.each do |lib|
            args << '-l' + File.basename(lib).gsub(/^lib/, "").gsub(/\.(a|so)$/, "")
          end
          args << '-Wl,--no-whole-archive'
        end

        deps.each do |dep|
          args << '-l' + File.basename(dep).gsub(/^lib/, "").gsub(/\.(a|so)$/, "")
        end

        # Workaround for GNU ld: this way we have specified one lib multiple times
        # command line so ld's dependency mechanism will find required functions
        # independently of its position in command line
        stub = []
        libs.each do |lib|
          args << lib
          stub.each do |s|
            args << s
          end
          stub << lib
        end

        args += rlibs

        $ext_android_platform_lib.each do |lib|
          args << "-l#{lib}"
        end

        ENV['LINKARGS'] = args.join(' ')
        ENV['LINKDEPS'] = (deps+extlibs).join(' ')

        args = ['-f', "\"#{File.join($builddir,'Rakefile')}\"", "link:#{abi}"]
        args << '--trace'
        cc_run('rake', args, nil, false, nil, true) or raise "Build failed: rhodes"
        #Jake.run3("rake #{args.join(' ')}")
      end
      print_timestamp('build:android:librhodes FINISH')
    end

    task :manifest => ["config:android", :extensions] do
      print_timestamp('build:android:manifest START')
      version = {'major' => 0, 'minor' => 0, 'patch' => 0, "build" => 0}
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
        elsif $app_config["version"] =~ /^(\d+)\.(\d+)\.(\d+)\.(\d+)$/
          version["major"] = $1.to_i
          version["minor"] = $2.to_i
          version["patch"] = $3.to_i
          version["build"] = $4.to_i
        else
          raise "Version number must be numeric and in one of these formats: major, major.minor, major.minor.patch, or major.minor.patch.build."
        end
      end

      version = version["major"]*1000000 + version["minor"]*10000 + version["patch"]*100 + version["build"]

      usesPermissions = ['android.permission.RECEIVE_BOOT_COMPLETED', 'android.permission.INTERNET', 'android.permission.PERSISTENT_ACTIVITY', 'android.permission.WAKE_LOCK']
      usesPermissions << 'android.permission.FOREGROUND_SERVICE' if ($target_sdk_level >= 28)

      $app_config["capabilities"].each do |cap|
        cap = ANDROID_PERMISSIONS[cap]
        next if cap.nil?
        cap = [cap] unless cap.is_a? Array

        cap.each do |cap_item|
          if cap_item.is_a? Proc
            #caps_proc << cap_item
            next
          end

          if cap_item.is_a? String

            if $target_sdk_level >= 30 && cap_item == 'WRITE_EXTERNAL_STORAGE' || cap_item == 'READ_EXTERNAL_STORAGE'
              usesPermissions << "android.permission.MANAGE_EXTERNAL_STORAGE"
            end
            usesPermissions << "android.permission.#{cap_item}"
            next
          end
        end
      end
      usesPermissions.uniq!

      hidden = get_boolean($app_config['hidden_app'])
      home_app = false
      if $app_config['android'] != nil
          if $app_config['android']['home_app'] != nil
              home_app = $app_config['android']['home_app']
          end
      end

      generator = ManifestGenerator.new JAVA_PACKAGE_NAME, $app_package_name, hidden, home_app, usesPermissions

      generator.versionName = $app_config["version"]
      generator.versionCode = version
      generator.allowBackup = ($app_config["android"]["allowBackup"] == "false" ? "false" : "true") if $app_config["android"]
      generator.installLocation = 'auto'
      generator.minSdkVer = $min_sdk_level
      generator.targetSdkVer = $target_sdk_level
      generator.maxSdkVer = $max_sdk_level
      generator.screenOrientation = $android_orientation unless $android_orientation.nil?
      generator.hardwareAcceleration = true if $app_config["capabilities"].index('hardware_acceleration')
      generator.apikey = $gapikey if $gapikey
      generator.debuggable = $debug
      generator.buildSdkVer = $found_api_level
      generator.buildToolsVer = $build_tools_ver
      generator.androidVerMaj = $major_version
      generator.androidVerMin = $minor_version

      generator.addUriParams $uri_scheme, $uri_host, $uri_path_prefix

      Dir.glob(File.join($app_builddir, 'extensions', '*', 'adds', 'AndroidManifest.rb')).each do |extscript|
        puts "Evaluating #{extscript}"
        eval(File.new(extscript).read)
      end
      Dir.glob(File.join($app_builddir, 'extensions', '*', 'adds', 'Manifest*.erb')).each do |exttemplate|
        puts "Adding template #{exttemplate}"
        generator.manifestManifestAdds << exttemplate
      end
      Dir.glob(File.join($app_builddir, 'extensions', '*', 'adds', 'Application*.erb')).each do |exttemplate|
        puts "Adding template #{exttemplate}"
        generator.applicationManifestAdds << exttemplate
      end

      manifest = generator.render $rhomanifesterb

      File.open($appmanifest, "w") { |f| f.write manifest }


      #######################################################
      # Deprecated staff below


      app_f = File.new($appmanifest)
      manifest_orig_doc = REXML::Document.new(app_f)
      app_f.close
      dst_manifest = manifest_orig_doc.elements["manifest"]
      dst_application = manifest_orig_doc.elements["manifest/application"]
      dst_main_activity = nil
      puts '$$$ try to found MainActivity'
      dst_application.elements.each("activity") do |a|
        puts '$$$ activity with attr = '+a.attribute('name', 'android').to_s
        if a.attribute('name', 'android').to_s == 'com.rhomobile.rhodes.RhodesActivity'
          puts '          $$$ FOUND !'
          dst_main_activity = a
        end
      end

      Dir.glob(File.join($app_builddir, 'extensions', '*', 'adds', 'AndroidManifest.xml')).each do |ext_manifest|

        if File.exists? ext_manifest
          puts 'AndroidManifest.xml['+ext_manifest+'] from native extension found !'

          manifest_ext_doc = REXML::Document.new(File.new(ext_manifest))

          src_manifest = manifest_ext_doc.elements["manifest"]

          src_application = manifest_ext_doc.elements["manifest/application"]

          if src_application != nil
            puts 'Extension Manifest process application item :'
            src_application.elements.each do |e|
              puts '$$$ process element with attr = '+e.attribute('name', 'android').to_s
              if e.attribute('name', 'android').to_s == 'com.rhomobile.rhodes.RhodesActivity'
                e.elements.each do |sube|
                  puts '         add item to MainActivity['+sube.xpath+']'
                  dst_main_activity.add sube
                end
              else
                puts '         add item ['+e.xpath+']'
                dst_application.add e
              end
            end
          end

          puts 'Extension Manifest process root <manifest> item :'
          src_manifest.elements.each do |e|
            p = e.xpath
            if p != '/manifest/application'
              dst_e = manifest_orig_doc.elements[p]
              if dst_e != nil
                if p == '/manifest/uses-sdk'
                  puts '         found and delete original item ['+p+']'
                  manifest_orig_doc.elements.delete p
                end
              end
              puts '         and new item ['+p+']'
              dst_manifest.add e
            end
          end

        else
          puts 'AndroidManifest change file ['+m+'] from native extension not found !'
        end

      end

      puts 'delete original manifest'
      File.delete($appmanifest)

      updated_f = File.open($appmanifest, "w")
      manifest_orig_doc.write updated_f, 2
      updated_f.close

      #rm tappmanifest
      print_timestamp('build:android:manifest FINISH')
      puts 'Manifest updated by extension is saved!'
    end

    task :resources => [:rhobundle, :extensions, :librhodes] do
      print_timestamp('build:android:resources START')

      set_app_name_android($appname)

      puts 'EXT:  add additional files to project before build'
      Dir.glob(File.join($app_builddir, 'extensions', '*', 'adds', '*')).each do |res|
        if File.directory?(res) && (res != '.') && (res != '..')
          puts "add resources from extension [#{res}] to [#{$tmpdir}]"
          cp_r res, $tmpdir
        end
      end

      set_app_name_android_post($appname)

      #copy icon after extension resources in case it overwrites them (like rhoelementsext...)
      set_app_icon_android

      if $config_xml
        puts "Copying custom config.xml"
        rawres_path = File.join($tmpdir, 'res', 'raw')
        mkdir_p rawres_path unless File.exist? rawres_path
        cp $config_xml, File.join(rawres_path, 'config.xml')
      end

      mkdir_p File.join($applibs,'armeabi')
      mkdir_p File.join($applibs,'armeabi-v7a')
      mkdir_p File.join($applibs,'x86')
      mkdir_p File.join($applibs,'arm64-v8a')

      # Add .so libraries
      Dir.glob($app_builddir + "/**/lib*.so").each do |lib|
        arch = File.basename(File.dirname(lib))
        file = File.basename(lib)

        ndkwrapper = NDKWrapper.new($androidndkpath)
        llvm_stl = ndkwrapper.get_llvm_stl_path(arch)
        arch = 'arm64-v8a' if arch == "aarch64"
        cp_r lib, File.join($applibs,arch,file)

        if File.exists? llvm_stl
          cp_r llvm_stl, File.join($applibs,arch)
        end

      end
      $ext_android_additional_lib.each do |lib|
        arch = File.basename(File.dirname(lib))
        file = File.basename(lib)
        cp_r lib, File.join($applibs,arch,file)
      end

      abi_prefab_alter_names = { "android.armeabi-v7a" => "arm", "android.arm64-v8a" => "aarch64", "android.x86" => "x86", "armeabi-v7a" => "arm", "android.x86_64" => "x86_64" }
      abi_prefab_target_dirs = { "android.armeabi-v7a" => "armeabi", "android.arm64-v8a" => "arm64-v8a", "android.x86" => "x86", "armeabi-v7a" => "armeabi", "android.x86_64" => "x86_64" }
      android_prefab_dir = AndroidTools::MavenDepsExtractor.instance.prefab_dirs
      android_prefab_dir.each do |dir|
        Dir.glob(dir + "/**/lib*.so").each do |lib|
          arch = File.basename(File.dirname(lib))
          if !$abis.include?(abi_prefab_alter_names[arch])
            next
          end
          arch = abi_prefab_target_dirs[arch]
          file = File.basename(lib)
          if Dir.exists? File.join($applibs,arch)
            cp_r lib, File.join($applibs,arch,file)
          end
        end
      end

      abi_alter_names = { "armeabi" => "arm", "arm64-v8a" => "aarch64", "x86" => "x86", "armeabi-v7a" => "arm", "x86_64" => "x86_64" }
      if !$rhodes_as_lib
        $android_jni_libs = AndroidTools::MavenDepsExtractor.instance.jni_libs
        $android_jni_libs.each do |lib|
          arch = File.basename(File.dirname(lib))
          if !$abis.include?(abi_alter_names[arch])
            next
          end

          arch = "armeabi" if arch == "armeabi-v7a"
          file = File.basename(lib)
          if Dir.exists? File.join($applibs,arch)
            cp_r lib, File.join($applibs,arch,file)
          end
        end
      end
      print_timestamp('build:android:resources FINISH')
    end

    task :fulleclipsebundle => [:resources, :librhodes] do
      #manifest = File.join $tmpdir,'AndroidManifest.xml'

      eclipse_res = File.join $projectpath,'res'
      eclipse_assets = File.join $projectpath,'assets'
      eclipse_libs = File.join $projectpath,'libs'
      #eclipse_manifest = File.join $projectpath,'AndroidManifest.xml'

      rm_rf eclipse_res
      rm_rf eclipse_assets
      rm_rf eclipse_libs
      #rm_rf eclipse_manifest

      mkdir_p eclipse_libs

      cp_r $appres, $projectpath
      cp_r $appassets, $projectpath
      cp_r $applibs, eclipse_libs
      #cp manifest, $projectpath
    end

    task :gencapabilitiesjava => "config:android" do
      # Generate Capabilities.java
      mkdir_p File.dirname $app_capabilities_java
      f = StringIO.new("", "w+")
      #File.open($app_capabilities_java, "w") do |f|
      f.puts "package #{JAVA_PACKAGE_NAME};"
      f.puts "public class Capabilities {"
      ANDROID_PERMISSIONS.keys.sort.each do |k|
        val = 'false'
        val = 'true' if $app_config["capabilities"].index(k) != nil
        f.puts "  public static final boolean #{k.upcase}_ENABLED = #{val};"
      end

      f.puts "  public static final boolean DEBUG_ENABLED = #{$debug.to_s};"

      f.puts "}"
      #end
      Jake.modify_file_if_content_changed($app_capabilities_java, f)
    end

    task :genpushjava => "config:android" do
      # Generate Push.java
      mkdir_p File.dirname $app_push_java
      f = StringIO.new("", "w+")
      #File.open($app_push_java, "w") do |f|
      f.puts "package #{JAVA_PACKAGE_NAME};"
      f.puts "public class Push {"
      f.puts "  public static final String SENDER = \"#{$push_sender}\";"
      if $push_notifications.nil?
        f.puts "  public static final String PUSH_NOTIFICATIONS =  \"none\";"
      else
        f.puts "  public static final String PUSH_NOTIFICATIONS =  \"#{$push_notifications}\";"
      end
      f.puts "};"
      #end
      Jake.modify_file_if_content_changed($app_push_java, f)
    end

    task :genloadlibsjava => "config:android" do
      mkdir_p File.dirname $app_native_libs_java
      f = StringIO.new("", "w+")
      #File.open($app_native_libs_java, "w") do |f|
      f.puts "package #{JAVA_PACKAGE_NAME};"
      f.puts "import android.util.Log;"
      f.puts "public class NativeLibraries {"
      f.puts "  private static final String TAG = NativeLibraries.class.getSimpleName();"
      f.puts "  public static void load() {"
      f.puts "    // Load native .so libraries"
      Dir.glob($app_builddir + "/**/lib*.so").reverse.each do |lib|
        next if lib =~ /noautoload/

        libname = File.basename(lib).gsub(/^lib/, '').gsub(/\.so$/, '')
        f.puts "    Log.d(TAG, \"Loading lib #{libname}\");"
        f.puts "    System.loadLibrary(\"#{libname}\");"
        f.puts "    Log.d(TAG, \"Lib #{libname} loaded\");"
      end
      #f.puts "    // Load native implementation of rhodes"
      #f.puts "    System.loadLibrary(\"rhodes\");"
      f.puts "  }"
      f.puts "};"
      #end
      Jake.modify_file_if_content_changed($app_native_libs_java, f)
    end

    task :genrholisteners => ['config:android:extensions', 'config:android'] do
      # RhodesActivity Listeners
      mkdir_p File.dirname $app_startup_listeners_java
      f = StringIO.new("", "w+")
      f.puts '// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!'
      f.puts 'package com.rhomobile.rhodes.extmanager;'
      f.puts ''
      f.puts 'class RhodesStartupListeners {'
      f.puts ''
      f.puts '  public static final String[] ourRunnableList = { ""'
      $ext_android_rhodes_activity_listener.each do |a|
        f.puts '       ,"'+a+'"'
      end
      f.puts '  };'
      f.puts '}'
      Jake.modify_file_if_content_changed($app_startup_listeners_java, f)
    end

    task :genrjava => [:manifest, :resources] do
      mkdir_p $app_rjava_dir

      $logger.info "Generate initial R.java at #{$app_rjava_dir}"

      $logger.debug $appres.to_s
      $logger.debug $app_rjava_dir.to_s

      unless ($rhodes_as_lib)
        Aapt2Helper.instance.build_resources( $build_bundle )
      else
        mkdir_p $appres_flats

        #walkthrough resources dir
        Dir.glob(File.join($appres,'**','*.*')) do |filepath|
          args = ["compile", PathToWindowsWay(filepath), "-o", PathToWindowsWay(File.dirname(filepath))]
          args << '-v' if Rake.application.options.trace
          Jake.run($aapt2, args)
        end

        res_dirs = AndroidTools::MavenDepsExtractor.instance.aapt2_res_dirs
        res_dirs.each do |dir|
          Dir.glob(File.join(dir,'**','*.*')) do |filepath|
            args = ["compile", PathToWindowsWay(filepath), "-o", PathToWindowsWay(File.dirname(filepath))]
            args << '-v' if Rake.application.options.trace
            Jake.run($aapt2, args)
          end
        end

        flaten_list_file_path = File.join($tmpdir, "flaten_list.txt")
        File.open(flaten_list_file_path, "w") do |f|     
          Dir.glob(File.join($appres,'**','*.flat')) do |filepath|
            f.write(filepath)            
            f.write(" ")
          end

          res_dirs.each do |dir|
            Dir.glob(File.join(dir,'**','*.flat')) do |filepath|
              f.write(filepath)            
              f.write(" ")
            end
          end
        end

        FormatManifestToAapt2Compat($appmanifest) #what is that?
        
        args = 
        [
          "link", 
          "-o", PathToWindowsWay(File.join($bindir, "_tmp.aar")), 
          "-I", PathToWindowsWay($androidjar),
          "--manifest", PathToWindowsWay($appmanifest), 
          "-A", PathToWindowsWay($appassets), 
          "--java", PathToWindowsWay($app_rjava_dir), 
          "-R", "@" + PathToWindowsWay(flaten_list_file_path),
          "--auto-add-overlay"
        ]
        
        args << "--output-text-symbols"
        args << PathToWindowsWay(File.join($app_rjava_dir, 'R.txt'))
        args << '-v' if Rake.application.options.trace

        require 'nokogiri'
        
        FormatManifestToAarCompat($appmanifest)
        
        Jake.run($aapt2, args)
        raise 'Error in AAPT2: ' + $aapt2 + " " + args.join(' ') unless $?.success?

        #how about some comments on this magic?
        if(File.exists?(File.join($app_rjava_dir, 'com', $vendor, $appname, 'R.java')))
          cp_r File.join($app_rjava_dir, 'com', $vendor, $appname, 'R.java'), File.join($app_rjava_dir, 'R.java')
          rm_rf File.join($app_rjava_dir, 'com', $vendor, $appname, 'R.java')
        elsif (File.exists?(File.join($app_rjava_dir, $app_package_name.split('.'), 'R.java')))
          cp_r File.join($app_rjava_dir, $app_package_name.split('.'), 'R.java'), File.join($app_rjava_dir, 'R.java')
          rm_rf File.join($app_rjava_dir, $app_package_name.split('.'), 'R.java')
        end

        clear_flats($appres)
        res_dirs = AndroidTools::MavenDepsExtractor.instance.aapt2_res_dirs
        res_dirs.each do |dir|
          clear_flats(dir)
        end            
      end            
      
      #We should've generated R.java at this point. Now let's duplicate it for our dependencies substituting package names
      packs = AndroidTools::MavenDepsExtractor.instance.extract_packages
      packs.each do |p|
        path_to_p = File.join $tmpdir, 'gen', p.split('.')
        mkdir_p path_to_p
        buf = File.new(File.join($app_rjava_dir, "R.java"), "r").read.gsub(/^\s*package\s*#{$app_package_name};\s*$/, "\npackage " + p + ";\n")
        File.open(File.join(path_to_p, "R.java"), "w") { |f| f.write(buf) }
      end

      mkdir_p File.join($app_rjava_dir, "R") if not File.exists? File.join($app_rjava_dir, "R")
      buf = File.new(File.join($app_rjava_dir, "R.java"), "r").read.gsub(/^\s*package\s*#{$app_package_name};\s*$/, "\npackage com.rhomobile.rhodes;\n")
      File.open(File.join($app_rjava_dir, "R", "R.java"), "w") { |f| f.write(buf) }

      $ext_android_library_deps.each do |package, path|
        if !File.directory?(path)
          puts "[WARN] Path for dependency #{package} does not exists (#{path})"
        end

        next if ((!package) or (package.empty?))

        r_dir = File.join $tmpdir, 'gen', package.split('.')
        mkdir_p r_dir
        buf = File.new(File.join($app_rjava_dir, 'R.java'), "r").read.gsub(/^\s*package\s*#{$app_package_name};\s*$/, "\npackage #{package};\n")
        File.open(File.join(r_dir,'R.java'), 'w') { |f| f.write(buf) }
      end
    end

    task :genreclipse => [:manifest, :resources] do
      mkdir_p $app_rjava_dir
      args = ["package", "-f", "-M", $appmanifest, "-S", $appres, "-A", $appassets, "-I", $androidjar, "-J", $app_rjava_dir]
      args += AndroidTools::MavenDepsExtractor.instance.aapt_args
      Jake.run($aapt, args)

      raise 'Error in AAPT: R.java' unless $?.success?

      Dir.glob(File.join $app_rjava_dir, '*.java') do |java|
        buf = File.new(java, 'r').read.gsub(/package .*$/, 'package com.rhomobile.rhodes;')
        File.open(java, 'w') { |f| f.write buf }
      end
      #buf = File.new($rho_android_r, "r").read.gsub(/^\s*import com\.rhomobile\..*\.R;\s*$/, "\nimport #{$app_package_name}.R;\n")
      #File.open($app_android_r, "w") { |f| f.write(buf) }

      #mkdir_p File.join($app_rjava_dir, "R") if not File.exists? File.join($app_rjava_dir, "R")
      #buf = File.new(File.join($app_rjava_dir, "R.java"), "r").read.gsub(/^\s*package\s*#{$app_package_name};\s*$/, "\npackage com.rhomobile.rhodes.R;\n")
      #"{b}"uf.gsub!(/public\s*static\s*final\s*int/, "public static int")
      #File.open(File.join($app_rjava_dir, "R", "R.java"), "w") { |f| f.write(buf) }
    end

    task :gensourceseclipse => [:genloadlibsjava, :genpushjava, :gencapabilitiesjava, :genrholisteners, :genreclipse]
    task :gensourcesjava => [:genloadlibsjava, :genpushjava, :gencapabilitiesjava, :genrholisteners, :genrjava]

    #desc "Build Rhodes for android"
    task :rhodes => [:rhobundle, :librhodes, :manifest, :resources, :gensourcesjava] do
      print_timestamp('build:android:rhodes START')
      rm_rf $tmpdir + "/Rhodes"
      mkdir_p $tmpdir + "/Rhodes"

      srclist = File.join($builddir, "RhodesSRC_build.files")
      newsrclist = File.join($tmpdir, "RhodesSRC_build.files")
      lines = []
      File.open(srclist, "r") do |f|
        while line = f.gets
          line.chomp!
          next if line =~ /\/AndroidR\.java\s*$/

          lines << "\"#{line}\""
        end
      end
      Dir.glob(File.join($tmpdir,'gen','**','*.java')) do |filepath|
        lines << "\"#{filepath}\""
      end

      File.open(newsrclist, "w") { |f| f.write lines.join("\n") }
      srclist = newsrclist

      classpath = $androidjar
      classpath += $path_separator + $google_classpath if $google_classpath
      classpath += $path_separator + File.join($tmpdir, 'Rhodes')
      classpath += $path_separator + AndroidTools::MavenDepsExtractor.instance.classpath($path_separator)

      javalibsdir = Jake.get_absolute("platform/android/lib")

      Dir.glob( File.join(javalibsdir,"*.jar" )) do |filepath|
        classpath += $path_separator + filepath
      end

      javafilelists = [srclist]

      extlist = File.join $app_builddir, "ext_build.files"
      if File.exists? extlist
        puts "#{extlist} is found! THere are addditional java files"
        javafilelists << extlist
      end

      jar = File.join($app_builddir, 'librhodes', 'Rhodes.jar')      

      java_build(jar, File.join($tmpdir, 'Rhodes'), classpath, javafilelists, $java_version)

      $android_jars = [jar]
      print_timestamp('build:android:rhodes FINISH')
    end

    task :extensions_java => [:rhodes, :extensions] do
      print_timestamp('build:android:extensions_java START')
      puts 'Compile extensions java code'

      classpath = $androidjar

      # Deprecated! Just for backward compatibility #########################
      classpath += $path_separator + $google_classpath if $google_classpath
      #######################################################################

      classpath += $path_separator + AndroidTools::MavenDepsExtractor.instance.classpath($path_separator)
      classpath += $path_separator + File.join($tmpdir, 'Rhodes')
      Dir.glob(File.join($app_builddir, '**', '*.jar')).each do |jar|
        classpath += $path_separator + jar
      end

      javalibsdir = Jake.get_absolute("platform/android/lib")

      Dir.glob( File.join(javalibsdir,"*.jar" )) do |filepath|
        classpath += $path_separator + filepath
      end

      $ext_android_additional_sources.each do |extpath, list|
        ext = File.basename(extpath)
        ext_classpath = classpath
        ext_classpath += $path_separator + $ext_android_extra_classpath[ext] if $ext_android_extra_classpath[ext]
        srclist = Tempfile.new "#{ext}SRC_build"
        lines = get_sources(list)
        lines.each do |line|
          line.chomp!
          next if line.empty?

          srclist.write "\"#{File.join(extpath, line)}\"\n"
        end
        srclist.close

        buildpath = File.join($tmpdir, ext)

        mkdir_p buildpath unless File.exists? buildpath

        extjar = File.join $app_builddir, 'extensions', ext, ext + '.jar'

        java_build(extjar, buildpath, ext_classpath, [srclist.path], $java_version)

        $android_jars << extjar

        classpath += $path_separator + extjar
      end

      print_timestamp('build:android:extensions_java FINISH')
      $android_jars += AndroidTools::MavenDepsExtractor.instance.jars
    end

    task :upgrade_package => ["config:set_android_platform", "config:common"] do
      print_timestamp('build:android:upgrade_package START')

      $skip_build_rhodes_main = true
      $skip_build_extensions = true
      $skip_build_xmls = true
      $use_prebuild_data = true
      $skip_checking_Android_SDK = true

      Rake::Task["config:android"].invoke

      Rake::Task['build:android:rhobundle'].invoke

      #puts '$$$$$$$$$$$$$$$$$$'
      #puts 'targetdir = '+$targetdir.to_s
      #puts 'bindir = '+$bindir.to_s
      android_targetdir = $targetdir #File.join($targetdir, 'android')
      mkdir_p android_targetdir if not File.exists? android_targetdir
      zip_file_path = File.join(android_targetdir, 'upgrade_bundle.zip')
      #Jake.build_file_map(File.join($srcdir, "apps"), "rhofilelist.txt")

      src_folder = $appassets #File.join($appassets, 'RhoBundle')
      src_folder = File.join(src_folder, 'apps')

      tmp_folder = $appassets + '_tmp_partial'
      rm_rf tmp_folder if File.exists? tmp_folder
      mkdir_p tmp_folder

      dst_tmp_folder = File.join(tmp_folder, 'RhoBundle')
      mkdir_p dst_tmp_folder
      # copy all
      cp_r src_folder, dst_tmp_folder

      dst_tmp_folder = File.join(dst_tmp_folder, 'apps')
      mkdir_p dst_tmp_folder

      Jake.zip_upgrade_bundle(tmp_folder, zip_file_path)

      rm_rf tmp_folder

      print_timestamp('build:android:upgrade_package FINISH')
    end

    task :upgrade_package_partial => ["config:set_android_platform", "config:common"] do

      print_timestamp('build:android:upgrade_package_partial START')
      #puts '$$$$$$$$$$$$$$$$$$'
      #puts 'targetdir = '+$targetdir.to_s
      #puts 'bindir = '+$bindir.to_s

      # process partial update

      $skip_build_rhodes_main = true
      $skip_build_extensions = true
      $skip_build_xmls = true
      $use_prebuild_data = true
      $skip_checking_Android_SDK = true

      Rake::Task["config:android"].invoke

      Rake::Task['build:android:rhobundle'].invoke


      add_list_full_name = File.join($app_path, 'upgrade_package_add_files.txt')
      remove_list_full_name = File.join($app_path, 'upgrade_package_remove_files.txt')

      src_folder = $appassets #File.join($appassets, 'RhoBundle')
      src_folder = File.join(src_folder, 'apps')

      tmp_folder = $appassets + '_tmp_partial'
      rm_rf tmp_folder if File.exists? tmp_folder
      mkdir_p tmp_folder

      dst_tmp_folder = File.join(tmp_folder, 'RhoBundle')
      mkdir_p dst_tmp_folder
      # copy all
      cp_r src_folder, dst_tmp_folder

      dst_tmp_folder = File.join(dst_tmp_folder, 'apps')
      mkdir_p dst_tmp_folder

      add_files = []
      if File.exists? add_list_full_name
        File.open(add_list_full_name, "r") do |f|
          while line = f.gets
            fixed_path = line.gsub('.rb', '.iseq').gsub('.erb', '_erb.iseq').chomp
            add_files << fixed_path
            puts '### ['+fixed_path+']'
          end
        end
      end

      remove_files = []
      if File.exists? remove_list_full_name
        File.open(remove_list_full_name, "r") do |f|
          while line = f.gets
            fixed_path = line.gsub('.rb', '.iseq').gsub('.erb', '_erb.iseq').chomp
            remove_files << fixed_path
            #puts '### ['+fixed_path+']'
          end
        end
      end

      psize = dst_tmp_folder.size+1
      Dir.glob(File.join(dst_tmp_folder, '**/*')).sort.each do |f|
        relpath = f[psize..-1]

        if File.file?(f)
          #puts '$$$ ['+relpath+']'
          if (not add_files.include?(relpath)) && (relpath != 'rhofilelist.txt')
            rm_rf f
          end
        end
      end

      Jake.build_file_map(dst_tmp_folder, "upgrade_package_add_files.txt")

      #if File.exists? add_list_full_name
      #   File.open(File.join(dst_tmp_folder, 'upgrade_package_add_files.txt'), "w") do |f|
      #      add_files.each do |j|
      #         f.puts "#{j}\tfile\t0\t0"
      #      end
      #   end
      #end

      if File.exists? remove_list_full_name
        File.open(File.join(dst_tmp_folder, 'upgrade_package_remove_files.txt'), "w") do |f|
          remove_files.each do |j|
            f.puts "#{j}"
            #f.puts "#{j}\tfile\t0\t0"
          end
        end
      end

      mkdir_p $targetdir if not File.exists? $targetdir
      zip_file_path = File.join($targetdir, "upgrade_bundle_partial.zip")
      Jake.zip_upgrade_bundle(tmp_folder, zip_file_path)
      rm_rf tmp_folder

      print_timestamp('build:android:upgrade_package_partial FINISH')

    end


    #desc "build all"
    task :all => [:rhobundle, :rhodes, :extensions_java]
  end
end

def FormatManifestToAarCompat(path)
  doc = File.open(path) { |f| Nokogiri::XML(f) }
  root = doc.xpath("//manifest/*")
  application = root.xpath("//application/*")

  doc2 = doc.xpath("//manifest")
  doc2[0].remove_attribute("versionCode")
  doc2[0].remove_attribute("installLocation")
  doc2[0]["xmlns:tools"] = "http://schemas.android.com/tools"

  root.each do |node|
    node["tools:node"] = "merge" if node.name.to_s == 'uses-permission'
    node["tools:node"] = "merge" if node.name.to_s == 'uses-feature'
    if node.name.to_s == 'application'
      node["tools:node"] = "merge"
      node.remove_attribute("name")
      node.remove_attribute("label")
      node.remove_attribute("icon")
    end
  end

  application.each do |node|
    node["tools:node"] = "merge" if node["tools:node"] != "replace"
    if node["android:name"] == 'com.rhomobile.rhodes.RhodesActivity'
      intent_filters = node.xpath(".//*/*")
      intent_filters.each do |child|
        if child["android:name"] == 'android.intent.action.MAIN'
          child.parent.remove
        end
      end
    end
  end

  new_doc = Nokogiri::XML::Document.parse(doc.to_s, nil, "UTF-8")
  File.open(path, "w") {|file| file << new_doc.to_xml }
end


def prepare_aar_package
  alljars = Dir.glob(File.join($app_builddir, '**', '*.jar'))
  #alljars += AndroidTools::MavenDepsExtractor.instance.jars

  require 'set'
  unique_jars = Set.new

  args = ['cvf', 'classes.jar', '-C', '.', '.']
  jarNamesCounter = 0
  
  $allclasses = File.join($tmpdir, '.allclasses')
  mkdir_p $allclasses

  puts "Creating classes.jar for aar package"
  
  if File.exist?(File.join($allclasses, 'classes.jar'))
    rm_rf File.join($allclasses, 'classes.jar')
  end

  alljars.each do |jar|
    Zip::File.open(jar) do |archive|
      archive.glob("**/*.*").each do |pfile|
        next if !(pfile.name =~ /.*\.class$/)
        target = File.join($allclasses, pfile.name)
        mkdir_p File.dirname(target)
        rm target if File.file?(target)
        archive.extract( pfile, File.join(target) )
      end
    end
  end

  if(Dir.exists?(File.join($allclasses, 'com', $vendor , $appname.downcase)))  
    FileUtils.remove_dir File.join($allclasses, 'com', $vendor , $appname.downcase)
  elsif(Dir.exists?(File.join($allclasses, $app_package_name.split('.'))))  
    FileUtils.remove_dir File.join($allclasses, $app_package_name.split('.'))
  end

  #res_dirs = AndroidTools::MavenDepsExtractor.instance.aapt2_res_dirs
  #res_dirs.each do |dir|
  #  Dir.glob(File.join(dir,'**')) do |filepath|
  #    #cp_r filepath, File.join($tmpdir, 'res', File.basename(filepath) + '-' + File.basename(File.expand_path("..", dir)))
  #    cp_r filepath, File.join($tmpdir, 'res', File.basename(filepath))
  #  end
  #end

  Jake.run($jarbin, args, $allclasses)

  unless $?.success?
    raise "Error while creating classes.jar"
  end

  resourcepkg = File.join($bindir, "#{$appname}-#{$app_config["version"]}.aar")

  args = ["cvf", resourcepkg, 'classes.jar']
  Jake.run($jarbin, args, $allclasses)
  unless $?.success?
    raise "Error packaging classes.jar file"
  end

  mkdir_p File.join($tmpdir, 'jni')

  Dir.glob(File.join($tmpdir, 'lib', '*')).each do |lib|
    cp_r lib, File.join($tmpdir, 'jni')
  end

  args = ["uf", resourcepkg]
  Dir.glob(File.join(File.join($tmpdir, 'jni'),'**','lib*.so')).each do |jni|
    arch = File.basename(File.dirname(jni))
    args << "jni/#{arch}/#{File.basename(jni)}"
  end
  Jake.run($jarbin, args, $tmpdir)
  unless $?.success?
    raise "Error packaging native libraries"
  end

  args = ["uf", resourcepkg, 'R.txt']
  Jake.run($jarbin, args, $app_rjava_dir)
  unless $?.success?
    raise "Error packaging R.txt file"
  end

  args = ["uf", resourcepkg, '-C', '.', 'res']
  Jake.run($jarbin, args, $tmpdir)

  FormatManifestToAarCompat($appmanifest)
  args = ["uf", resourcepkg, 'AndroidManifest.xml']
  Jake.run($jarbin, args, File.dirname($appmanifest))

end

def create_bundle()
  $logger.info "Creating android bundle"

  builder = Aapt2Helper.instance

  bundle = builder.build_aab

  signed_bundle = File.join($targetdir,"#{$appname}-bundle-signed.aab")  

  builder.debug = $debug
  if $debug
    builder.keystore = $keystore
    builder.keypass = $keypass
    builder.storepass = $storepass
    builder.storealias = $storealias
  end
  builder.sign_aab( bundle, signed_bundle )  

  apk = File.join($targetdir,"#{$appname}-universal.apk")
  apks = File.join($targetdir,"#{$appname}.apks")
  rm apks if File.file? apks
  Jake.run( $javabin, ["-jar", $bundletool, "build-apks","--bundle=#{signed_bundle}", "--output=#{apks}", "--mode=universal"] )
  rm apk if File.file? apk
  Zip::File.open(apks) { |zip|
    e = zip.get_entry('universal.apk')
    zip.extract(e,apk)
  }


end

namespace "package" do
  task :android => "build:android:all" do
    print_timestamp('package:android START')

    if $rhodes_as_lib
      prepare_aar_package()
      next
    end
    
    $logger.info "Running DEX builder"    

    alljars = Dir.glob(File.join($app_builddir, '**', '*.jar'))
    alljars += AndroidTools::MavenDepsExtractor.instance.jars

    jarsForDX = []

    require 'set'
    unique_jars = Set.new

    useProguard = false;
    useProguard = $app_config["android"]["useproguard"] == "true" ? true : false if $app_config["android"]
    proguardUserRules = $app_config["android"]["proguardrules"] if $app_config["android"]

    proguardDir = File.join($startdir, "platform", "android", "proguard")
    rm_rf File.join($tmpdir, "proguard") if File.exists? File.join($tmpdir, "proguard")
    cp_r proguardDir, $tmpdir
    proguardDir = File.join($tmpdir, "proguard")
    proguardTempJarDir = File.join(proguardDir, "TempJars")
    proguardPreBuild = File.join(proguardTempJarDir, "preguard")
    proguardPostBuild = File.join(proguardTempJarDir, "postguard")
    proguardRules = File.join(proguardDir, 'proguard-base-rules.pro')
    if useProguard
      rm_rf proguardTempJarDir if File.exists? proguardTempJarDir
      mkdir_p proguardTempJarDir
      mkdir_p proguardPreBuild
      mkdir_p proguardPostBuild

      if !proguardUserRules.nil?
        rules = [File.join(proguardDir, 'proguard-base-rules.pro')]
        rules << File.join($app_path, proguardUserRules)
        f = File.open(File.join(proguardDir, 'proguard-generated-rules.pro'), "w")
        rules.each do |f_name|
          f_in = File.open(f_name, "r")
          f_in.each {|f_str| f.puts(f_str)}
          f.puts("\n\r")
          f_in.close
        end
        f.close
        proguardRules = File.join(proguardDir, 'proguard-generated-rules.pro')
      end

    end

    jarNamesCounter = 0
    alljars.each { |jar|
      basename = File.basename(jar);
      #FIXME: UGLYHACK FROM ALEX. Needs to add dependency filter to maven extractor
      isFromMaven = (basename == 'classes.jar')# && Pathname.new(jar).split.include?('.m2')
      if (!unique_jars.member?(basename)) || isFromMaven
        if !useProguard
          jarsForDX << jar
        else
          jarNamesCounter += 1
          cp jar, File.join(proguardPreBuild, jarNamesCounter.to_s + '.jar')
        end
        unique_jars.add(basename)
      end
    }

    if useProguard
      progArgs = [];
      progArgs << '-jar'
      progArgs << File.join(proguardDir, 'proguard.jar')
      progArgs << '@' + proguardRules
      Jake.run(File.join($java, 'java'+$exe_ext), progArgs)
      jarsForDX << File.join(proguardPostBuild, "classes-processed.jar")
    end

    unless AndroidTools::DexBuilder.instance.build( jarsForDX, "#{File.join($bindir,'classes.dex')}")
      raise "Error running DEX builder"
    end

    resourcepkg = $bindir + "/rhodes.ap_"

    $logger.info "Packaging Assets and Jars"

    Aapt2Helper.instance.build_intermediate_apk(resourcepkg)
   
    puts "Packaging Native Libs"

    args = ["uf", resourcepkg]
    Dir.glob(File.join($applibs,'**','lib*.so')).each do |lib|
      arch = File.basename(File.dirname(lib))
      args << "lib/#{arch}/#{File.basename(lib)}"

      unless not $debug
        gdbserver_path = File.join($androidndkpath, "prebuilt", "android-arm", "gdbserver", "gdbserver") #TODO: packing gdbserver for all platforms
        if File.exist?(gdbserver_path)
          path_to_lib = File.join $tmpdir, 'lib'
          cp_r gdbserver_path, File.join(path_to_lib, arch)
          args << "lib/#{arch}/gdbserver"
        else
          puts "gdbserver not found!"
        end
      end

    end
    Jake.run($jarbin, args, $tmpdir)
    unless $?.success?
      raise "Error packaging native libraries"
    end

    $logger.info( "Packaging java resources" )

    respath = File.join( $tmpdir, '.javares' )
    rm_r respath if File.directory? respath

    alljars.each do |jar|
      Zip::File.open(jar) do |archive|
        archive.glob("**/*.*").each do |pfile|
          next if pfile.name =~ /^META-INF/
          #extract everything except .class files
          next if pfile.name =~ /.*\.class$/
          target = File.join(respath,pfile.name)
          mkdir_p( File.dirname(target) )
          rm target if File.file?(target)
          archive.extract( pfile, File.join(target) )
        end
      end
    end

    args = [ Rake.application.options.trace ? "uvf" : "uf", resourcepkg]

    Dir.glob(File.join(respath,"**/*")).each do |f|
      next if not File.file?(f)
      relpath  = Pathname.new(f).relative_path_from(Pathname.new(respath)).to_s
      $logger.debug "Will pakage java resource: #{f} into #{relpath}"
      args << relpath
    end

    Jake.run($jarbin, args, respath) if File.directory?(respath)
    rm_rf proguardTempJarDir if File.exists? proguardTempJarDir

    if $build_bundle
      create_bundle()
    end

    puts "Finish packaging"
    print_timestamp('package:android FINISH')
  end
end

namespace "device" do
  namespace "android" do

    desc "Build debug self signed for device"
    task :debug => "package:android" do
      print_timestamp('device:android:debug START')

      final_apkfile = File.join $targetdir, "#{$appname}-debug.apk"

      builder = ApkBuilder.new
      builder.sdk_path = $androidsdkpath
      builder.dex_path = File.join $bindir, "classes.dex"
      builder.output_path = final_apkfile
      builder.res_pkg = File.join $bindir, "rhodes.ap_"
      builder.debug = true

      builder.build

      File.open(File.join(File.dirname(final_apkfile), "app_info.txt"), "w") do |f|
        f.puts $app_package_name
      end
      print_timestamp('device:android:debug FINISH')
    end

    task :install => :debug do
      apkfile = $targetdir + "/" + $appname + "-debug.apk"
      Jake.run $adb, ['-d', 'wait-for-device']
      puts "Install APK file"
      Jake.run($adb, ["-d", "install", "-r", apkfile])
      unless $?.success?
        raise "Error installing APK file"
      end
      puts "Install complete"
    end

    task :studio_build => ['config:android'] do
      $skip_so_build = true
      $abis = $app_config['android']['abis'] if $app_config["android"]
      $abis = ['arm'] unless $abis
      abi = $abis[0]

      if abi == 'arm'
        realabi = 'armeabi-v7a'
      else
        realabi = abi
      end



      project_app_path = File.join $app_path,'project','android_studio', 'app'
      librhodes_path = File.join project_app_path, 'build', 'intermediates',
                                 'cmake', $debug ? 'debug' : 'release', 'obj', realabi, 'librhodes.so'

      target_path = File.join $app_builddir, 'librhodes', 'lib', realabi
      if !File.exists?(target_path)
        mkdir_p target_path
      end
      cp_r librhodes_path, target_path
      Rake::Task["device:android:production"].invoke
    end

    desc "Build production signed for device"
    task :production => "package:android" do
      print_timestamp('device:android:production START')

      if not File.exists? $keystore
        AndroidTools.generateKeystore( $keystore, $storealias, $storepass, $keypass )
      end

      final_apkfile = File.join $targetdir, "#{$appname}_signed.apk"

      builder = ApkBuilder.new
      builder.sdk_path = $androidsdkpath
      builder.dex_path = File.join $bindir,"classes.dex"      
      builder.output_path = final_apkfile
      builder.res_pkg = File.join $bindir, "rhodes.ap_"
      builder.debug = false
      builder.keystore = $keystore
      builder.keypass = $keypass
      builder.storepass = $storepass
      builder.storealias = $storealias

      builder.build

      File.open(File.join(File.dirname(final_apkfile), "app_info.txt"), "w") do |f|
        f.puts $app_package_name
      end
      print_timestamp('device:android:production FINISH')
    end

    #task :getlog => "config:android" do
    #  AndroidTools.get_app_log($appname, true) or exit 1
    #end
  end
end

#namespace "emulator" do
#  namespace "android" do
#    task :getlog => "config:android" do
#      AndroidTools.get_app_log($appname, false) or exit 1
#    end
#  end
#end

def run_as_spec(device_flag, uninstall_app, gmsaas_recipe_uuid = "")
  Rake::Task["device:android:debug"].invoke

  if gmsaas_recipe_uuid.to_s != ''
    gmsaas_android_instance = AndroidTools.run_gmsaas_emulator(gmsaas_recipe_uuid)
  elsif device_flag == '-e'
    Rake::Task["config:android:emulator"].invoke
  else
    Rake::Task["config:android:device"].invoke
  end

  log_name = $app_path + '/RhoLogSpec.txt'
  File.delete(log_name) if File.exist?(log_name)

  AndroidTools.logclear(device_flag)
  # Start emulator with options: hidden window display and wipe user data
  AndroidTools.run_emulator(:hidden => true, :wipe => true) if device_flag == '-e' && gmsaas_recipe_uuid.to_s == ''

  begin
    do_uninstall(device_flag)
  rescue Exception => e
    puts "Uninstalling application exception: " + e.to_s
  end

  # Failsafe to prevent eternal hangs
  Thread.new {
    sleep 10000
    if device_flag == '-e'
      #AndroidTools.kill_adb_and_emulator
      puts "%%% was AndroidTools.kill_adb_and_emulator !!!"
    else
      #AndroidTools.kill_adb_logcat device_flag, log_name
      puts "%%% was AndroidTools.kill_adb_logcat device_flag, log_name !!!"
    end
  }

  apkfile = File.expand_path(File.join $targetdir, $appname + "-debug.apk")
  AndroidTools.load_app_and_run(device_flag, apkfile, $app_package_name)
  AndroidTools.logcat(BuildConfig.get_key('android/logcatFilter',nil), device_flag, log_name)

  Jake.before_run_spec
  start = Time.now

  puts "CurTime is "+Time.now.to_s
  puts "Waiting for application ..."


  tc_start_time = Time.now
  tc_overtime = false
  tc_timeout_in_seconds = 10*60

  while !tc_overtime do
    app_is_running = AndroidTools.application_running(device_flag, $app_package_name)
    $logger.debug "%%% app_is_running="+app_is_running.to_s
    if app_is_running
      tc_overtime = true
    end
    if ((Time.now.to_i - tc_start_time.to_i) > tc_timeout_in_seconds)
      tc_overtime = true
    end
    sleep(5) unless tc_overtime
  end
  sleep(1)
  puts "CurTime is "+Time.now.to_s
  app_is_runningz = AndroidTools.application_running(device_flag, $app_package_name)
  puts "%%% app_is_runningz FINAL ="+app_is_runningz.to_s
  sleep(1)

  puts "Waiting for log file: #{log_name}"

  tc_start_time = Time.now
  tc_overtime = false
  tc_timeout_in_seconds = 5*60

  while !tc_overtime do
    if File.exist?(log_name)
      tc_overtime = true
    end
    if ((Time.now.to_i - tc_start_time.to_i) > tc_timeout_in_seconds)
      tc_overtime = true
    end
    sleep(5) unless tc_overtime
  end

  puts "CurTime is "+Time.now.to_s

  logfile_is_exist = File.exist?(log_name)
  puts "%%% logfile is exist FINAL ="+logfile_is_exist.to_s

  if !File.exist?(log_name)
    puts "Cannot read log file: " + log_name
    exit(1)
  end


  timeout_in_seconds = 25*60

  timeout_output_in_seconds = 3*60
  last_output_time = Time.now

  log_lines = []

  start_logging = Time.now
  is_timeout = false

  last_spec_line = ""
  last_spec_iseq_line = ""

  is_correct_stop = false
  is_exit_by_app_not_run = false

  app_exit_start_time = nil
  app_exit_timeout_in_seconds = 60
  app_is_running = true

  puts "Start reading log ..."
  File.open(log_name, 'r:UTF-8') do |io|
    $logger.debug "%%% io="+io.to_s
    end_spec = false
    while !end_spec do
      $logger.debug "%%% while start"
      io.each do |line|
        #puts "%%% line="+line.to_s
        $logger.debug line
        log_lines << line
        if line.class.method_defined? "valid_encoding?"
          end_spec = !Jake.process_spec_output(line) if line.valid_encoding?
        else
          end_spec = !Jake.process_spec_output(line)
        end

        is_mspec_stop = line =~ /MSpec runner stopped/
        is_terminated = line =~ /\| \*\*\*Terminated\s+(.*)/

        is_correct_stop = true if is_mspec_stop || is_terminated

        end_spec = true if is_mspec_stop

        if end_spec
          puts "%%% stop spec !"
          puts "%%% stop spec by this line : ["+line.to_s+"]"
        end

        last_spec_line = line if line =~ /_spec/
        last_spec_iseq_line = line if line =~ /_spec.iseq/

        #check for timeout
        if (Time.now.to_i - start_logging.to_i) > timeout_in_seconds
          end_spec = true
          is_timeout = true
          puts "%%% TIMEOUT !!!"
        end
        if (Time.now.to_i - last_output_time.to_i) > timeout_output_in_seconds
          last_output_time = Time.now
          puts "%%% please wait - application still running ..."
        end

        break if end_spec
      end
      if (Time.now.to_i - last_output_time.to_i) > timeout_output_in_seconds
        last_output_time = Time.now
        puts "%%% please wait - application still running ..."
      end
      if app_is_running
        app_is_running = AndroidTools.application_running(device_flag, $app_package_name)
        puts "%%% application is not runned on simulator !!!" if !app_is_running
        is_exit_by_app_not_run = !app_is_running
        if !app_is_running
          app_exit_start_time = Time.now
        end
      end

      if !app_is_running
        if (Time.now.to_i - app_exit_start_time.to_i) > app_exit_timeout_in_seconds
          end_spec = true
          puts "%%% application is not runned - stop waiting dealy for log processing !"
        end
      end
      #break unless app_is_running
      sleep(5) unless end_spec
    end
  end

  puts "Processing spec results ..."
  Jake.process_spec_results(start)

  if is_timeout || (is_exit_by_app_not_run && !is_correct_stop)  || !is_correct_stop
    puts "Tests has issues : is_timeout["+is_timeout.to_s+"], timeout["+timeout_in_seconds.to_s+" sec], app_exit_unexpected["+is_exit_by_app_not_run.to_s+"], not_correct_terminated_line["+(!is_correct_stop).to_s+"] !"
    puts "last_spec_line = ["+last_spec_line.to_s+"]"
    puts "last_spec_iseq_line = ["+last_spec_iseq_line.to_s+"]"
    puts "last spec executed = ["+$latest_test_line.to_s+"]"
    puts "This is last 5000 lines from log :"
    idx = log_lines.size-5000
    if idx < 0
      idx = 0
    end
    while idx < log_lines.size
      puts "line ["+idx.to_s+"]: "+log_lines[idx]
      idx = idx + 1
    end
  end

  # stop app
  #uninstall_app = true if uninstall_app.nil? # by default uninstall spec app
  #do_uninstall(device_flag) if uninstall_app and ((device_flag != '-e') or AndroidTools.is_emulator_running)
  #if device_flag == '-e'
  #AndroidTools.kill_adb_and_emulator
  #else
  #AndroidTools.kill_adb_logcat(device_flag, log_name)
  #end

  if gmsaas_recipe_uuid.to_s != ''
    AndroidTools.stop_gmsaas_emulator(gmsaas_android_instance)
  end

  $stdout.flush

  exit 1 if is_timeout
  exit 1 if $total.to_i==0
  exit 1 if !is_correct_stop
  exit $failed.to_i

end

namespace "run" do
  namespace "android" do

    task :build => ['run:android:emulator:build']
    task :run   => ['run:android:emulator:run'  ]
    task :debug => ['run:android:emulator:debug']

    desc "Run downloaded binary package on emulator"
    task "simulator:package", [:package_file, :package_name] => ['config:android:emulator'] do |t, args|
      package_file = args.package_file
      package_name = args.package_name.nil? ? $app_package_name : args.package_name

      throw "You must pass package name" if package_file.nil?
      throw "No file to run" if !File.exists?(package_file)

      AndroidTools.run_emulator(:hidden => ((ENV['TRAVIS'] != nil) && (ENV['TRAVIS'] != "")))

      AndroidTools.load_app_and_run('-e', File.expand_path(package_file), package_name)
    end

    desc "Run downloaded binary package on device"
    task "device:package", [:package_path, :device_id] => ['config:android:device'] do |t, args|
      args.with_defaults(:device_id => '-d')

      throw "You must pass path to package" if args.package_path.nil?

      package_path = File.expand_path(args.package_path)

      throw "Package file does not exist: #{package_path}" if !File.exists?(package_path)

      AndroidTools.load_app_and_run(args.device_id, package_path, $app_package_name)
    end

    task :spec => "run:android:emulator:spec" do
    end

    task :get_log => "config:android" do
      puts "log_file=" + $applog_path
    end

    task :emulator => ['run:android:emulator:build', 'run:android:emulator:run']

    namespace "emulator" do

      task :build => ['device:android:debug']
      task :debug => ['run:android:emulator:run']
      task :run => ['config:android:emulator'] do
        AndroidTools.kill_adb_logcat('-e')
        AndroidTools.run_emulator(:hidden => ((ENV['TRAVIS'] != nil) && (ENV['TRAVIS'] != "")))

        apkfile = File.expand_path(File.join $targetdir, $appname + "-debug.apk")
        AndroidTools.load_app_and_run('-e', apkfile, $app_package_name)

        AndroidTools.logcat_process(BuildConfig.get_key('android/logcatFilter',nil),'-e')
      end

      task :spec, :uninstall_app do |t, args|
          Jake.decorate_spec { run_as_spec('-e', args.uninstall_app) }
      end
    end

    desc "build, install and run on device"
    task :device, [:device_id] => ['run:android:device:build', 'run:android:device:run']

    namespace "gmsaas" do
      task :spec, [:arg1] do |t, args|
        Jake.decorate_spec { run_as_spec('-e', args.uninstall_app, args[:arg1]) }
      end
    end

    namespace "device" do

      task :build => ['device:android:debug']
      task :debug => ['run:android:device:run']
      task :run, [:device_id] => ['config:android'] do |t, args|
        args.with_defaults(:device_id => '-d')
        AndroidTools.kill_adb_logcat(args.device_id)

        apkfile = File.join $targetdir, $appname + "-debug.apk"
        AndroidTools.load_app_and_run(args.device_id, apkfile, $app_package_name)

        AndroidTools.logcat_process(BuildConfig.get_key('android/logcatFilter',nil),args.device_id)
      end

      task :spec, :uninstall_app do |t, args|
        Jake.decorate_spec { run_as_spec('-d', args.uninstall_app) }
      end
    end

    rhosim_task = lambda do |name, &block|
      task name => ["config:set_android_platform", "config:common"] do
        $emuversion = $app_config["android"]["version"] unless $app_config["android"].nil?
        $emuversion = $config["android"]["version"] if $emuversion.nil? and !$config["android"].nil?
        $rhosim_config = "platform='android'\r\n"
        $rhosim_config += "os_version='#{$emuversion}'\r\n" if $emuversion
        block.()
      end
    end

    desc "Run application on RhoSimulator"
    rhosim_task.(:rhosimulator) { Rake::Task["run:rhosimulator"].invoke }
    namespace :rhosimulator do
      rhosim_task.(:build) { Rake::Task["run:rhosimulator:build"].invoke         }
      rhosim_task.(:debug) { Rake::Task["run:rhosimulator:run"  ].invoke('wait') }
    end
  end

  desc "build and launch emulator"
  task :android => "run:android:emulator"
end

namespace "uninstall" do
  def do_uninstall(flag)
    args = []
    args << flag
    args << "uninstall"
    args << $app_package_name
    for i in 0..20
      result = Jake.run($adb, args)
      unless $?.success?
        puts "Error uninstalling application"
        exit 1
      end

      if result.include?("Success")
        puts "Application uninstalled successfully"
        break
      else
        if result.include?("Failure")
          puts "Application is not installed on the device"
          break
        else
          puts "Error uninstalling application"
          exit 1 if i == 20
        end
      end
      sleep(5)
    end

  end

  namespace "android" do
    task :emulator => "config:android" do
      unless AndroidTools.is_emulator_running
        puts "WARNING!!! Emulator is not up and running"
        exit 1
      end
      do_uninstall('-e')
    end

    desc "uninstall from device"
    task :device => "config:android" do
      unless AndroidTools.is_device_running
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
  task :android => ["clean:android:all", "clean:common"]

  namespace "android" do
    task :files => "config:android" do
      rm_rf $targetdir
      rm_rf $app_builddir
      Dir.glob(File.join($bindir, "*.*")) { |f| rm f, :force => true }
      rm_rf $srcdir
      rm_rf $tmpdir
    end
    task :all => :files
  end
end

namespace :stop do
  namespace :android do
    namespace :debug do

      def killRuby
        if RUBY_PLATFORM =~ /windows|cygwin|mingw/
          # Windows
          `taskkill /F /IM ruby.exe`
        else
          `killall -9 ruby`
        end
      end

      task :emulator do #=> "stop:android:emulator"do
        AndroidTools.kill_adb_logcat('-e')
        killRuby
      end

      task :device do #=> "stop:android:device" do
        AndroidTools.kill_adb_logcat('-d')
        killRuby
      end
    end #end of debug

    task :emulator do
      AndroidTools.kill_adb_and_emulator
    end

    task :device do
      device_flag = '-d'
      do_uninstall(device_flag)
      log_name = $app_path + '/RhoLogSpec.txt'
      AndroidTools.kill_adb_logcat device_flag, log_name
    end
  end
end
