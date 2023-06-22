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


$early_init = false

class QtProjectGenerator
  attr_accessor :rhoRoot
  attr_accessor :extRoot
  attr_accessor :nameApp
  attr_accessor :organization
  attr_accessor :desktopAppName
  attr_accessor :enableQWebEngine
  attr_accessor :versionApp
  attr_accessor :buildMode
  attr_accessor :enableDBUS
  attr_accessor :ymlBluetooth
  attr_accessor :ymlSpdscanner

  def render_profile(erbPath)
    tpl = File.read erbPath
    erb = ERB.new tpl
    erb.result binding
  end
end

class ScriptGenerator
  attr_accessor :isNixSystem
  attr_accessor :merAddress
  attr_accessor :merPort
  attr_accessor :merPkey
  attr_accessor :projectPath
  attr_accessor :merSdkTools
  attr_accessor :merSharedHome
  attr_accessor :merSharedSrc
  attr_accessor :merShTgtName
  attr_accessor :merTgtName
  attr_accessor :merUserName
  attr_accessor :merDevName

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
  attr_accessor :makeFile
  attr_accessor :rpmPath
  attr_accessor :sfdkExe
  attr_accessor :rpmvalPath
  attr_accessor :deployPath
  attr_accessor :nThreads


  def build()
    Dir.chdir(projectPath) do
      qmakeCommand = "\"#{qmakePath}\" #{proPath} -spec linux-g++"
      if system(qmakeCommand) != true
        raise "Error: qmake command failed: #{qmakeCommand}"
      end
      
      makeCommand = "\"#{makePath}\" -f #{makeFile} qmake_all"
      if system(makeCommand) != true
        raise "Error: make command failed: #{makeCommand}"
      end

      if system("\"#{makePath}\"") != true
        raise "Error: make command failed: \"#{makePath}\""
      end

    end
    #«C:\AuroraOS\settings\AuroraOS-SDK\libsfdk\build-target-tools\Aurora Build Engine\AuroraOS-4.0.2.209-base-i486.default\qmake.cmd» C:\Users\User\rhotemplate\ru.orgtemplate.rhotemplate.pro -spec linux-g++
    #«C:\AuroraOS\settings\AuroraOS-SDK\libsfdk\build-target-tools\Aurora Build Engine\AuroraOS-4.0.2.209-base-i486.default\make.cmd» -f C:/Users/User/build-ru.orgtemplate.rhotemplate-AuroraOS_4_0_2_209_base_i486_in_Aurora_Build_Engine-Release/Makefile qmake_all
    #«C:\AuroraOS\settings\AuroraOS-SDK\libsfdk\build-target-tools\Aurora Build Engine\AuroraOS-4.0.2.209-base-i486.default\make.cmd» 
  end

  def clear()
    #«C:\AuroraOS\bin\sfdk.exe» engine exec sdk-manage target refresh AuroraOS-4.0.2.209-base-i486
    #«C:\AuroraOS\bin\sfdk.exe» build-requires reset
    #«C:\AuroraOS\settings\AuroraOS-SDK\libsfdk\build-target-tools\Aurora Build Engine\AuroraOS-4.0.2.209-base-i486.default\make.cmd» clean
  end

  def rpm()
    #build()

    Dir.chdir(projectPath) do
      sfdkPackage = "\"#{sfdkExe}\" package"
      if system(sfdkPackage) != true
        raise "Error: make command failed: #{sfdkPackage}"
      end
    end

    #arm
    #«C:\AuroraOS\bin\sfdk.exe» package
    #«C:\AuroraOS\bin\sfdk.exe» tools exec AuroraOS-4.0.2.209-base-armv7hl.default.default rpm-validator --profile regular C:\Users\User\build-ru.orgtemplate.rhotemplate-AuroraOS_4_0_2_209_base_armv7hl_default_in_Aurora_Build_Engine-Release\RPMS\ru.orgtemplate.rhotemplate-0.1-1.armv7hl.rpm
    
    #i486
    #«C:\AuroraOS\bin\sfdk.exe» -c "device=Aurora OS эмулятор 4.0.2.209-base" package
    #«C:\AuroraOS\bin\sfdk.exe» -c "device=Aurora OS эмулятор 4.0.2.209-base" tools exec AuroraOS-4.0.2.209-base-i486.default rpm-validator --profile regular C:\Users\User\build-ru.orgtemplate.rhotemplate-AuroraOS_4_0_2_209_base_i486_in_Aurora_Build_Engine-Release\RPMS\ru.orgtemplate.rhotemplate-0.1-1.i486.rpm
  end

  def validateRpm(rpmPath)
    Dir.chdir(projectPath) do
      sfdkValidate = "\"#{sfdkExe}\" tools exec #{$current_target} rpm-validator --profile regular #{rpmPath}"
      if system(sfdkValidate) != true
        raise "Error: make command failed: #{sfdkValidate}"
      end
    end
  end

end

def isWindows?
  if /cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM
      return true
  else
      return false
  end
end

def PathToWindowsWay(path)
  if isWindows?
    return path.gsub("/", "\\")
  else
    return path
  end
  
end

def QuotedStrNixWay(str)
  return isWindows? ? str : '"' + str + '"' 
end

def clrfTOlf(path)
  text = File.open(path).read.gsub(/\r\n?/, "\n")
  File.open(path, "w") { |f| f.write(text) }
end

def getAuroraEnginePath()
  return File.join($sailfishdir, "settings", "AuroraOS-SDK", "libsfdk", "build-target-tools", "Aurora Build Engine")
end

def getTargetPath(target)
  sdkDir = File.join(getAuroraEnginePath(), target)
  return sdkDir.gsub("\\", "/")
end

namespace "config" do
  task :set_sailfish_platform do
    $current_platform = "sailfish"
    puts `#{"chcp 65001"}`
    
  end

  task :early_init do
    $early_init = true
    Rake::Task["config:sailfish"].execute()
  end

  task :sailfish => [:set_sailfish_platform, "switch_app"] do

    $homeDir = Dir.home.gsub(":", "").gsub("\\", "/")
    $homeDir[0] = $homeDir[0].downcase
    $homeDir = '/' + $homeDir

    print_timestamp('config:sailfish START')
    Rake::Task["config:common"].invoke()

    if $config["env"]["paths"]["sailfish"].nil?
      raise "Please set path to sailfish sdk in rhobuld.yaml"
    end
    $sailfishdir = $config["env"]["paths"]["sailfish"]


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

    if $early_init
      next
    end

    if !$app_config["name"].nil?    
      $appname = $app_config["name"]
      $final_name_app = "" + "#{$appname.downcase}"
    else
      raise "Name application is not found in build.yaml!"
    end

    if !$app_config["version"].nil?
      $version_app = $app_config["version"]
    else
      $version_app = "1.0"
    end

    if $app_config["sailfish"]["target_sdk"].nil?
    
      require 'nokogiri'
      current_targets = File.join($sailfishdir, "mersdk", "targets", "targets.xml")
      current_targets = current_targets.gsub("\\", "/")

      doc = File.open(current_targets) { |f| Nokogiri::XML(f) }
      $current_build_sdk_dir = ""
      doc.xpath("//target").each do |t|
        target = t.attr("name")
        puts target
        if target.include? $target_arch and Dir.exist?(getTargetPath(target))
          $current_target = target
          break
        end
      end
    else
      $current_target = $app_config["sailfish"]["target_sdk"]
    end

    if Dir.exist?(getAuroraEnginePath())
      $current_build_sdk_dir = getTargetPath($current_target)
    else
      puts "Can't recognize build SDK!"
      exit 1
    end
      
    $sysName = "Aurora"

    if !File.exists?($current_build_sdk_dir)
      puts "No such target (" + $current_target + ") exists"
      exit 1
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

    $target_path = File.join($app_path, "bin", "target", "sailfish")
    mkdir_p $target_path

    if !$app_config.nil? && !$app_config["sailfish"].nil?
      $host_name = ""
      $user_name = "nemo"

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["type"].nil?
        $dev_type = $app_config["sailfish"]["device"]["type"]
      else
        $dev_type = "vbox"
      end

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["user"].nil?
        $user_name = $app_config["sailfish"]["device"]["user"]
      end
      
      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["device_name"].nil?
        $dev_name = $app_config["sailfish"]["device"]["device_name"]
      elsif $dev_type == "real"
        raise "Please set device name for real device in device section!\n" +        
        "Add device if required (sailfish:device:add_device) and set 'device_name' field in build.yml.\n" +
        "For list device use: sailfish:device:list\n"
      elsif $dev_type == "vbox"
          $dev_name = "#{$sysName} Emulator"
      end

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["host"].nil?
        $host_name = $app_config["sailfish"]["device"]["host"]
      else
        host = get_value($dev_name, "ip")
        if host.empty?
          if $dev_type == "vbox"
            puts "Ip address not found! Set localhost as default."
            $host_name = "localhost"
          else
            puts "Not found ip address for real device!"
            $host_name = ""
          end
        else
          $host_name = host
        end
      end

    end

    if $config["env"]["paths"]["vbox"].nil? and isWindows?
      raise "Please set vbox env variable in rhobuild.yml..."
    end
    $virtualbox_path = "VBoxManage"
    if isWindows?
      $virtualbox_path = File.join($config["env"]["paths"]["vbox"], "VBoxManage.exe") 
    end

    $ssh_key = nil
    $pwd_host = nil

    if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["key"].nil?
      $ssh_key = $app_config["sailfish"]["device"]["key"]
    else
      $ssh_key = File.join($sailfishdir, "vmshare", get_value($dev_name, "sshkeypath"), "nemo")
      $ssh_key = $ssh_key.gsub("\\", "/")
      puts "path to ssh key: #{$ssh_key}"
    end

    if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["password"].nil?
      $pwd_host = $app_config["sailfish"]["device"]["password"]
    end

    if $pwd_host.nil? && $ssh_key.nil?
      raise "Key or password for running app not found, set it!"
    end

    if !$app_config["sailfish"]["build_threads"].nil?
      $build_threads = $app_config["sailfish"]["build_threads"]
    else
      $build_threads = 1
    end

#    $enableQWebEngine = ""
#    if !$app_config["sailfish"]["enable_web_engine"].nil?
#      if (($app_config["sailfish"]["enable_web_engine"] == true) || ($app_config["sailfish"]["enable_web_engine"] == "true"))
#        $enableQWebEngine = "ENABLE_Q_WEB_ENGINE"
#      end
#    end

    if !$app_config["sailfish"]["organization"].nil?
      $organization = $app_config["sailfish"]["organization"]
    end

    $enableDBUS = ""
    $ymlBluetooth = ""
    $ymlSpdscanner = ""
    if $app_config["extensions"].include?("bluetooth")
      $enableDBUS = "QT += dbus bluetooth"
      $ymlBluetooth = "- Qt5Bluetooth"
      $ymlSpdscanner = "- qt5-qtconnectivity-qtsdpscanner"
    end


    

    Rake::Task["build:sailfish:startvm"].invoke()
  end

end

def exec_ssh_command(session, cmd)
  f = File.open(File.join($app_path, "logcat.txt"), "w") do |file|
  puts "ssh command: #{cmd}"
    result = session.exec!(cmd) do |chanel, stream, line|
      p line
      file << line
    end
  end
end

def exec_ssh_with_sudo_command(session, cmd)
  puts cmd
  channel = session.open_channel do |channel, success|
    channel.on_data do |channel, data|
        if data.to_s.include?("Password")
          puts data.to_s
          puts "password sent: #{$pwd_host}"
          channel.send_data "#{$pwd_host}\n"
        end
    end
    channel.request_pty
    channel.exec("devel-su #{cmd}")
    channel.wait
  end
  channel.wait
end


namespace "run" do
  task :sailfish => ["config:sailfish"] do

      require 'net/ssh'
      require 'net/scp'

    session_ssh = nil
    puts $ssh_key
    puts "Connecting to device"
    if !$ssh_key.nil?    
      Net::SSH.start($host_name, $user_name, :host_key => "ssh-rsa", :keys => [ $ssh_key ]) do |session| 
        exec_ssh_command(session, "xdg-open /usr/share/applications/#{$final_name_app}.desktop")  
      end  
    else   
      if $pwd_host.nil?
        raise "Key or password not found set it in device section! or add device"
      end 
      Net::SSH.start($host_name, $user_name, :password => $pwd_host) do |session|       
        exec_ssh_command(session, "xdg-open /usr/share/applications/#{$final_name_app}.desktop")    
      end
    end

  end
end

def get_value(device, key)
  device_xml = File.join $sailfishdir, "vmshare", "devices.xml"
  device_xml = device_xml.gsub("\\", "/")

  require 'nokogiri'
  doc = File.open(device_xml) { |f| Nokogiri::XML(f) }
  doc.xpath("//devices/device").each do |t|
    if t.attr("name") == device
      value = t.xpath("./#{key}").text
      return value.to_s 
    end
  end
  return ""
end

namespace "device" do
  namespace "sailfish" do
    task :production => "build:sailfish:rhobundle" do
      Rake::Task["build:sailfish:rhodes"].invoke()
    end

    task :install => ["config:sailfish"] do
      Rake::Task["build:sailfish:startvm"].invoke()
      Rake::Task["build:sailfish:deploy"].invoke()
    end

    task :add_device  => ["config:early_init"] do

        require 'net/ssh'
        require 'net/scp'

      print_timestamp('device:sailfish:add_device starting')
      device_xml = File.join $sailfishdir, "vmshare", "devices.xml"
      device_xml = device_xml.gsub("\\", "/")

      ssh_dir = File.join $sailfishdir, "vmshare", "ssh", "private_keys"
      ssh_dir = ssh_dir.gsub("\\", "/")

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["device_name"].nil?
        $dev_name = $app_config["sailfish"]["device"]["device_name"]
      else
        puts "Enter device name: "      
        $dev_name = STDIN.gets
        $dev_name = $dev_name.chomp("\n")
      end

      if !$app_config["sailfish"]["device"].nil? && !$app_config["sailfish"]["device"]["host"].nil?
        $host_name = $app_config["sailfish"]["device"]["host"]
        puts "host: #{$host_name}"
      else
        puts "Enter network address: "      
        $host_name = STDIN.gets
        $host_name = $host_name.chomp("\n")
      end

      require 'nokogiri'
      puts device_xml
      doc = File.open(device_xml) { |f| Nokogiri::XML(f) }
      doc.xpath("//devices/device").each do |t|
        ip = t.xpath("./ip").text
        name = t.attr("name")
        if name == $dev_name
          raise "Device with this name already registered!"
        end
      end

      puts "Enter password from device, to generate the ssh key: "
      $pwd_host = STDIN.noecho(&:gets).chomp
      $user_name = "defaultuser"

      mkdir_p File.join(ssh_dir, $dev_name)

      yes = true
      if(File.exists? File.join(ssh_dir, $dev_name, "defaultuser"))
        puts "Key for this device exists, do you want to replace it? (yes/no)"      
        yes = STDIN.gets == "yes\n"
      end

      if(yes)
        puts "Starting session"
        Net::SSH.start($host_name, $user_name, :password => $pwd_host) do |session|    
          exec_ssh_command(session, "[ -e ~/nemo ] && rm ~/nemo")
          exec_ssh_command(session, "[ -e ~/nemo.pub ] && rm ~/nemo.pub")       
          exec_ssh_command(session, "ssh-keygen -f ~/nemo -t rsa -b 2048 -N \"\"")
          exec_ssh_command(session, "cat ~/nemo.pub >> ~/.ssh/authorized_keys")
          session.scp.download!("/home/#{$user_name}/nemo", File.join(ssh_dir, $dev_name, "nemo"))
          session.scp.download!("/home/#{$user_name}/nemo.pub", File.join(ssh_dir, $dev_name, "nemo.pub"))
          exec_ssh_command(session, "rm ~/nemo")
          exec_ssh_command(session, "rm ~/nemo.pub")      
        end
      end

      devices_node = doc.xpath("//devices/device")
      new_node = Nokogiri::XML::Builder.new { |xml|
        xml.device("name" => $dev_name, "type" => "real") do
          xml.ip { xml.text($host_name) }
          xml.sshkeypath { xml.text("ssh/private_keys/#{$dev_name}") }
        end
      }
      devices_node.last.parent.add_child(new_node.doc.root)

      File.open(device_xml, 'w') { |file| file.write(doc.to_s) }
    end

    task :list_device  => ["config:early_init"] do
      print_timestamp('device:sailfish:list_device starting')
      device_xml = File.join $sailfishdir, "vmshare", "devices.xml"
      device_dir = File.join $sailfishdir, "vmshare"
      device_xml = device_xml.gsub("\\", "/")
      device_dir = device_dir.gsub("\\", "/")

      require 'nokogiri'
      puts device_xml
      puts "==============="
      doc = File.open(device_xml) { |f| Nokogiri::XML(f) }
      doc.xpath("//devices/device").each do |t|
        ip = t.xpath("./ip").text
        name = t.attr("name")
        sshkeypath = t.xpath("./sshkeypath").text

        puts "ip: #{ip}"
        puts "name: #{name}"
        puts "sshkeypath: #{File.join device_dir, sshkeypath}"
        puts "==============="
      end
    end

  end
end

namespace "clean" do
  task :sailfish => "clean:sailfish:files"
  namespace "sailfish" do
    task :files => "config:sailfish" do
      clean_path = File.join($app_path, "project", "qt", "clean.cmd")
      system(clean_path)
      if File.exists?($project_path)
        Dir.glob(File.join($project_path, "*.*")) { |f| rm f, :force => true }
        Dir.entries(File.join($project_path)).each do |d|
          rm_rf File.join($project_path, d)
        end
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

def add_extension_to_main_project(ext)
   main_project = File.join($project_path, $final_name_app, "#{$final_name_app}.pro")

   f = File.open(main_project, "r")
   text = ''
   f.each_line do |line|
     if line.include?('SUBDIRS') and !line.include?("$$PWD/../#{ext.downcase}")
      #before continue /r/n/ symbols and \\
      puts line
      line.insert(line.index('\\'), " $$PWD/../#{ext.downcase}")
      #line = line + "$$PWD/../#{ext.downcase}/r/n"
     end
     text = text + line
   end
   f.close

   f = File.open(main_project, "w") {|file| file << text }
end

def add_extension_to_rhodes_project(ext)

  rhodes_project = File.join($project_path, "rhodes", "rhodes.pro")

  f = File.open(rhodes_project, "r")
  text = ''
  ext_exists = false

  f.each_line do |line|
    if line.include?("$$PWD/../extensions -l#{ext.downcase}")
      ext_exists = true
    end
    text = text + line
  end
  f.close

  if !ext_exists
    text = text + "unix:!macx: LIBS += -L$$PWD/../extensions -l#{ext.downcase} \n"
    #text = text + "unix:!macx: PRE_TARGETDEPS += $$PWD/../extensions/lib#{ext.downcase}.a \n"
  end

  f = File.open(rhodes_project, "w") {|file| file << text }
  return 
end

def vm_is_started?
  require 'open3'

  output = ""
  cmd = "\"" + $virtualbox_path + "\"" + " list runningvms"
  Open3.popen3(cmd) do |stdin, stdout, stderr, wait_thr|
    if wait_thr.value != 0 
        raise "Shell return error: #{wait_thr.value.to_s}\n STDOUT: #{stdout.read}" 
    end
    output = stdout.read
  end
  return output.include?("#{$sysName} Build Engine")
end

def deploy_bundle(session)
  path_to_bundle = "/usr/share/#{$final_name_app}/data"
  puts "Clear bundle directory"
  exec_ssh_command(session, "rm -rf /home/nemo/.local/share/Rhomobile/#{$final_name_app}/rho")
  exec_ssh_with_sudo_command(session, "rm -rf /usr/share/#{$final_name_app}/data/rho")
  puts "Push bundle on device at #{path_to_bundle}"
  session.scp.upload!(File.join($project_path, $final_name_app, 'data', 'rho'), 
     "/home/#{$user_name}/RPMS", :recursive => [true])
  exec_ssh_with_sudo_command(session, "mv /home/#{$user_name}/RPMS/rho #{path_to_bundle}")
  exec_ssh_with_sudo_command(session, "chown -R root:root #{path_to_bundle}")
end

def force_install(session)
  exec_ssh_command(session, "mkdir /home/#{$user_name}/RPMS/#{$final_name_app}")
  puts "scp From : " + File.join($project_path, $final_name_app, "RPMS")
  puts "To : " + "/home/#{$user_name}/RPMS/#{$final_name_app}"
  session.scp.upload!(File.join($project_path, $final_name_app, 'RPMS'),
     "/home/#{$user_name}/RPMS/#{$final_name_app}", :recursive => [true])

  Dir[File.join($project_path, $final_name_app, "RPMS/**/*")].each do |file_name|
    puts File.basename(file_name, ".rpm")
    exec_ssh_with_sudo_command(session, "rpm -e #{File.basename(file_name, ".rpm")} --nodeps")
  end
  exec_ssh_with_sudo_command(session, "rpm -ivh /home/#{$user_name}/RPMS/#{$final_name_app}/RPMS/*.rpm --nodeps")
  exec_ssh_command(session, "rm -rf /home/#{$user_name}/RPMS/#{$final_name_app}")
end

namespace "build"  do
  namespace "sailfish" do

    task :startvm => "config:sailfish" do
      if $virtualbox_path.empty? 
        raise "Please, set VirtualBox variable environment..."
      end

      if !vm_is_started?
        system("\"" + $virtualbox_path + "\"" + " startvm \"#{$sysName} Build Engine\" --type headless") 
        puts "Waiting 40 seconds vm..."
        sleep 40.0
      end 
    end

    task :stopvm => "config:sailfish" do
      if $virtualbox_path.empty? 
        raise "Please, set VirtualBox variable environment..."
      end
      system("\"" + $virtualbox_path + "\"" + " controlvm \"#{$sysName} Build Engine\" poweroff")
    end
    
    task :rhobundle => ["project:sailfish:qt"] do
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

      if !$skip_build_extensions
        Rake::Task["build:sailfish:extensions"].execute
      end
      Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
    end

    task :deploy => ['config:sailfish'] do
      deploy_path = File.join($app_path, "project", "qt", "deploy.cmd")
      system(deploy_path)
    end

    task :deploy_bundle => ['config:sailfish'] do

        require 'net/ssh'
        require 'net/scp'

      $skip_build_extensions = true
      Rake::Task["build:sailfish:rhobundle"].execute
      puts "Deploy bundle started"
      session_ssh = nil
      puts "Connecting to device"
      if !$ssh_key.nil?    
        Net::SSH.start($host_name, $user_name, :host_key => "ssh-rsa", :keys => [ $ssh_key ]) do |session| 
          deploy_bundle(session)
        end  
      else    
        if $pwd_host.nil?
          raise "Key or password not found set it in device section! or add device"
        end
        Net::SSH.start($host_name, $user_name, :password => $pwd_host) do |session|       
          deploy_bundle(session)
        end
      end
    end

    task :force_install => ['config:sailfish'] do
        require 'net/ssh'
        require 'net/scp'
      $skip_build_extensions = true
      session_ssh = nil
      puts "Connecting to device"
      if !$ssh_key.nil?    
        Net::SSH.start($host_name, $user_name, :host_key => "ssh-rsa", :keys => [ $ssh_key ]) do |session| 
          force_install(session)
        end  
      else    
        if $pwd_host.nil?
          raise "Key or password not found set it in device section! or add device"
        end
        Net::SSH.start($host_name, $user_name, :password => $pwd_host) do |session|       
          force_install(session)
        end
      end
    end

    

    task :rhodes => ["project:sailfish:qt"] do
      print_timestamp('build:sailfish:rhodes START')

      $buildEnv.build()
      $buildEnv.rpm()
      exit 0

      $target_rpm = ""
      Dir[File.join($project_path, $final_name_app, "RPMS/**/*")].each do |file_name|
        if file_name.include?("#{$final_name_app}-#{$version_app}")
          $target_rpm = file_name
          puts "target rpm: " + $target_rpm
        end
      end

      $buildEnv.validateRpm($target_rpm)

      cp_r $target_rpm, $target_path
    end

    task :extensions => ['project:sailfish:qt'] do
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

        erb_project_path = ext_config[$current_platform]["proerb"] if ( ext_config && ext_config[$current_platform] && ext_config[$current_platform]["proerb"])
        next unless (erb_project_path)

        erb_project_path = File.join ext_path, erb_project_path
        puts "Path to erb project: " + erb_project_path

        generator = QtProjectGenerator.new
        generator.rhoRoot = $rhodes_path_build_engine   
        generator.extRoot = ext_path.gsub(File.expand_path('~'), $homeDir)   
        generator.desktopAppName = $appname
        generator.nameApp = $final_name_app 
        generator.buildMode = $connf_build
        generator.enableQWebEngine = $enableQWebEngine
        generator.enableDBUS = $enableDBUS
        generator.ymlBluetooth = $ymlBluetooth
        generator.ymlSpdscanner = $ymlSpdscanner

        mkdir_p File.join($project_path, ext.downcase)

        File.open(File.join($project_path, ext.downcase, "#{ext.downcase}.pro"), 'w' ) { |f| f.write generator.render_profile( erb_project_path ) }
        add_extension_to_main_project(ext)
        if ext.downcase == "coreapi"
          next
        end
        add_extension_to_rhodes_project(ext)
      end
      add_extension_to_rhodes_project("coreapi")
    end

  end
end

namespace 'project' do
  namespace 'sailfish' do
    task :qt => ['config:sailfish'] do
      $rhodes_path = File.absolute_path '.'
      $rhodes_path_build_engine = $rhodes_path.gsub(File.expand_path('~'), $homeDir)
      
      curl_erb_path = $rhodes_path + "/platform/sailfish/build/curl.pro.erb"
      rubylib_erb_path = $rhodes_path + "/platform/sailfish/build/rubylib.pro.erb"
      rholib_erb_path = $rhodes_path + "/platform/sailfish/build/rholib.pro.erb"
      unzip_erb_path = $rhodes_path + "/platform/sailfish/build/unzip.pro.erb"
      zlib_erb_path = $rhodes_path + "/platform/sailfish/build/zlib.pro.erb"
      sqlite3_erb_path = $rhodes_path + "/platform/sailfish/build/sqlite3.pro.erb"
      syncengine_erb_path = $rhodes_path + "/platform/sailfish/build/syncengine.pro.erb"
      rhodes_erb_path = $rhodes_path + "/platform/sailfish/build/rhodes.pro.erb"
      app_erb_path = $rhodes_path + "/platform/sailfish/build/SailfishRhodes.pro.erb"

      desktop_erb_path = $rhodes_path + "/platform/sailfish/build/rpm/SailfishRhodes.desktop.erb"
      spec_erb_path = $rhodes_path    + "/platform/sailfish/build/rpm/SailfishRhodes.spec.erb"
      #priv_erb_path = $rhodes_path    + "/platform/sailfish/build/rpm/SailfishRhodes.erb"

      build_erb_path = $rhodes_path + "/platform/sailfish/build/rho_build.cmd.erb"
      clean_erb_path = $rhodes_path + "/platform/sailfish/build/rho_clean.cmd.erb"
      rpm_erb_path = $rhodes_path + "/platform/sailfish/build/rho_rpm.cmd.erb"
      rpmval_erb_path = $rhodes_path + "/platform/sailfish/build/rho_rpmvalidation.cmd.erb"
      deploy_erb_path = $rhodes_path + "/platform/sailfish/build/rho_deploy.cmd.erb"

      mkdir_p $project_path
      mkdir_p File.join($project_path, "curl")
      mkdir_p File.join($project_path, "rubylib")
      mkdir_p File.join($project_path, "rholib")
      mkdir_p File.join($project_path, "unzip")
      mkdir_p File.join($project_path, "zlib")
      mkdir_p File.join($project_path, "sqlite3")
      mkdir_p File.join($project_path, "syncengine")
      mkdir_p File.join($project_path, "rhodes")
      mkdir_p File.join($project_path, $final_name_app)
      mkdir_p File.join($project_path, $final_name_app, "rpm")
      #mkdir_p File.join($project_path, $final_name_app, "privileges")

      #if !File.exists?(File.join($project_path, $final_name_app, "qml")) 
        cp_r File.join($rhodes_path, "platform/shared/qt/sailfish/qml"), File.join($project_path, $final_name_app)
        File.rename(
          File.join($project_path, $final_name_app, "qml", "sailfishrhodes.qml"), 
          File.join($project_path, $final_name_app, "qml", "#{$final_name_app}.qml"))
      #end

      if !File.exists?(File.join($project_path, $final_name_app, "icons")) 
        cp_r File.join($rhodes_path, "platform/shared/qt/sailfish/icons"), File.join($project_path, $final_name_app)
        Dir[File.join($project_path, $final_name_app, "icons/**/*")].each do |file_name|
          File.rename(file_name, File.join(File.dirname(file_name), "#{$organization}.#{$final_name_app}.png")) if File.file? file_name
        end
      end

      generator = QtProjectGenerator.new
      generator.rhoRoot = $rhodes_path_build_engine
      generator.desktopAppName = $appname
      generator.nameApp = $final_name_app
      generator.organization = $organization
      generator.enableQWebEngine = $enableQWebEngine
      generator.versionApp = $version_app     
      generator.buildMode = $connf_build
      generator.enableDBUS = $enableDBUS
      generator.ymlBluetooth = $ymlBluetooth
      generator.ymlSpdscanner = $ymlSpdscanner

      File.open(File.join($project_path, "curl", "curl.pro"), 'w' ) { |f| f.write generator.render_profile( curl_erb_path ) }
      File.open(File.join($project_path, "rubylib", "rubylib.pro"), 'w' ) { |f| f.write generator.render_profile( rubylib_erb_path ) }
      File.open(File.join($project_path, "rholib", "rholib.pro"), 'w' ) { |f| f.write generator.render_profile( rholib_erb_path ) }
      File.open(File.join($project_path, "unzip", "unzip.pro"), 'w' ) { |f| f.write generator.render_profile( unzip_erb_path ) }
      File.open(File.join($project_path, "zlib", "zlib.pro"), 'w' ) { |f| f.write generator.render_profile( zlib_erb_path ) }
      File.open(File.join($project_path, "sqlite3", "sqlite3.pro"), 'w' ) { |f| f.write generator.render_profile( sqlite3_erb_path ) }
      File.open(File.join($project_path, "syncengine", "syncengine.pro"), 'w' ) { |f| f.write generator.render_profile( syncengine_erb_path ) }

      if !isWindows?
        clrfTOlf(File.join($project_path, "curl", "curl.pro"))
        clrfTOlf(File.join($project_path, "rubylib", "rubylib.pro"))
        clrfTOlf(File.join($project_path, "rholib", "rholib.pro"))
        clrfTOlf(File.join($project_path, "unzip", "unzip.pro"))
        clrfTOlf(File.join($project_path, "zlib", "zlib.pro"))
        clrfTOlf(File.join($project_path, "sqlite3", "sqlite3.pro"))
        clrfTOlf(File.join($project_path, "syncengine", "syncengine.pro"))
      end


      File.open(File.join($project_path, $final_name_app, "#{$organization}.#{$final_name_app}.desktop"), 'w' ) { |f| f.write generator.render_profile( desktop_erb_path ) }
      File.open(File.join($project_path, $final_name_app, "rpm", "#{$final_name_app}.spec"), 'w' ) { |f| f.write generator.render_profile( spec_erb_path ) }
      #File.open(File.join($project_path, $final_name_app, "privileges", "#{$final_name_app}"), 'w' ) { |f| f.write generator.render_profile( priv_erb_path ) }
      
      rho_name_project = File.join($project_path, "rhodes", "rhodes.pro")
      if !File.exists?(rho_name_project)
        File.open(rho_name_project, 'w' ) { |f| f.write generator.render_profile( rhodes_erb_path ) }
      end
      
      app_name_project = File.join($project_path, $final_name_app, "#{$final_name_app}.pro")
      if !File.exists?(app_name_project)
        File.open(app_name_project, 'w' ) { |f| f.write generator.render_profile( app_erb_path ) }
      end
      
      $buildEnv = BuildScriptGenerator.new
      
      $buildEnv.isNixSystem = !isWindows?
      $buildEnv.merAddress = "127.0.0.1"
      $buildEnv.merPort = 2222
      $buildEnv.merPkey = File.join $sailfishdir, "vmshare/ssh/private_keys/engine/mersdk"
      $buildEnv.projectPath = QuotedStrNixWay(File.join($project_path, $final_name_app))
      $buildEnv.merSdkTools = QuotedStrNixWay(PathToWindowsWay($current_build_sdk_dir))
      $buildEnv.merSharedHome = QuotedStrNixWay(File.expand_path('~'))
      $buildEnv.merSharedSrc = QuotedStrNixWay(File.expand_path('~'))
      $buildEnv.merShTgtName = QuotedStrNixWay(File.join($sailfishdir, "mersdk", "targets"))
      $buildEnv.merTgtName = QuotedStrNixWay($current_target)
      $buildEnv.merUserName = "mersdk"
      $buildEnv.merDevName = QuotedStrNixWay($dev_name)

      cmd_suffix = isWindows? ? '.cmd' : ''
      $buildEnv.qmakePath = PathToWindowsWay(File.join($current_build_sdk_dir, "qmake" + cmd_suffix))
      $buildEnv.proPath = PathToWindowsWay(File.join($project_path, $final_name_app, "#{$final_name_app}.pro"))
      $buildEnv.makeFile = PathToWindowsWay(File.join($project_path, $final_name_app, "Makefile"))
      $buildEnv.buildMode = $connf_build.downcase
      $buildEnv.qmlMode = "qml_#{$connf_build.downcase}" 
      $buildEnv.makePath = PathToWindowsWay(File.join($current_build_sdk_dir, "make" + cmd_suffix))
      $buildEnv.rpmPath =  PathToWindowsWay(File.join($current_build_sdk_dir, "rpm" + cmd_suffix))
      $buildEnv.rpmvalPath =  PathToWindowsWay(File.join($current_build_sdk_dir, "rpmvalidation" + cmd_suffix))
      $buildEnv.deployPath =  PathToWindowsWay(File.join($current_build_sdk_dir, "deploy" + cmd_suffix))
      $buildEnv.nThreads = $build_threads
      $buildEnv.sfdkExe = QuotedStrNixWay(File.join($sailfishdir, "bin", "sfdk.exe"))

    end
  end
end


