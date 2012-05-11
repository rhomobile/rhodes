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

def kill_detool
  begin 
    `taskkill /F /IM detool.exe`
  rescue Exception => e
  end  
end

def sign (cabfile)
  puts "Singing .cab file"

  cabsigntool = $cabwiz[0, $cabwiz.index("CabWiz")] + "Security\\CabSignTool\\cabsigntool" if $config["env"]["paths"]["cabwiz"]
  cabsigntool = "cabsigntool" if cabsigntool.nil?

  signature = $config["build"]["wmsign"]
  cab_in  = cabfile
  cab_out = cabfile

  #TODO: need better solution, maybe just let know users on wiki to add bin dir of MS SDKs to PATH variable
  #Assuming that MS SDKs intalled to default path on c: or d: disk
  if Dir.exists?("C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\bin")
    ENV['PATH'] = ENV['PATH'] + "C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\bin" + ";"
  end
  if Dir.exists?("D:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\bin")
    ENV['PATH'] = ENV['PATH'] + "D:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\bin" + ";"
  end

  if system(cabsigntool + " "  + cab_in + " " + cab_out + " -F " + signature)
    puts "\nSigned successfully.\n\n"
  else
    puts "\nFailed to sign .cab file!\n\n"
  end

  $stdout.flush
end

namespace "config" do
  task :set_wm_platform do
    $current_platform = "wm" unless $current_platform
  end

  task :set_win32_platform do
    $current_platform = "win32" unless $current_platform
    $sdk = "Win32"
  end

  task :wm => [:set_wm_platform, "config:common"] do
    puts " $current_platform : #{$current_platform}"

    unless $sdk
      $sdk = "Windows Mobile 6 Professional SDK (ARMV4I)"
      $sdk = $app_config["wm"]["sdk"] if $app_config["wm"] && $app_config["wm"]["sdk"]
    end

    $rubypath = "res/build-tools/RhoRuby.exe" #path to RubyMac
    $builddir = $config["build"]["wmpath"] + "/build"
    $vcbindir = $config["build"]["wmpath"] + "/bin"
    $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]
    $bindir = $app_path + "/bin"
    $rhobundledir =  $app_path + "/RhoBundle"
    $log_file = $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"]
    $srcdir =  $bindir + "/RhoBundle"
    $buildcfg = $app_config["buildcfg"] unless $buildcfg
    $buildcfg = "Release" unless $buildcfg

    if $sdk == "Windows Mobile 6 Professional SDK (ARMV4I)"
        $targetdir = $bindir + "/target/wm6p"
    else
        $targetdir = $bindir + "/target/#{$sdk}"
    end
        
    $tmpdir =  $bindir +"/tmp"
    $vcbuild = $config["env"]["paths"]["vcbuild"]
    $vcbuild = "vcbuild" if $vcbuild.nil?
    $nsis    = $config["env"]["paths"]["nsis"]
    $nsis    = "makensis.exe" if $nsis.nil?
    $cabwiz = File.join($config["env"]["paths"]["cabwiz"], "cabwiz.exe") if $config["env"]["paths"]["cabwiz"]
    $cabwiz = "cabwiz" if $cabwiz.nil?
    $webkit_capability = !($app_config["capabilities"].nil? or $app_config["capabilities"].index("webkit_browser").nil?) 
    $motorola_capability = !($app_config["capabilities"].nil? or $app_config["capabilities"].index("motorola").nil?) 
    $wk_data_dir = "/Program Files" # its fake value for running without motorola extensions. do not delete
    $additional_dlls_path = nil
    $additional_regkeys = nil
    $use_direct_deploy = "yes"

    begin
      if $webkit_capability || $motorola_capability
        require "rhoelements-data"
        $wk_data_dir = $data_dir[0]
      end
    rescue Exception => e
      puts "rhoelements gem is't found, webkit capability is disabled"
      $webkit_capability = false
      $motorola_capability = false
    end
        
    unless $build_solution
        $build_solution = 'rhodes.sln'
    end

    if $app_config["wm"].nil?
      $port = "11000"
    else
      $port = $app_config["wm"]["logport"].nil? ? "11000" : $app_config["wm"]["logport"]
    end

    $excludelib = ['**/builtinME.rb','**/ServeME.rb','**/dateME.rb','**/rationalME.rb']

    #$app_config["extensions"] = [] unless $app_config["extensions"].is_a? Array
    #if $app_config["wm"] and $app_config["wm"]["extensions"]
    #  $app_config["extensions"] += $app_config["wm"]["extensions"]
    #  $app_config["wm"]["extensions"] = nil
    #end

    $wm_emulator = $app_config["wm"]["emulator"] if $app_config["wm"] and $app_config["wm"]["emulator"]
    $wm_emulator = "Windows Mobile 6 Professional Emulator" unless $wm_emulator

    $use_shared_runtime = (($app_config["use_shared_runtime"].nil? && ($app_config["wm"].nil? || $app_config["wm"]["use_shared_runtime"].nil?)) ? nil : 1 )
    #puts $app_config["wm"]["use_shared_runtime"].inspect
    #puts $use_shared_runtime.inspect
  end
end

namespace "build" do
  namespace "wm" do
    task :extensions => "config:wm" do
      if not $use_shared_runtime.nil? then next end

      if $additional_dlls_path.nil?
        puts 'new $additional_dlls_paths'
        $additional_dlls_paths = Array.new
      end
      
      $regkeys = Array.new
      
      $app_config["extensions"].each do |ext|
        $app_config["extpaths"].each do |p|
          extpath = File.join(p, ext, 'ext')
          commin_ext_path = File.join(p, ext)
          ext_config_path = File.join(p, ext, "ext.yml")
          ext_config = nil

          next unless File.exists? File.join(extpath, "build.bat")
          
          puts "ext_config_path - " + ext_config_path.to_s
          if File.exist? ext_config_path
            ext_config = YAML::load_file(ext_config_path)
          end

          puts "extpath - " + commin_ext_path.to_s
          chdir commin_ext_path 
            if ext_config != nil && ext_config["files"] != nil
              puts "ext_config[files] - " + ext_config["files"].to_s
              $additional_dlls_paths << File.expand_path(ext_config["files"])
            end
            
            puts 'start read reg key'
            if ext_config != nil && ext_config["regkeys"] != nil
              ext_config["regkeys"].each do |key|
                puts "extension " + ext + " add regkey to cab. key: " + key
                $regkeys << key
              end
            end
            puts 'end read reg key'
          chdir $startdir

          ENV['RHO_PLATFORM'] = $current_platform
          ENV['RHO_BUILD_CONFIG'] = 'Release'
          ENV['PWD'] = $startdir
          ENV['RHO_ROOT'] = ENV['PWD']

          ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", 'wm', "bin", $sdk, "rhodes", $current_platform == 'wm' ? "Release" : $buildcfg)
          ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform",  'wm', "bin", $sdk, "extensions", ext)
          ENV['VCBUILD'] = $vcbuild
          ENV['SDK'] = $sdk

          chdir extpath
          puts `build.bat`
          chdir $startdir

          break
        end
      end
      
      #test
      #$additional_dlls_paths.each do |x|
      #  puts " - " + x.to_s
      #end
      #exit
    end

    #    desc "Build wm rhobundle"
    task :rhobundle => ["config:wm", "build:bundle:noxruby", "build:wm:extensions"] do
	    Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
    end

    task :rhodes => ["config:wm", "build:wm:rhobundle"] do
      if not $use_shared_runtime.nil? then next end

      chdir $config["build"]["wmpath"]

      cp $app_path + "/icon/icon.ico", "rhodes/resources" if File.exists? $app_path + "/icon/icon.ico"

      args = ['/M4', $build_solution, "\"Release|#{$sdk}\""]
      puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
      puts Jake.run($vcbuild,args)
      unless $? == 0
        puts "Error building"
        exit 1
      end
      chdir $startdir
    end

    task :devrhobundle => ["config:set_wm_platform", "build:wm:rhobundle", "win32:after_bundle"]
    
    task :upgrade_package => ["build:wm:rhobundle"] do        
      mkdir_p $targetdir if not File.exists? $targetdir
      zip_file_path = File.join($targetdir, "upgrade_bundle.zip")
      Jake.zip_upgrade_bundle( $bindir, zip_file_path)
    end

  end #wm
  
  namespace "win32" do
    task :extensions => "config:wm" do
      $app_config["extensions"].each do |ext|
        $app_config["extpaths"].each do |p|
          extpath = File.join(p, ext, 'ext')
          next unless File.exists? File.join(extpath, "build.bat")

          ENV['RHO_PLATFORM'] = 'win32'
          ENV['RHO_BUILD_CONFIG'] = $rhosimulator_build ? 'Release' : $buildcfg
          ENV['PWD'] = $startdir
          ENV['RHO_ROOT'] = ENV['PWD']
          ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", "wm", "bin", "win32", "rhodes", $rhosimulator_build ? "SimulatorRelease" : $buildcfg)
          ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform", "wm", "bin", "win32", "extensions", ext)
          ENV['VCBUILD'] = $vcbuild
          ENV['SDK'] = $sdk

          puts Jake.run("build.bat", [], extpath)
          break
        end
      end
    end

    #    desc "Build win32 rhobundle"
    #task :rhobundle => ["config:wm", "build:bundle:noxruby", "build:wm:extensions"] do
    #end

    task :rhobundle => ["config:set_win32_platform", "build:wm:rhobundle", :after_bundle] do
    end

    task :set_debug_config do
        $buildcfg = 'debug'
    end

    task :devrhobundle => ["config:set_win32_platform", :set_debug_config, "build:wm:rhobundle", :after_bundle] do
    end

    task :after_bundle do
      win32rhopath = 'platform/wm/bin/win32/rhodes/' + $buildcfg + '/rho/'
      mkdir_p win32rhopath
      namepath = File.join(win32rhopath,"name.txt")
      old_appname = File.read(namepath) if File.exists?(namepath)

      confpath = File.join(win32rhopath,"apps/rhoconfig.txt.changes")
      confpath_content = File.read(confpath) if File.exists?(confpath)

      rm_rf win32rhopath + 'lib'
      rm_rf win32rhopath + 'apps'
      rm_rf win32rhopath + 'db' if old_appname != $appname

      cp_r $srcdir + '/lib', win32rhopath
      cp_r $srcdir + '/apps', win32rhopath
      cp_r $srcdir + '/db', win32rhopath

      File.open(namepath, "w") { |f| f.write($appname) }
      File.open(confpath, "w") { |f| f.write(confpath_content) }  if old_appname == $appname && confpath_content && confpath_content.length()>0

    end

    task :rhosimulator => ["config:set_win32_platform", "config:wm", "build:rhosimulator_version"] do
      $rhosimulator_build = true
      $config["platform"] = $current_platform
      chdir $startdir
      init_extensions(pwd, nil)
      Rake::Task["build:win32:extensions"].invoke

      chdir $config["build"]["wmpath"]

      qtdir = ENV['QTDIR']
      unless (qtdir !~/^\s*$/) and File.directory?(qtdir)
        puts "\nPlease, set QTDIR environment variable to Qt root directory path"
        exit 1
      end

      args = ['/M4', $build_solution, '"SimulatorRelease|win32"']
      puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
      puts Jake.run($vcbuild,args)

      chdir $startdir

      unless $? == 0
        puts "Error building"
        exit 1
      end

      target_path = File.join( $startdir, "platform/win32/RhoSimulator/")
      if not File.directory?(target_path)
        Dir.mkdir(target_path)
      end
      cp File.join($startdir, $vcbindir, "win32/rhodes/SimulatorRelease/rhosimulator.exe"), target_path

      cp File.join(qtdir, "bin/phonon4.dll"), target_path
      cp File.join(qtdir, "bin/QtCore4.dll"), target_path
      cp File.join(qtdir, "bin/QtGui4.dll"), target_path
      cp File.join(qtdir, "bin/QtNetwork4.dll"), target_path
      cp File.join(qtdir, "bin/QtWebKit4.dll"), target_path
      target_if_path = File.join(target_path, 'imageformats/')
      if not File.directory?(target_if_path)
        Dir.mkdir(target_if_path)
      end
      cp File.join(qtdir, "plugins/imageformats/qgif4.dll"), target_if_path
      cp File.join(qtdir, "plugins/imageformats/qico4.dll"), target_if_path
      cp File.join(qtdir, "plugins/imageformats/qjpeg4.dll"), target_if_path
      cp File.join(qtdir, "plugins/imageformats/qmng4.dll"), target_if_path
      cp File.join(qtdir, "plugins/imageformats/qsvg4.dll"), target_if_path
      cp File.join(qtdir, "plugins/imageformats/qtiff4.dll"), target_if_path
    end

  end

  #desc "Build rhodes for win32"
  task :win32 => ["build:win32:rhobundle"] do
    chdir $config["build"]["wmpath"]

    args = ['/M4', $build_solution,  "\"" + $buildcfg + '|win32"']
    puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
    puts Jake.run($vcbuild,args)

    chdir $startdir

    unless $? == 0
      puts "Error building"
      exit 1
    end
  end
end

namespace "device" do

  namespace "wm" do
    desc "Build production for device or emulator"
    task :production => ["config:wm","build:wm:rhobundle","build:wm:rhodes"] do

      wm_icon = $app_path + '/icon/icon.ico'
      if $use_shared_runtime.nil? then
        out_dir = $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/Release/"
        cp out_dir + "rhodes.exe", out_dir + $appname + ".exe"
        cp $startdir + "/res/build-tools/license_rc.dll", out_dir + "license_rc.dll"
      else
        shortcut_content = '"\\Program Files\\RhoElements\\RhoElements.exe" -approot="\\Program Files\\' + $appname + '"'
        if File.exists? wm_icon then
          shortcut_content = shortcut_content + '?"\\Program Files\\' + $appname + '\\rho\\icon\\icon.ico"'
        end
        shortcut_content = shortcut_content.length().to_s + '#' + shortcut_content
        File.open($srcdir + '/../' + $appname + ".lnk", "w") { |f| f.write(shortcut_content) }
      end

      chdir $builddir

      build_platform = 'wm6'
      build_platform = 'wm653' if $sdk == "Windows Mobile 6.5.3 Professional DTK (ARMV4I)"
      build_platform = 'ce5' if $sdk == "MC3000c50b (ARMV4I)"

      icon_dest = $srcdir + '/icon'
      rm_rf icon_dest
      if not $use_shared_runtime.nil? then
        rm_rf $srcdir + '/lib'
        if File.exists? wm_icon then
          mkdir_p icon_dest if not File.exists? icon_dest
          cp wm_icon, icon_dest
        end
      end

      args = ['build_inf.js', $appname + ".inf", build_platform, '"' + $app_config["name"] +'"', $app_config["vendor"], '"' + $srcdir + '"', $hidden_app, 
        ($webkit_capability ? "1" : "0"), $wk_data_dir, (($use_shared_runtime.nil?) ? "0" : "1"), ($motorola_capability ? "1" : "0")]

      if $use_shared_runtime.nil? then
         $additional_dlls_paths.each do |path|
            args << path
         end
      end
      
      reg_keys_filename = File.join(File.dirname(__FILE__), "regs.txt");
      puts 'remove file with registry keys'
      if File.exists? reg_keys_filename  
        rm reg_keys_filename 
      end
      
       if $regkeys && $regkeys.size > 0
        puts 'add registry keys to file'
        $regkey_file = File.new(reg_keys_filename, "w+")
      
        $regkeys.each do |key|
          $regkey_file.puts(key + "\n")
        end
        
        $regkey_file.close   
      end  
      
      puts Jake.run('cscript',args)
      unless $? == 0
        puts "Error running build_inf"
        exit 1
      end

      args = [$appname + ".inf"]
      puts Jake.run($cabwiz, args)
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

      mkdir_p $bindir if not File.exists? $bindir
      mkdir_p $targetdir if not File.exists? $targetdir
      mv $appname + ".inf", $targetdir
      mv $appname + ".cab", $targetdir

      File.open(File.join($targetdir,"app_info.txt"), "w") { |f| f.write( $app_config["vendor"] + " " + $appname + "/" + $appname + ".exe") }

      if (not $config["build"]["wmsign"].nil?) and $config["build"]["wmsign"] != ""
        sign $targetdir + '/' +  $appname + ".cab";
      end

      rm_f "cleanup.js"

      chdir $startdir
    end
  end
end

namespace "clean" do

  desc "Clean wm"
  task :wm => "clean:wm:all" do
  end

  namespace "wm" do
    task :rhodes => ["config:wm"] do
      rm_rf $vcbindir + "/#{$sdk}"
      rm_rf $targetdir
    end
    task :all => "clean:wm:rhodes"
  end

  desc "Clean win32"
  task :win32 => [ "config:wm" ]do
    rm_rf $vcbindir + "/win32"
    #rm_rf $targetdir
  end
end

namespace "run" do
  task :mylogserver => ["config:wm"] do
    Jake.run_rho_log_server($app_path)
  end

  def gelLogPath
    log_file_path =  File.join($app_path, $log_file)
    return log_file_path
  end

  desc "Build and run on WM6 emulator"
  task :wm => ["device:wm:production"] do
    if $use_direct_deploy == "no" 
      Rake::Task["run:wm:cab"].execute
    else
      # kill all running detool
      kill_detool

      cd $startdir + "/res/build-tools"
      detool = "detool.exe"
    
      puts "\nStarting application on the WM6 emulator\n\n"
      log_file = gelLogPath

      File.delete($app_path + "/started")  if File.exists?($app_path + "/started")
      Jake.run_rho_log_server($app_path)
      puts "Start Log Server. Wating while Log Server is started..."
      while true do
        if File.exists?($app_path + "/started")
          break
        end
        sleep(1)
      end

      if $webkit_capability and ($use_shared_runtime.nil?)
        wk_args   = [ 'wk-emu', "\"#{$wm_emulator}\"", '"'+ $wk_data_dir.gsub(/"/,'\\"') + '"', '"'+ $appname + '"']
        Jake.run2( detool, wk_args, {:nowait => false})
      end

      if $additional_dlls_paths
        $additional_dlls_paths.each do |path|
          add_files_args   = [ 'wk-emu', "\"#{$wm_emulator}\"", '"'+ path.gsub(/"/,'\\"') + '"', '"'+ $appname + '"']
          Jake.run2( detool, add_files_args, {:nowait => false})
        end
      end

      args = [ 'emu', "\"#{$wm_emulator}\"", '"'+$appname.gsub(/"/,'\\"')+'"', '"'+$srcdir.gsub(/"/,'\\"')+'"', '"'+((not $use_shared_runtime.nil?) ? $srcdir + '/../' + $appname + '.lnk' : $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/Release/" + $appname + ".exe").gsub(/"/,'\\"')+'"' , $port,  '"'+$startdir + "/res/build-tools/license_rc.dll" + '"']
      Jake.run2( detool, args, {:nowait => false})
    end
  end

  namespace "wm" do
    task :get_log => "config:wm" do
      puts "log_file=" + gelLogPath
    end

    desc "Run application on RhoSimulator"    
    task :rhosimulator => ["config:set_wm_platform", "config:common"] do
      $rhosim_config = "platform='wm'\r\n"
      Rake::Task["run:rhosimulator"].invoke
    end
    
    task :rhosimulator_debug => ["config:set_wm_platform", "config:common"] do
      $rhosim_config = "platform='wm'\r\n"
      Rake::Task["run:rhosimulator_debug"].invoke
    end

    desc "Build and run on the Windows Mobile device"
    task :device => ["device:wm:production"] do
      if $use_direct_deploy == "no" 
        Rake::Task["run:wm:device:cab"].execute
      else
        # kill all running detool
        kill_detool

        cd $startdir + "/res/build-tools"
        detool = "detool.exe"     

        puts "\nStarting application on the device"
        puts "Please, connect you device via ActiveSync.\n\n"
        log_file = gelLogPath

        File.delete($app_path + "/started")  if File.exists?($app_path + "/started")
 	  
        Jake.run_rho_log_server($app_path)
        puts "RhoLogServer is starting"
        while true do
          if File.exists?($app_path + "/started")
            break
          end
          sleep(1)
        end    

        if $webkit_capability and ($use_shared_runtime.nil?)
          wk_args   = [ 'wk-dev', '"'+ $wk_data_dir.gsub(/"/,'\\"') + '"', '"'+ $appname + '"']
          Jake.run2( detool, wk_args, {:nowait => false})
        end

        if $additional_dlls_paths
          $additional_dlls_paths.each do |path|
            add_files_args   = [ 'wk-dev', '"'+ path.gsub(/"/,'\\"') + '"', '"'+ $appname + '"']
            Jake.run2( detool, add_files_args, {:nowait => false})
          end
        end

        args = [ 'dev', '"'+$appname.gsub(/"/,'\\"')+'"', '"'+$srcdir.gsub(/"/,'\\"')+'"', '"'+((not $use_shared_runtime.nil?) ? $srcdir + '/../' + $appname + '.lnk' : $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/Release/" + $appname + ".exe").gsub(/"/,'\\"')+'"', $port,  '"'+$startdir + "/res/build-tools/license_rc.dll" + '"']
        Jake.run2( detool, args, {:nowait => false})
      end
    end

    task :spec => ["device:wm:production"] do
        # kill all running detool
        kill_detool

        cd $startdir + "/res/build-tools"
        detool = "detool.exe"    
        args   = [ 'emu', "\"#{$wm_emulator}\"", '"'+$appname.gsub(/"/,'\\"')+'"', '"'+$srcdir.gsub(/"/,'\\"')+'"', '"'+($startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/Release/" + $appname + ".exe").gsub(/"/,'\\"')+'"' , $port]
        puts "\nStarting application on the WM6 emulator\n\n"
        log_file = gelLogPath

        #remove log file
        rm_rf log_file if File.exists?(log_file)

        File.delete($app_path + "/started")  if File.exists?($app_path + "/started")
        Jake.run_rho_log_server($app_path)
        puts "RhoLogServer is starting"
        while true do
          if File.exists?($app_path + "/started")
            break
          end
          sleep(1)
        end

        Jake.before_run_spec
        start = Time.now
        
        #Jake.run2( detool, ['log', log_file, $port], {:nowait => true})
        Jake.run2( detool, args, {:nowait => false})

        puts "waiting for log: " + log_file
        
        for i in 0..120
		if !File.exist?(log_file)
			sleep(1)
		else
			break
		end
        end

	    if !File.exist?(log_file)
		    puts "Can not read log file: " + log_file
		    exit(1)
        end

        puts "start read log"
        
        io = File.new(log_file, "r")
        waiting_count = 0
        end_spec = false
        while !end_spec do
            line_count = 0
            io.each do |line|
                #puts line
                end_spec = !Jake.process_spec_output(line)
                break if end_spec
                line_count += 1
            end
            if line_count==0
                waiting_count += 1
            else
                waiting_count = 0
            end
            if waiting_count > 240
                puts "spec application hung (240 seconds timeout)"
                end_spec = true
            end
            sleep(1) unless end_spec
        end
        io.close

        Jake.process_spec_results(start)
        
        $stdout.flush
        chdir $startdir
    end

    task :phone_spec do
      Jake.run_spec_app('wm','phone_spec')
      exit 1 if $total.to_i==0
      exit $failed.to_i
    end

    task :framework_spec do
      Jake.run_spec_app('wm','framework_spec')
      exit 1 if $total.to_i==0
      exit $failed.to_i
    end

    namespace "device" do
      desc "Build, install .cab and run on the Windows Mobile device"
      task :cab => ["device:wm:production"] do
        # kill all running detool
        kill_detool

        cd $startdir + "/res/build-tools"
        detool = "detool.exe"
        args   = ['devcab', $targetdir + '/' +  $appname + ".cab", $appname, (($use_shared_runtime.nil?) ? "0" : "1")]
        puts "\nStarting application on the device"
        puts "Please, connect you device via ActiveSync.\n\n"
        log_file = gelLogPath

        # temporary disable log from device (caused enormous delays)
        # Jake.run2( detool, ['log', log_file, $port], {:nowait => true})
        Jake.run(detool,args)
      end
    end

    desc "Build, install .cab and run on WM6 emulator"
    task :cab => ["device:wm:production"] do
      # kill all running detool
      kill_detool

      cd $startdir + "/res/build-tools"
      detool = "detool.exe"
      args   = ['emucab', "\"#{$wm_emulator}\"", $targetdir + '/' +  $appname + ".cab", $appname, (($use_shared_runtime.nil?) ? "0" : "1")]
      log_file = gelLogPath

      Jake.run2( detool, ['log', log_file, $port], {:nowait => true})

      puts "\nStarting application on the WM6 emulator\n\n"
      Jake.run(detool,args)
    end
  end

  desc "Run win32"
  task :win32 => ["build:win32"] do
    args = [' ']
    #    chdir $config["build"]["wmpath"]
    #    Thread.new { Jake.run("bin\\win32\\rhodes\\Debug\\rhodes", args) }
    Jake.run2 "bin\\win32\\rhodes\\" + $buildcfg + "\\rhodes.exe", args, {:directory => $config["build"]["wmpath"], :nowait => true}

    $stdout.flush
    chdir $startdir

    unless $? == 0
      puts "Error to run rhodes for win32"
      exit 1
    end
  end

  namespace "rhosimulator" do
    task :get_log => "config:common" do
      $log_file = $app_config["applog"].nil? ? "applog.txt" : $app_config["applog"]
      puts "log_file=" + File.join($app_path, "rhosimulator", $log_file)
    end
  end

  namespace "win32" do
    task :delete_db do
      $buildcfg = $app_config["buildcfg"]
      $buildcfg = "Release" unless $buildcfg
    
      db_path = 'platform/wm/bin/win32/rhodes/' + $buildcfg + '/rho/db'
      rm_rf db_path if File.exists?(db_path)
    end

    task :spec => [:delete_db, "build:win32"] do
      #remove log file
      win32rhopath = 'platform/wm/bin/win32/rhodes/' + $buildcfg + '/rho/'
      win32logpath = File.join(win32rhopath,"RhoLog.txt")
      win32logpospath = File.join(win32rhopath,"RhoLog.txt_pos")
      win32configpath = File.join(win32rhopath,"apps/rhoconfig.txt.changes")
      rm_rf win32logpath if File.exists?(win32logpath)
      rm_rf win32logpospath if File.exists?(win32logpospath)
      rm_rf win32configpath if File.exists?(win32configpath)

      Jake.before_run_spec
      start = Time.now

      args = [' ']
      Jake.run2( "bin\\win32\\rhodes\\" + $buildcfg + "\\rhodes.exe", args, {:directory => $config["build"]["wmpath"], :nowait => false}) do |line|
        Jake.process_spec_output(line)
      end
      Jake.process_spec_results(start)

      $stdout.flush
      chdir $startdir
    end

    task :phone_spec do
      Jake.run_spec_app('win32','phone_spec')
      exit 1 if $total.to_i==0
      exit $failed.to_i
    end

    task :framework_spec do
      Jake.run_spec_app('win32','framework_spec')
      exit 1 if $total.to_i==0
      exit $failed.to_i
    end

    desc "Build production for Motorola device"
    task :production => ["build:win32"] do

      out_dir = $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/" + $buildcfg + "/"
      puts "out_dir - "  + out_dir
      cp out_dir + "rhodes.exe", out_dir + $appname + ".exe"
      cp out_dir + $appname + ".exe", $bindir + "/" + $appname + ".exe"

      script_name = File.join($startdir, "platform", "wm", "build", "rhodes.nsi")
      app_script_name = File.join($startdir, "platform", "wm", "build")
      app_script_name += "/" + $appname + ".nsi"

      # custumize install script for application
      install_script = File.read(script_name)
      install_script = install_script.gsub(/%APPNAME%/, $appname)
      install_script = install_script.gsub(/%APP_EXECUTABLE%/, $appname + ".exe") 
      install_script = install_script.gsub(/%SECTOIN_TITLE%/, "\"This installs " + $appname + "\"")
      install_script = install_script.gsub(/%SMPROGDIR%/, $appname)
      install_script = install_script.gsub(/%FINISHPAGE_TEXT%/, "\"Thank you for installing " + $appname + " \\r\\n\\n\\n\"")
      install_script = install_script.gsub(/%APPINSTALLDIR%/, "C:\\" + $appname)
      install_script = install_script.gsub(/%APPICON%/, "rhodes.ico")
      install_script = install_script.gsub(/%SCUNISTALLPATH%/, "\"$SMPROGRAMS\\" + $appname + "\\Uninstall " + $appname + ".lnk\"")
      install_script = install_script.gsub(/%SCAPPPATH%/, "\"$SMPROGRAMS\\" + $appname + "\\" + $appname + ".lnk\"")
      install_script = install_script.gsub(/%APPEXEPATH%/, "\"C:\\" + $appname + "\\" + $appname + ".exe\"")
      install_script = install_script.gsub(/%SECTION_NAME%/, "\"" + $appname + "\"")
      File.open(app_script_name, "w") { |file| file.puts install_script }

      cp app_script_name, $bindir
      puts "$appname - " + $appname
      cp out_dir + "/" + $appname + ".exe", $bindir
      cp $startdir + "/res/icons/rhodes.ico", $bindir
      rm app_script_name

      chdir $bindir

      cp_r  $srcdir, $bindir + "/rho"
      puts "$nsis - " + $nsis
      args = [$bindir + "/" + $appname + ".nsi"]
      puts "arg = " + args.to_s
      puts Jake.run2($nsis, args, {:nowait => false} )

    end
  end

end
