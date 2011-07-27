
namespace "config" do
        task :set_sym_platform do
                $current_platform = "symbian" unless $current_platform
        end

        task :symbian => [:set_sym_platform, "config:common"] do
                $rubypath = "res/build-tools/RhoRuby.exe"
                $zippath = "res/build-tools/7za.exe"
                $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
                $bindir = $app_path + "/bin"
                $rhobundledir =  $app_path + "/RhoBundle"
                $srcdir =  $bindir + "/RhoBundle"
                $targetdir = $bindir + "/target/sym"
                $tmpdir =  $bindir +"/tmp"
                $sdkprefix = $config["env"]["paths"]["qtsymbian-sdk"]+"/SDKs/Symbian1Qt473"
                $epocroot = "/QtSDK/Symbian/SDKs/Symbian1Qt473/"
                $qmake = $sdkprefix+"/bin/qmake.exe"
                $make = $sdkprefix+"/epoc32/tools/make.exe"
                $symbiandir = $config["env"]["paths"]["qtsymbian-sdk"]
                $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']
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

      rm_r $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/apps"
      rm_r $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/db"
      rm_r $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes/lib"
      cp_r $srcdir + "/apps", $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes"
      cp_r $srcdir + "/db", $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes"
      cp_r $srcdir + "/lib", $startdir + "/"+$config["build"]["symbianpath"]+"/rhodes"
      cp $app_path + "/icon/icon.svg", $config["build"]["symbianpath"]+"/rhodes/rhodes.svg"
    end

   task :rhodesdev => ["config:symbian"] do
     #implement app icon support
     ENV['EPOCROOT'] = $epocroot
     chdir $config["build"]["symbianpath"]
     ENV['PATH'] = $sdkprefix+"/epoc32/tools;"+$symbiandir+"/tools/perl/bin;C:/Windows/system32;"+$sdkprefix+"/epoc32/gcc/bin;"+$symbiandir+"/tools/gcce4/bin;"+$sdkprefix+"/bin;"+$sdkprefix+"/mkspecs/default;"+$symbiandir+"/tools/gcce4/arm-none-symbianelf/bin;"
     ENV['GCCPATH']= $symbiandir+"/tools/gcce4"
     ENV['DEFALT_MKSPEC_PATH']=$sdkprefix+"/mkspecs/default"
     ENV['QMAKE_PATH']=$sdkprefix+"/bin"


     args = ['rhodes.pro', '-r','-spec', 'symbian-abld', "\"CONFIG+=release\"", '-after', "\"OBJECTS_DIR=obj\"", "\"MOC_DIR=moc\"", "\"UI_DIR=ui\"", "\"RCC_DIR=rcc\"", '-after', "\"OBJECTS_DIR=obj\"",
             "\"MOC_DIR=moc\"", "\"UI_DIR=ui\"", "\"RCC_DIR=rcc\""]
     puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
     puts Jake.run($qmake,args)
     unless $? == 0
       puts "Error building"
       exit 1
     end

     args = ['release-gcce', '-w']
     puts Jake.run($make,args)
     unless $? == 0
       puts "Error building"
       exit 1
     end

     args = ['sis']
     puts Jake.run($make,args)
     unless $? == 0
     puts "Error building"
        exit 1
     end

     mkdir_p $targetdir if not File.exists? $targetdir
     rm $targetdir+"/"+$appname+".sis" if File.exists? $targetdir+"/"+$appname+".sis"
     mv $startdir + "/"+$config["build"]["symbianpath"] + "/rhodes/rhodes.sis", $targetdir+"/"+$appname+".sis"

     chdir $startdir
    end
  end
end

namespace "clean" do
    task :rhodes => ["config:symbian"] do
        ENV['EPOCROOT'] = $epocroot
        chdir $config["build"]["symbianpath"]
        ENV['PATH'] = $sdkprefix+"/epoc32/tools;"+$symbiandir+"/tools/perl/bin;C:/Windows/system32;"+$sdkprefix+"/epoc32/gcc/bin;"+$symbiandir+"/tools/gcce4/bin;"+$sdkprefix+"/bin;"+$sdkprefix+"/mkspecs/default;"+$symbiandir+"/tools/gcce4/arm-none-symbianelf/bin;"
        ENV['GCCPATH']= $symbiandir+"/tools/gcce4"
        ENV['DEFALT_MKSPEC_PATH']=$sdkprefix+"/mkspecs/default"
        ENV['QMAKE_PATH']=$sdkprefix+"/bin"

        args = ['distclean']
        puts Jake.run($make,args)
        unless $? == 0
        puts "Error cleaning"
            exit 1
        end

        rm_rf $targetdir
    end
    task :symbian => "clean:rhodes" do
    end
end

namespace "device" do
  namespace "symbian" do
    desc "Build production for device"
    task :production => ["config:symbian","build:symbian:rhobundle","build:symbian:rhodesdev"] do



    end
  end
end
