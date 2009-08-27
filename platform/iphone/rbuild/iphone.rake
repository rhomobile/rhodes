namespace "config" do
  task :iphone => ["config:common"] do
    $rubypath = "rhodes/rhodes-build/res/RubyMac" #path to RubyMac
    iphonepath = $config["build"]["iphonepath"]
    $bbver = $config["env"]["bbver"]
    $builddir = iphonepath + "/rbuild"
    $bindir = iphonepath + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $targetdir = iphonepath + "/target" 
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir =  $bindir +"/tmp"

  end
end

namespace "build" do
  namespace "iphone" do
    desc "Build iphone rhobundle"
    task :rhobundle => ["config:iphone"] do
      currentdir = pwd
      chdir 'platform/iphone'
      rm_rf 'bin'
      rm_rf 'build/Debug-*'
      rm_rf 'build/Release-*'
      
      chdir $startdir

      Rake::Task["build:bundle:noxruby"].execute

    end
  end
end