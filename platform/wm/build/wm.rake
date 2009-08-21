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
      unless $? == 0
        puts "Error building wm"
        exit 1
      end

      chdir currentdir
    end
  end
end