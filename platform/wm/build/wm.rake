namespace "config" do
  task :wm => ["config:common"] do
    #TODO: Implement
  end
end

namespace "build" do
  namespace "wm" do
    desc "Build wm rhobundle"
    task :rhobundle => ["config:wm"] do
      currentdir = pwd
      chdir 'platform/wm/build'
      puts `ant.bat RhoBundle -Dapps.dir="#{$config["env"]["app"]}"`
      chdir currentdir
    end
  end
end