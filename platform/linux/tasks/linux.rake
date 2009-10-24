namespace "config" do
  task :linux => ["config:common"] do
    $config["platform"] = "linux"
    $rubypath = "platform/linux/target/compiler/rubylinux" #path to RubyMac
    $bindir = $app_path + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']
    $tmpdir =  $bindir +"/tmp"

    $homedir = `echo ~`.to_s.strip

  end
end

namespace "build" do
  namespace "linux" do
    task :rhobundle =>["config:linux"] do
      mkdir_p $srcdir unless File.exists? $srcdir

      Rake::Task["build:bundle:noxruby"].execute
      
    end
  end
end