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


namespace "config" do
  task :linux => ["switch_app", "config:qt"] do

    $rubypath = "platform/linux/target/compiler/rubylinux" #path to RubyMac
    $bindir = $app_path + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

    #$builddir = $config["build"]["linuxpath"] + "/build"
    #$vcbindir = $config["build"]["linuxpath"] + "/bin"
    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
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

def generate_extensions_pri(extensions_lib, pre_targetdeps)
  ext_dir = File.join($startdir, 'platform/linux/bin/extensions')
  mkdir_p ext_dir if not File.exists? ext_dir
  File.open(File.join(ext_dir, 'extensions.pri'), "wb") do |fextensions|
    fextensions.write(%{SOURCES += ../../ruby/ext/rho/extensions.c
    LIBS += /LIBPATH:../../../linux/bin/extensions#{extensions_lib}
    PRE_TARGETDEPS += #{pre_targetdeps}
    })
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
      generate_extensions_pri(extensions_lib, pre_targetdeps)
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

    $target_path = File.join( $startdir, $vcbindir, $sdk, 'rhodes', $buildcfg)
    if not File.directory?($target_path)
      Dir.mkdir($target_path)
    end

    cp File.join($startdir, "platform/linux/bin/RhoSimulator/RhoSimulator"), 
    File.join($target_path, 'rhodes')

    chdir $startdir
  end

end

namespace "device" do
  namespace "linux" do
    task :production => ["build:linux:rhobundle"] do

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
    #rm_rf $targetdir
    rm_rf File.join($startdir, 'platform/shared/qt/rhodes/GeneratedFiles')
    rm_rf File.join($startdir, 'platform/linux/bin')
    rm_rf File.join($app_path, "bin/tmp") if File.exists? File.join($app_path, "bin/tmp")
    rm_rf File.join($app_path, "bin/RhoBundle") if File.exists? File.join($app_path, "bin/RhoBundle")
  end
end


namespace "run" do
  task :linux do


  end
end