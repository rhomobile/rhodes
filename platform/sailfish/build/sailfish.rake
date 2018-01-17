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

require File.dirname(__FILE__) + '/../../../lib/build/BuildConfig'

require 'pathname'
require 'tempfile'

include FileUtils

require 'erb'

class QtProjectGenerator
  attr_accessor :rhoRoot
  attr_accessor :nameApp
  attr_accessor :buildMode

  def render_profile(erbPath)
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end

class ScriptGenerator
  attr_accessor :merPort
  attr_accessor :merPkey
  attr_accessor :projectPath
  attr_accessor :merSdkTools
  attr_accessor :merSharedHome
  attr_accessor :merSharedSrc
  attr_accessor :merShTgtName
  attr_accessor :merTgtName
  attr_accessor :merUserName

  def render_script(erbPath)
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end

class BuildScriptGenerator < ScriptGenerator
  attr_accessor :qmakePath
  attr_accessor :proPath
  attr_accessor :buildMode
  attr_accessor :qmlMode
  attr_accessor :makePath
end

def isWindows?
  if /cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM
      return true
  else
      return false
  end
end

namespace "config" do
  task :set_sailfish_platform do
    $current_platform = "sailfish"
  end

  task :sailfish => :set_sailfish_platform do
    print_timestamp('config:sailfish START')
    Rake::Task["config:common"].invoke()
    Rake::Task["config:qt"].invoke()

    if !$app_config.nil? && !$app_config["sailfish"].nil?
      $connf_build = "Release"
      if !$app_config["sailfish"]["configuration"].nil?
        $connf_build = $app_config["sailfish"]["configuration"]
      end
      
      $target_arch = "arm"
      if !$app_config["sailfish"]["target"].nil?
        $target_arch = $app_config["sailfish"]["target"]
      end

      puts "Target arch: " + $target_arch
      puts "Target configuration: " + $connf_build
    else
      raise "Sailfish section is not found!"
    end

    if !$app_config["name"].nil?    
      $appname = $app_config["name"]
      $final_name_app = "harbour-" + "#{$appname}"
    else
      raise "Name application is not found in build.yaml!"
    end

    require 'nokogiri'
    current_targets = File.join($qtdir, "mersdk", "targets", "targets.xml")
    current_targets = current_targets.gsub("\\", "/")

    doc = File.open(current_targets) { |f| Nokogiri::XML(f) }
    $current_build_sdk_dir = ""
    doc.xpath("//target").each do |t|
      target = t.attr("name")
      if target.include? $target_arch
        $current_target = target
        $current_build_sdk_dir = File.join($qtdir, "settings", "SailfishOS-SDK", 
        "mer-sdk-tools", "Sailfish OS Build Engine", target)
        $current_build_sdk_dir = $current_build_sdk_dir.gsub("\\", "/")
        break
      end
    end

    if $current_build_sdk_dir == ""
      raise "Build arch sdk not found!"
    end

    puts "Current building sdk: " + $current_build_sdk_dir
    $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
    $rho_path =  File.join($app_path, "data", "rho")
    $srcdir = $rho_path
    $project_path = File.join($app_path, "project", "qt")
    $app_project_path = File.join($project_path, $final_name_app)

  end

end

namespace "device" do
  namespace "sailfish" do
    task :production => "config:sailfish" do
    end
  end
end

namespace "clean" do
  task :sailfish => "clean:sailfish:files"
  namespace "sailfish" do
    task :files => "config:sailfish" do
      clean_path = File.join($app_path, "project", "qt", "clean.cmd")
      system(clean_path)
      Dir.glob(File.join($project_path, "*.*")) { |f| rm f, :force => true }
      Dir.entries(File.join($project_path)).each do |d|
        rm_rf File.join($project_path, d)
      end
      rm_rf $srcdir if File.exists?($srcdir)
      #rm_rf $targetdir
      #rm_rf $app_builddir
      #rm_rf $tmpdir
    end
    task :build => "config:sailfish" do
      clean_path = File.join($app_path, "project", "qt", "clean.cmd")
      system(clean_path)
    end
  end
end

namespace "build" do
  namespace "sailfish" do
    task :rhobundle => ["config:sailfish"] do
      print_timestamp('build:sailfish:rhobundle START')
      
      $data_path = File.join($app_project_path, "data")
      unless File.directory?($data_path)
        mkdir $data_path
      end

      $rho_path =  File.join($data_path, "rho")
      $srcdir = $rho_path

      rm_rf File.join($rho_path)
      unless File.directory?($rho_path)
        mkdir $rho_path
      end
      Rake::Task["build:bundle:noxruby"].invoke
    end

    task :rhodes => ["project:sailfish:qt"] do
      print_timestamp('build:sailfish:rhodes START')
      build_path = File.join($app_path, "project", "qt", "build.cmd")
      system(build_path)
    end

  end
end

namespace 'project' do
  namespace 'sailfish' do
    task :qt => ['config:sailfish'] do
      rhodes_path = File.absolute_path '.'
      rhodes_path_build_engine = rhodes_path.gsub(File.expand_path('~'), "/home/mersdk/share")
      
      curl_erb_path = rhodes_path + "/platform/sailfish/build/curl.pro.erb"
      rubylib_erb_path = rhodes_path + "/platform/sailfish/build/rubylib.pro.erb"
      rholib_erb_path = rhodes_path + "/platform/sailfish/build/rholib.pro.erb"
      sqlite3_erb_path = rhodes_path + "/platform/sailfish/build/sqlite3.pro.erb"
      syncengine_erb_path = rhodes_path + "/platform/sailfish/build/syncengine.pro.erb"
      rhodes_erb_path = rhodes_path + "/platform/sailfish/build/rhodes.pro.erb"
      app_erb_path = rhodes_path + "/platform/sailfish/build/harbour-SailfishRhodes.pro.erb"
      coreapi_erb_path = rhodes_path + "/platform/sailfish/build/coreapi.pro.erb"
      build_erb_path = rhodes_path + "/platform/sailfish/build/rho_build.cmd.erb"
      clean_erb_path = rhodes_path + "/platform/sailfish/build/rho_clean.cmd.erb"

      mkdir_p $project_path
      mkdir_p File.join($project_path, "curl")
      mkdir_p File.join($project_path, "rubylib")
      mkdir_p File.join($project_path, "rholib")
      mkdir_p File.join($project_path, "sqlite3")
      mkdir_p File.join($project_path, "syncengine")
      mkdir_p File.join($project_path, "rhodes")
      mkdir_p File.join($project_path, $final_name_app)
      mkdir_p File.join($project_path, "coreapi")

      generator = QtProjectGenerator.new
      generator.rhoRoot = rhodes_path_build_engine
      generator.nameApp = $final_name_app
      generator.buildMode = $connf_build
      
      File.open(File.join($project_path, "curl", "curl.pro"), 'w' ) { |f| f.write generator.render_profile( curl_erb_path ) }
      File.open(File.join($project_path, "rubylib", "rubylib.pro"), 'w' ) { |f| f.write generator.render_profile( rubylib_erb_path ) }
      File.open(File.join($project_path, "rholib", "rholib.pro"), 'w' ) { |f| f.write generator.render_profile( rholib_erb_path ) }
      File.open(File.join($project_path, "sqlite3", "sqlite3.pro"), 'w' ) { |f| f.write generator.render_profile( sqlite3_erb_path ) }
      File.open(File.join($project_path, "syncengine", "syncengine.pro"), 'w' ) { |f| f.write generator.render_profile( syncengine_erb_path ) }
      File.open(File.join($project_path, "rhodes", "rhodes.pro"), 'w' ) { |f| f.write generator.render_profile( rhodes_erb_path ) }
      File.open(File.join($project_path, $final_name_app, "#{$final_name_app}.pro"), 'w' ) { |f| f.write generator.render_profile( app_erb_path ) }
      File.open(File.join($project_path, "coreapi", "coreapi.pro"), 'w' ) { |f| f.write generator.render_profile( coreapi_erb_path ) }

      build_script_generator = BuildScriptGenerator.new
      build_script_generator.merPort = 2222
      build_script_generator.merPkey = File.join $qtdir, "vmshare/ssh/private_keys/engine/mersdk"
      build_script_generator.projectPath = File.join($project_path, $final_name_app)
      build_script_generator.merSdkTools = $current_build_sdk_dir.gsub("/", "\\")
      build_script_generator.merSharedHome = File.expand_path('~')
      build_script_generator.merSharedSrc = File.expand_path('~')
      build_script_generator.merShTgtName = File.join($qtdir, "mersdk", "targets")
      build_script_generator.merTgtName = $current_target
      build_script_generator.merUserName = "mersdk"

      #TODO: windows paths way - temporary
      build_script_generator.qmakePath = File.join($current_build_sdk_dir, "qmake.cmd").gsub("/", "\\")
      build_script_generator.proPath = File.join($project_path, $final_name_app, "#{$final_name_app}.pro").gsub("/", "\\")
      build_script_generator.buildMode = $connf_build.downcase
      build_script_generator.qmlMode = "qml_#{$connf_build.downcase}" 
      build_script_generator.makePath = File.join($current_build_sdk_dir, "make.cmd").gsub("/", "\\")

      File.open(File.join($project_path, "build.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( build_erb_path ) }
      File.open(File.join($project_path, "clean.cmd"), 'w' ) { |f| f.write build_script_generator.render_script( clean_erb_path ) }
    end
  end
end

#MER_SSH_PORT, 2222
#MER_SSH_PRIVATE_KEY, C:/SailfishOS/vmshare/ssh/private_keys/engine/mersdk
#MER_SSH_PROJECT_PATH, C:/Users/n0men/Desktop/tau/testapp/project/qt/curl
#MER_SSH_SDK_TOOLS, C:\SailfishOS\settings\SailfishOS-SDK\mer-sdk-tools\Sailfish OS Build Engine\SailfishOS-2.1.3.7-armv7hl
#MER_SSH_SHARED_HOME, C:/Users/n0men
#MER_SSH_SHARED_SRC, C:/Users/n0men/
#MER_SSH_SHARED_TARGET, C:/SailfishOS/mersdk/targets
#MER_SSH_TARGET_NAME, SailfishOS-2.1.3.7-armv7hl
#MER_SSH_USERNAME, mersdk

