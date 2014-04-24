require File.join(pwd, 'lib/build/jake.rb')
require 'fileutils'
require 'rhodes/containers'

namespace 'device' do
  namespace 'android' do
    task :make_container, :target_path => 'device:android:production' do |t,args|

      target_path = args[:target_path]

      puts "Target path for prebuilt binaries: #{args}"

      FileUtils.mkdir_p target_path

      FileUtils.cp( File.join($bindir,'classes.dex'), target_path )

      FileUtils.mkdir_p File.join(target_path,'native','lib')

      Dir.glob(File.join($bindir,'tmp','**','lib*.so')) { |lib|
        arch = File.basename(File.dirname(lib))
        FileUtils::mkdir_p File.join(target_path,'native','lib',arch)
        FileUtils.cp(lib, File.join(target_path,'native','lib',arch))
      }

      FileUtils.cp( File.join($app_path,'build.yml'), target_path )
      
    end

    def determine_prebuild_path(config)
      return Rhodes::Containers::get_container_path_prefix('android', config)
    end

    def make_app_bundle
      $use_prebuild_data = true
      Rake::Task['build:android:rhobundle'].execute
      return $appassets
    end

    def generate_manifest(prebuilt_config,app_config)

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

      usesPermissions = ['android.permission.INTERNET', 'android.permission.PERSISTENT_ACTIVITY', 'android.permission.WAKE_LOCK']

      capabilities = []
      capabilities.concat(app_config['capabilities']) if app_config['capabilities']
      capabilities.concat(prebuilt_config['capabilities']) if prebuilt_config['capabilities']
      capabilities.uniq!

      capabilities.each do |cap|
        cap = ANDROID_PERMISSIONS[cap]
        next if cap.nil?
        cap = [cap] unless cap.is_a? Array

        cap.each do |cap_item|
          if cap_item.is_a? Proc
            #caps_proc << cap_item
            next
          end

          if cap_item.is_a? String
            usesPermissions << "android.permission.#{cap_item}"
            next
          end
        end
      end
      usesPermissions.uniq!

      hidden = get_boolean(prebuilt_config['hidden_app'])

      generator = ManifestGenerator.new JAVA_PACKAGE_NAME, $app_package_name, hidden, usesPermissions

      generator.versionName = prebuilt_config["version"]
      generator.versionCode = version
      generator.installLocation = 'auto'
      generator.minSdkVer = $min_sdk_level
      generator.maxSdkVer = $max_sdk_level
      generator.screenOrientation = $android_orientation unless $android_orientation.nil?
      generator.hardwareAcceleration = true if $app_config["capabilities"].index('hardware_acceleration')
      generator.apikey = $gapikey if $gapikey

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
      puts 'Manifest updated by extension is saved!'

      return $appmanifest
    end

    def build_resources(prebuilt_builddir)
      set_app_name_android($appname)

      puts 'EXT:  add additional files to project before build'
      Dir.glob(File.join(prebuilt_builddir, 'extensions', '*', 'adds', '*')).each do |res|
        if File.directory?(res) && (res != '.') && (res != '..')
          puts "add resources from extension [#{res}] to [#{$tmpdir}]"
          cp_r res, $tmpdir
        end
      end

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
      # Add .so libraries
      Dir.glob($app_builddir + "/**/lib*.so").each do |lib|
        arch = File.basename(File.dirname(lib))
        file = File.basename(lib)
        cp_r lib, File.join($applibs,arch,file)
      end
=begin
      $ext_android_additional_lib.each do |lib|
        arch = File.basename(File.dirname(lib))
        file = File.basename(lib)
        cp_r lib, File.join($applibs,arch,file)
      end
=end
      return $appres
    end

    def get_underscore_files_from_bundle(bundle_path)
      underscores = []

      Dir.glob(File.join(bundle_path, "**/*")).each do |f|
        next unless File.basename(f) =~ /^_/
        relpath = Pathname.new(f).relative_path_from(Pathname.new($tmpdir)).to_s
        undersrores << relpath
      end

      return underscores
    end

    def get_native_libs_path(prebuilt_path)
      return File.join(prebuilt_path,'native')
    end

    def make_output_path
      return $targetdir + '/' + $appname + '_signed.apk'
    end

    def make_package(manifest_path, resources_path, assets_path, underscore_files, native_libs_path, classes_dex, output_path)

      resourcepkg = $bindir + "/rhodes.ap_"

      puts "Packaging Assets and Jars"

      args = ["package", "-f", "-M", manifest_path, "-S", resources_path, "-A", assets_path, "-I", $androidjar, "-F", resourcepkg]
      if $no_compression
        $no_compression.each do |ext|
          args << '-0'
          args << ext
        end
      end

      Jake.run($aapt, args)
      unless $?.success?
        raise "Error running AAPT (1)"
      end

      # Workaround: manually add files starting with '_' because aapt silently ignore such files when creating package
      underscore_files.each do |relpath|
        puts "Add #{relpath} to #{resourcepkg}..."
        args = ["uf", resourcepkg, relpath]
        Jake.run($jarbin, args, $tmpdir)
        unless $?.success?
          raise "Error packaging assets"
        end
      end

      puts "Packaging Native Libs"

      args = ["uf", resourcepkg]
      Dir.glob(File.join(native_libs_path,'**','lib*.so')) do |lib|
        arch = File.basename(File.dirname(lib))
        args << "lib/#{arch}/#{File.basename(lib)}"
      end
      Jake.run($jarbin, args, native_libs_path)
      unless $?.success?
        raise "Error packaging native libraries"
      end

      dexfile = classes_dex
      simple_apkfile = $targetdir + "/" + $appname + "_tmp.apk"
      final_apkfile = output_path
      signed_apkfile = $targetdir + "/" + $appname + "_tmp_signed.apk"
      resourcepkg = $bindir + "/rhodes.ap_"

      apk_build $androidsdkpath, simple_apkfile, resourcepkg, dexfile, false

      if not File.exists? $keystore
        puts "Generating private keystore..."
        mkdir_p File.dirname($keystore) unless File.directory? File.dirname($keystore)

        args = []
        args << "-genkey"
        args << "-alias"
        args << $storealias
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
        Jake.run($keytool, args)
        unless $?.success?
          puts "Error generating keystore file"
          exit 1
        end
      end

      puts "Signing APK file"
      args = []
      args << "-sigalg"
      args << "MD5withRSA"
      args << "-digestalg"
      args << "SHA1"
      args << "-verbose"
      args << "-keystore"
      args << $keystore
      args << "-storepass"
      args << $storepass
      args << "-signedjar"
      args << signed_apkfile
      args << simple_apkfile
      args << $storealias
      Jake.run($jarsigner, args)
      unless $?.success?
        puts "Error running jarsigner"
        exit 1
      end

      puts "Align APK file"
      args = []
      args << "-f"
      args << "-v"
      args << "4"
      args << '"' + signed_apkfile + '"'
      args << '"' + final_apkfile + '"'
      Jake.run($zipalign, args)
      unless $?.success?
        puts "Error running zipalign"
        exit 1
      end
      #remove temporary files
      rm_rf simple_apkfile
      rm_rf signed_apkfile

      File.open(File.join(File.dirname(final_apkfile), "app_info.txt"), "w") do |f|
        f.puts $app_package_name
      end

    end


    task :production_with_prebuild_binary do
      production_with_prebuild_binary
    end

    def production_with_prebuild_binary
      Rake::Task['config:android'].invoke

      prebuilt_path = determine_prebuild_path($app_config)
      bundle_path = make_app_bundle


      prebuilt_config = Jake.config(File.open(File.join(prebuilt_path, 'build.yml')))

      manifest_path = generate_manifest(prebuilt_config,$app_config)

      prebuilt_builddir = File.join(bundle_path,'bin','target','android',$confdir)
      resources_path = build_resources(prebuilt_builddir)

      assets_path = bundle_path

      underscore_files = get_underscore_files_from_bundle(bundle_path)

      #jars = build_java

      native_libs_path = get_native_libs_path(prebuilt_path)

      output_path = make_output_path

      classes_dex = File.join(prebuilt_path,'classes.dex')
      make_package(manifest_path,resources_path,assets_path,underscore_files,native_libs_path, classes_dex, output_path)
    end
  end
end


=begin
namespace 'build' do
  namespace 'android' do

    def get_bundle_map( stream )
      bundle_map = {}

      stream.each { |line|
        vals = line.split('|')

        fpath = vals[0]
        type = vals[1]
        len = vals[2].to_i
        timestamp = vals[3].strip!.to_i

        bundle_map[fpath] = { :type => type, :len => len, :timestamp => timestamp }
      }
      return bundle_map
    end

    task :repack do
      puts 'Repacking final APK with updated bundle'

      Rake::Task['config:android'].execute
      Rake::Task['build:android:rhobundle'].execute

      if File.exists?(resourcepkg) then
        puts 'Make diff maps and repack only changed files'

        begin
          zipfile = Zip::File.open(resourcepkg)

          packed_items = {}

          zipfile.get_input_stream('assets/RhoBundleMap.txt') do |filelist|
            packed_items = get_bundle_map(filelist)
          end

          zipfile.close

          bundle_items = {}

          File.open( File.join( $appassets, 'RhoBundleMap.txt' ) ) do |filelist|
            bundle_items = get_bundle_map(filelist)
          end

          remove_items = []
          add_items = []

          packed_items.each { |key,value| remove_items << key if (!bundle_items.has_key?(key) and value[:type]=='file') }

          bundle_items.each { |key,value|
            if value!=packed_items[key] and value[:type]=='file' then
              add_items << key unless key=='rho.dat'
              remove_items << key if packed_items.has_key?(key)
            end
          }

          currentdir = Dir.pwd()
          Dir.chdir $appassets+'/..'

          unless remove_items.empty? then
            args = [ 'remove', resourcepkg ]

            remove_items.each { |key|
              puts "Removing item from package: #{key}"
              args << 'assets/'+key
              packed_items.delete(key)
            }

            Jake.run($aapt, args)
            unless $?.success?
              raise "Error running AAPT (1)"
            end
          end

          unless add_items.empty? then
            args = [ 'add', resourcepkg ]

            add_items.each { |key|
              puts "Adding item to package: #{key}:#{bundle_items[key]}"
              args << 'assets/'+key
            }

            Jake.run($aapt, args)
            unless $?.success?
              raise "Error running AAPT (1)"
            end
          end


          has_changes = !(remove_items.empty? and add_items.empty?)

          if has_changes then
            puts 'Replacing bundle map and commiting package changes'

            Jake.run($aapt, ['remove',resourcepkg,'assets/RhoBundleMap.txt','assets/rho.dat'])
            unless $?.success?
              raise "Error running AAPT (1)"
            end

            Jake.run($aapt, ['add',resourcepkg,'assets/RhoBundleMap.txt','assets/rho.dat'])
            unless $?.success?
              raise "Error running AAPT (1)"
            end
          else
            puts "No changes detected in the bundle, do nothing"
          end

          Dir.chdir currentdir

          #zipfile.close

        rescue => e
          puts "EXCEPTION: #{e.inspect}"
          raise e
        end

        Rake::Task['device:android:production'].execute if has_changes

      else
        puts 'Pack everything from scratch'
        Rake::Task['device:android:production'].invoke
      end
    end
  end
end
=end