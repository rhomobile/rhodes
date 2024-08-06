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

namespace "config" do
  task :set_wp8_platform do
    $current_platform = "wp8" unless $current_platform
  end

  #task :set_wp8_app_path do
  #	ENV['RHO_APP_PATH'] = $app_path
  # end

  task :wp8_ARM_Release do
    $sdk = 'ARM'
    $build_config = 'Release'
  end

  task :wp8_ARM_Debug do
    $sdk = 'ARM'
    $build_config = 'Debug'
  end

  task :wp8_Win32_Release do
    $sdk = 'Win32'
    $build_config = 'Release'
  end

  task :wp8_Win32_Debug do
    $sdk = 'Win32'
    $build_config = 'Debug'
  end

  task :wp8 => [:set_wp8_platform, "config:common"] do

    #if ENV['RHO_APP_PATH'] != nil
    #	$app_path = ENV['RHO_APP_PATH']
    #end

    $rubypath = "res/build-tools/RhoRuby.exe"
    $zippath = "res/build-tools/7za.exe"
    $wp7runner = "res/build-tools/RhoAppRunnerWP8.exe"
    $wp7logserver = "res/build-tools/RhoLogserver.exe"
    $builddir = $config["build"]["wp8path"] + "/build"
    $vcbindir = $config["build"]["wp8path"] + "/bin"
    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
    $bindir = $app_path + "/bin"
    $rhobundledir = $app_path + "/RhoBundle"
    $srcdir = $bindir + "/RhoBundle"
    $targetdir = $bindir + "/target/wp8"
    $tmpdir = $bindir +"/tmp"

    $msbuild = $config["env"]["paths"]["msbuild"]
    $msbuild = "msbuild" if $msbuild.nil?

    #$sdk          = "Windows Phone 8 SDK"
    #$sdk          = $app_config["wpsdk"] unless $app_config["wpsdk"].nil?

    $rhodes_bin_dir = "#{$startdir}/#{$vcbindir}/#{$sdk}/rhodes/#{$build_config}"

    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']

    $productid = $app_config["wp8"]["productid"] if $app_config["wp8"]
    $productid = $app_config["wp"]["productid"] if $app_config["wp"] && !$productid

    if !$productid
      puts "Add wp8:productid to application build.yml"
      puts "productid is GUID in format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
      puts "for example:"
      puts "wp8:"
      puts "  productid: 'fd55c4d0-51fa-012e-7844-3caec51bd50e'"

      exit 1
    end
  end
end

def addRhobundleFilesToCacheFile()
  xml_path = File.join($rhodes_bin_dir, "XapCacheFile.xml")
  doc = REXML::Document.new(File.open(xml_path))
  chdir $srcdir

  Dir.glob(File.join("**", '*.*')).each do |f|
    doc.root[1, 0] = REXML::Element.new "file lastWriteTime='" + File.mtime(f).strftime("%m/%d/%Y %I:%M:%S %p") + "' source='" + $srcdir.gsub("/", "\\") + "\\" + f.gsub("/", "\\") + "' archivePath='rho\\" + f.gsub("/", "\\") + "'"
  end

  File.open(xml_path, "w") { |f| doc.write f, 2; f.close }

  chdir $startdir

  mkdir_p $config["build"]["wp8path"] + "/rhodes/obj/#{$build_config}" if not File.exist? $config["build"]["wp8path"] + "/rhodes/obj/#{$build_config}"
  cp xml_path, $config["build"]["wp8path"] + "/rhodes/obj/#{$build_config}"
end

def addbundletoxap()
  tmp_dir = File.join($srcdir, "tmp")
  rho_dir = File.join(tmp_dir, "rho")

  rm_rf tmp_dir
  mkdir tmp_dir
  mkdir File.join(tmp_dir, 'rho')
  mkdir File.join(tmp_dir, 'rho', 'apps')

  appsFiles = FileList.new
  appsFiles.include(File.join($srcdir, "apps", "*.*"))

  #cp_r File.join($srcdir, "public"), tmp_dir
  cp File.join($srcdir, "RhoBundleMap.txt"), tmp_dir
  cp_r File.join($srcdir, "apps"), rho_dir
  #cp_r appsFiles, File.join(rho_dir, 'apps')
  #cp_r File.join($srcdir, "public"), File.join(rho_dir, 'apps')
  cp_r File.join($srcdir, "lib"), rho_dir
  cp_r File.join($srcdir, "db"), rho_dir

  chdir $startdir
  args = []
  args << "a"
  args << "-tzip"
  args << File.join($rhodes_bin_dir, "rhodes.xap")
  args << tmp_dir + "/*"
  puts Jake.run($zippath, args)
end

def addconfigtoxap()
  tmp_dir = File.join($srcdir, "tmp")
  rho_dir = File.join(tmp_dir, "rho")

  rm_rf tmp_dir
  mkdir tmp_dir
  mkdir File.join(tmp_dir, 'rho')
  mkdir File.join(tmp_dir, 'rho', 'apps')

  cp File.join($srcdir, "apps/rhoconfig.txt"), rho_dir

  chdir $startdir
  args = []
  args << "a"
  args << "-tzip"
  args << File.join($rhodes_bin_dir, "rhodes.xap")
  args << tmp_dir + "/*"
  puts Jake.run($zippath, args)
end

def setCSharpEnvironment(csharp_impl)
  if csharp_impl
    ENV['TARGET_TEMP_DIR'] = File.join($startdir, "platform", $current_platform, "bin", $sdk, "extensions", $cur_ext, $build_config)
    ENV['TARGET_EXT_DIR'] = File.join($startdir, "platform", $current_platform, "bin", $sdk, "rhoruntime", $build_config)
    ENV['BUILD_CSHARP_IMPL'] = "yes"
  else
    ENV['TARGET_TEMP_DIR'] = File.join($startdir, "platform", $current_platform, "bin", $sdk, "rhoruntime", $build_config)
    ENV['TARGET_EXT_DIR'] = ''
    ENV['BUILD_CSHARP_IMPL'] = "no"
  end
end

namespace "build" do
  namespace "wp8" do

    task :extensions => "config:wp8" do
      $app_config["extensions"].each do |ext|
        $cur_ext = ext
        $app_config["extpaths"].each do |p|

          project_path = nil
          extpath = File.join(p, ext)
          extyml = File.join(extpath, "ext.yml")
          if File.file? extyml
            extconf = Jake.config(File.open(extyml))
            project_paths = extconf["project_paths"]
            project_path = project_paths[$current_platform] if (project_paths && project_paths[$current_platform])
            extconf_wp8 = (!extconf[$current_platform].nil?) ? extconf[$current_platform] : Hash.new
            csharp_impl_all = (!extconf_wp8['csharp_impl'].nil?) ? true : false
          else
            csharp_impl_all = false
            extconf_wp8 = Hash.new
          end

          ENV['RHO_PLATFORM'] = $current_platform
          ENV['RHO_ROOT'] = $startdir
          ENV['RHO_VSPROJ_SDK_PLATFORM'] = $sdk
          ENV['RHO_BUILD_CONFIG'] = $build_config
          ENV['TEMP_FILES_DIR'] = File.join($startdir, "platform", $current_platform, "bin", $sdk, "extensions", ext) # unused by rake build with 'project_path'
          ENV['VCBUILD'] = $msbuild
          ENV['RHO_EXT_NAME'] = ext
          setCSharpEnvironment(csharp_impl_all) 

          if (project_path)
            ENV['RHO_PROJECT_PATH'] = File.join(p, ext, project_path)
            Jake.run3('rake', File.join($startdir, 'lib/build/extensions'))

            if (!extconf_wp8['libraries'].nil?) && (extconf_wp8['libraries'].is_a? Array)
              extconf_wp8['libraries'].each do |lib|
                extconf_wp8_lib = !extconf_wp8[lib.downcase].nil? ? extconf_wp8[lib.downcase] : Hash.new
                if !extconf_wp8_lib['project_path'].nil?
                  setCSharpEnvironment( csharp_impl_all || (!extconf_wp8_lib['csharp_impl'].nil?) )
                  ENV['RHO_PROJECT_PATH'] = File.join(p, ext, extconf_wp8_lib['project_path'])
                  Jake.run3('rake --trace', File.join($startdir, 'lib/build/extensions'))
                end
              end
            end

            break
          else
            extpath = File.join(p, ext, 'ext')
            next unless File.exist? File.join(extpath, "build.bat")

            Jake.run3('build.bat', extpath)
            break

          end

        end
      end
    end

    desc "Build WP8 rhobundle"
    task :rhobundle_noext => ["config:wp8", "build:bundle:noxruby", :rhobundlemap] do
      #move public folder to root
      #confpath_content = File.read($srcdir + "/apps/rhoconfig.txt") if File.exist?($srcdir + "/apps/rhoconfig.txt")
      #confpath_content += "\r\n" + "rhologurl=http://" + $rhologhostaddr + ":" + $rhologhostport.to_s() if !confpath_content.include?("rhologurl=")
      #File.open($srcdir + "/apps/rhoconfig.txt", "w") { |f| f.write(confpath_content) } if confpath_content && confpath_content.length()>0

      #cp_r $srcdir + "/apps/public", $srcdir + "/public"
      #rm_r $srcdir + "/apps/public"
    end

    task :upgrade_package => [:rhobundle_noext, :extensions] do        
      mkdir_p $targetdir if not File.exist? $targetdir
      zip_file_path = File.join($targetdir, "upgrade_bundle.zip")
      Jake.zip_upgrade_bundle( $bindir, zip_file_path)
    end

    # create file with map of bundle files 
    task :rhobundlemap do

    end

    # build native code
    task :rhodes => ["config:wp8"] do
      chdir $startdir

      out_dir = $startdir + "/"+ $config["build"]["wp8path"] +"/rhodes"
      cp $app_path + "/icon/icon.png", out_dir if File.exist? $app_path + "/icon/icon.ico"

      chdir $config["build"]["wp8path"]

      doc = REXML::Document.new(File.open($startdir+"/"+$config["build"]["wp8path"]+"/rhodes/Properties/WMAppManifest.xml"))
      doc.elements.each("Deployment/App") { |element|
        element.attributes["ProductID"]   = "{"+$productid+"}"
        element.attributes["Title"]       = $app_config["name"]
        element.attributes["Description"] = $app_config["name"]
        element.attributes["Author"]      = $app_config["vendor"]
        element.attributes["Publisher"]   = $app_config["vendor"]
      }

      File.open($startdir + "/"+$config["build"]["wp8path"] + "/rhodes/Properties/WMAppManifest.xml", "w") { |f| doc.write f; f.close }

      args = ['rhodes.sln', "/p:Configuration=#{$build_config}", "/p:Platform=#{$sdk}", '/p:VisualStudioVersion=12.0', '/t:Build'] #'/p:PlatformToolset=v110'

      puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
      Jake.run($msbuild, args)
      unless $? == 0
        puts "Error building"
        exit 1
      end

      chdir $startdir
    end

    task :package => [:rhobundle_noext, :extensions, :rhodes] do
      #addbundletoxap()

      cp File.join($rhodes_bin_dir, "rhodes.xap"), File.join($rhodes_bin_dir, $appname + ".xap")

      mkdir_p $bindir if not File.exist? $bindir
      mkdir_p $targetdir if not File.exist? $targetdir
      mv File.join($rhodes_bin_dir, $appname + ".xap"), $targetdir

    end

    task :package_rhobundle, [:sdk, :configuration] do |t, args|
      throw "You must pass in sdk(x86, ARM)" if args.sdk.nil?
      throw "You must pass in configuration(Debug, Release)" if args.configuration.nil?

      $sdk = args.sdk == 'x86' ? 'Win32' : args.sdk
      $build_config = args.configuration

      Rake::Task["build:wp8:rhobundle_noext"].invoke

      addRhobundleFilesToCacheFile()
      addbundletoxap()

    end

    task :devrhobundle, [:sdk, :configuration] do |t, args|
      throw "You must pass in sdk(Win32, ARM)" if args.sdk.nil?
      throw "You must pass in configuration(Debug, Release)" if args.configuration.nil?

      $sdk = args.sdk
      $build_config = args.configuration

      Rake::Task["build:wp8:rhobundle_noext"].invoke
      Rake::Task["build:wp8:extensions"].invoke
    end

    task :extension_libs, [:sdk, :configuration] => :devrhobundle do
      confdir = ($build_config.to_s.downcase == 'release') ? 'release' : 'debug'

      app_builddir = File.join($targetdir, confdir)
      chdir $startdir

      libsrcdir = File.join($startdir, 'platform', $current_platform, 'bin', $sdk, 'rhoruntime', $build_config)

      $app_extensions_list.each do |ext, commin_ext_path|
        next if commin_ext_path == nil

        extpath = File.join(commin_ext_path, 'ext')
        extyml = File.join(commin_ext_path, 'ext.yml')
        if File.file? extyml
          prebuiltpath = Dir.glob(File.join(extpath, '**', 'wp8'))
          if prebuiltpath != nil && prebuiltpath.count > 0
            target = File.join(app_builddir, 'extensions', ext)
            libdstdir = File.join(target, 'lib', $sdk)
            mkdir_p libdstdir

            Dir.glob(File.join(libsrcdir, '*.lib')).each do |lib|
              cp(lib, libdstdir) if File.basename(lib, '.lib').downcase == ext.downcase
            end

            # copy all files from extension folder to extension binary folder
            Dir.glob(File.join(commin_ext_path, '*')).each do |artefact|
              if (artefact != extpath) && (artefact != extyml) && (File.file?(artefact) || File.directory?(artefact))
                cp_r artefact, target
              end
            end
          end
        end
      end
    end

  end
end

#def get_app_log()
#  args = []
#  args << $app_config["wp"]["productid"]
#  args << ""
#  args << ""
#  args << ""
#  args << "RhoLog.txt"
#
#  cc_run($wp7runner, args) or return false
#  puts "RhoLog.txt stored to " + $app_path
#  return true
#end
#
#def run_rho_log_server()
#  system("START rake run:wp:rhologserver[#{$app_path}]")
#end

namespace "device" do
  namespace "wp8" do

    desc "Build production for device"
    task :production => ["config:wp8_ARM_Release", "build:wp8:package"] do
    end

	task :getlog => ["config:wp8"] do
		args = []
		args << $productid
		args << $app_config["name"]
		args << $app_path + "/icon/icon.png"
		args << $targetdir + "/" + $appname + ".xap"
		args << "dev"
		$path = $app_path+"/rholog.txt"
		$path.gsub!('/', '\\')
		args << $path
		puts Jake.run($wp7runner, args)

		cc_run($wp7runner, args) or return false
		puts "RhoLog.txt stored to " + $app_path
	end
  end
end

namespace "emulator" do
  namespace "wp8" do

    desc "Build production for device"
    task :production => ["config:wp8_Win32_Release", "build:wp8:package"] do
      addRhobundleFilesToCacheFile()
    end

	task :getlog => ["config:wp8"] do
		args = []
		args << $productid
		args << $app_config["name"]
		args << $app_path + "/icon/icon.png"
		args << $targetdir + "/" + $appname + ".xap"
		args << "emu"
		$path = $app_path+"/rholog.txt"
		$path.gsub!('/', '\\')
		args << $path
		puts Jake.run($wp7runner, args)

		cc_run($wp7runner, args) or return false
		puts "RhoLog.txt stored to " + $app_path
	end

  end
end

namespace "clean" do
  desc "Clean wp"
  task :wp8 => "clean:wp8:all"

  namespace "wp8" do
    task :rhodes => ["config:wp8"] do
      rm_rf $vcbindir
      rm_rf $targetdir
      
      rm_rf File.join($app_path, "bin/tmp") if File.exist? File.join($app_path, "bin/tmp")
      rm_rf File.join($app_path, "bin/RhoBundle") if File.exist? File.join($app_path, "bin/RhoBundle")
      
    end
    task :all => ["clean:wp8:rhodes", "clean:common"]
  end
end

def runWP8spec(rakeCommand)
      Jake.decorate_spec do
        Rake::Task[rakeCommand].invoke
        Jake.before_run_spec
        start = Time.now
        log_file = getLogPath

        puts "waiting for log: " + log_file

        for i in 0..1200
          if !File.exist?(log_file)
            sleep(1)
          else
            break
          end
        end

        if !File.exist?(log_file)
          puts "Can not read log file: " + log_file
          exit(1)
        end

        puts "start read log"

        io = File.new(log_file, "r")
        waiting_count = 0
        end_spec = false

        while !end_spec do
          line_count = 0

          io.each do |line|
            end_spec = !Jake.process_spec_output(line)
            break if end_spec
            line_count += 1
          end

          if line_count==0
            waiting_count += 1
          else
            waiting_count = 0
          end

          if waiting_count > 240
            puts "spec application hung (240 seconds timeout)"
            end_spec = true
          end

          sleep(1) unless end_spec
        end

        io.close

        Jake.process_spec_results(start)

        $stdout.flush
        chdir $startdir

      end
end

namespace "run" do

  def getLogPath
    log_file_path = File.join($app_path, $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"])
    return log_file_path
  end

  desc "Build, install .xap and run on WP8 emulator"
  task :wp8 => ["emulator:wp8:production"] do

    if $productid != nil

      #File.delete($app_path + "/started") if File.exist?($app_path + "/started")
      #Jake.run_rho_log_server($app_path)
      #puts "RhoLogServer is starting"

      #while (1)
      #  if File.exist?($app_path + "/started")
      #    break
      #  end
      #end

	  #addconfigtoxap()

      cp File.join($rhodes_bin_dir, "rhodes.xap"), File.join($rhodes_bin_dir, $appname + ".xap")
      mv File.join($rhodes_bin_dir, $appname + ".xap"), $targetdir

      args = []
      args << $productid
      args << $app_config["name"]
      args << $app_path + "/icon/icon.png"
      args << $targetdir + "/" + $appname + ".xap"
      args << "emu"
      puts Jake.run($wp7runner, args)

      #while(1)
      #	sleep(1000)
      #end
      #$rhologfile.close
    else
      puts "productid must be set in build.yml"
      puts "productid's format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
    end
  end

  namespace "wp8" do

    task :get_log => "config:wp8" do
      puts "log_file=" + getLogPath
    end

    rhosim_task = lambda do |name, &block|
      task name => ["config:set_wp8_platform", "config:common"] do
        $rhosim_config = "platform='wp8'\r\n"
        block.()
      end
    end

    desc "Run application on RhoSimulator"
    rhosim_task.(:rhosimulator) { Rake::Task["run:rhosimulator"].invoke }
    namespace :rhosimulator do
      rhosim_task.(:build) { Rake::Task["run:rhosimulator:build"].invoke         }
      rhosim_task.(:debug) { Rake::Task["run:rhosimulator:run"  ].invoke('wait') }
    end

    task :spec do
      runWP8spec("run:wp8")
    end

    namespace "device" do
      task :spec do
        runWP8spec("run:wp8:device")
      end
    end

    desc "Build, install .xap and run on WP8 device"
    task :device => ["device:wp8:production"] do
      addRhobundleFilesToCacheFile()

      if $productid != nil
        #system("START " + $wp7logserver + " " + $app_path + "/rholog.txt")
        #File.delete($app_path + "/started") if File.exist?($app_path + "/started")
        #Jake.run_rho_log_server($app_path)

        #puts "RhoLogServer is starting"
        #while (1)
        #  if File.exist?($app_path + "/started")
        #    break
        #  end
        #end

		#addconfigtoxap()

        cp File.join($rhodes_bin_dir, "rhodes.xap"), File.join($rhodes_bin_dir, $appname + ".xap")
        mv File.join($rhodes_bin_dir, $appname + ".xap"), $targetdir

        args = []
        args << $productid
        args << $app_config["name"]
        args << $app_path + "/icon/icon.png"
        args << $targetdir + "/" + $appname + ".xap"
        args << "dev"
        puts Jake.run($wp7runner, args)

        #while(1)
        #sleep(1000)
        #end
        #$rhologfile.close
      else
        puts "productid must be set in build.yml"
        puts "productid's format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
      end
    end
  end
end

namespace :stop do
  task :wp8 do
    Jake.get_process_list.each do |p|
      Process.kill(9, p[:pid].to_i) if p[:cmd].include? 'XDE.exe'
    end
  end
end
