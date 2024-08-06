require File.join(pwd, 'lib/build/jake.rb')
require 'fileutils'

namespace 'device' do
  namespace 'android' do
    task :make_container, [:target_path] => 'device:android:production' do |t,args|

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
      
      cp_r( File.join($bindir,'tmp', 'assets' ), File.join( target_path, 'assets' ) )
      cp_r( File.join($bindir,'tmp', 'res' ), File.join( target_path, 'res' ) )

      FileUtils.cp( File.join($app_path,'build.yml'), target_path )
      
      #save manifest changes
      $ext_android_manifest_changes.each do |ext, manifest_changes|
        addspath = File.join(target_path, 'extensions', ext, 'adds')
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
      
    end

  module AndroidPrebuild

    def self.determine_prebuild_path(config)
      RhoPackages.request 'rhodes-containers'
      require 'rhodes/containers'
      Rhodes::Containers::get_container_path_prefix('android', config)
    end

    def self.make_app_bundle
      print_timestamp('AndroidPrebuild.make_app_bundle START')
      $use_prebuild_data = true
      $skip_build_rhodes_main = true
      $skip_build_extensions = true
      $skip_build_xmls = true
      $skip_build_js_api_files = true

      Rake::Task['build:android:rhobundle'].execute
      print_timestamp('AndroidPrebuild.make_app_bundle FINISH')
      return $appassets
    end

    def self.generate_manifest(prebuilt_path,prebuilt_config,app_config)
      print_timestamp('AndroidPrebuild.generate_manifest START')
      version = {'major' => 0, 'minor' => 0, 'patch' => 0, "build" => 0}
      versionName = '0.0'
      if $app_config["version"]
        versionName = $app_config["version"]
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
      capabilities.uniq!

      capabilities.each do |cap|
        cap = ANDROID_PERMISSIONS[cap]
        next if cap.nil?
        cap = [cap] unless cap.is_a? Array

        cap.each do |cap_item|
          if cap_item.is_a? String
            usesPermissions << "android.permission.#{cap_item}"
            next
          end
        end
      end
      usesPermissions.uniq!

      hidden = get_boolean(prebuilt_config['hidden_app'])
      home_app = false
      if app_config['android'] != nil
          if app_config['android']['home_app'] != nil
              home_app = app_config['android']['home_app']
          end
      end


      generator = ManifestGenerator.new JAVA_PACKAGE_NAME, $app_package_name, hidden, home_app, usesPermissions

      generator.versionName = versionName
      generator.versionCode = version
      generator.installLocation = 'auto'
      generator.minSdkVer = $min_sdk_level
      generator.targetSdkVer = $target_sdk_level
      generator.maxSdkVer = $max_sdk_level
      generator.screenOrientation = $android_orientation unless $android_orientation.nil?
      generator.hardwareAcceleration = true if $app_config["capabilities"].index('hardware_acceleration')
      generator.apikey = $gapikey if $gapikey

      generator.addUriParams $uri_scheme, $uri_host, $uri_path_prefix

      puts "Apply app's extensions manifest changes in generator..."
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

      puts "Apply container's extensions manifest changes in generator..."
      $app_config['extensions'].each { |ext|
        addspath = File.join(prebuilt_path,'extensions',ext,'adds')
        if (File.directory?(addspath))
        
            extscript = File.join(addspath,'AndroidManifest.rb')
            if (File.file?(extscript))
                puts "Evaluating #{extscript}"
                eval(File.new(extscript).read)
            end

            Dir.glob(File.join(addspath, 'Manifest*.erb')).each do |exttemplate|
                puts "Adding template #{exttemplate}"
                generator.manifestManifestAdds << exttemplate
            end

            Dir.glob(File.join(addspath, 'Application*.erb')).each do |exttemplate|
                puts "Adding template #{exttemplate}"
                generator.applicationManifestAdds << exttemplate
            end
            

        end
        
      }

      manifest = generator.render $rhomanifesterb
      File.open($appmanifest, "w") { |f| f.write manifest }

      ext_manifest_changes = []

      puts "Collecting legacy manifest changes for container extensions..."
      $app_config['extensions'].each { |ext|
        extmanifest = File.join(prebuilt_path,'extensions',ext,'adds','AndroidManifest.xml')
        if (File.file?(extmanifest))
            ext_manifest_changes << extmanifest
        end
      }

      puts "Collecting legacy manifest changes for app extensions..."
      Dir.glob(File.join($app_builddir, 'extensions', '*', 'adds', 'AndroidManifest.xml')).each do |ext_manifest|
        if File.file? ext_manifest
            ext_manifest_changes << ext_manifest
        end
      end

      puts "Applying legacy manifest changes..."
      apply_manifest_ext_changes($appmanifest,ext_manifest_changes)
      print_timestamp('AndroidPrebuild.generate_manifest FINISH')
      return $appmanifest
    end

    def self.apply_manifest_ext_changes(target_manifest, manifest_changes)
      #######################################################
      # Deprecated staff below
      print_timestamp('AndroidPrebuild.apply_manifest_ext_changes START')

      app_f = File.new(target_manifest)
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

      manifest_changes.each do |ext_manifest|

        if File.exist? ext_manifest
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
      File.delete(target_manifest)

      updated_f = File.open(target_manifest, "w")
      manifest_orig_doc.write updated_f, 2
      updated_f.close
      print_timestamp('AndroidPrebuild.apply_manifest_ext_changes FINISH')
      puts 'Manifest updated by extension is saved!'
    end

    def self.build_resources(prebuilt_builddir)
      print_timestamp('AndroidPrebuild.build_resources START')
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
      print_timestamp('AndroidPrebuild.build_resources FINISH')
      return $appres
    end

    def self.get_underscore_files_from_bundle(bundle_path)
      underscores = []

      Dir.glob(File.join(bundle_path, "**/*")).each do |f|
        next unless File.basename(f) =~ /^_/
        relpath = Pathname.new(f).relative_path_from(Pathname.new($tmpdir)).to_s
        underscores << relpath
      end

      return underscores
    end

    def self.get_native_libs_path(prebuilt_path)
      return File.join(prebuilt_path,'native')
    end

    def self.make_output_path
      return $targetdir + '/' + $appname + '_signed.apk'
    end

    def self.make_package(manifest_path, resources_path, assets_path, underscore_files, native_libs_path, classes_dex, output_path)
      print_timestamp('AndroidPrebuild.make_package START')
      resourcepkg = $bindir + "/rhodes.ap_"

      puts "Packaging Assets and Jars"
      print_timestamp('Packaging Assets and Jars START')

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
      print_timestamp('Packaging Assets and Jars FINISH')

      # Workaround: manually add files starting with '_' because aapt silently ignore such files when creating package
      print_timestamp('Packaging underscore START')
      underscore_files.each do |relpath|
        puts "Add #{relpath} to #{resourcepkg}..."
        args = ["uf", resourcepkg, relpath]
        Jake.run($jarbin, args, $tmpdir)
        unless $?.success?
          raise "Error packaging assets"
        end
      end
      print_timestamp('Packaging underscore FINISH')

      puts "Packaging Native Libs"

      print_timestamp('Packaging Native Libs START')

      args = ["uf", resourcepkg]

      abis = $abis

      #replace 'arm' to 'armeabi'
      abis.map! { |abi| abi == 'arm' ? 'armeabi' : abi }

      Dir.glob(File.join(native_libs_path,'**','lib*.so')) do |lib|
        arch = File.basename(File.dirname(lib))
        args << "lib/#{arch}/#{File.basename(lib)}" if abis.include?(arch)
        abis.delete(arch)
      end

      puts "WARNING: Requested ABIs not found in container: #{abis}" unless abis.empty?

      Jake.run($jarbin, args, native_libs_path)
      unless $?.success?
        raise "Error packaging native libraries"
      end
      print_timestamp('Packaging Native Libs FINISH')
      dexfile = classes_dex
      simple_apkfile = $targetdir + "/" + $appname + "_tmp.apk"
      final_apkfile = output_path
      signed_apkfile = $targetdir + "/" + $appname + "_tmp_signed.apk"
      resourcepkg = $bindir + "/rhodes.ap_"


      print_timestamp('build APK START')
      apk_build $androidsdkpath, simple_apkfile, resourcepkg, dexfile, false

      print_timestamp('build APK FINISH')

      if not File.exist? $keystore
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

      print_timestamp('Signing APK file START')
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

      print_timestamp('Signing APK file FINISH')

      puts "Align APK file"

      print_timestamp('Align APK file START')
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
      print_timestamp('Align APK file FINISH')
      #remove temporary files
      rm_rf simple_apkfile
      rm_rf signed_apkfile

      File.open(File.join(File.dirname(final_apkfile), "app_info.txt"), "w") do |f|
        f.puts $app_package_name
      end
      print_timestamp('AndroidPrebuild.make_package FINISH')
    end

    def self.merge_assets( prebuilt_assets, app_assets )
        print_timestamp('AndroidPrebuild.merge_assets START')
        target_assets = app_assets + '_merged'
        
        FileUtils.mkdir_p( target_assets )
        
        cp_r( File.join(prebuilt_assets,'.'), target_assets, { :verbose => true } )
        cp_r( File.join(app_assets,'.'), target_assets, { :verbose => true } )

        target_public_api = File.join(target_assets,'apps','public','api')
        FileUtils.mkdir_p( target_public_api )
        cp_r( File.join(prebuilt_assets,'apps','public','api','.'), target_public_api, { :verbose => true } )

        target_db = File.join( target_assets, 'db' )
        FileUtils.mkdir_p( target_db )
        cp_r( File.join(prebuilt_assets,'db','.'), target_db, { :verbose => true } )

        hash = nil
        ["apps", "db", "lib"].each do |d|
            # Calculate hash of directories
            hash = get_dir_hash(File.join(target_assets, d), hash)
        end

        rm File.join(target_assets, "hash")
        rm File.join(target_assets, "name")
        rm File.join(target_assets, "rho.dat")

        Jake.build_file_map(target_assets, "rho.dat")

        File.open(File.join(target_assets, "hash"), "w") { |f| f.write(hash.hexdigest) }
        File.open(File.join(target_assets, "name"), "w") { |f| f.write($appname) }
        print_timestamp('AndroidPrebuild.merge_assets FINISH')
        return target_assets
    end

    def self.merge_resources( prebuilt_res, app_res )
        print_timestamp('AndroidPrebuild.merge_resources START')
        target_res = app_res + '_merged'
        
        FileUtils.mkdir_p( target_res )
        
        cp_r( File.join(prebuilt_res,'.'), target_res, { :verbose => true } )

        rhostrings = File.join(prebuilt_res, "values", "strings.xml")
        appstrings = File.join(target_res, "values", "strings.xml")
        doc = REXML::Document.new(File.new(rhostrings))
        doc.elements["resources/string[@name='app_name']"].text = $appname
        File.open(appstrings, "w") { |f| doc.write f }

        iconappname = File.join($app_path, "icon", "icon.png")

        ['drawable', 'drawable-hdpi', 'drawable-mdpi', 'drawable-ldpi'].each do |dpi|
            drawable = File.join(target_res, dpi)
            iconresname = File.join(drawable, "icon.png")
            rm_f iconresname
            cp iconappname, iconresname if File.exist? drawable
        end

        #cp_r( File.join(app_res,'.'), target_res, { :verbose => true } )
        print_timestamp('AndroidPrebuild.merge_resources FINISH')
        return target_res
    end

    def self.production_with_prebuild_binary
      print_timestamp('AndroidPrebuild.production_with_prebuild_binary START')
      Rake::Task['config:android'].invoke

      #Support build.yml settings on cloud by copying to rhoconfig.txt
      Rake::Task['config:common:ymlsetup'].invoke

      prebuilt_path = determine_prebuild_path($app_config)

      bundle_path = make_app_bundle

      prebuilt_config = Jake.config(File.open(File.join(prebuilt_path, 'build.yml')))

      manifest_path = generate_manifest(prebuilt_path,prebuilt_config,$app_config)

      prebuilt_builddir = File.join(bundle_path,'bin','target','android',$confdir)
      resources_path = build_resources(prebuilt_builddir)

      assets_path = merge_assets( File.join( prebuilt_path,'assets' ), bundle_path )
      resources_path = merge_resources( File.join( prebuilt_path,'res' ), resources_path )

      underscore_files = get_underscore_files_from_bundle(bundle_path)

      #jars = build_java

      native_libs_path = get_native_libs_path(prebuilt_path)

      output_path = make_output_path

      classes_dex = File.join(prebuilt_path,'classes.dex')
      make_package(manifest_path,resources_path,assets_path,underscore_files,native_libs_path, classes_dex, output_path)
      print_timestamp('AndroidPrebuild.production_with_prebuild_binary FINISH')
    end

  end

    task :production_with_prebuild_binary do
      AndroidPrebuild.production_with_prebuild_binary
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

      if File.exist?(resourcepkg) then
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
