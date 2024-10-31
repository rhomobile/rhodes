#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2020 Tau Technologies, Inc.
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
require 'erb'

namespace "config" do
	task :set_current_platform_linux do
		$current_platform = "linux"
	end

	task :linux => ["config:set_current_platform_linux", "switch_app", "config:qt", "config:sys_recognize"] do
		$rubypath = "platform/linux/target/compiler/rubylinux" #path to RubyLixux
		$bindir = $app_path + "/bin"
		$srcdir =  $bindir + "/RhoBundle"
		$excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

		$appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
		$appname = $appname.gsub(/[^0-9a-zA-Z\-\+\.]/,'')
		$vendor = $app_config['vendor']
		$vendor = "Tau Technologies" if $vendor.nil?
		$rhobundledir =  $app_path + "/RhoBundle"
		$log_file = $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"]
		$buildcfg = $app_config["buildcfg"] unless $buildcfg
		$buildcfg = "Release" unless $buildcfg
		$detoolappflag = $js_application == true ? "js" : "ruby"
		$tmpdir = File.join($bindir, "tmp")
		$qt_project_dir = File.join( $startdir, 'platform/shared/qt/' )

		$homedir = `echo ~`.to_s.strip
		$current_platform = "linux"
		$qmake_makespec = "linux-g++"
		Rake::Task["config:common"].invoke()

		$connf_build = "Release"
		if !$app_config.nil? && !$app_config["linux"].nil?
			puts "Target configuration: " + $connf_build
		else
			puts "Linux section is not found!"
		end

		if !$app_config["version"].nil?
			$version_app = $app_config["version"]
		else
			$version_app = "1.0"
		end

		$excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
		$rho_path =  File.join($app_path, "data", "rho")
		$project_path = File.join($app_path, "project", "qt")
		$app_project_path = File.join($project_path, $appname.downcase)

		$target_path = File.join($app_path, "bin", "target", "linux")
		mkdir_p $target_path
	end

	task :sys_recognize do
		$architecture = Jake.run("uname", ["-m"])
		name_out = Jake.run('hostnamectl', [])
		if name_out.downcase().include? "ubuntu"
			$ubuntu = true
			puts "Current system is Ubuntu"
		elsif name_out.downcase().include? "astra"
			$astra = true
			puts "Current system is Astra Linux"
		elsif name_out.downcase().include? ":alt:"
			$altlinux = true
			puts "Current system is Alt Linux"
		elsif name_out.downcase().include? ":redos:"
			$redos = true
			puts "Current system is Red OS Linux"
			$qmake_addition_args = '"LIBS += -L/usr/lib64/libglvnd/ -lGL"'
		elsif name_out.downcase().include? ":rosa:"
			$rosalinux = true
			puts "Current system is Rosa Linux"
		else
			puts "Fail! Current system has not been recognized while cunfiguration."
			exit 1
		end
	end


	namespace "linux" do
		task :application do
			$app_version = '1.0'
			$app_version = $app_config["version"] unless $app_config["version"].nil?

			require $startdir + "/lib/rhodes.rb"
			fversion = StringIO.new("", "w+")
			fversion.write( "#define RHOSIMULATOR_NAME \"RhoSimulator\"\n" )
			fversion.write( "#define RHOSIMULATOR_VERSION \"#{Rhodes::VERSION}\"\n" )
			fversion.write( "#define APPLICATION_NAME \"#{$appname}\"\n" )
			fversion.write( "#define APPLICATION_VERSION \"#{$app_version}\"\n" )
			Jake.modify_file_if_content_changed( File.join($startdir, 'platform/shared/qt/rhodes/RhoSimulatorVersion.h'), fversion )

			$app_icon_path = $app_path + "/icon/icon.ico"
			$app_icon_path = $startdir + "/res/icons/rhodes.ico" unless File.exist? $app_icon_path
			cp $app_icon_path, $startdir + "/platform/shared/qt/rhodes/resources/rho.ico"

			$qt_icon_path = $app_path + "/icon/icon.png"
			$qt_icon_path = $startdir + "/res/icons/rhodes.png" unless File.exist? $qt_icon_path
			cp $qt_icon_path, $startdir + "/platform/shared/qt/rhodes/resources/rho.png"
			qrcfile = $startdir + "/platform/shared/qt/rhodes/GeneratedFiles/" + $buildcfg + "/qrc_simulator.cpp"
			rm qrcfile if File.exist? qrcfile
		end
	end
end

namespace "build" do
	namespace "linux" do
		task :rhobundle => ["config:linux", "build:bundle:noxruby"] do
			if !$skip_build_extensions
				Rake::Task["build:linux:extensions"].execute
			end
			Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
		end

		task :appbundle => ["config:linux", "build:bundle:noxruby"] do
			Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
		end

		task :extensions => "config:linux" do
			require $startdir + "/lib/rhodes.rb"
			fversion = StringIO.new("", "w+")
			fversion.write( "#define RHOSIMULATOR_NAME \"RhoSimulator\"\n" )
			fversion.write( "#define RHOSIMULATOR_VERSION \"#{Rhodes::VERSION}\"\n" )
			fversion.write( "#define APPLICATION_NAME \"#{$appname}\"\n" )
			fversion.write( "#define APPLICATION_VERSION \"#{$app_version}\"\n" )
			Jake.modify_file_if_content_changed( File.join($startdir, 'platform/shared/qt/rhodes/RhoSimulatorVersion.h'), fversion )

			extensions_lib = ''

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
				next unless (File.exist?( File.join(extpath, "build") ) || project_path)

				if (ext != 'openssl.so')
					extensions_lib << "LIBS += -L$$PWD/../../../linux/bin/extensions/ -l#{ext.downcase}\n"
					extensions_lib << "PRE_TARGETDEPS += $$PWD/../../../linux/bin/extensions/lib#{ext.downcase}.a\n\n"
				end

				if (project_path)

					ENV['RHO_PLATFORM'] = 'linux'
					ENV['PWD'] = $startdir
					ENV['RHO_ROOT'] = $startdir
					ENV['RHO_VSPROJ_SDK_PLATFORM'] = $sdk
					if ext.downcase() == "coreapi" && $rhosimulator_build
							ENV['RHO_BUILD_CONFIG'] = 'SimulatorRelease'
					else
							ENV['RHO_BUILD_CONFIG'] = $rhosimulator_build ? 'Release' : $buildcfg
							ENV['TARGET_EXT_DIR_SIM'] = File.join($startdir, "platform", 'linux', "bin", $sdk, "rhodes", $rhosimulator_build ? "SimulatorRelease" : $buildcfg)
					end

					ENV['TEMP_FILES_DIR'] = File.join($startdir, "platform", "linux", "bin", "extensions", ext)
					ENV['RHO_PROJECT_PATH'] = File.join(commin_ext_path, project_path)
					ENV['TARGET_TEMP_DIR'] = File.join($startdir, "platform", "linux", "bin", "extensions")

					ENV['RHO_EXT_NAME']=ext
					Jake.run3('rake --trace', File.join($startdir, 'lib/build/extensions'))
				else
					ENV['RHO_PLATFORM'] = 'linux'
					ENV['RHO_BUILD_CONFIG'] = $rhosimulator_build ? 'Release' : $buildcfg
					ENV['PWD'] = $startdir
					ENV['RHO_ROOT'] = ENV['PWD']
					ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", "linux", "bin", "extensions")
					ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform", "linux", "bin", "extensions", ext)
					ENV['RHO_VSPROJ_SDK_PLATFORM'] = $sdk
					ENV['RHO_QMAKE'] = $qmake
					ENV['RHO_QMAKE_VARS'] = $rhosimulator_build ? 'RHOSIMULATOR_BUILD=1' : ""
					ENV['RHO_QMAKE_SPEC'] = $qmake_makespec
					ENV['RHO_VSCMNTOOLS'] = $vscommontools

					if($debug and !$rhosimulator_build)
						ENV['RHO_QMAKE_VARS'] = ENV['RHO_QMAKE_VARS'] + " CONFIG+=debug CONFIG-=release"
					end
					puts ENV['QTDIR']
					Jake.run3('./build', extpath, {}, true)
				end
			end

			ext_dir = File.join($startdir, 'platform/linux/bin/extensions')
			mkdir_p ext_dir if not File.exist? ext_dir
			File.open(File.join(ext_dir, 'extensions.pri'), "wb") do |fextensions|
				fextensions.puts "SOURCES += ../../ruby/ext/rho/extensions.c"
				fextensions.puts extensions_lib
			end
		end


		task :after_bundled do
			linux_rhopath = $tmpdir + '/rho'
			mkdir_p linux_rhopath
			namepath = File.join(linux_rhopath,"name.txt")
			old_appname = File.read(namepath) if File.exist?(namepath)

			confpath = File.join(linux_rhopath,"apps/rhoconfig.txt.changes")
			confpath_content = File.read(confpath) if File.exist?(confpath)

			linux_rhopath = linux_rhopath + '/'
			rm_rf linux_rhopath + 'lib'
			rm_rf linux_rhopath + 'apps'
			rm_rf linux_rhopath + 'db' if old_appname != $appname

			cp_r $srcdir + '/lib', linux_rhopath
			cp_r $srcdir + '/apps', linux_rhopath
			cp_r $srcdir + '/db', linux_rhopath

			File.open(namepath, "w") { |f| f.write($appname) }
			File.open(confpath, "w") { |f| f.write(confpath_content) }  if old_appname == $appname && confpath_content && confpath_content.length()>0
		end
	end

	task :linux => ["build:linux:rhobundle", "config:linux:application"] do
		puts "Starting to build linux application in " + $qt_project_dir
		chdir $config["build"]["linuxpath"]

		ENV['RHO_QMAKE_SPEC'] = $qmake_makespec
		ENV['RHO_VSCMNTOOLS'] = $vscommontools


		target_app_name = File.join($target_path, $appname)
		if !File.exist?(target_app_name)
			if $qmake_addition_args != nil and $qmake_addition_args != ''
				Jake.run3('"$QTDIR/bin/qmake" -o Makefile -r -spec $RHO_QMAKE_SPEC "CONFIG-=debug" "CONFIG+=release" ' + 
					$qmake_addition_args + ' RhoSimulator.pro', $qt_project_dir)
			else
				Jake.run3('"$QTDIR/bin/qmake" -o Makefile -r -spec $RHO_QMAKE_SPEC "CONFIG-=debug" "CONFIG+=release" RhoSimulator.pro', 
					$qt_project_dir)
			end
			Jake.run3('make clean', $qt_project_dir)
			Jake.run3('make all', $qt_project_dir)
			puts "Copying to dir" + $target_path

			if not File.directory?($target_path)
				FileUtils.mkdir_p($target_path)
			end

			cp File.join($startdir, "platform/linux/bin/RhoSimulator/RhoSimulator"), target_app_name
		end
		rho_path = File.join($target_path, "rho")
		rm_rf rho_path if File.exist?(rho_path)
		cp_r File.join($app_path, "bin/RhoBundle"), rho_path

		chdir $startdir
	end

end

namespace "device" do
	namespace "linux" do
		namespace "production" do
			def createFolders()
				opt_path = File.join($target_path, "opt", "#{$appname}")

				if not File.directory?(opt_path)
					FileUtils.mkdir_p opt_path
				end

				icon = File.join($app_path, "icon/icon.ico")
				if !File.exist? icon
					icon = File.join($startdir, "platform" , "wm", "rhodes", "resources", "icon.ico")
				end

				cp icon, opt_path
				FileUtils.mv(File.join($target_path, "rho"), opt_path, :verbose => true, :force => true)
				FileUtils.chmod_R(0777, File.join(opt_path, "rho"))
				FileUtils.mv(File.join($target_path, $appname), opt_path, :verbose => true, :force => true)

				desktop_path = File.join($target_path, "usr", "share", "applications")
				if not File.directory?(desktop_path)
					FileUtils.mkdir_p desktop_path
				end

				File.open(File.join(desktop_path, "#{$appname}.desktop"), 'w') { |file|
					file.write("[Desktop Entry]\n")
					file.write("Type=Application\n")
					file.write("Version=#{$version_app}\n")
					file.write("Name=#{$appname}\n")
					file.write("GenericName=\"Web Browser\"\n")
					file.write("Exec=/opt/#{$appname}/#{$appname}\n")
					file.write("Icon=/opt/#{$appname}/icon.ico\n")
				}
			end

			def addDepLibs()
				ldd_out = Jake.run("ldd", [File.join($target_path, $appname)])
				app_libs = File.join($target_path, "opt", "#{$appname}", "app_libs")
				if not File.directory?(app_libs)
				  FileUtils.mkdir_p app_libs
				end

				depsarray = ldd_out.scan( /(\/[\/\.\w\-\+]*\.so[\.\d]*)/)
				depsarray.uniq.each{
				  |dep| appdep = dep[0]

				  target_dep_name = appdep.scan(/\/(libQ[\.\w\-\+]*\.so[\.\d]*)/)
				  target_dep_name.uniq.each{
				    |deplibname|
				    cp appdep, File.join(app_libs, deplibname[0])
				    puts "Adding lib #{appdep}"
				  }
				}
			end

			task :deb => ["build:linux"] do
				createFolders()

				control_template = File.read File.join( $startdir, "platform", "linux", "tasks", "control.erb")
				erb = ERB.new control_template

				debian_dir = File.join($target_path, "DEBIAN")
				if not File.directory?(debian_dir)
					FileUtils.mkdir_p debian_dir
				end
				File.open(File.join(debian_dir, "control"), 'w' ) { |f| f.write erb.result binding }

				Jake.run3('fakeroot dpkg-deb --build linux', File.join($app_path, "bin", "target"))
				rm_rf $target_path
				FileUtils.mkdir_p $target_path
				FileUtils.mv(File.join($app_path, "bin", "target", "linux.deb"), File.join($target_path, "#{$appname}_#{$version_app}_amd64.deb"))
			end

			task :rpm => ["build:linux"] do
				createFolders()
				target_folder = File.join($app_path, "bin", "target")
				$linuxroot = File.join(target_folder, "linux")
				$buildroot = File.join($linuxroot, "rpmbuildroot")


				$bin_file = "linux.tar"
				$bin_archive = File.join(target_folder, $bin_file)

				rm $bin_archive if File.exist? $bin_archive
				FileUtils.mv($linuxroot, File.join(target_folder, "#{$appname}-#{$version_app}"))
				Jake.run3("tar -cvf #{$bin_file} #{$appname}-#{$version_app}", target_folder)
				FileUtils.rm_r File.join(target_folder, "#{$appname}-#{$version_app}") if File.exist? File.join(target_folder, "#{$appname}-#{$version_app}")

				FileUtils.mkdir_p $buildroot
				FileUtils.mkdir_p File.join($buildroot, "SOURCES")
				FileUtils.mkdir_p File.join($buildroot, "BUILD")
			    FileUtils.mkdir_p File.join($buildroot, "BUILDROOT")
				FileUtils.mkdir_p File.join($buildroot, "RPMS")
				FileUtils.mkdir_p File.join($buildroot, "SPECS")
				FileUtils.mkdir_p File.join($buildroot, "SRPMS")

				FileUtils.mv($bin_archive, File.join($buildroot, "SOURCES", "#{$appname}.tar"))
				$bin_archive = File.join($buildroot, "SOURCES", "#{$appname}.tar")

				control_template = File.read File.join( $startdir, "platform", "linux", "tasks", "rpm_spec.erb")
				erb = ERB.new control_template
				File.open(File.join($buildroot, "rpm.spec"), 'w' ) { |f| f.write erb.result binding }
                
                lint_exceptions_template = File.read File.join( $startdir, "platform", "linux", "tasks", "linter_exceptions.erb")
				erb = ERB.new lint_exceptions_template
				File.open(File.join($buildroot, "SOURCES", "#{$appname}.rpmlintrc"), 'w' ) { |f| f.write erb.result binding }
                

				puts Jake.run3("rpmbuild --define \"_topdir #{$buildroot}\" --define '_build_id_links none' --define 'debug_package %{nil}' -bb rpm.spec", $buildroot)

				Dir.glob(File.join($buildroot, "**", "*.rpm")).each do | filename |
					FileUtils.mv(filename, File.join($linuxroot, File.basename(filename)))
					puts filename
				end
				FileUtils.rm_r $buildroot

			end
		end

		task :production => ["config:sys_recognize", "clean:linux"] do
            $linter_exceptions = []
            $additional_args = ""
			if $ubuntu
				$deps = "qt5-default, libqt5webengine5, libqt5webenginecore5, libqt5webenginewidgets5, libqt5multimedia5"
				Rake::Task['device:linux:production:deb'].invoke
			elsif $astra
				$deps = "libqt5widgets5, libqt5gui5, libqt5network5, libqt5webengine5, libqt5webenginecore5, libqt5webenginewidgets5, libqt5multimedia5"
				Rake::Task['device:linux:production:deb'].invoke
			elsif $altlinux
				$create_buildroot = true
				$deps = ["libqt5-qml", "libqt5-quickwidgets", "libqt5-webenginecore", "libqt5-webengine", "libqt5-core", "libqt5-gui", 
				"libqt5-network", "libqt5-multimedia", "libgmp", "libstdc++"]
				Rake::Task['device:linux:production:rpm'].invoke
			elsif $redos
				$deps = ["qt5", "qt5-qtbase", "qt5-qtbase-common", "qt5-qtbase-gui", "qt5-qtwebengine", 
				"qt5-qtmultimedia", "qt5-qtwebchannel", "gmp", "libstdc++"]
				Rake::Task['device:linux:production:rpm'].invoke
			elsif $rosalinux
				#$create_buildroot = true
				$architecture = "noarch"
                #$additional_args = "%global _nonzero_exit_pkgcheck_terminate_build 0"
                $linter_exceptions = ["W: no-binary", "E: arch-independent-package-contains-binary-or-object", "E: dir-or-file-in-opt", "E: non-standard-dir-perm", "W: desktopfile-without-binary", "W: dir-or-file-in-opt", "E: non-standard-executable-perm", "E: explicit-lib-dependency", "W: no-documentation", "E: invalid-desktopfile", "W: script-without-shebang", "E: non-standard-group", "W: no-url-tag","E: world-writable", "E: wrong-script-end-of-line-encoding"]
				$deps = ["lib64qt5webenginecore5", "lib64qt5webenginewidgets5" , "lib64qt5webengine5" , "lib64qt5multimedia5", "lib64qt5gui5", "lib64gmp10"]
				Rake::Task['device:linux:production:rpm'].invoke
			else
				puts "Fail! The current system has not been recognized whild production."
				exit 1
			end
			puts "Finished"
		end
	end
end

namespace "run" do
	task :linux => ["build:linux"] do
		Jake.run3(File.join($target_path, $appname) + ' --remote-debugging-port=9090')
	end

	namespace "linux" do
		task :spec do
			Jake.decorate_spec do
			  	#Rake::Task['config:linux'].invoke
			    Rake::Task['build:linux'].invoke

			    linuxrhopath = File.join($target_path, 'rho')

			    dbpath = File.join(linuxrhopath, 'db')
			  	rm_rf dbpath if File.exist?(dbpath)

			    linuxlogpath = File.join(linuxrhopath,"rholog.txt")
			    linuxlogpospath = File.join(linuxrhopath, "rholog.txt_pos")
			    linuxconfigpath = File.join(linuxrhopath, "apps", "rhoconfig.txt.changes")
			    rm_rf linuxlogpath if File.exist?(linuxlogpath)
			    rm_rf linuxlogpospath if File.exist?(linuxlogpospath)
			    rm_rf linuxconfigpath if File.exist?(linuxconfigpath)

			    Jake.before_run_spec
			    start = Time.now
			    args = [' ']

			    targetFile = File.join("..", $appname)
			    start = Time.now
			    counter = 0
			    Jake.run2(targetFile, args, {:directory => linuxrhopath, :nowait => false}) do |line|
			      counter += 1
			    end
			    counter = 0
			    sleep(5)

			    current_spec_name = ''
			    failed_specs = []
			    File.open(linuxlogpath, 'r:UTF-8').each do |line|
			      counter += 1
			      Jake.process_spec_output(line)
			      currentSpecArray = line.scan( /.*\[([\w\\\/\_]{5,})\].*/)
			      currentSpecArray.uniq.each{
				    |specname| current_spec_name = specname
				  }

				  failCheckArray = line.scan(/^.*(FAIL:).*[^(RHO: not supported)]$/)
				  if (failCheckArray.length > 0)
				  	failed_specs += current_spec_name
				  	puts "Fail on line #{counter}"
				  end
			    end

			    puts failed_specs.uniq

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

namespace "clean" do
	namespace "linux" do
		task :rhosimulator do
			rhoSimDir = File.join( $startdir, "platform/linux/RhoSimulator" )
			FileUtils.rm_rf("#{rhoSimDir}/.", secure: true)
		end
	end

	task :linux => ["config:linux", "clean:common"]do
		rm_rf $tmpdir
		rm_rf File.join($startdir, 'platform/shared/qt/rhodes/GeneratedFiles')
		rm_rf File.join($startdir, 'platform/linux/bin')

		#common_target_path = File.join($app_path, "bin")
		#rm_rf common_target_path if File.exist? common_target_path

		rm_rf File.join($app_path, "bin", "tmp") if File.exist? File.join($app_path, "bin", "tmp")
		rm_rf File.join($app_path, "bin", "RhoBundle") if File.exist? File.join($app_path, "bin", "RhoBundle")
        rm_rf File.join($app_path, "bin", "target") if File.exist? File.join($app_path, "bin", "target")
	end
end
