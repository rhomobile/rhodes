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

#common function tools to work with android devices
#
# uses following globals
# $adb, $applog_path

#USE_TRACES = Rake.application.options.trace

require 'pp'
require 'open3'

module AndroidTools

def fill_api_levels(sdkpath)
    $api_levels = Hash.new
    $market_versions = Hash.new
    $platforms = Hash.new
    max_apilevel = 0
    max_platform = nil

    Dir.glob(File.join(sdkpath, "platforms", "*")).each do |platform|
      props = File.join(platform, "source.properties")
      unless File.file? props
        puts "+++ WARNING! No source.properties found in #{platform}"
        next
      end

      apilevel = 0
      marketversion = nil
      File.open(props, "r") do |f|
        while line = f.gets
          apilevel = $1.to_i if line =~ /^\s*AndroidVersion\.ApiLevel\s*=\s*([0-9]+)\s*$/
          marketversion = $1 if line =~ /^\s*Platform\.Version\s*=\s*([^\s]*)\s*$/
        end
      end

      puts "+++ API LEVEL of #{platform}: #{apilevel}" if USE_TRACES

      if apilevel != 0
        $api_levels[marketversion] = apilevel
        $market_versions[apilevel] = marketversion
        $platforms[apilevel] = File.basename platform
        if apilevel > max_apilevel
          max_apilevel = apilevel
          max_platform = File.basename platform
        end
      end
    end

    max_platform
end
module_function :fill_api_levels

def get_installed_market_versions
    $api_levels.keys
end
module_function :get_installed_market_versions

def get_installed_api_levels
    $market_versions.keys
end
module_function :get_installed_api_levels

def get_market_version(apilevel)
    $market_versions[apilevel]
end
module_function :get_market_version

def get_api_level(marketversion)
    $api_levels[marketversion]
end
module_function :get_api_level

def get_platform(apilevel)
    $platforms[apilevel]
end
module_function :get_platform

def get_addon_classpath(addon_pattern, apilevel = nil)

#libnames

    if USE_TRACES
      puts "Looking for name pattern: #{addon_pattern}"
      puts "Looking for API level: #{apilevel}" if apilevel
    end

    found_name = nil
    found_libpatterns = nil
    found_classpath = nil
    found_apilevel = nil
    namepattern = Regexp.new("^name=(.+)$")
    addonnamepattern = Regexp.new(addon_pattern)
    libspattern = Regexp.new("^libraries=(.+)$")

    Dir.glob(File.join($androidsdkpath, 'add-ons', '*')).each do |dir|
        next unless File.directory? dir

        libs = {}
        cur_apilevel = nil
        cur_name = nil
        classpath = nil
        props = File.join(dir, 'manifest.ini')
        unless File.file? props
            puts "+++ WARNING: no manifest.ini found in #{dir}"
            next
        end

        File.open(props, 'r') do |f|
          while line = f.gets
            line.strip!
            
            if namepattern =~ line
              puts "Parsing add-on: #{$1}" if USE_TRACES
              cur_name = $1
              break unless addonnamepattern =~ $1
              next
            end
            
            next unless cur_name
            
            if line =~ /^api=([0-9]+)$/
              cur_apilevel = $1.to_i

              puts "API level of #{dir}: #{cur_apilevel}" if USE_TRACES

              break if apilevel and apilevel != cur_apilevel
              break if found_apilevel and found_apilevel > cur_apilevel

              found_name = cur_name
              found_apilevel = cur_apilevel
              found_libpatterns = nil
              
              next
            end

            if found_libpatterns.nil? 
              if libspattern =~ line
                found_libpatterns = []
                libnames = $1.split(';')
                libnames.each do |name|

                  
                  found_libpatterns << Regexp.new("^(#{name})=(.+);.*$")
                end
                
                puts "Library patterns: #{found_libpatterns.inspect}" if USE_TRACES
                
              end
              next
            end
            
            found_libpatterns.each do |pat|
              if(pat =~ line)
                libs[$1] = $2
              end
            end

          end
        end

        next unless cur_apilevel
        next unless found_apilevel
        next if apilevel and apilevel != found_apilevel
        next if found_apilevel and cur_apilevel < found_apilevel

        libs.each do |name, file|
          if classpath
            classpath += $path_separator
          else
            classpath = ''
          end
          classpath += File.join(dir,'libs',file)
        end

        next unless classpath

        found_classpath = classpath
        puts "classpath: #{found_classpath}, API level: #{found_apilevel}" if USE_TRACES

    end

    unless found_classpath
      msg = "No Android SDK add-on found: #{addon_pattern}"
      msg += "; API level: #{apilevel}" if apilevel
      raise msg
    end

    if USE_TRACES
      puts "Add-on name: #{found_name}"
      puts "Add-on API level: #{found_apilevel}"
      puts "Add-on classpath: #{found_classpath}"
    end

    found_classpath
end
module_function :get_addon_classpath

def get_app_log (appname, device, silent = false)
  pkgname = "com.#{$vendor}." + appname.downcase.gsub(/[^A-Za-z_0-9]/, '')
  path = File.join('/data/data', pkgname, 'rhodata', 'RhoLog.txt')
  Jake.run($adb, [device ? '-d' : '-e', 'pull', path, $app_path]) or return false
  puts "RhoLog.txt stored to " + $app_path
  return true
end
module_function :get_app_log

def is_emulator_running
  system("\"#{$adb}\" start-server")
  `"#{$adb}" devices`.split("\n")[1..-1].each do |line|
    return true if line =~ /^emulator/
  end
  return false
end
module_function :is_emulator_running

def is_device_running
  system("\"#{$adb}\" start-server")
  `"#{$adb}" devices`.split("\n")[1..-1].each do |line|
    return true if line !~ /^emulator/
  end
  return false
end
module_function :is_device_running

def  run_emulator(options = {})
  system("\"#{$adb}\" start-server")

  unless is_emulator_running
    puts "Need to start emulator" if USE_TRACES

    if $appavdname
      $avdname = $appavdname
    else
      $avdname = "rhoAndroid" + $emuversion.gsub(/[^0-9]/, "")
      $avdname += "google" if $use_google_addon_api
      #$avdname += "motosol" if $use_motosol_api
    end

    puts $androidtargets.inspect

    raise "Target platform for Android #{$emuversion} is not installed. Please, install corresponding packages in Android SDK Manager or correct build.yml values." if $androidtargets[get_api_level($emuversion)].nil?

    targetid = $androidtargets[get_api_level($emuversion)][:id]

    puts "Using Android SDK target: #{$androidtargets[get_api_level($emuversion)].inspect}" if USE_TRACES

    abi = nil
    sdk_abis = $androidtargets[get_api_level($emuversion)][:abis]

    if sdk_abis            
      if $abis.include?("x86")
        #first look for x86 abis
        sdk_abis.each do |cur_abi|
          if cur_abi =~ /x86/
            abi = cur_abi 
            break
          end
        end
      end

      unless abi
        sdk_abis.each do |cur_abi|
          if cur_abi =~ /armeabi/
            abi = cur_abi
            break
          end
        end
      end

      raise "ARM-based emulator image is not found for selected target: #{$androidtargets[get_api_level($emuversion)][:abis].inspect}" unless abi
    end

    #replace whitespaces in AVD name with underscores
    $avdname.tr!(' ', '_');

    unless File.directory?( File.join(ENV['HOME'], ".android", "avd", "#{$avdname}.avd" ) )
      puts "Emulator API level: #{get_api_level($emuversion)}"
      puts "Emulator params: #{$androidtargets[get_api_level($emuversion)].inspect}"
      if USE_TRACES
        puts "AVD name: #{$avdname}, emulator version: #{$emuversion}, target id: #{targetid}"
      end
      raise "Unable to create AVD image. No appropriate target API for SDK version: #{$emuversion}" unless targetid
      createavd = "\"#{$androidbin}\" create avd --name #{$avdname} --target #{targetid} --sdcard 128M"
      createavd = createavd + " --abi #{abi}" if abi
      puts "Creating AVD image: #{createavd}"
      IO.popen(createavd, 'r+') do |io|
        io.puts "\n"
        while line = io.gets
          puts line
        end
      end

    else
      raise "Unable to run Android emulator. No appropriate target API for SDK version: #{$emuversion}" unless targetid
    end

    # Start the emulator, check on it every 5 seconds until it's running
    cmd = "\"#{$emulator}\""
    cmd << " -no-window" if options[:hidden]
    cmd << " -avd #{$avdname}"
    cmd << " -wipe-data" if options[:wipe]
    # cmd << " -verbose"

    start_emulator(cmd)

    cmd_start_emu = "#{$adb} -e wait-for-device shell getprop sys.boot_completed"
    puts cmd_start_emu
    raise "Android emulator failed to start." unless system(cmd_start_emu)

    puts "Waiting up to 600 seconds for emulator to be ready ..."
    startedWaiting = Time.now
    adbRestarts = 1
    while (Time.now - startedWaiting < 600 )
        sleep 5
        now = Time.now
        started = false
        booted = true
        Jake.run2 $adb, ["-e", "shell", "ps"], :system => false, :hideerrors => false do |line|
            #puts line
            booted = false if line =~ /bootanimation/
            started = true if line =~ /android\.process\.acore/
            true
        end
        #puts "started: #{started}, booted: #{booted}"
        unless started and booted
            printf("%.2fs: ",(now - startedWaiting))
            if (now - startedWaiting) > (180 * adbRestarts)
              # Restart the adb server every 60 seconds to prevent eternal waiting
              puts "Appears hung, restarting adb server"
              kill_adb_logcat('-e')
              Jake.run($adb, ['start-server'], nil, true)
              adbRestarts += 1

              rm_f $applog_path if !$applog_path.nil?
              logcat_process()
            else
              puts "Still waiting..."
            end
        else
            puts "Success"
            puts "Device is ready after " + (Time.now - startedWaiting).to_s + " seconds"
            break
        end
    end
    raise "Emulator still isn't up and running, giving up" unless is_emulator_running
  end

  puts "Emulator is up and running"
  $stdout.flush
end
module_function :run_emulator

def device_options(device)
  if device =~ /^-/
    return [device]
  else
    return ['-s', device]
  end
end
module_function :device_options

def run_application (device_flag, pkgname)
  puts "Starting application.."
  args = device_options(device_flag)
  args << "shell"
  args << "am"
  args << "start"
  args << "-a"
  args << "android.intent.action.MAIN"
  args << "-n"
  args << pkgname + "/#{JAVA_PACKAGE_NAME}.RhodesActivity"
  Jake.run($adb, args)
end
module_function :run_application

def application_running(device_flag, pkgname)
  pkg = pkgname.gsub(/\./, '\.')
  system("\"#{$adb}\" start-server")
  `"#{$adb}" #{device_options(device_flag).join(' ')} shell ps`.split.each do |line|
    return true if line =~ /#{pkg}/
  end
  false
end
module_function :application_running

def load_app_and_run(device_flag, apkfile, pkgname)
  device_id = nil
  device = device_flag
  if device_flag == '-d'
    device = 'connected device'
  elsif device_flag == '-e'
    device = 'started emulator'
  else
    device_id = device_flag
  end

  @@logger.info "Loading package..."

  argv = [$adb] + device_options(device_flag)
  argv << 'install'
  argv << '-r'
  argv << apkfile

  cmd = argv.join(' ')
  argv = cmd if RUBY_VERSION =~ /^1\.8/

  count = 0
  done = false
  child = nil
  while count < 20
    poutres = ""
    perrres = ""
    begin
      status = Timeout::timeout(300) do
        @@logger.debug "CMD: #{cmd}"

        Open3.popen3(cmd) do |pin,pout,perr,wait_thr|
          child = pout.pid
          while line = pout.gets
            poutres << line
            @@logger.debug "POUT RET: #{line}"
          end

          while line = perr.gets
            perrres << line
            @@logger.debug "PERR RET: #{line}"
          end

        end
      end
    rescue Timeout::Error
      @@logger.error "Timeout error, killing #{child}"

      Process.kill 9, child if child
      if theoutput == ""
        @@logger.error "Timeout reached while empty output: killing adb server and retrying..."
        `#{$adb} kill-server`
        count += 1
        sleep 1
        next
      else
        @@logger.error "Timeout reached: try to run application"
        done = true
        break
      end
    end

    @@logger.info( "Upload output: #{poutres.to_s}")
    @@logger.info( "Upload error output: #{perrres.to_s}")

    if poutres.to_s.match(/Success/) or perrres.to_s.match(/Success/)
      done = true
      break
    end
    
    @@logger.error "Failed to load (possibly because emulator/device is still offline) - retrying"
    $stdout.flush
    sleep 1
    count += 1
  end

  run_application(device_flag, pkgname) if done
end
module_function :load_app_and_run

def kill_adb_logcat(device_flag, log_path = $applog_path)
  puts 'Look for \'adb logcat\' processes'

  device_opts = device_options device_flag
  match_str = device_opts.join(' ')

  #cmd_re = Regexp.new "\"?\"?#{$adb}\"?\s+(-(d|e|s\s+\S+))\s+logcat\s+>\s+\"?#{log_path}\"?\"?"
  cmd_re = /"?#{Regexp.escape($adb)}"?\s+(-(d|e|s\s+\S+))\s+logcat\s+.*?>\s+"?#{Regexp.escape(log_path)}"?/
  processes = Jake.get_process_list
  log_shell_pids = []

  processes.each do |proc|
    match_data = cmd_re.match proc[:cmd]
    if match_data
      log_shell_pids << proc[:pid] unless match_data[1] == match_str
    end
  end

  log_pids = []
  processes.each do |proc|
    log_shell_pids.each do |log_shell_pid|
      if proc[:ppid]
        log_pids << proc[:pid] if proc[:ppid] == log_shell_pid
      end
    end
  end

  log_pids += log_shell_pids

  log_pids.each do |pid|
    if RUBY_PLATFORM =~ /(win|w)32$/
      system "taskkill /PID #{pid}"
    else
      system "kill -TERM #{pid}"
    end
  end

  sleep 1

  processes1 = Jake.get_process_list
  log_pids1 = []

  processes1.each do |proc|
    log_pids.each do |pid|
      log_pids1 << proc[:pid] if pid == proc[:pid]
    end
  end

  log_pids1.each do |pid|
    if RUBY_PLATFORM =~ /(win|w)32$/
      system "taskkill /F /PID #{pid}"
    else
      system "kill -KILL #{pid}"
    end
  end

end
module_function :kill_adb_logcat

def kill_adb_and_emulator
  if RUBY_PLATFORM =~ /windows|cygwin|mingw/
    # Windows
    `taskkill /F /IM adb.exe`
  else
    `killall -9 adb 2> /dev/null`
  end
  stop_emulator
end
module_function :kill_adb_and_emulator


def process_filter(filter)
  resultF = []

  unless filter.nil? || filter.empty?
    items = filter.strip.split(/\s+/)

    items.each do |item|
      if item =~ /(.+?):(.+)/
        cat = $1
        level = $2.upcase

        if level =~ /^[VDIWEFS]$/
          resultF << "#{cat}:#{level}"
        else
          BuildOutput.put_log(BuildOutput::WARNING,["Unknown log priority level '#{level}' for '#{cat}' in '#{item}'",'Accepted priority levels are:','V - Verbose (lowest priority)','D - Debug','I - Info','W - Warning','E - Error','F - Fatal','S - Silent (highest priority, on which nothing is ever printed)'],'Logcat filter')
        end
      else
        BuildOutput.put_log(BuildOutput::WARNING,["Invalid logcat filtering format for '#{item}'",'Valid format is \'tag:priority\'','More documentation at http://developer.android.com/tools/debugging/debugging-log.html#filteringOutput'],'Logcat filter')
      end
    end
  end

  unless resultF.empty?
    resultF << '*:S'
    BuildOutput.put_log(BuildOutput::NOTE,["Logcat filter is set to #{resultF.join(' ')} all other items will be filtered out"],'Logcat filter')
  end

  resultF
end
module_function :process_filter

def logcat(filter = nil, device_flag = '-e', log_path = $applog_path)
  if !log_path.nil?
    device_opts = device_options device_flag
    device_re = device_opts.join('\s+')

    cmd_re = /"?#{Regexp.escape($adb)}"?\s+#{device_re}\s+logcat\s+.*?>\s+"?#{Regexp.escape(log_path)}"?/
    pids = Jake.get_process_list
    log_pids = []

    pids.each do |proc|
      log_pids << proc if proc[:cmd] =~ cmd_re
    end

    if log_pids.empty?
      rm_rf log_path if File.exist?(log_path)
      puts 'Starting new logcat'
      data = process_filter(filter)
      Thread.new { Jake.run($adb, device_opts + ['logcat'] + data + [ '>', "\"#{log_path}\""], nil, true) }
    end
  end
end
module_function :logcat

def logcat_process(filter = nil, device_flag = '-e', log_path = $applog_path)
  if !log_path.nil?
    device_opts = device_options device_flag
    device_re = device_opts.join('\s+')

    cmd_re = /"?#{Regexp.escape($adb)}"?\s+#{device_re}\s+logcat\s+.*?>\s+"?#{Regexp.escape(log_path)}"?/
    pids = Jake.get_process_list
    log_pids = []

    pids.each do |proc|
      log_pids << proc if proc[:cmd] =~ cmd_re
    end

    if log_pids.empty?
      puts 'Starting new logcat process'
      data = process_filter(filter).join(' ')
      Thread.new { system("\"#{$adb}\" #{device_opts.join(' ')} logcat #{data} > \"#{log_path}\" ") }
    end
  end
end
module_function :logcat_process

def logclear(device_flag = '-e')
  return if(device_flag == '-e' and !is_emulator_running)
  Jake.run($adb, device_options(device_flag) + ['logcat', '-c'], nil, true)
end
module_function :logclear

def read_manifest_package(path)
  manifestpath = File.join path, 'AndroidManifest.xml'
  if File.exists? manifestpath
    puts "library manifest: #{manifestpath}"

    manifestdoc = REXML::Document.new(File.new(manifestpath))
    manifesttag = manifestdoc.elements["manifest"]

    return manifesttag.attribute('package').to_s
  end
  ''
end
module_function :read_manifest_package


  def self.jarsigner= v
    @@jarsigner=v
  end

  def self.zipalign= v
    @@zipalign=v
  end

  def self.keytool= v
    @@keytool=v
  end

  def self.logger= v
    @@logger=v
  end

  def signApk( inputApk, outputApk, keystore, keypass, storepass, storealias  )
    @@logger.info "Signing APK file"

    args = []
    args << "-sigalg"
    args << "MD5withRSA"
    args << "-digestalg"
    args << "SHA1"
    args << "-verbose"
    args << "-keystore"
    args << keystore
    args << "-storepass"
    args << storepass
    args << "-signedjar"
    args << outputApk
    args << inputApk
    args << storealias if storealias
    
    Jake.run2(@@jarsigner, args, :hide_output => !USE_TRACES )
    unless $?.success?
      @@logger.error "Error running jarsigner"
      exit 1
    end
  end
  module_function :signApk

  def signApkDebug( inputApk, outputApk )
    @@logger.info "Align Debug APK file"

    keystore = File.join(Dir.home,'/.android/debug.keystore')
    #Try to find debug keystore in another location
    keystore = File.join(ENV['USERPROFILE'],'/.android/debug.keystore') unless File.file?(keystore)
    raise "Can't find debug keystore in user's home folder" unless File.file?(keystore)

    signApk( inputApk, outputApk, keystore, 'android', 'android', 'androiddebugkey' )
  end
  module_function :signApkDebug

  def alignApk( inputApk, outputApk )
    args = []
    args << "-f"
    args << "-v"
    args << "4"
    args << inputApk
    args << outputApk
    out = Jake.run2(@@zipalign, args, :hide_output => !USE_TRACES )
    puts out if USE_TRACES
    unless $?.success?
      @@logger.error "Error running zipalign"
      exit 1
    end
  end
  module_function :alignApk

  def generateKeystore( keystore, storealias, storepass, keypass )
    @@logger.info "Generating private keystore..."
    
    mkdir_p File.dirname($keystore) unless File.directory? File.dirname($keystore)

    args = []
    args << "-genkey"
    args << "-alias"
    args << storealias
    args << "-keyalg"
    args << "RSA"
    args << "-validity"
    args << "20000"
    args << "-keystore"
    args << keystore
    args << "-storepass"
    args << storepass
    args << "-keypass"
    args << keypass
    Jake.run(@@keytool, args)
    unless $?.success?
      @@logger.error "Error generating keystore file"
      exit 1
    end
  end
  module_function :generateKeystore

  def findSdkLibJar(sdk_root)
    libdir = File.join(sdk_root, 'tools', 'lib' )
    libpattern = 'sdklib*.jar'
    sdklibjar = File.join(libdir,'sdklib.jar')

    sdklibversion = [0,0,0]

    Dir.glob( File.join( libdir, libpattern )).each do |lib|

      fname = File.basename( lib, '.jar' )

      @@logger.debug("Parsing version for: #{fname}")

      m = fname.match(/^.*-(\d+)\.(\d+).(\d+)/)
      next unless m
      
      version = m.captures

      $logger.debug "Parsed version for #{lib}: #{version[0]}.#{version[1]}.#{version[2]}"

      version.each_with_index do |d,i|
        if d && (d.to_i > sdklibversion[i])
          sdklibversion = [version[0].to_i,version[1].to_i,version[2].to_i]
          sdklibjar = lib
          break
        end
      end
    end

    raise "Could not find sdklib*.jar: #{sdklibjar}" unless File.file?(sdklibjar)

    @@logger.info "Using SDK library: #{sdklibjar}"

    sdklibjar
  end
  module_function :findSdkLibJar


end #module AndroidTools

def start_emulator(cmd)
  # sometimes emulator fails to add itself to adb device list
  # lets reduce failure rate trying to start an emulator several times
  10.times do
    # start emulator async
    Thread.start do
      Jake.run3_dont_fail(cmd)
    end

    # wait while emulator appears in adb device list
    30.times do
      sleep 1
      out = Jake.run4("#{$adb} devices")
      lines = out.chomp.split("\n");
      return if lines.length > 1
    end

    # the time is out and there is no emulator in device list
    puts 'Warning: An emulator is not visible in adb device list. Lets start it again.'
    stop_emulator
    # Restart adb server
    `#{$adb} kill-server`
    `#{$adb} start-server`

  end
  fail "Can't start an emulator."
end

def stop_emulator
  if RUBY_PLATFORM =~ /windows|cygwin|mingw/
    # Windows
    Jake.run3_dont_fail('taskkill /F /IM emulator-arm.exe')
    Jake.run3_dont_fail('taskkill /F /IM emulator.exe')
  else
    Jake.run3_dont_fail('killall -9 emulator-arm 2> /dev/null')
    Jake.run3_dont_fail('killall -9 emulator64-arm 2> /dev/null')
    Jake.run3_dont_fail('killall -9 emulator 2> /dev/null')
  end
end
