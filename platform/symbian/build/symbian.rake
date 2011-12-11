
namespace "config" do
        task :set_sym_platform do
                $current_platform = "symbian" unless $current_platform
        end

        task :symbian => [:set_sym_platform, "config:common"] do
                $rubypath = "res/build-tools/RhoRuby.exe"
                $zippath = "res/build-tools/7za.exe"
                $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
                $ver = $app_config["symbianver"].nil? ? "9.4" : $app_config["symbianver"]
                $symver = "Symbian1Qt473"
                if($ver == "9.5")
                    $symver = "Symbian3Qt473"
                end

                $bindir = $app_path + "/bin"
                $rhobundledir =  $app_path + "/RhoBundle"
                $srcdir =  $bindir + "/RhoBundle"
                $targetdir = $bindir + "/target/sym"
                $tmpdir =  $bindir +"/tmp"
                $jom = $config["env"]["paths"]["qtsymbian-sdk"]  + "/QtCreator/bin/jom.exe"
                $sdkprefix = $config["env"]["paths"]["qtsymbian-sdk"]+"/Symbian/SDKs/"+$symver
                $sdkprefix_emu = $config["env"]["paths"]["qtsymbian-sdk"]+"/Simulator/Qt/msvc2005"
                $epocroot = $config["env"]["paths"]["qtsymbian-sdk"]+"/Symbian/SDKs/"+$symver+"/"
                $epocroot = $epocroot[2,$epocroot.length()]
                $qmake = $sdkprefix+"/bin/qmake.exe"
                $make = $sdkprefix+"/epoc32/tools/make.exe"
                $qmake_emu = $sdkprefix_emu+"/bin/qmake.exe"
                $symbiandir = $config["env"]["paths"]["qtsymbian-sdk"]+"/Symbian"
                $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']
                $msvs = $config["env"]["paths"]["msvs2005"]

            if !$app_config["symbian"] || !$app_config["symbian"]["uid"]
                puts "Add symbian:uid to application build.yml"
                puts "You can find more information about UID here - http://docs.rhomobile.com/rhodes/build#build-for-symbian"
                exit 1
            end

            content = File.open($startdir + "/"+$config["build"]["symbianpath"] + "/rhodes/rhodes.pro").map { |line| line}
            content[14] = "symbian:TARGET = "+$appname+"\n"
            content[15] = "symbian:TARGET.UID3 = "+$app_config["symbian"]["uid"]+"\n"

            File.open($startdir + "/"+$config["build"]["symbianpath"] + "/rhodes/rhodes.pro", "w") {
            |f| f.write(content.join()) ;
            f.close
            }
        end
end

namespace "build" do
  namespace "symbian" do
    task :extensions => "config:symbian" do
      $app_config["extensions"].each do |ext|
        $app_config["extpaths"].each do |p|
          extpath = File.join(p, ext, 'ext')
          next unless File.exists? File.join(extpath, "build.bat")

          ENV['RHO_PLATFORM'] = 'symbian'
          ENV['PWD'] = $startdir
          ENV['RHO_ROOT'] = ENV['PWD']
          ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", "symbian", "bin", $sdk, "rhodes", "Release")
          ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform", "symbian", "bin", $sdk, "extensions", ext)
          ENV['VCBUILD'] = $vcbuild
          ENV['SDK'] = $sdk

          puts Jake.run("build.bat", [], extpath)
          break
        end
      end
    end

    desc "Build symbian rhobundle"
    task :rhobundle => ["config:symbian"] do
      Rake::Task["build:bundle:noxruby"].execute

      rm_r $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/apps" if File.exists? $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/apps"
      rm_r $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/db" if File.exists? $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/db"
      rm_r $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/lib" if File.exists? $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/lib"
      cp_r $srcdir + "/apps", $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes"
      cp_r $srcdir + "/db", $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes"
      cp_r $srcdir + "/lib", $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes"
      cp $app_path + "/icon/icon.svg", $config["build"]["symbianpath"]+"/rhodes/"+$appname+".svg" if File.exists? $app_path + "/icon/icon.svg"
    end

   task :rhodesdev => ["config:symbian"] do
     ENV['EPOCROOT'] = $epocroot
     chdir $config["build"]["symbianpath"]
     ENV['PATH'] = $sdkprefix+"/epoc32/tools;"+$symbiandir+"/tools/perl/bin;"+$symbiandir+"/tools/sbs/bin;"+$symbiandir+"/tools/sbs/win32/python27;C:/Windows/system32;"+$sdkprefix+"/epoc32/gcc/bin;"+$symbiandir+"/tools/gcce4/bin;"+$sdkprefix+"/bin;"+$sdkprefix+"/mkspecs/default;"+$symbiandir+"/tools/gcce4/arm-none-symbianelf/bin;"
     ENV['GCCPATH']= $symbiandir+"/tools/gcce4"
     ENV['DEFALT_MKSPEC_PATH']=$sdkprefix+"/mkspecs/default"
     ENV['QMAKE_PATH']=$sdkprefix+"/bin"
     #ENV['QT_SIS_OPTIONS']="-u -o"


     if($ver == "9.4")
     args = ['rhodes.pro', '-r','-spec', 'symbian-abld', "\"CONFIG+=release\"", '-after', "\"OBJECTS_DIR=obj\"", "\"MOC_DIR=moc\"", "\"UI_DIR=ui\"", "\"RCC_DIR=rcc\"", '-after', "\"OBJECTS_DIR=obj\"",
             "\"MOC_DIR=moc\"", "\"UI_DIR=ui\"", "\"RCC_DIR=rcc\""]
     else
     args = ['rhodes.pro', '-r','-spec', 'symbian-sbsv2', "\"CONFIG+=release symbian3\"", '-after', "\"OBJECTS_DIR=obj\"", "\"MOC_DIR=moc\"", "\"UI_DIR=ui\"", "\"RCC_DIR=rcc\"", '-after', "\"OBJECTS_DIR=obj\"",
             "\"MOC_DIR=moc\"", "\"UI_DIR=ui\"", "\"RCC_DIR=rcc\""]
     end
     puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
     puts Jake.run($qmake,args)
     unless $? == 0
       puts "Error building"
       exit 1
     end

     args = ['release-gcce', '-w']
     #args = ['release-winscw', '-w']
     puts Jake.run($make,args)
     unless $? == 0
       puts "Error building"
       exit 1
     end

     #chdir "rhodes"

     #args = ['installer_sis']
     args = ['sis']
     puts Jake.run($make,args)
     #unless $? == 0
     #puts "Error building"
     #   exit 1
     #end

     chdir "rhodes"
     args = ['unsigned_installer_sis']
     puts Jake.run($make,args)
     #unless $? == 0
     #puts "Error building"
     #   exit 1
     #end

     mkdir_p $targetdir if not File.exists? $targetdir
     rm $targetdir+"/"+$appname+"_unsigned.sis" if File.exists? $targetdir+"/"+$appname+"_unsigned.sis"
     cp $startdir + "/"+$config["build"]["symbianpath"] + "/rhodes/"+$appname+"_installer_unsigned.sis", $targetdir+"/"+$appname+"_installer_unsigned.sis"

     rm $targetdir+"/"+$appname+".sis" if File.exists? $targetdir+"/"+$appname+".sis"
     cp $startdir + "/"+$config["build"]["symbianpath"] + "/rhodes/"+$appname+".sis", $targetdir+"/"+$appname+".sis"

     chdir $startdir
   end

   task :rhodesemu => ["config:symbian"] do
     chdir $config["build"]["symbianpath"]
     ENV['DEFALT_MKSPEC_PATH']=$sdkprefix_emu+"/mkspecs/default"
     ENV['QTDIR']=$sdkprefix_emu+"/bin"
     ENV['QMAKESPEC']=$sdkprefix_emu+"/mkspecs/default"
     ENV['INCLUDE'] = $msvs+"/VC/include;"+$msvs+"/VC/PlatformSDK/Include;"+$msvs+"/VC/atlmfc/include"
     ENV['LIB'] = $msvs+"/VC/lib;"+$msvs+"/VC/PlatformSDK/Lib;"+$msvs+"/VC/atlmfc/lib"
     ENV['PATH'] = $sdkprefix_emu+"/bin;"+$msvs+"/VC/bin;C:/Windows/system32;"+$msvs+"/Common7/IDE"

     chdir "../"
     mkdir "rhodes-symbian-emulator-build" if not File.exists? "rhodes-symbian-emulator-build"
     chdir "rhodes-symbian-emulator-build"

     args = ['../symbian/rhodes_win32.pro', '-r','-spec', 'win32-msvc2005', "\"CONFIG+=release\""]
     puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
     puts Jake.run( $qmake_emu,args)
     unless $? == 0
       puts "Error building"
       exit 1
     end

     args = ['-nologo', '-j', '2', '-f', 'Makefile']
     puts Jake.run($jom,args)
     unless $? == 0
       puts "Error building"
       exit 1
     end

     rm_r "apps" if File.exists? "apps"
     rm_r "db" if File.exists? "db"
     rm_r "lib" if File.exists? "lib"

     #mv "rhodes/apps", pwd
     #mv "rhodes/db", pwd
     #mv "rhodes/lib", pwd

     cp_r $srcdir + "/apps", pwd
     cp_r $srcdir + "/db", pwd
     cp_r $srcdir + "/lib", pwd

     cp "rhodes/release/rhodes.exe", pwd if File.exists? "rhodes/release/rhodes.exe"

     puts Jake.run("rhodes.exe",[])
   end
  end
end

namespace "clean" do
    task :rhodes => ["config:symbian"] do
        ENV['EPOCROOT'] = $epocroot
        chdir $config["build"]["symbianpath"]
        ENV['PATH'] = $sdkprefix+"/epoc32/tools;"+$symbiandir+"/tools/perl/bin;"+$symbiandir+"/tools/sbs/bin;"+$symbiandir+"/tools/sbs/win32/python27;C:/Windows/system32;"+$sdkprefix+"/epoc32/gcc/bin;"+$symbiandir+"/tools/gcce4/bin;"+$sdkprefix+"/bin;"+$sdkprefix+"/mkspecs/default;"+$symbiandir+"/tools/gcce4/arm-none-symbianelf/bin;"
        ENV['GCCPATH']= $symbiandir+"/tools/gcce4"
        ENV['DEFALT_MKSPEC_PATH']=$sdkprefix+"/mkspecs/default"
        ENV['QMAKE_PATH']=$sdkprefix+"/bin"

        args = ['distclean']
        puts Jake.run($make,args)
        #unless $? == 0
        #puts "Error cleaning"
        #    exit 1
        #end

        rm_rf "../rhodes-symbian-emulator-build"
        rm_rf $targetdir
    end
    task :symbian => "clean:rhodes" do
    end
end

namespace "device" do
  namespace "symbian" do
    desc "Build production for device"
    task :production => ["build:symbian:rhobundle","build:symbian:rhodesdev"] do



    end

    task :run => ["config:symbian","production"] do
        ENV['EPOCROOT'] = $epocroot
        chdir $config["build"]["symbianpath"]
        ENV['PATH'] = $sdkprefix+"/epoc32/tools;"+$symbiandir+"/tools/perl/bin;C:/Windows/system32;"+$sdkprefix+"/epoc32/gcc/bin;"+$symbiandir+"/tools/gcce4/bin;"+$sdkprefix+"/bin;"+$sdkprefix+"/mkspecs/default;"+$symbiandir+"/tools/gcce4/arm-none-symbianelf/bin;"
        ENV['GCCPATH']= $symbiandir+"/tools/gcce4"
        ENV['DEFALT_MKSPEC_PATH']=$sdkprefix+"/mkspecs/default"
        ENV['QMAKE_PATH']=$sdkprefix+"/bin"

        chdir "rhodes"

        args = ['deploy']
        puts Jake.run($make,args)
        unless $? == 0
        puts "Error starting"
            exit 1
        end

        chdir "rhodes"

        args = ['runonphone']
        puts Jake.run($make,args)
        unless $? == 0
        puts "Error starting"
            exit 1
        end

    end
  end
end

namespace "run" do
    task :symbian => ["build:symbian:rhobundle","build:symbian:rhodesemu"] do



    end
    
  namespace "symbian" do    
    task :rhosimulator => ["config:set_sym_platform","config:common"] do
       $rhosim_config = "platform='symbian'\r\n"
       Rake::Task["run:rhosimulator"].invoke            
    end
    
    task :rhosimulator_debug => ["config:set_sym_platform","config:common"] do
       $rhosim_config = "platform='symbian'\r\n"
       Rake::Task["run:rhosimulator_debug"].invoke            
    end    
  end
    
end
