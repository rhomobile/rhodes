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
  task :linux => ["switch_app"] do

    $rubypath = "platform/linux/target/compiler/rubylinux" #path to RubyMac
    $bindir = $app_path + "/bin"
    $srcdir =  $bindir + "/RhoBundle"
    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/TestServe.rb']

    #$builddir = $config["build"]["linuxpath"] + "/build"
    #$vcbindir = $config["build"]["linuxpath"] + "/bin"
    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
    $rhobundledir =  $app_path + "/RhoBundle"
    $log_file = $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"]
    $buildcfg = $app_config["buildcfg"] unless $buildcfg
    $buildcfg = "Release" unless $buildcfg
    $detoolappflag = $js_application == true ? "js" : "ruby"
    $tmpdir = File.join($bindir, "tmp")

    $homedir = `echo ~`.to_s.strip   
    $current_platform = "linux"
    Rake::Task["config:common"].invoke()

    if !$app_config.nil? && !$app_config["linux"].nil?
      $connf_build = "Release"
      puts "Target configuration: " + $connf_build
    else
      raise "Linux section is not found!"
    end
    if !$app_config["name"].nil?    
      $appname = $app_config["name"]
      $final_name_app = "#{$appname.downcase}"
    else
      raise "Name application is not found in build.yaml!"
    end

    if !$app_config["version"].nil?
      $version_app = $app_config["version"]
    else
      $version_app = "1.0"
    end
    
    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
    $rho_path =  File.join($app_path, "data", "rho")
    $project_path = File.join($app_path, "project", "qt")
    $app_project_path = File.join($project_path, $final_name_app)

    $target_path = File.join($app_path, "bin", "target", "linux")
    mkdir_p $target_path
  end
end

def add_extension_to_pri_file(ext)
  puts "extention " + ext
end

namespace "build" do
  namespace "linux" do
    task :rhobundle => ["config:linux", "build:bundle:noxruby"] do     
      if !$skip_build_extensions
        Rake::Task["build:linux:extensions"].execute
      end
      Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
    end

    task :extensions => ['config:linux'] do
      puts "$app_extensions_list : #{$app_extensions_list}"

      ext_link = "unix:!macx: PRE_TARGETDEPS += $$PWD/../extensions"
      $app_extensions_list.each do |ext, ext_path |
        next unless (ext_path)
        print_timestamp('process extension "' + ext + '" START')

        extpath = File.join( ext_path, 'ext')
        ext_config_path = File.join( ext_path, "ext.yml")
        ext_config = nil
          
        if File.exist? ext_config_path
          ext_config = Jake.config(File.open(ext_config_path))
        end

        mkdir_p File.join($project_path, ext.downcase)

        add_extension_to_pri_file(ext)
        if ext.downcase == "coreapi"
          next
        end
        add_extension_to_pri_file(ext)
      end
      add_extension_to_pri_file("coreapi")
    end
  end
end

namespace "run" do
  task :linux do


  end
end