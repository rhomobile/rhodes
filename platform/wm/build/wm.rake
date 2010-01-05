namespace "config" do
  task :wm => ["config:common"] do
    $config["platform"] = "wm"
    $rubypath = "res/build-tools/RhoRuby.exe" #path to RubyMac
    $builddir = $config["build"]["wmpath"] + "/build"
    $vcbindir = $config["build"]["wmpath"] + "/bin"
    
    $bindir = $app_path + "/bin"
    $rhobundledir =  $app_path + "/RhoBundle"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = $bindir + "/target/wm6p"
    $tmpdir =  $bindir +"/tmp"
    $vcbuild = "vcbuild"
    $sdk = "Windows Mobile 6 Professional SDK (ARMV4I)"
    $sdk = $app_config["wmsdk"] unless $app_config["wmsdk"].nil?

    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']
  end
end

namespace "build" do
  namespace "wm" do
#    desc "Build wm rhobundle"
    task :rhobundle => ["config:wm"] do
      Rake::Task["build:bundle:noxruby"].execute
    end

    task :rhodes => ["config:wm", "build:wm:rhobundle"] do
      chdir $config["build"]["wmpath"]

      args = ['/M4', 'rhodes.sln', "\"Release|#{$sdk}\""]
      puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
      puts Jake.run($vcbuild,args)
      unless $? == 0
        puts "Error building"
        exit 1
      end
      chdir $startdir
    end

    task :devrhobundle => "win32:devrhobundle"
  end
  
  namespace "win32" do
    task :devrhobundle => ["wm:rhobundle"] do
        win32rhopath = 'platform/wm/bin/win32/rhodes/Debug/rho/'
        mkdir_p win32rhopath
        rm_rf win32rhopath + 'lib'      
        rm_rf win32rhopath + 'apps'
        
        cp_r $srcdir + '/lib', win32rhopath
        cp_r $srcdir + '/apps', win32rhopath      
        cp_r $srcdir + '/db', win32rhopath      
    end
  end
end

namespace "device" do
  namespace "wm" do
    desc "Build production for device or emulator"
    task :production => ["config:wm","build:wm:rhobundle","build:wm:rhodes"] do
      
      chdir $builddir
      
      cp $app_path + "/icon/icon.ico", "../rhodes/resources" if File.exists? $app_path + "/icon/icon.ico"

      args = ['build_inf.js', 'rhodes.inf', 'wm6', '"' + $app_config["name"] +'"', $app_config["vendor"], '"' + $srcdir + '"']
      puts Jake.run('cscript',args)
      unless $? == 0
        puts "Error running build_inf"
        exit 1
      end        
      
      args = ['rhodes.inf']
      puts Jake.run($config["env"]["paths"]["cabwiz"] + "/cabwiz.exe",args)
      unless $? == 0
        puts "Error running cabwiz"
        exit 1
      end        
      
      args = ['cleanup.js']
      puts Jake.run('cscript',args)
      unless $? == 0
        puts "Error running cleanup.js"
        exit 1
      end    

      mkdir_p $bindir if not File.exists? $bindir
      mkdir_p $targetdir if not File.exists? $targetdir
      mv "rhodes.inf", $targetdir
      mv "rhodes.cab", $targetdir

      rm_f "cleanup.js"

      chdir $startdir

    end
  end
end

namespace "clean" do
  desc "Clean wm"
  task :wm => "clean:wm:all"
  namespace "wm" do
    task :rhodes => ["config:wm"] do
      rm_rf $vcbindir + "/#{$sdk}"

      #rm_rf $tmpdir
      #rm_rf $targetdir +"/../"
      rm_rf $bindir
      rm_rf $rhobundledir
      
    end
    task :all => "clean:wm:rhodes"
  end
end
