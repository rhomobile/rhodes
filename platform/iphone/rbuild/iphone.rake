namespace "config" do
  task :iphone => ["config:common"] do
    #TODO: Implement
  end
end

namespace "build" do
  namespace "iphone" do
    desc "Build iphone rhobundle"
    task :rhobundle => ["config:iphone"] do
      currentdir = pwd
      chdir 'platform/iphone/rbuild'
      puts `ant RhoBundle -Dapps.dir="#{$config["env"]["app"]}"`
      chdir currentdir
    end
  end
end