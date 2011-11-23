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
# $adb

#USE_TRACES = Rake.application.options.trace

module AndroidTools

def fill_api_levels(sdkpath)
    $api_levels = Hash.new
    $market_versions = Hash.new
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

def run_application (target_flag)
  args = []
  args << target_flag
  args << "shell"
  args << "am"
  args << "start"
  args << "-a"
  args << "android.intent.action.MAIN"
  args << "-n"
  args << $app_package_name + "/#{JAVA_PACKAGE_NAME}.RhodesActivity"
  Jake.run($adb, args)
end
module_function :run_application

def application_running(flag, pkgname)
  pkg = pkgname.gsub(/\./, '\.')
  system("\"#{$adb}\" start-server")
  `"#{$adb}" #{flag} shell ps`.split.each do |line|
    return true if line =~ /#{pkg}/
  end
  false
end
module_function :application_running

def  kill_adb
    # stop app
    if RUBY_PLATFORM =~ /(win|w)32$/
        # Windows
        `taskkill /F /IM adb.exe`
    else
        `killall -9 adb`
    end
end
module_function :kill_adb

def kill_adb_and_emulator
  if RUBY_PLATFORM =~ /windows|cygwin|mingw/
    # Windows
    `taskkill /F /IM adb.exe`
    `taskkill /F /IM emulator-arm.exe`
    `taskkill /F /IM emulator.exe`
  else
    `killall -9 adb`
    `killall -9 emulator-arm`
    `killall -9 emulator`
  end
end
module_function :kill_adb_and_emulator

def logcat(device_flag = '-e', log_path = $applog_path)
  if !log_path.nil?
    rm_rf log_path if File.exist?(log_path)
    Thread.new { Jake.run($adb, [device_flag, 'logcat', '>>', log_path], nil, true) }
  end
end
module_function :logcat

def logcat_process(device_flag = '-e', log_path = $applog_path)
  if !log_path.nil?
    Thread.new { system("\"#{$adb}\" #{device_flag} logcat >> \"#{log_path}\" ") }  
  end
end
module_function :logcat_process

def logclear(device_flag = '-e')
  return if(device_flag == '-e' and !is_emulator_running)
  Jake.run($adb, [device_flag, 'logcat', '-c'], nil, true) 
end
module_function :logclear

end
