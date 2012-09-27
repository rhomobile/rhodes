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
  task :set_wince_platform do
    $current_platform = "wince" unless $current_platform
    $sdk = "MC3000c50b (ARMV4I)"
  end

  task :set_wm_platform do
    $current_platform = "wm" unless $current_platform
  end

  task :set_win32_platform do
    $current_platform = "win32" unless $current_platform
    $sdk = "win32"
  end

  task :wm => [:set_wm_platform, "config:common"] do
    puts " $current_platform : #{$current_platform}"

    unless $sdk
      $sdk = "Windows Mobile 6 Professional SDK (ARMV4I)"
      $sdk = $app_config["wm"]["sdk"] if $app_config["wm"] && $app_config["wm"]["sdk"]
      value = ENV['rho_wm_sdk']
      $sdk = value if value      
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

    $wm_emulator = $app_config["wm"]["emulator"] if $app_config["wm"] and $app_config["wm"]["emulator"]
    $wm_emulator = "Windows Mobile 6 Professional Emulator" unless $wm_emulator

    $use_shared_runtime = (($app_config["use_shared_runtime"].nil? && ($app_config["wm"].nil? || $app_config["wm"]["use_shared_runtime"].nil?)) ? nil : 1 )
      
      puts "$sdk [#{$sdk}]"
  end

  namespace "win32" do

    task :qt do
      $vscommontools = ENV['VS90COMNTOOLS']
      unless !$vscommontools.nil? && ($vscommontools !~ /^\s*$/) && File.directory?($vscommontools)
        puts "\nPlease, set VS90COMNTOOLS environment variable to Common7\Tools directory path of Visual Studio 2008"
        exit 1
      end
      $qtdir = ENV['QTDIR']
      unless !$qtdir.nil? && ($qtdir !~ /^\s*$/) && File.directory?($qtdir)
        puts "\nPlease, set QTDIR environment variable to Qt root directory path"
        exit 1
      end
    end

    task :application do
      $app_version = '1.0'
      $app_version = $app_config["version"] unless $app_config["version"].nil?
      
      require $startdir + "/lib/rhodes.rb"
      fversion = StringIO.new("", "w+")          
      fversion.write( "#define RHOSIMULATOR_NAME \"RhoSimulator\"\n" )
      fversion.write( "#define RHOSIMULATOR_VERSION \"#{Rhodes::VERSION}\"\n" )
      fversion.write( "#define APPLICATION_NAME \"#{$appname}\"\n" )
      fversion.write( "#define APPLICATION_VERSION \"#{$app_version}\"\n" )
      Jake.modify_file_if_content_changed( File.join($startdir, 'platform/shared/qt/rhodes/RhoSimulatorVersion.h'), fversion )  

      $app_icon_path = $app_path + "/icon/icon.ico"
      $app_icon_path = $startdir + "/res/icons/rhodes.ico" unless File.exists? $app_icon_path
      cp $app_icon_path, $startdir + "/platform/wm/rhodes/resources/icon.ico"
      resfile = $startdir + "/platform/wm/bin/win32/rhodes/" +$buildcfg + "/Rhodes.res"
      rm resfile if File.exists? resfile

      $qt_icon_path = $app_path + "/icon/icon.png"
      $qt_icon_path = $startdir + "/res/icons/rhodes.png" unless File.exists? $qt_icon_path
      cp $qt_icon_path, $startdir + "/platform/shared/qt/rhodes/resources/rho.png"
      qrcfile = $startdir + "/platform/shared/qt/rhodes/GeneratedFiles/" + $buildcfg + "/qrc_simulator.cpp"
      rm qrcfile if File.exists? qrcfile
    end

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
          
          #puts "ext_config_path - " + ext_config_path.to_s
          if File.exist? ext_config_path
            ext_config = Jake.config(File.open(ext_config_path))
          end

          puts "#{ext_config}"
          is_prebuilt = ext_config && ext_config['wm'] && ext_config['wm']['exttype'] && ext_config['wm']['exttype'] == 'prebuilt'
          next unless (File.exists?( File.join(extpath, "build.bat") ) || is_prebuilt )

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

          if ENV["TARGET_EXT_DIR"]
            ENV['TARGET_TEMP_DIR'] = File.join(ENV["TARGET_EXT_DIR"], ext)
          else
           ENV['TARGET_TEMP_DIR'] = File.join(ENV['PWD'], "platform", 'wm', "bin", $sdk, "rhodes", $current_platform == 'wm' ? "Release" : $buildcfg)
          end

          ENV['TEMP_FILES_DIR'] = File.join(ENV['PWD'], "platform",  'wm', "bin", $sdk, "extensions", ext)
          ENV['VCBUILD'] = $vcbuild
          ENV['SDK'] = $sdk

          if File.exists? File.join(extpath,'build.bat')
            chdir extpath
            puts `build.bat`
            chdir $startdir
          elsif is_prebuilt
            file_mask = File.join(extpath, 'wm/lib/*.lib' ) 
            puts "PREBUILD: #{file_mask}"
            
            mkdir_p ENV['TARGET_TEMP_DIR'] unless File.exist? ENV['TARGET_TEMP_DIR']
            Dir.glob( file_mask ).each do |lib|
              cp_r lib, ENV['TARGET_TEMP_DIR']
            end
          end
          
          break
        end
      end      
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
    
    task :upgrade_package_partial => ["build:wm:rhobundle"] do
        #puts '$$$$$$$$$$$$$$$$$$'
        #puts 'targetdir = '+$targetdir.to_s
        #puts 'bindir = '+$bindir.to_s

        # process partial update
      
        add_list_full_name = File.join($app_path, 'upgrade_package_add_files.txt')
        remove_list_full_name = File.join($app_path, 'upgrade_package_remove_files.txt')
      
        src_folder = File.join($bindir, 'RhoBundle')
        src_folder = File.join(src_folder, 'apps')
      
        tmp_folder = $bindir + '_tmp_partial'
        rm_rf tmp_folder if File.exists? tmp_folder
        mkdir_p tmp_folder

        dst_tmp_folder = File.join(tmp_folder, 'RhoBundle')
        mkdir_p dst_tmp_folder
        # copy all
        cp_r src_folder, dst_tmp_folder
        
        dst_tmp_folder = File.join(dst_tmp_folder, 'apps')
        mkdir_p dst_tmp_folder

        add_files = []
        if File.exists? add_list_full_name
           File.open(add_list_full_name, "r") do |f|
              while line = f.gets
                 fixed_path = line.gsub('.rb', '.iseq').gsub('.erb', '_erb.iseq').chop
                 add_files << fixed_path
                 puts '### ['+fixed_path+']'
              end
           end
        end
        
        remove_files = []
        if File.exists? remove_list_full_name
           File.open(remove_list_full_name, "r") do |f|
              while line = f.gets
                 fixed_path = line.gsub('.rb', '.iseq').gsub('.erb', '_erb.iseq').chop
                 remove_files << fixed_path
                 #puts '### ['+fixed_path+']'
              end
           end
        end

        psize = dst_tmp_folder.size+1
        Dir.glob(File.join(dst_tmp_folder, '**/*')).sort.each do |f|
          relpath = f[psize..-1]

          if File.file?(f)
             #puts '$$$ ['+relpath+']'
             if not add_files.include?(relpath)
                 rm_rf f
             end 
          end
        end
        
        Jake.build_file_map( dst_tmp_folder, "upgrade_package_add_files.txt" )
                 
        #if File.exists? add_list_full_name
        #   File.open(File.join(dst_tmp_folder, 'upgrade_package_add_files.txt'), "w") do |f|
        #      add_files.each do |j|
        #         f.puts "#{j}\tfile\t0\t0"
        #      end
        #   end
        #end

        if File.exists? remove_list_full_name
           File.open(File.join(dst_tmp_folder, 'upgrade_package_remove_files.txt'), "w") do |f|
              remove_files.each do |j|
                 f.puts "#{j}"
                 #f.puts "#{j}\tfile\t0\t0"
              end
           end
        end

        mkdir_p $targetdir if not File.exists? $targetdir
        zip_file_path = File.join($targetdir, "upgrade_bundle_partial.zip")
        Jake.zip_upgrade_bundle( tmp_folder, zip_file_path)
        rm_rf tmp_folder
    end    

  end #wm
  
  namespace "win32" do
    
    task :deployqt => "config:win32:qt" do
      vsredistdir = File.join($vscommontools, "../../VC/redist/x86/Microsoft.VC90.CRT")
      cp File.join(vsredistdir, "msvcm90.dll"), $target_path
      cp File.join(vsredistdir, "msvcp90.dll"), $target_path
      cp File.join(vsredistdir, "msvcr90.dll"), $target_path
      cp File.join(vsredistdir, "Microsoft.VC90.CRT.manifest"), $target_path
      vsredistdir = File.join($vscommontools, "../../VC/redist/x86/Microsoft.VC90.OPENMP")
      cp File.join(vsredistdir, "vcomp90.dll"), $target_path
      cp File.join(vsredistdir, "Microsoft.VC90.OpenMP.manifest"), $target_path
      cp File.join($startdir, "lib/extensions/openssl.so/ext/win32/bin/libeay32.dll"), $target_path
      cp File.join($startdir, "lib/extensions/openssl.so/ext/win32/bin/ssleay32.dll"), $target_path
      cp File.join($qtdir, "bin/phonon4.dll"), $target_path
      cp File.join($qtdir, "bin/QtCore4.dll"), $target_path
      cp File.join($qtdir, "bin/QtGui4.dll"), $target_path
      cp File.join($qtdir, "bin/QtNetwork4.dll"), $target_path
      cp File.join($qtdir, "bin/QtWebKit4.dll"), $target_path
      target_if_path = File.join($target_path, 'imageformats/')
      if not File.directory?(target_if_path)
        Dir.mkdir(target_if_path)
      end
      cp File.join($qtdir, "plugins/imageformats/qgif4.dll"), target_if_path
      cp File.join($qtdir, "plugins/imageformats/qico4.dll"), target_if_path
      cp File.join($qtdir, "plugins/imageformats/qjpeg4.dll"), target_if_path
      cp File.join($qtdir, "plugins/imageformats/qmng4.dll"), target_if_path
      cp File.join($qtdir, "plugins/imageformats/qsvg4.dll"), target_if_path
      cp File.join($qtdir, "plugins/imageformats/qtiff4.dll"), target_if_path
    end

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
        $buildcfg = 'Debug'
    end

    task :set_release_config do
        $buildcfg = 'Release'
    end

    task :devrhobundle => ["config:set_win32_platform", :set_debug_config, "build:wm:rhobundle", "config:win32:application", :after_bundle] do
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

    task :rhosimulator => ["config:rhosimulator", "config:set_win32_platform", "config:wm", "build:rhosimulator_version", "config:win32:qt"] do
      $config["platform"] = $current_platform
      chdir $startdir
      init_extensions(pwd, nil)
      Rake::Task["build:win32:extensions"].invoke

      cp $startdir + "/res/icons/rhosim.png", $startdir + "/platform/shared/qt/rhodes/resources/rho.png"

      chdir $config["build"]["wmpath"]

      args = ['/M4', $build_solution, '"SimulatorRelease|Win32"']
      puts "\nThe following step may take several minutes or more to complete depending on your processor speed\n\n"
      puts Jake.run($vcbuild,args)

      chdir $startdir

      unless $? == 0
        puts "Error building"
        exit 1
      end

      $target_path = File.join( $startdir, "platform/win32/RhoSimulator/")
      if not File.directory?($target_path)
        Dir.mkdir($target_path)
      end
      cp File.join($startdir, $vcbindir, "win32/rhodes/SimulatorRelease/rhosimulator.exe"), $target_path

      Rake::Task["build:win32:deployqt"].invoke
    end

  end

  #desc "Build rhodes for win32"
  task :win32 => ["build:win32:rhobundle", "config:win32:application"] do
    chdir $config["build"]["wmpath"]

    args = ['/M4', $build_solution,  "\"" + $buildcfg + '|Win32"']
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

  namespace "wince" do
    desc "Build production for device or emulator"
    task :production => ["config:set_wince_platform", "config:wm", "build:wm:rhobundle","build:wm:rhodes", "device:wm:production"] do
    end
  end

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

  namespace "win32" do
    desc "Build installer for Windows"
    task :production => ["build:win32:set_release_config", "config:win32:qt", "build:win32"] do

      out_dir = $startdir + "/" + $vcbindir + "/#{$sdk}" + "/rhodes/" + $buildcfg + "/"
      puts "out_dir - "  + out_dir

      mkdir_p $targetdir
      mkdir_p $tmpdir
      mkdir_p out_dir

      cp out_dir + "rhodes.exe", $tmpdir + "/" + $appname + ".exe"
      cp File.join($startdir, "res/build-tools/win32/license_rc.dll"), $tmpdir

      script_name = File.join($startdir, "platform", "wm", "build", "rhodes.nsi")
      app_script_name = File.join($tmpdir, $appname + ".nsi")

      license_filename = "LICENSE.txt"
      license_file = File.join($app_path, license_filename)
      license_present = '#'
      license_line = ''
      if File.exists? license_file
        cp license_file, $tmpdir
        license_present = ''
        license_line = 'File "' + license_filename + '"'
      end

      readme_filename = "README.html"
      readme_file = File.join($app_path, readme_filename)
      readme_present = '#'
      readme_line = '#'
      if File.exists? readme_file
        cp readme_file, $tmpdir
        readme_present = ''
        readme_line = 'File "' + readme_filename + '"'
      end

      # custumize install script for application
      install_script = File.read(script_name)
      install_script = install_script.gsub(/%OUTPUTFILE%/, $targetdir + "/" + $appname + "-setup.exe" )
      install_script = install_script.gsub(/%APPNAME%/, $appname)
      install_script = install_script.gsub(/%APPVERSION%/, $app_version)
      install_script = install_script.gsub(/%APP_EXECUTABLE%/, $appname + ".exe") 
      install_script = install_script.gsub(/%SECTOIN_TITLE%/, "\"This installs " + $appname + "\"")
      install_script = install_script.gsub(/%FINISHPAGE_TEXT%/, "\"Thank you for installing " + $appname + " \\r\\n\\n\\n\"")
      install_script = install_script.gsub(/%APPINSTALLDIR%/, "C:\\" + $appname)
      install_script = install_script.gsub(/%APPICON%/, "icon.ico")
      install_script = install_script.gsub(/%GROUP_NAME%/, $app_config["vendor"])
      install_script = install_script.gsub(/%SECTION_NAME%/, "\"" + $appname + "\"")
      install_script = install_script.gsub(/%LICENSE_FILE%/, license_line)
      install_script = install_script.gsub(/%LICENSE_PRESENT%/, license_present)
      install_script = install_script.gsub(/%README_FILE%/, readme_line)
      install_script = install_script.gsub(/%README_PRESENT%/, readme_present)
      install_script = install_script.gsub(/%VENDOR%/, $app_config["vendor"])
      File.open(app_script_name, "w") { |file| file.puts install_script }

      puts "$appname - " + $appname

      cp $app_icon_path, $tmpdir + "/icon.ico"

      File.open(File.join($targetdir,"app_info.txt"), "w") { |f| f.write( $app_config["vendor"] + "/" + $appname + "/" + $appname + ".exe") }

      chdir $tmpdir

      target_rho_dir = File.join($tmpdir, "rho")
      rm_rf target_rho_dir
      mv $srcdir, target_rho_dir

      $target_path = $tmpdir
      Rake::Task["build:win32:deployqt"].invoke

      puts "$nsis - " + $nsis
      args = [$tmpdir + "/" + $appname + ".nsi"]
      puts "arg = " + args.to_s

      puts Jake.run2($nsis, args, {:nowait => false} )
      
    end
  end
end

namespace "clean" do

  desc "Clean windows ce"
  task :wince => "clean:wince:all" do
  end

  desc "Clean wm"
  task :wm => "clean:wm:all" do
  end

  namespace "wince" do
    task :rhodes => ["config:set_wince_platform", "config:wm"] do
      rm_rf $vcbindir + "/#{$sdk}"
      rm_rf $targetdir
    end
    task :all => "clean:wince:rhodes"
  end

  namespace "wm" do
    task :rhodes => ["config:wm"] do
      rm_rf $vcbindir + "/#{$sdk}"
      rm_rf $targetdir
    end
    task :all => "clean:wm:rhodes"
  end

  desc "Clean win32"
  task :win32 => [ "config:set_win32_platform", "config:wm" ]do
    rm_rf $vcbindir + "/win32"
    rm_rf $tmpdir
    rm_rf $targetdir
    rm_rf File.join($startdir, 'platform/shared/qt/rhodes/GeneratedFiles')
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

  task :wince  do
    puts "Windows CE emulator not working on windows system."
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

  namespace "wince" do
    task :get_log => "run:wm" do
    end

    desc "Run application on RhoSimulator"    
    task :rhosimulator => ["config:set_wince_platform", "config:common"] do      
      $rhosim_config = "platform='wm'\r\n"
      Rake::Task["run:rhosimulator"].invoke
    end
    
    task :rhosimulator_debug => ["config:set_wince_platform", "config:common"] do
      $rhosim_config = "platform='wm'\r\n"
      Rake::Task["run:rhosimulator_debug"].invoke
    end
    
    desc "Build and run on the Windows CE device"
    task :device => ["device:wince:production"] do      
    end

    namespace "device" do
      desc "Build, install .cab and run on the Windows CE device"
      task :cab => ["device:wince:production"] do
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
  
    cp File.join($startdir, "res/build-tools/win32/license_rc.dll"), File.join( $config["build"]["wmpath"], "bin/win32/rhodes", $buildcfg )

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
    desc "Run application on RhoSimulator"    
    task :rhosimulator => ["config:set_win32_platform", "config:common"] do
      $rhosim_config = "platform='win32'\r\n"
      Rake::Task["run:rhosimulator"].invoke
    end
    
    task :rhosimulator_debug => ["config:set_win32_platform", "config:common"] do
      $rhosim_config = "platform='win32'\r\n"
      Rake::Task["run:rhosimulator_debug"].invoke
    end

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

  end

end
