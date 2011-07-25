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

      current = pwd
      chdir $srcdir
      puts `zip rhobundle.zip -r apps lib`
      chdir current
    end
  end
end

namespace "desktop" do
  task :win32, [:simpath] => "build:linux:rhobundle" do |t,args|
    chdir $srcdir
    rm_f File.join($srcdir,"rhodes-simulator.zip")
    mkdir "tmp"
    cp_r "#{args.simpath}/rho", "tmp/"
    cp_r "#{$srcdir}/apps", "tmp/rho/"
    cp_r "#{$srcdir}/lib", "tmp/rho/"
    cp_r "#{args.simpath}/rhodes.exe", "tmp/"
    chdir "tmp"
    puts `zip ../rhodes-simulator.zip -r *`
    chdir $srcdir
    rm_rf "tmp"
  end

  task :osx, [:simpath] => "build:linux:rhobundle" do |t,args|
    chdir $srcdir
    rm_f File.join($srcdir,"rhodes-simulator.zip")
    mkdir "tmp"
    cp_r "#{args.simpath}", "tmp/"
    rm_rf "tmp/Rhodes Launcher.app/Contents/Resources/rhorunner.app/apps"
    rm_rf "tmp/Rhodes Launcher.app/Contents/Resources/rhorunner.app/lib"

    cp_r "#{$srcdir}/apps", "tmp/Rhodes Launcher.app/Contents/Resources/rhorunner.app/"
    cp_r "#{$srcdir}/lib", "tmp/Rhodes Launcher.app/Contents/Resources/rhorunner.app/"

    rm "tmp/Rhodes Launcher.app/Contents/Resources/rhorunner.app/icon.png"
    cp "#{$app_path}/icon/icon.png", "tmp/Rhodes Launcher.app/Contents/Resources/rhorunner.app/"
     
    chdir "tmp"
    puts `zip ../rhodes-simulator.zip -r *`
    chdir $srcdir
    rm_rf "tmp"
  end

end
