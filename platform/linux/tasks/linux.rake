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
          next unless (File.exists?( File.join(extpath, "build.bat") ) || project_path)

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