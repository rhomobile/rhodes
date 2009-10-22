namespace "config" do
  task :wm => ["config:common"] do
    $config["platform"] = "wm"
    $rubypath = "res/build-tools/RhoRuby.exe" #path to RubyMac
    wmpath = $config["build"]["wmpath"]
    $builddir = wmpath + "/build"
    $bindir = wmpath + "/bin"
    $srcdir =  wmpath + "/bin/RhoBundle"
    $targetdir = wmpath + "/target/wm6p"
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir =  $bindir +"/tmp"
    $vcbuild = "vcbuild"
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

      args = ['/M4', 'rhodes.sln', '"Release|Windows Mobile 6 Professional SDK (ARMV4I)"']
      puts Jake.run($vcbuild,args)
      unless $? == 0
        puts "Error building"
        exit 1
      end
      chdir $startdir
    end
  end
end

namespace "device" do
  namespace "wm" do
    desc "Build production for device or emulator"
    task :production => ["config:wm","build:wm:rhobundle","build:wm:rhodes"] do
      
      chdir $builddir
      
      args = ['build_inf.js', 'rhodes.inf', 'wm6']
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

      bindir = $startdir + "/" + $bindir
      targetdir = $startdir + "/" + $targetdir
      mkdir_p bindir if not File.exists? bindir
      mkdir_p targetdir if not File.exists? targetdir
      mv "rhodes.inf", bindir
      mv "rhodes.cab", targetdir

      rm_f "cleanup.js"

      chdir $startdir

    end
  end
end

namespace "clean" do
  desc "Clean wm"
  task :wm => "clean:wm:all"
  namespace "wm" do
    task :rhodes do
      rm_rf $bindir + "/Windows Mobile 6 Professional SDK (ARMV4I)"
    end
    task :all => "clean:wm:rhodes"
  end
end
