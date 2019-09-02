#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2019 Tau Technologies, Inc.
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
  task :linux => ["switch_app", "config:qt"] do

    $rubypath = "platform/linux/target/compiler/rubylinux" #path to RubyMac
    $bindir = $app_path + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
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

    if !$app_config.nil? && !$app_config["linux"].nil?
      $connf_build = "Release"
      puts "Target configuration: " + $connf_build
    else
      raise "Linux section is not found!"
    end
    if !$app_config["name"].nil?    
      $appname = $app_config["name"]
      $final_name_app = "#{$appname.downcase}"
    else
      raise "Name application is not found in build.yaml!"
    end

    if !$app_config["version"].nil?
      $version_app = $app_config["version"]
    else
      $version_app = "1.0"
    end
    
    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
    $rho_path =  File.join($app_path, "data", "rho")
    $project_path = File.join($app_path, "project", "qt")
    $app_project_path = File.join($project_path, $final_name_app)

    $target_path = File.join($app_path, "bin", "target", "linux")
    mkdir_p $target_path
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
      $app_icon_path = $startdir + "/res/icons/rhodes.ico" unless File.exists? $app_icon_path
      cp $app_icon_path, $startdir + "/platform/shared/qt/rhodes/resources/rho.ico"

      $qt_icon_path = $app_path + "/icon/icon.png"
      $qt_icon_path = $startdir + "/res/icons/rhodes.png" unless File.exists? $qt_icon_path
      cp $qt_icon_path, $startdir + "/platform/shared/qt/rhodes/resources/rho.png"
      qrcfile = $startdir + "/platform/shared/qt/rhodes/GeneratedFiles/" + $buildcfg + "/qrc_simulator.cpp"
      rm qrcfile if File.exists? qrcfile
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

    task :extensions => "config:linux" do
      require $startdir + "/lib/rhodes.rb"
      fversion = StringIO.new("", "w+")          
      fversion.write( "#define RHOSIMULATOR_NAME \"RhoSimulator\"\n" )
      fversion.write( "#define RHOSIMULATOR_VERSION \"#{Rhodes::VERSION}\"\n" )
      fversion.write( "#define APPLICATION_NAME \"#{$appname}\"\n" )
      fversion.write( "#define APPLICATION_VERSION \"#{$app_version}\"\n" )
      Jake.modify_file_if_content_changed( File.join($startdir, 'platform/shared/qt/rhodes/RhoSimulatorVersion.h'), fversion )

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
          next unless (File.exists?( File.join(extpath, "build") ) || project_path)

          if ext != 'openssl.so'
            if ext_config.has_key?('libraries')
              ext_config["libraries"].each { |name_lib|
                extensions_lib << " #{name_lib}.so"
                pre_targetdeps << " ../../../linux/bin/extensions/#{name_lib}.so"
              }
            else
              extensions_lib << " #{ext}.so"
              pre_targetdeps << " ../../../linux/bin/extensions/#{ext}.so"
            end
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
      mkdir_p ext_dir if not File.exists? ext_dir
      File.open(File.join(ext_dir, 'extensions.pri'), "wb") do |fextensions|
        fextensions.write(%{SOURCES += ../../ruby/ext/rho/extensions.c
        LIBS += /LIBPATH:../../../linux/bin/extensions#{extensions_lib}
        PRE_TARGETDEPS += #{pre_targetdeps}
        })
      end
    end
  

    task :after_bundle do
      linux_rhopath = $tmpdir + '/rho'
      mkdir_p linux_rhopath
      namepath = File.join(linux_rhopath,"name.txt")
      old_appname = File.read(namepath) if File.exists?(namepath)

      confpath = File.join(linux_rhopath,"apps/rhoconfig.txt.changes")
      confpath_content = File.read(confpath) if File.exists?(confpath)

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

    #Jake.run3('rhosimulator_linux_build', $qt_project_dir)
    
    Jake.run3('"$QTDIR/bin/qmake" -o Makefile -r -spec $RHO_QMAKE_SPEC "CONFIG-=debug" "CONFIG+=release" RhoSimulator.pro', $qt_project_dir)
    Jake.run3('make clean', $qt_project_dir)
    Jake.run3('make all', $qt_project_dir)
    puts "Copying to dir" + $target_path

    if not File.directory?($target_path)
      Dir.mkdir($target_path)
    end

    cp File.join($startdir, "platform/linux/bin/RhoSimulator/RhoSimulator"), 
    File.join($target_path, $appname)
    cp_r File.join($app_path, "bin/RhoBundle"), File.join($target_path, "rho")

    chdir $startdir
  end

end

namespace "device" do
  namespace "linux" do
    task :production => ["build:linux"] do
      #ldd_out = Jake.run("ldd", [File.join($target_path, $appname)])
      name_out = Jake.run('uname', ["-a"])

      if name_out.downcase().include? "ubuntu"
        puts "The current system is Ubuntu"
        $deps = "qt5-default, libqt5webengine5, libqt5webenginecore5, libqt5webenginewidgets5, libqt5multimedia5"
      elsif name_out.downcase().include? "arch"
        $deps = "qt5-base-5.13.0-7, qt5-connectivity-5.13.0-1, qt5-declarative-5.13.0-1, qt5-imageformats-5.13.0-2, qt5-multimedia-5.13.0-1, qt5-networkauth-5.13.0-1, qt5-quickcontrols2-5.13.0-1, qt5-tools-5.13.0-2, qt5-translations-5.13.0-1, qt5-webchannel-5.13.0-1, qt5-webengine-5.13.0-4, qt5-webglplugin-5.13.0-1, qt5-websockets-5.13.0-1"
        puts "The current system is Archlinux"
      else
        puts "Fail! The current system has not been recognized."
        exit 1
      end          
      
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
        #file.write("Exec=env LD_LIBRARY_PATH=\"/opt/#{$appname}/app_libs\" /opt/#{$appname}/#{$appname}\n") 
        file.write("Exec=/opt/#{$appname}/#{$appname}\n") 
        file.write("Icon=/opt/#{$appname}/icon.ico\n") 
      }
      
      #app_libs = File.join($target_path, "opt", "#{$appname}", "app_libs")
      #if not File.directory?(app_libs)
      #  FileUtils.mkdir_p app_libs
      #end

      #depsarray = ldd_out.scan( /(\/[\/\.\w\-\+]*\.so[\.\d]*)/)
      #depsarray.uniq.each{ 
      #  |dep| appdep = dep[0]

      #  target_dep_name = appdep.scan(/\/(libQ[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libicu[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libav[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libwebp[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libre[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libva[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libopenmp[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libopenj[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libvpx[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libx26[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libswresampl[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libevent[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libbluray[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libchrom[\.\w\-\+]*\.so[\.\d]*)/)
      #  target_dep_name += appdep.scan(/\/(libmpg[\.\w\-\+]*\.so[\.\d]*)/)

      #  target_dep_name.uniq.each{ 
      #    |deplibname| 
      #    cp appdep, File.join(app_libs, deplibname[0])
      #    puts "Adding lib #{appdep}" 
      #  }
      #}
      
      control_template = File.read File.join( $startdir, "platform", "linux", "tasks", "debian_package", "control.erb")
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
  end
end

namespace "run" do
  task :linux => ["build:linux"] do
    Jake.run3(File.join($target_path, $appname))
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
    #rm_rf $targetdir
    rm_rf File.join($startdir, 'platform/shared/qt/rhodes/GeneratedFiles')
    rm_rf File.join($startdir, 'platform/linux/bin')
    rm_rf $target_path if File.exists? $target_path
    rm_rf File.join($app_path, "bin/tmp") if File.exists? File.join($app_path, "bin/tmp")
    rm_rf File.join($app_path, "bin/RhoBundle") if File.exists? File.join($app_path, "bin/RhoBundle")
  end
end


namespace "run" do
  task :linux do


  end
end