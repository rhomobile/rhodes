#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2020 Rhomobile, Inc.
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

require 'rubygems'

namespace "config" do

	task :set_win32_platform do
		$current_platform = "win32" unless $current_platform
		$sdk = "win32"
	end

	task :win32 => ["config:set_win32_platform", "config:win32:qt", "config:common"] do
		$rubypath = get_ruby_path
		$platformdir = File.join("platform", "win32") 
	    $vcbindir = File.join($platformdir, "bin")
	    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
	    $bindir = File.join($app_path, "bin")    

	    $rhobundledir =  File.join($app_path, "RhoBundle")
	    $log_file = $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"]
	    $srcdir =  File.join($bindir, "RhoBundle")
	    $buildcfg = $app_config["buildcfg"] unless $buildcfg
	    $buildcfg = "Release" unless $buildcfg
	    $detoolappflag = $js_application == true ? "js" : "ruby"
	    $tmp_dir = File.join($bindir, "tmp")

	    $targetdir = File.join($bindir, "target", $sdk)
	    $tmpdir  =  File.join($bindir, "tmp")

	    $vcbuild = $config["env"]["paths"]["vcbuild"]
	    $vcbuild = "vcbuild" if $vcbuild.nil?

	    $nsis    = $config["env"]["paths"]["nsis"]
	    $nsis    = "makensis.exe" if $nsis.nil?


		ENV['RHO_PLATFORM'] = $current_platform
		ENV['VCBUILD'] = $vcbuild

	    if $wk_data_dir.nil?
	      $wk_data_dir = File.join($startdir, "libs", "data") #"/Program Files" # its fake value for running without symbol extensions. do not delete
	      begin
	        if $webkit_capability || $symbol_capability
	          require "rhoelements-data"
	          $wk_data_dir = File.join($startdir, "libs", "data")
	        end
	      rescue Exception => e

	      end
	    end

	    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']

	    puts "$sdk [#{$sdk}]"
	end

	namespace "win32" do
		namespace :wm do
		  task :ignore_vsprops do
		    $wm_win32_ignore_vsprops = true
		  end
		end

		task :qt => ["config:qt"] do
		  next if $prebuild_win32
		  $vscommontools = ENV['VS140COMNTOOLS']
		  $qmake_makespec = 'win32-msvc'
	   
		  $qt_project_dir = File.join( $startdir, 'platform','shared','qt')
		end

		task :application do
			$app_version = '1.0'
			$app_version = $app_config["version"] unless $app_config["version"].nil?

			require $startdir + "/lib/rhodes.rb"
			fversion = StringIO.new("", "w+")          
			fversion.write( "#define RHOSIMULATOR_NAME \"RhoSimulator\"\n" )
			fversion.write( "#define RHOSIMULATOR_VERSION \"#{Rhodes::VERSION}\"\n" )
			fversion.write( "#define APPLICATION_NAME \"#{$appname}\"\n" )
			fversion.write( "#define APPLICATION_VERSION \"#{$app_version}\"\n" )
			Jake.modify_file_if_content_changed( File.join($startdir, 'platform','shared','qt','rhodes','RhoSimulatorVersion.h'), fversion )  

			$app_icon_path = $app_path + "/icon/icon.ico"
			$app_icon_path = $startdir + "/res/icons/rhodes.ico" unless File.exist? $app_icon_path
			cp $app_icon_path, $startdir + "/platform/shared/qt/rhodes/resources/rho.ico"

			$qt_icon_path = $app_path + "/icon/icon.png"
			$qt_icon_path = $startdir + "/res/icons/rhodes.png" unless File.exist? $qt_icon_path
			target_icon_path = $startdir + "/platform/shared/qt/rhodes/resources/rho.png"
			if !FileUtils.compare_file($qt_icon_path, target_icon_path)
				cp $qt_icon_path, target_icon_path
			end
			qrcfile = $startdir + "/platform/shared/qt/rhodes/GeneratedFiles/" + $buildcfg + "/qrc_simulator.cpp"
			rm qrcfile if File.exist? qrcfile
		end

	end

end

namespace "build" do
	namespace "win32" do
	def set_vcvarsall()
		if $vcvarsall == nil
			$vs_version = nil

			msvc_version = $app_config["win32"]["msvc"] if $app_config && $app_config["win32"] && $app_config["win32"]["msvc"]
			if (msvc_version == "2015" or msvc_version == "2017" or msvc_version == "2019")
				$vs_version = msvc_version.to_i
			elsif (msvc_version != nil)
				puts "MSVC version " + msvc_version + " is not supported"
				exit 1
			end

			if ($vs_version == nil)
				puts "Searching for MSVC version from the qtdiag output"
				Jake.run2(File.join($qtdir, 'bin', 'qtdiag'), [' '], {:directory => File.join($qtdir, 'bin'), :nowait => false}) do |line|
				  if line == nil
			      elsif line.include? "MSVC 2015"
			      	$vs_version = 2015
			      	break
			      elsif line.include? "MSVC 2017"
			      	$vs_version = 2017
			      	break
			      elsif line.include? "MSVC 2019"
			      	$vs_version = 2019
			      	break
			      end
			    end
			end

		    puts $vs_version
		    if ($vs_version == nil)
		      	puts "Using QT with unsupported VS version!"
		      	exit 1
		    end

			vcvars = Dir.glob(File.join("C:","Program Files*","Microsoft Visual Studio*","**","vcvarsall.bat"))
			vcvars.each do |batfile|
				if $vs_version == 2015 && batfile.include?("14.0")
					$vcvarsall = batfile
				elsif $vs_version == 2017 && batfile.include?("2017")
					$vcvarsall = batfile
				elsif $vs_version == 2019 && batfile.include?("2019")
					$vcvarsall = batfile
				end
			end
			if ($vcvarsall == nil)
				puts "Can't find vcvarsall.bat in standart locations"
				exit 1
			end
		end
		if File.exist? $vcvarsall
			ENV["RHO_VSCMNTOOLS"] = "\"" + $vcvarsall + "\" x86"
	    else
	    	raise "Can't locate vcvarsall.bat"
	    end
	end

	def runBuldBatFile(bat, extpath)
		set_vcvarsall()

		string = nil
		bat_filepath = File.join(extpath, bat)
		File.open(bat_filepath, "r") { |file| string = file.read}

		if (string != nil)
			old_vscmn = "call \"%RHO_VSCMNTOOLS%..\\..\\VC\\vcvarsall.bat\" x86"
			if (string.include? old_vscmn)
				string.gsub!(old_vscmn, "call %RHO_VSCMNTOOLS%")
				File.open(bat_filepath, "w") { |file| file << string }
			end
		end
		
		Jake.run3(bat, extpath)
	end

    task :rhobundle, ["config:set_win32_platform", "config:win32", "config:qt", "config:win32:qt", :after_bundle, :exclude_dirs] do
		Rake::Task["config:win32"].invoke
		rm_rf $srcdir
		Rake::Task["build:bundle:noxruby"].invoke
		Rake::Task["build:win32:extensions"].execute if !$skip_build_extensions
		Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
    end


    task :rhodeslib_lib, [:target_path] => ["build:win32:set_release_config"] do |t, args|
      print_timestamp('build:win32:rhodeslib_lib START')
      $rhodes_as_lib = true
      Rake::Task['build:win32'].invoke
      createWin32Production(false, true)
      target_path = args[:target_path]
      #cp_r File.join($bindir, "#{$appname}-#{$app_config["version"]}.aar"), target_path

      Dir.glob(File.join($tmpdir,'**','*.dll')) do |p|
          Jake.copyIfNeeded p, target_path
      end

      if($debug)
        Jake.copyIfNeeded File.join($tmpdir, "rhodeslibd.dll"), target_path
        Jake.copyIfNeeded File.join($tmpdir, "rhodeslibd.lib"), target_path
      else
        Jake.copyIfNeeded File.join($tmpdir, "rhodeslib.dll"), target_path
        Jake.copyIfNeeded File.join($tmpdir, "rhodeslib.lib"), target_path
      end

      rhoruby_dir = File.join($startdir, 'platform', 'shared', 'rhoruby', 'api')
      rhodeslib_h = File.join($startdir, 'platform', 'shared', 'qt', 'rhodes', 'rhorubyVersion', 'rhodeslib.h')
      mkdir_p File.join(target_path, 'rhoruby') if not File.exist?  File.join(target_path, 'rhoruby')
      cp_r rhoruby_dir, File.join(target_path, 'rhoruby')
      cp_r rhodeslib_h, File.join(target_path, 'rhoruby')

      print_timestamp('build:win32:rhodeslib_lib FINISH')
    end

    task :rhodeslib_bundle, [:target_path]  => ["build:win32:set_release_config", "config:common"] do |t, args|
      print_timestamp('build:win32:rhodeslib_bundle START')

      target_path = args[:target_path]
      $skip_build_rhodes_main = true
      $skip_build_extensions = true
      $skip_build_xmls = true
      $use_prebuild_data = true

      Rake::Task['config:win32:qt'].invoke

      appname = $app_config["name"] ? $app_config["name"] : "rhorunner"
      appname_fixed = appname.split(/[^a-zA-Z0-9]/).map { |w| w }.join("")

      Rake::Task['build:win32:rhobundle'].invoke

      mkdir_p target_path if not File.exist? target_path
      cp_r File.join($bindir, "RhoBundle"), target_path
    end


    task :deployqt => "config:win32:qt" do


    	set_vcvarsall()

    	mcvcpdlls = Dir.glob(File.join(File.dirname($vcvarsall), "..", ".." , "**", "redist", "**", "x86", "Microsoft.*.CRT","msvcp140.dll"))
    	mcvcpdlls.delete_if{|var| var.include? "onecore" }
    	mcvcpdlls.delete_if{|var| var.include? "spectre" }
    	mcvcpdlls.sort
		if (mcvcpdlls.length() == 0)
			puts "Can't find any C++ redistributable libraries for the current MSVC compiler. Please, install them and try again."
			exit 1
		end
		
		vcdlls = Dir.glob(File.join(File.dirname(mcvcpdlls.last()), "..", "**" ,"*.dll"))
		vcdlls.each do |dllpath|
			cp dllpath, $target_path if !File.exist?(File.join($target_path, File.basename(dllpath)))
		end
		puts "Joining msvc140 libs"

        cp File.join($startdir, "lib/extensions/openssl.so/ext/win32/bin/libeay32.dll"), $target_path
	    cp File.join($startdir, "lib/extensions/openssl.so/ext/win32/bin/ssleay32.dll"), $target_path

		possible_targets = [ $appname, 'rhosimulator', 'rhodes', 'rholaunch' ]
		format ="Found QT Version : #{$QVersion}" 
		targetFile = File.join($target_path, "rhodes.exe")

		begin
		possible_targets.each do |target|
			targetFile = File.join($target_path, target + ".exe")
			puts "checking " + targetFile
			break if File.file?(targetFile)
		end
		
		$logger.debug "Looking for app executable: #{targetFile}"                  
		raise "#{targetFile} not found" unless File.file?(targetFile)
		Jake.run3("#{File.join($qtdir, 'bin/windeployqt --release --no-quick-import --force')} \"#{targetFile}\"")
		
		rescue Exception => e
			$logger.error "ERROR: #{e.inspect}\n#{e.backtrace}"
		end

    end

    task :extensions => "config:win32" do
      next if $prebuild_win32

      extensions_lib = ''
      pre_targetdeps = ''

      puts "$app_extensions_list : #{$app_extensions_list}"
      $app_extensions_list.each do |ext, commin_ext_path |      
          puts "extension #{ext} [#{commin_ext_path}]"
          next unless commin_ext_path
          
          extpath = File.join(commin_ext_path, 'ext')
          ext_config_path = File.join(commin_ext_path, "ext.yml")
          ext_config = nil
          if File.exist? ext_config_path
            ext_config = Jake.config(File.open(ext_config_path))
          end
          
          project_path = ext_config["project_paths"][$current_platform] if ( ext_config && ext_config["project_paths"] && ext_config["project_paths"][$current_platform])
          next unless (File.exist?( File.join(extpath, "build.bat") ) || project_path)

          if ext != 'openssl.so'
            if ext_config.has_key?('libraries')
              ext_config["libraries"].each { |name_lib|
                extensions_lib << " #{name_lib}.lib"
                pre_targetdeps << " ../../../win32/bin/extensions/#{name_lib}.lib"
              }
            else
              extensions_lib << " #{ext}.lib"
              pre_targetdeps << " ../../../win32/bin/extensions/#{ext}.lib"
            end
          end

          ENV['PWD'] = $startdir
          ENV['TARGET_TEMP_DIR'] = File.join($startdir, $vcbindir, "extensions")
          ENV['TEMP_FILES_DIR'] = File.join($startdir, $vcbindir, "extensions", ext)

          if (project_path)	              
              ENV['RHO_ROOT'] = $startdir
              ENV['RHO_VSPROJ_SDK_PLATFORM'] = $sdk
              if ext.downcase() == "coreapi" && $rhosimulator_build
                  ENV['RHO_BUILD_CONFIG'] = 'SimulatorRelease'
              else    
                  ENV['RHO_BUILD_CONFIG'] = $rhosimulator_build ? 'Release' : $buildcfg
                  ENV['TARGET_EXT_DIR_SIM'] = File.join($startdir, $vcbindir)
              end
              
              ENV['RHO_PROJECT_PATH'] = File.join(commin_ext_path, project_path)
              ENV['RHO_EXT_NAME']=ext                
              Jake.run3('rake --trace', File.join($startdir, 'lib/build/extensions'))
          else
              ENV['RHO_BUILD_CONFIG'] = $rhosimulator_build ? 'Release' : $buildcfg
              ENV['RHO_ROOT'] = ENV['PWD']
              
              ENV['RHO_VSPROJ_SDK_PLATFORM'] = $sdk
              ENV['RHO_QMAKE'] = $qmake
              ENV['RHO_QMAKE_VARS'] = $rhosimulator_build ? 'RHOSIMULATOR_BUILD=1' : ""
              ENV['RHO_QMAKE_SPEC'] = $qmake_makespec	              

              if($debug and !$rhosimulator_build)
                ENV['RHO_QMAKE_VARS'] = ENV['RHO_QMAKE_VARS'] + " CONFIG+=debug CONFIG-=release" 
              end
              runBuldBatFile('build.bat', extpath)
          end
      end 
      generate_extensions_pri(extensions_lib, pre_targetdeps)
    end

    def generate_extensions_pri(extensions_lib, pre_targetdeps)
      ext_dir = File.join($startdir, 'platform/win32/bin/extensions')
      mkdir_p ext_dir if not File.exist? ext_dir
      File.open(File.join(ext_dir, 'extensions.pri'), "wb") do |fextensions|
        fextensions.write(%{SOURCES += ../../ruby/ext/rho/extensions.c
LIBS += /LIBPATH:../../../win32/bin/extensions#{extensions_lib}
PRE_TARGETDEPS += #{pre_targetdeps}
})
      end
    end


    task :rhobundle => [] do
    end

    task :upgrade_package => ["build:win32:rhobundle"] do        
      mkdir_p $targetdir if not File.exist? $targetdir
      zip_file_path = File.join($targetdir, "upgrade_bundle.zip")
      Jake.zip_upgrade_bundle( $bindir, zip_file_path)
    end

    task :set_debug_config do
        $buildcfg = 'Debug'
    end

    task :set_release_config do
        $buildcfg = 'Release'
    end

    task :devrhobundle => ["config:set_win32_platform", :set_debug_config, "build:win32:rhobundle", "config:win32:application", :after_bundle] do
    end

    task :after_bundle do
      win32rhopath = File.join($prebuild_win32 ? $tmpdir : File.join($vcbindir, "rhodes", 'rho'))
      mkdir_p win32rhopath
      namepath = File.join(win32rhopath,"name.txt")
      old_appname = File.read(namepath) if File.exist?(namepath)

      confpath = File.join(win32rhopath,"apps/rhoconfig.txt.changes")
      confpath_content = File.read(confpath) if File.exist?(confpath)

      if $prebuild_win32
        $target_path = $tmpdir
        if not File.directory?($target_path)
          Dir.mkdir($target_path)
        end
        target_rho_dir = File.join($tmpdir, "rho")
        rm_rf target_rho_dir
        mv $srcdir, target_rho_dir
      else
        win32rhopath = win32rhopath + '/'
        rm_rf win32rhopath + 'lib'
        rm_rf win32rhopath + 'apps'
        rm_rf win32rhopath + 'db' if old_appname != $appname

        cp_r $srcdir + '/lib', win32rhopath
        cp_r $srcdir + '/apps', win32rhopath
        cp_r $srcdir + '/db', win32rhopath
      end

      File.open(namepath, "w") { |f| f.write($appname) }
      File.open(confpath, "w") { |f| f.write(confpath_content) }  if old_appname == $appname && confpath_content && confpath_content.length()>0

    end

    task :rhosimulator => ["clean:win32:rhosimulator", "config:rhosimulator", "config:win32", "build:rhosimulator_version", "config:win32:qt"] do
      $config["platform"] = $current_platform
      chdir $startdir
      init_extensions(pwd, nil)
      Rake::Task["build:win32:extensions"].invoke

      cp File.join($startdir,"res","icons","rhosim.png"), File.join($startdir,"platform","shared","qt","rhodes","resources","rho.png")

      ENV['RHO_QMAKE_SPEC'] = $qmake_makespec

      set_vcvarsall()
      if $rhodes_as_lib
        if($debug)
          Jake.run3('rhoruby_win32_build_debug.bat "RHOSIMULATOR_BUILD=1"', $qt_project_dir)
        else
          Jake.run3('rhoruby_win32_build.bat "RHOSIMULATOR_BUILD=1"', $qt_project_dir)
        end
      else
        if($debug && !$rhosimulator_build)
          Jake.run3('rhosimulator_win32_build_debug.bat "RHOSIMULATOR_BUILD=1"', $qt_project_dir)
        else
          Jake.run3('rhosimulator_win32_build.bat "RHOSIMULATOR_BUILD=1"', $qt_project_dir)
        end
      end

      chdir $startdir

      $target_path = File.join( $startdir, "platform/win32/RhoSimulator/")
      if not File.directory?($target_path)
        Dir.mkdir($target_path)
      end
      cp File.join($startdir, $vcbindir, "RhoSimulator","RhoSimulator.exe"), $target_path

      Rake::Task["build:win32:deployqt"].invoke


      directory = $target_path
      $zipfile_name = File.join($target_path, "RhoSimulator.zip")

      args = []
      args << "a"
      args << "-tzip"
      args << $zipfile_name
      args << directory + "/*"
      puts Jake.run($zippath, args)
    end

    task :rhosimulator_to  => ["build:win32:rhosimulator"] do
	      ARGV.each { |a| task a.to_sym do ; end }
	      targetfileName = ARGV[1]

	      dir = File.dirname(targetfileName)
	      if not File.directory?(dir)
	        FileUtils.mkdir_p(dir)
	      end
	      cp $zipfile_name, targetfileName
	      
	    end

	end

	#desc "Build rhodes for win32"
	task :win32 => ["build:win32:rhobundle", "config:win32:application"] do
		next if $prebuild_win32

		chdir File.join($startdir, $platformdir)

		ENV['RHO_QMAKE_SPEC'] = $qmake_makespec
		set_vcvarsall()
		if ($rhodes_as_lib)
		  if($debug)
		    Jake.run3('rhoruby_win32_build_debug.bat "DESKTOPAPP_BUILD=1"', $qt_project_dir)
		  else
		    Jake.run3('rhoruby_win32_build.bat "DESKTOPAPP_BUILD=1"', $qt_project_dir)
		  end
		else
		  if($debug)
		    Jake.run3('rhosimulator_win32_build_debug.bat "DESKTOPAPP_BUILD=1"', $qt_project_dir)
		  else
		    Jake.run3('rhosimulator_win32_build.bat "DESKTOPAPP_BUILD=1"', $qt_project_dir)
		  end
		end
		$target_path = File.join( $startdir, $vcbindir, 'rhodes')
		if not File.directory?($target_path)
		  FileUtils.mkdir_p($target_path)
		end
		if ($rhodes_as_lib)

		  if($debug)
		    cp File.join($startdir, $vcbindir, "RhoSimulator" , "rhodeslibd.dll"), File.join($target_path)
		    cp File.join($startdir, $vcbindir, "RhoSimulator" , "rhodeslibd.lib"), File.join($target_path)
		  else
		    cp File.join($startdir, $vcbindir, "RhoSimulator" , "rhodeslib.dll"), File.join($target_path)
		    cp File.join($startdir, $vcbindir, "RhoSimulator" , "rhodeslib.lib"), File.join($target_path)
		  end
		else
		  cp File.join($startdir, $vcbindir, "RhoSimulator" , "RhoSimulator.exe"), File.join($target_path, 'rhodes.exe')
		end

		chdir $startdir
	end

end

namespace "device" do

	def createWin32Production(skip_deployqt = false, skip_nsis = false)
		if $debug
		  puts "\n\n\nYou can't build a production in debug mode. Type 'build: release' instead of 'build: debug' in root section of your build.yml.\n\n"
		end
		out_dir = File.join($startdir, $vcbindir, "rhodes")
		puts "out_dir - "  + out_dir

		mkdir_p $targetdir unless skip_nsis
		mkdir_p $tmpdir unless skip_deployqt
		mkdir_p out_dir

		if($rhodes_as_lib)
		  if($debug)
		    cp_r File.join(out_dir , "rhodeslibd.dll"), $tmpdir
		    cp_r File.join(out_dir , "rhodeslibd.lib"), $tmpdir
		  else
		    cp_r File.join(out_dir , "rhodeslib.dll"), $tmpdir
		    cp_r File.join(out_dir , "rhodeslib.lib"), $tmpdir
		  end
		else
		  cp File.join(out_dir , "rhodes.exe"), File.join($tmpdir, $appname + ".exe")
		end

		script_name = File.join($startdir, "platform", "win32", "build", "rhodes.nsi")
		app_script_name = File.join($tmpdir, $appname + ".nsi")

		license_filename = "LICENSE.txt"
		license_file = File.join($app_path, license_filename)
		license_present = '#'
		license_line = ''
		if File.exist? license_file
		  cp license_file, $tmpdir
		  license_present = ''
		  license_line = 'File "' + license_filename + '"'
		end

		readme_filename = "README.html"
		readme_file = File.join($app_path, readme_filename)
		readme_present = '#'
		readme_line = '#'
		if File.exist? readme_file
		  cp readme_file, $tmpdir
		  readme_present = ''
		  readme_line = 'File "' + readme_filename + '"'
		end

		if !skip_nsis
		  vspec_files = ''
		   if Jake.getBuildBoolProp('deployqt', $app_config, true)
		     #vspec_files += "  File /r \"imageformats\"\n"
		      #if($qtversionindex.nil? || $qtversionindex > 1)
		         #vspec_files += "  File /r \"platforms\"\n"
		      #end
		      vspec_files += "  File /r *.dll\n"
		   end
		  # custumize install script for application
		  $appdisplay_version=$app_version + '.0.0'
		  if $app_config.has_key?('vendor')
		  	$vendorname = $app_config["vendor"]
		  else
		  	$vendorname = "Tau Technologies"
		  end

		  install_script = File.read(script_name)
		  install_script = install_script.gsub(/%OUTPUTFILE%/, $targetdir + "/" + $appname + "-setup.exe" )
		  install_script = install_script.gsub(/%APPNAME%/, $appname)
		  install_script = install_script.gsub(/%APPVERSION%/, $app_version)
		  install_script = install_script.gsub(/%APPDISPLAYVERSION%/, $appdisplay_version)
		  install_script = install_script.gsub(/%APP_EXECUTABLE%/, $appname + ".exe") 
		  install_script = install_script.gsub(/%SECTOIN_TITLE%/, "\"This installs " + $appname + "\"")
		  install_script = install_script.gsub(/%FINISHPAGE_TEXT%/, "\"Thank you for installing " + $appname + " \\r\\n\\n\\n\"")
		  install_script = install_script.gsub(/%APPINSTALLDIR%/, "C:\\" + $appname)
		  install_script = install_script.gsub(/%APPICON%/, "icon.ico")
		  install_script = install_script.gsub(/%GROUP_NAME%/, $vendorname)
		  install_script = install_script.gsub(/%SECTION_NAME%/, "\"" + $appname + "\"")
		  install_script = install_script.gsub(/%LICENSE_FILE%/, license_line)
		  install_script = install_script.gsub(/%LICENSE_PRESENT%/, license_present)
		  install_script = install_script.gsub(/%README_FILE%/, readme_line)
		  install_script = install_script.gsub(/%README_PRESENT%/, readme_present)
		  install_script = install_script.gsub(/%QT_VSPEC_FILES%/, vspec_files)
		  install_script = install_script.gsub(/%VENDOR%/, $vendorname)
		  File.open(app_script_name, "w") { |file| file.puts install_script }
		end

		puts "$appname - " + $appname

		cp $app_icon_path, $tmpdir + "/icon.ico"
		cp $qt_icon_path, $tmpdir + "/icon.png"

		if !skip_nsis
		  File.open(File.join($targetdir,"app_info.txt"), "w") { |f| f.write( $vendorname + "/" + $appname + "/" + $appname + ".exe") }
		end

		chdir $tmpdir

		if !skip_deployqt
		  target_rho_dir = File.join($tmpdir, "rho")
		  rm_rf target_rho_dir
		  mv $srcdir, target_rho_dir
		end

		$target_path = $tmpdir
		Rake::Task["build:win32:deployqt"].invoke unless skip_deployqt

		if !skip_nsis
		  puts "$nsis - " + $nsis
		  args = ['"' + $tmpdir + "/" + $appname + ".nsi\""]
		  puts "arg = " + args.to_s
		  puts Jake.run2($nsis, args, {:nowait => false} )
		end
	end


	namespace "win32" do
		desc "Build installer for Windows"
		task :production => ["build:win32:set_release_config", "build:win32"] do
		  createWin32Production()
		end

		desc "Build Windows for production, use prebuild binaries"
		task :production_with_prebuild_binary do
		  $prebuild_win32 = true
		  Rake::Task["device:win32:build_with_prebuild_binary"].invoke
		end
		task :build_with_prebuild_binary => ["build:win32:set_release_config", "build:win32:rhobundle", "config:win32:application"] do
		  container_path = determine_prebuild_path_win('win32', $app_config)

		  #Support build.yml settings on cloud by copying to rhoconfig.txt
		  Rake::Task['config:common:ymlsetup'].invoke

		  Rake::Task['device:win32:apply_container'].invoke(container_path)
		  createWin32Production(true,false)
		end

		desc 'Creates application container. See also device:win32:apply_container.'
		task :make_container, [:container_prefix_path] => ["build:win32:set_release_config", "build:win32"] do |t, args|
		  createWin32Production(false,true)

		  container_prefix_path = args[:container_prefix_path]

		  Dir.glob("#{container_prefix_path}*") {|f| rm_r(f)}
		  mkdir_p container_prefix_path

		  cd $startdir
		  rhodes_gem_paths = ([
		    'platform/wm/bin/win32/rhodes/**/rhodes.exe'
		  ].map {|p| Dir.glob(p)}).flatten
		  bin_exclude_files = [
		    'bin/tmp/*.exe',
		    'bin/tmp/*.time',
		    'bin/tmp/*.ico',
		    'bin/tmp/*.png',
		    'bin/tmp/rho/RhoBundleMap.txt',
		    'bin/tmp/rho/apps/rhoconfig.txt*',
		    'bin/tmp/rho/apps/rhofilelist.txt',
		    'bin/tmp/rho/apps/app',
		    'bin/tmp/rho/apps/app_manifest.txt',
		    'bin/tmp/rho/apps/public/public.txt'
		  ]

		  pack_7z($app_path, ['bin'], File.join(container_prefix_path, 'application_override.7z'), bin_exclude_files)
		  pack_7z($startdir, rhodes_gem_paths, File.join(container_prefix_path, 'rhodes_gem_override.7z'))
		end

		desc 'Applies application container. See also device:win32:make_container'
		task :apply_container, [:container_prefix_path] do |t, args|
		  container_prefix_path = args[:container_prefix_path]

		  unpack_7z($app_path, File.join(container_prefix_path, 'application_override.7z'))
		  unpack_7z($startdir, File.join(container_prefix_path, 'rhodes_gem_override.7z'))
		end

	end

end

namespace "clean" do

	desc "Clean win32"
	task :win32 => ["config:set_win32_platform", "config:win32", "clean:common"]do
		rm_rf File.join($vcbindir, "win32")
		rm_rf $tmpdir
		rm_rf $targetdir
		rm_rf File.join($startdir, 'platform','shared','qt','rhodes','GeneratedFiles')
		rm_rf File.join($startdir, 'platform','win32','bin')
		rm_rf File.join($startdir, 'platform','wm','bin')

		rm_rf File.join($app_path, "bin","tmp") if File.exist? File.join($app_path, "bin", "tmp")
		rm_rf File.join($app_path, "bin","RhoBundle") if File.exist? File.join($app_path, "bin", "RhoBundle")
	end

end


namespace "run" do

	def copyBundleToExecutableDir()
		app_rhodir = File.join($startdir, $vcbindir, "rhodes", 'rho')
		rm_rf app_rhodir if File.exist?(app_rhodir)
		bundleDir = File.join($bindir, 'RhoBundle')
		FileUtils.cp_r bundleDir, app_rhodir
	end	

	desc "Run win32"
	task :win32 => ["build:win32"] do
		unless $prebuild_win32
		  rundir = File.join($startdir, $vcbindir, "rhodes")
		  $target_path = rundir
		  rundir = File.join(rundir, "rho");
		  exefile = "../rhodes.exe"
		else
		  rundir = $target_path
		  exefile = File.join($target_path, $appname + '.exe')
		end

		Rake::Task["build:win32:deployqt"].invoke unless $prebuild_win32

		copyBundleToExecutableDir()

		cp $qt_icon_path, $target_path + "/icon.png"
		args = ['--remote-debugging-port=9090']
		#    chdir rundir
		#    Thread.new { Jake.run("bin\\win32\\rhodes\\Debug\\rhodes", args) }
		Jake.run2 exefile, args, {:directory => rundir, :nowait => true}

		$stdout.flush
		chdir $startdir

		unless $? == 0
		  puts "Error to run rhodes for win32"
		  exit 1
		end
	end

	namespace "win32" do
		rhosim_task = lambda do |name, &block|
		    task name => ["config:set_win32_platform", "config:common"] do
			    $rhosim_config = "platform='win32'\r\n"
			    block.()
		    end
		end

		desc "Run application on RhoSimulator"
		rhosim_task.(:rhosimulator) { Rake::Task["run:rhosimulator"].invoke }
		namespace :rhosimulator do
		    rhosim_task.(:build) { Rake::Task["run:rhosimulator:build"].invoke         }
		    rhosim_task.(:debug) { Rake::Task["run:rhosimulator:run"  ].invoke('wait') }
		end

		task :package do
			$prebuild_win32 = true
			Rake::Task["run:win32:run_with_prebuild_binary"].invoke
		end

		task :run_with_prebuild_binary => ["build:win32:set_release_config", "build:win32:rhobundle", "config:win32:application"] do
			container_path = determine_prebuild_path_win('win32', $app_config)
			Rake::Task['device:win32:apply_container'].invoke(container_path)

			createWin32Production(true,true)

			Rake::Task['run:win32'].invoke()
		end

		task :delete_db => ["config:win32"] do
		  $buildcfg = $app_config["buildcfg"]
		  $buildcfg = "Release" unless $buildcfg

		  db_path = File.join($vcbindir, "rhodes", 'rho', 'db')
		  rm_rf db_path if File.exist?(db_path)
		end
		#run:win32:spec
		task :spec => [:delete_db] do
			Jake.decorate_spec do
			    Rake::Task['build:win32'].invoke
			    copyBundleToExecutableDir()
			    #remove log file
			    win32rhopath = File.join($startdir, $vcbindir, "rhodes", 'rho')
			    win32logpath = File.join(win32rhopath, "RhoLog.txt")
			    win32logpospath = File.join(win32rhopath, "RhoLog.txt_pos")
			    win32configpath = File.join(win32rhopath,"apps", "rhoconfig.txt.changes")
			    rm_rf win32logpath if File.exist?(win32logpath)
			    rm_rf win32logpospath if File.exist?(win32logpospath)
			    rm_rf win32configpath if File.exist?(win32configpath)

			    Jake.before_run_spec
			    start = Time.now

			    args = [' ']

			    targetFile = "../rhodes.exe"
			    targetDirectory = win32rhopath
			    targetFileFullName = File.join(targetDirectory, targetFile)
			    start = Time.now
			    if File.exist? targetFileFullName
			      Jake.run3("#{File.join($qtdir, 'bin', 'windeployqt')} #{targetFileFullName}")
			    end
			    counter = 0
			    Jake.run2(targetFile, args, {:directory => targetDirectory, :nowait => false}) do |line|
			      counter += 1
			    end
			    counter = 0
			    sleep(5)
			    File.open(win32logpath, 'r:UTF-8').each do |line|
			      counter += 1
			      Jake.process_spec_output(line)
			    end
			    puts "Checked lines: " + counter.to_s
			    Jake.process_spec_results(start)

			    $stdout.flush
			    chdir $startdir
			    
			    if ($failed > 0) 
			      puts "Specs failed with " + $failed.to_s + " failes"
			      exit 1
			    end
			end
		end
	end
end



namespace 'stop' do
  task :win32 => ['config:win32'] do
    Jake.get_process_list.each do |p|
      next unless p[:cmd] =~ /^bin\\win32\\rhodes\\rhodes\.exe /
      Jake.run2('taskkill.exe', ['/F', '/PID', p[:pid]], {:hide_output => true})
    end
  end
end

