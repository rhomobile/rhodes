#common function tools to work with android devices
#
# uses following globals
# $adb

#USE_TRACES = Rake.application.options.trace

module AndroidTools

def get_app_log (appname, device, silent = false)
  pkgname = "com.#{$vendor}." + appname.downcase.gsub(/[^A-Za-z_0-9]/, '')
  path = File.join('/data/data', pkgname, 'rhodata', 'RhoLog.txt')
#  cc_run($adb, [device ? '-d' : '-e', 'pull', path, $app_path]) or return false
  Jake.run($adb, [device ? '-d' : '-e', 'pull', path, $app_path]) or return false
  puts "RhoLog.txt stored to " + $app_path
  return true
end
module_function :get_app_log  

def is_emulator_running
  `"#{$adb}" devices`.split("\n")[1..-1].each do |line|
    return true if line =~ /^emulator/
  end
  return false
end
module_function :is_emulator_running

def is_device_running
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
  `"#{$adb}" #{flag} shell ps`.split.each do |line|
    return true if line =~ /#{pkg}/
  end
  false
end
module_function :application_running

def kill_adb_and_emulator
  if RUBY_PLATFORM =~ /windows|cygwin|mingw/
    # Windows
    `taskkill /F /IM adb.exe`
    `taskkill /F /IM emulator.exe`
  else
    `killall -9 adb`
    `killall -9 emulator`
  end
end
module_function :kill_adb_and_emulator

def logcat(device_flag = '-e', log_path = $applog_path)
  if !log_path.nil?
    rm_rf log_path unless !File.exist(log_path)
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
  Jake.run($adb, [device_flag, 'logcat', '-c'], nil, true) 
end
module_function :logclear

end
