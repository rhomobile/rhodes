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
  task :set_uwp_platform do
    $current_platform = "uwp" unless $current_platform
  end

  task :release do
    targetsdk = 'ARM'
    targetsdk = $app_config["uwp"]["target"] if $app_config["uwp"] && $app_config["uwp"]["target"]
    if targetsdk == 'ARM' || targetsdk == 'arm'
      $sdk = 'ARM'
      $build_config = 'Release'
      puts "Building ARM application"
    elsif targetsdk == 'X86' || targetsdk == 'x86'
      $sdk = 'Win32'
      $build_config = 'Release'
      puts "Building x86 application"
    else
      puts "Wrong target sdk, choose 'x86' or 'ARM'"
      exit 1 
    end

  end

  task :uwp_ARM_Release do
    $sdk = 'ARM'
    $build_config = 'Release'
  end

  task :uwp_ARM_Debug do
    $sdk = 'ARM'
    $build_config = 'Debug'
  end

  task :uwp_Win32_Release do
    $sdk = 'Win32'
    $build_config = 'Release'
  end

  task :uwp_Win32_Debug do
    $sdk = 'Win32'
    $build_config = 'Debug'
  end

  task :uwp => [:set_uwp_platform, "config:common"] do

    #if ENV['RHO_APP_PATH'] != nil
    #	$app_path = ENV['RHO_APP_PATH']
    #end

    $rubypath = "res/build-tools/RhoRuby.exe"
    $zippath = "res/build-tools/7za.exe"
    $wp7runner = "res/build-tools/RhoAppRunnerUWP.exe"
    $wp7logserver = "res/build-tools/RhoLogserver.exe"
    $uwppath = $config["build"]["uwppath"]
    $builddir = $uwppath + "/build"
    $vcbindir = $uwppath + "/bin"
    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
    $bindir = $app_path + "/bin"
    $rhobundledir = $app_path + "/RhoBundle"
    $srcdir = $bindir + "/RhoBundle"
    $targetdir = $bindir + "/target/uwp"
    $tmpdir = $bindir +"/tmp"
    $winAppDeploy = "C:/Program Files (x86)/Windows Kits/10/bin/x86/WinAppDeployCmd.exe"
    $msbuild = $config["env"]["paths"]["msbuild"]
    $msbuild = "msbuild" if $msbuild.nil?

    $appxbuilder = "C:/Program Files (x86)/Windows Kits/10/bin/x86/makeappx.exe" 


    $rhodes_bin_dir = "#{$startdir}/#{$vcbindir}/#{$sdk}/rhodes/#{$build_config}"
    $targetpath = $rhodes_bin_dir + "/AppxPackageDir/"
    $targetAppxFileName = File.join($rhodes_bin_dir, 'AppxPackageDir/rhodes.appx')
    $sertificateFileName = "#{$startdir}/#{$config["build"]["uwppath"]}/rhodes/rhodes_TemporaryKey.pfx"

    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']

    $productid = $app_config["uwp"]["productid"] if $app_config["uwp"]
    $productid = $app_config["wp"]["productid"] if ($app_config["wp"] && !$productid)

    if !$productid
      puts "Add uwp:productid to application build.yml"
      puts "productid is GUID in format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
      puts "for example:"
      puts "uwp:"
      puts "  productid: 'caa0300f-1021-40aa-bb98-27c5547616fa'"

      exit 1
    end
  end
end



def createBoundleInTemp()
  tmp_dir = File.join($bindir, "tmp")
  rho_dir = File.join(tmp_dir, "rho")

  rm_rf tmp_dir
  mkdir tmp_dir
  mkdir File.join(tmp_dir, 'rho')
  mkdir File.join(tmp_dir, 'rho', 'apps')

  cp File.join($srcdir, "RhoBundleMap.txt"), tmp_dir
  cp_r File.join($srcdir, "apps"), rho_dir
  cp_r File.join($srcdir, "lib"), rho_dir
  cp_r File.join($srcdir, "db"), rho_dir
  cp File.join($srcdir, "apps/rhoconfig.txt"), rho_dir
  return rho_dir
end

def addbundletoxapUWP()
  rho_dir = createBoundleInTemp()

  tempRhodesPath = $targetpath + "rhodes"
  rm_rf tempRhodesPath

  chdir $startdir
  args = []
  args << "unpack"
  args << "/p"
  args << $targetAppxFileName
  args << "/d"
  args << tempRhodesPath
  args << "/l"
  puts Jake.run($appxbuilder, args)

  cp_r rho_dir, tempRhodesPath
  rm_rf $targetAppxFileName

  args = []
  args << "pack"
  args << "/d"
  args << tempRhodesPath
  args << "/p"
  args << $targetAppxFileName
  args << "/l"
  puts Jake.run($appxbuilder, args)

  rm_rf tempRhodesPath

end

def addconfigtoxapUWP()
  tmp_dir = File.join($bindir, "tmp")
  rho_dir = File.join(tmp_dir, "rho")

  rm_rf tmp_dir
  mkdir tmp_dir
  mkdir File.join(tmp_dir, 'rho')
  mkdir File.join(tmp_dir, 'rho', 'apps')

  cp File.join($srcdir, "apps/rhoconfig.txt"), rho_dir
  return rho_dir
end

def signApplication()  
  args = []
  args << "sign"
  args << "/fd"
  args << "SHA256"
  args << "/a"
  args << "/f"
  args << $sertificateFileName
  args << $targetAppxFileName

  Jake.run("C:/Program Files (x86)/Windows Kits/10/bin/x86/signtool.exe", args) if File.exist?($targetAppxFileName)
end

def addRhobundleFilesToCacheFileUWP()
  #addconfigtoxapUWP()
  #addbundletoxapUWP()
  #signApplication()
end

def setCSharpEnvironmentUWP(csharp_impl)
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
  namespace "uwp" do

    task :extensions => "config:uwp" do
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
            extconf_uwp = (!extconf[$current_platform].nil?) ? extconf[$current_platform] : Hash.new
            csharp_impl_all = (!extconf_uwp['csharp_impl'].nil?) ? true : false
          else
            csharp_impl_all = false
            extconf_uwp = Hash.new
          end

          ENV['RHO_PLATFORM'] = $current_platform
          ENV['RHO_ROOT'] = $startdir
          ENV['RHO_VSPROJ_SDK_PLATFORM'] = $sdk
          ENV['RHO_BUILD_CONFIG'] = $build_config
          ENV['TEMP_FILES_DIR'] = File.join($startdir, "platform", $current_platform, "bin", $sdk, "extensions", ext) # unused by rake build with 'project_path'
          ENV['VCBUILD'] = $msbuild
          ENV['RHO_EXT_NAME'] = ext
          setCSharpEnvironmentUWP(csharp_impl_all) 

          if (project_path)
            ENV['RHO_PROJECT_PATH'] = File.join(p, ext, project_path)
            Jake.run3('rake', File.join($startdir, 'lib/build/extensions'))

            if (!extconf_uwp['libraries'].nil?) && (extconf_uwp['libraries'].is_a? Array)
              extconf_uwp['libraries'].each do |lib|
                extconf_uwp_lib = !extconf_uwp[lib.downcase].nil? ? extconf_uwp[lib.downcase] : Hash.new
                if !extconf_uwp_lib['project_path'].nil?
                  setCSharpEnvironmentUWP( csharp_impl_all || (!extconf_uwp_lib['csharp_impl'].nil?) )
                  ENV['RHO_PROJECT_PATH'] = File.join(p, ext, extconf_uwp_lib['project_path'])
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

    desc "Build UWP rhobundle"
    task :rhobundle_noext => ["config:uwp", "build:bundle:noxruby", :rhobundlemap] do
      #addconfigtoxapUWP()
      rho_dir = createBoundleInTemp()
      #signApplication()

      appx_dir = File.join($rhodes_bin_dir, "AppX")
      #rm_rf appx_dir
      FileUtils.mkpath appx_dir
      cp_r rho_dir, appx_dir
    end

    def copyFilesToRho()
      target_dir = File.join($uwppath, "rhodes")
      #addconfigtoxapUWP()
      rho_dir = createBoundleInTemp()
      puts "Files dir " + File.join(target_dir, "rho")
      rm_rf File.join(target_dir, "rho") if Dir.exist?(File.join(target_dir, "rho"))

      FileUtils.mkpath target_dir
      cp_r rho_dir, target_dir
    end

    task :copy_files_to_rho => ["config:uwp", "build:bundle:noxruby", :rhobundlemap] do
      copyFilesToRho()
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
    task :rhodes => ["config:uwp"] do
      chdir $startdir

      out_dir = $startdir + "/"+ $config["build"]["uwppath"] +"/rhodes"
      cp $app_path + "/icon/icon.png", out_dir if File.exist? $app_path + "/icon/icon.ico"

      chdir $config["build"]["uwppath"]

      doc = REXML::Document.new(File.open($startdir+"/"+$config["build"]["uwppath"]+"/rhodes/Package.appxmanifest"))
      doc.elements.each("Package/Applications/Application/uap:VisualElements") { |element|
        #element.attributes["ProductID"]   = "{"+$productid+"}"
        element.attributes["DisplayName"]            = $app_config["name"]
        element.attributes["Description"]            = $app_config["name"]
      }
      doc.elements.each("Package/Properties/DisplayName") { |element|
        element.text = $app_config["name"]
      }
      doc.elements.each("Package/Properties/PublisherDisplayName") { |element|
        element.text = $app_config["vendor"]
      }

      File.open($startdir + "/"+$config["build"]["uwppath"] + "/rhodes/Package.appxmanifest", "w") { |f| doc.write f; f.close }

      copyFilesToRho()

      args = ['rhodes.sln', "/p:Configuration=#{$build_config}", "/p:Platform=#{$sdk}", '/p:VisualStudioVersion=14.0', 
        '/t:Build', '/p:UapAppxPackageBuildMode=StoreUpload','/p:AppxPackageDir=' + $rhodes_bin_dir + '/AppxPackageDir'] 

      puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
      Jake.run($msbuild, args)

      appxFileName = ""
      Dir[$rhodes_bin_dir + '/AppxPackageDir/*/**/rhodes*.appx'].each { |f|  appxFileName = f if File.file?(f) }
      cp appxFileName, $targetAppxFileName

      unless $? == 0
        puts "Error building"
        exit 1
      end

      chdir $startdir
    end

    task :package => [:rhobundle_noext, :extensions, :rhodes] do
      addRhobundleFilesToCacheFileUWP()
      
      #cp File.join($rhodes_bin_dir, "rhodes.xap"), File.join($rhodes_bin_dir, $appname + ".xap")

      mkdir_p $bindir if not File.exist? $bindir
      mkdir_p $targetdir if not File.exist? $targetdir
      cp $targetAppxFileName, File.join($targetdir, $appname + ".appx")

      dependenciesDir = ""
      Dir[$rhodes_bin_dir + '/AppxPackageDir/*/**/Dependencies'].each { |f|  dependenciesDir = f if Dir.exist?(f) }
      cp_r dependenciesDir, $targetdir if Dir.exist?(dependenciesDir) 

      puts $targetdir

    end

    task :package_rhobundle, [:sdk, :configuration] do |t, args|
      throw "You must pass in sdk(x86, ARM)" if args.sdk.nil?
      throw "You must pass in configuration(Debug, Release)" if args.configuration.nil?

      $sdk = args.sdk == 'x86' ? 'Win32' : args.sdk
      $build_config = args.configuration

      Rake::Task["build:uwp:rhobundle_noext"].invoke
    end

    task :prepare_rhobundle, [:sdk, :configuration] do |t, args|
      throw "You must pass in sdk(x86, ARM)" if args.sdk.nil?
      throw "You must pass in configuration(Debug, Release)" if args.configuration.nil?
      $sdk = args.sdk == 'x86' ? 'Win32' : args.sdk
      $build_config = args.configuration

      Rake::Task["build:uwp:copy_files_to_rho"].invoke
    end


    task :devrhobundle, [:sdk, :configuration] do |t, args|
      throw "You must pass in sdk(Win32, ARM)" if args.sdk.nil?
      throw "You must pass in configuration(Debug, Release)" if args.configuration.nil?

      $sdk = args.sdk
      $build_config = args.configuration

      Rake::Task["build:uwp:rhobundle_noext"].invoke
      Rake::Task["build:uwp:extensions"].invoke
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
          prebuiltpath = Dir.glob(File.join(extpath, '**', 'uwp'))
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
  namespace "uwp" do

    desc "Build production for device"
    task :production => ["switch_app", "config:release", "build:uwp:copy_files_to_rho", "build:uwp:package"] do
      puts "Now you can install application and dependencies by using the Windows PowerShell command [Add-AppxPackage -Path \"Path to Appx file\"]."
    end

  	task :getlog => ["config:uwp"] do
  		args = []
  		args << $productid
  		args << $app_config["name"]
  		args << $app_path + "/icon/icon.png"
  		args << $targetdir + "/" + $appname + ".appx"
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
  namespace "uwp" do

    desc "Build production for device"
    task :production => ["switch_app", "config:uwp_Win32_Release", "build:uwp:copy_files_to_rho", "build:uwp:package"] do
     addRhobundleFilesToCacheFileUWP()
    end

	task :getlog => ["config:uwp"] do
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
  task :uwp => "clean:uwp:all"

  namespace "uwp" do
    task :rhodes => ["config:uwp"] do
      rm_rf $vcbindir
      rm_rf $targetdir
      
      rm_rf File.join($app_path, "bin/tmp") if File.exist? File.join($app_path, "bin/tmp")
      rm_rf File.join($app_path, "bin/RhoBundle") if File.exist? File.join($app_path, "bin/RhoBundle")
      rm_rf File.join(File.join($uwppath, "rhodes"), "rho") if Dir.exist?(File.join(File.join($uwppath, "rhodes"), "rho"))
      
    end
    task :all => ["clean:uwp:rhodes", "clean:common"]
  end
end

def runuwpspec(rakeCommand)
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

def getProductIdFromAPPX(filename)
  filedir = filename[0..-5]
  rm_rf filedir if Dir.exist?(filedir)

  args = []
  args << "unpack"
  args << "/p"
  args << filename
  args << "/d"
  args << filedir
  args << "/l"
  puts Jake.run($appxbuilder, args)

  prID = ""
  manifest = File.join(filedir, "AppxManifest.xml")
  File.open manifest do |file|
    prID = file.find { |line| line =~ /ProductId=\"\w+-\w+-\w+-\w+-\w+/ }
  end
  prID = prID[/ProductId=\"\w+-\w+-\w+-\w+-\w+\"/]
  prID = prID[/\w+-\w+-\w+-\w+-\w+/]
  rm_rf filedir if Dir.exist?(filedir)
  return prID
end

def deployLibToEmulator(filename)
  args = []
  args << getProductIdFromAPPX(filename)
  args << "0"
  args << "2"
  args << filename
  args << "emulibs"
  puts Jake.run($wp7runner, args) 
end

namespace "run" do

  def getLogPath
    log_file_path = File.join($app_path, $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"])
    return log_file_path
  end

  desc "Build, install .appx and run on universal windows platform emulator"
  task :uwp => ["emulator:uwp:production"] do

    if $productid != nil

      cp $targetAppxFileName, File.join($rhodes_bin_dir, $appname + ".appx")
      #mv File.join($rhodes_bin_dir, $appname + ".appx"), $targetdir

      Dir[$rhodes_bin_dir + '/AppxPackageDir/*/**/Dependencies/x86/*.appx'].each { |f|  deployLibToEmulator(f) if File.file?(f) }
      Dir[$rhodes_bin_dir + '/AppxPackageDir/*/**/Dependencies/Win32/*.appx'].each { |f|  deployLibToEmulator(f) if File.file?(f) }

      args = []
      args << $productid
      args << "32"
      args << "2"
      args << $targetAppxFileName
      args << "emu"
      puts Jake.run($wp7runner, args)

    else
      puts "productid must be set in build.yml"
      puts "productid's format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
    end
  end

  namespace "uwp" do
    def uninstallApplication(deviceGuid, targetAppxFN)
          args = []
          args << "uninstall"
          args << "-file"
          args << "\""+targetAppxFN+"\""
          args << "-g"
          args << "\"" + deviceGuid + "\""
          puts Jake.run($winAppDeploy, args)
    end

    def updateApplication(deviceGuid, targetAppxFN)
          args = []
          args << "update"
          args << "-file"
          args << "\""+targetAppxFN+"\""
          args << "-g"
          args << "\"" + deviceGuid + "\""
          puts Jake.run($winAppDeploy, args)
    end

    def deployApplication(deviceGuid, targetAppxFN, dependencyDir)
          args = []
          args << "install"
          args << "-file"
          args << "\""+targetAppxFN+"\""
          args << "-g"
          args << "\"" + deviceGuid + "\""
          if (dependencyDir.nil?)
            args << "-dependency"
            args << dependencyDir
          end
          puts Jake.run($winAppDeploy, args)
    end

    task :get_log => "config:uwp" do
      puts "log_file=" + getLogPath
    end

    rhosim_task = lambda do |name, &block|
      task name => ["config:set_uwp_platform", "config:common"] do
        $rhosim_config = "platform='uwp'\r\n"
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
      runuwpspec("run:uwp")
    end

    namespace "device" do
      task :spec do
        runuwpspec("run:uwp:device")
      end
    end

    desc "Build, install .AppX and run on universal windows platform device"
    task :device => ["device:uwp:production"] do
      addRhobundleFilesToCacheFileUWP()

      if $productid != nil
        cp $targetAppxFileName, $targetdir
       
        arg = ["devices"] 
        devicesResult =Jake.run($winAppDeploy, arg)
        deviceGuid = devicesResult[/\w+-\w+-\w+-\w+-\w+/]
        if ((deviceGuid != nil) && (deviceGuid != ""))

          dependencyDir = nil
          Dir[$rhodes_bin_dir + '/AppxPackageDir/*/**/Dependencies/ARM/'].each { |f|  dependencyDir = f if Dir.exist?(f) }
          
          uninstallApplication(deviceGuid, $targetAppxFileName)
          deployApplication(deviceGuid, $targetAppxFileName, dependencyDir)

        else
          puts "Error: no available devices connected"
        end
      else
        puts "productid must be set in build.yml"
        puts "productid's format is xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
      end
    end
  end
end

namespace :stop do
  task :uwp do
    Jake.get_process_list.each do |p|
      Process.kill(9, p[:pid].to_i) if p[:cmd].include? 'XDE.exe'
    end
  end
end
