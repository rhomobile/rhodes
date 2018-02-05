#------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2008-2014 Rhomobile, Inc.
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

require_relative 'lib/build/rho_packages.rb'
require File.join(File.dirname(__FILE__), 'lib/build/required_time.rb')

# RequiredTime.hook()

$task_execution_time = false

require 'find'
require 'erb'

#require 'rdoc/task'
require 'base64'
require 'digest/sha2'
require 'digest/md5'
require 'io/console'
require 'json'
require 'net/https'
require 'open-uri'
require 'openssl'
require 'pathname'
require 'rexml/document'
require 'securerandom'
require 'uri'
require 'logger'
require 'rake'

# It does not work on Mac OS X. rake -T prints nothing. So I comment this hack out.
# NB: server build scripts depend on proper rake -T functioning.
=begin
#Look, another big fat hack. Make it so we can remove tasks from rake -T by setting comment to nil
module Rake
  class Task
    attr_accessor :comment
  end
end
=end

# Restore process error mode on Windows.
# Error mode controls wether system error message boxes will be shown to user.
# Java disables message boxes and we enable them back.
#if RUBY_PLATFORM =~ /(win|w)32$/
#  require 'win32/process'
#  class WindowsError
#    include Windows::Error
#  end
#  WindowsError.new.SetErrorMode(0)
#end

#------------------------------------------------------------------------

$app_basedir = pwd
$is_webkit_engine = false
$startdir = File.dirname(__FILE__)
$startdir.gsub!('\\', '/')

chdir File.dirname(__FILE__), :verbose => (Rake.application.options.trace == true)


require File.join(pwd, 'lib/build/jake.rb')
require File.join(pwd, 'lib/build/GeneratorTimeChecker.rb')
require File.join(pwd, 'lib/build/GeneralTimeChecker.rb')
require File.join(pwd, 'lib/build/CheckSumCalculator.rb')
require File.join(pwd, 'lib/build/SiteChecker.rb')
require File.join(pwd, 'lib/build/ExtendedString.rb')
require File.join(pwd, 'lib/build/rhohub.rb')
require File.join(pwd, 'lib/build/BuildOutput.rb')
require File.join(pwd, 'lib/build/BuildConfig.rb')
require File.join(pwd, 'lib/build/RhoHubAccount.rb')

require File.join(pwd, 'lib/build/rhoDevelopment.rb')



$timestamp_start_milliseconds = 0


module Rake
  class Application
    attr_accessor :current_task
  end
  class Task
    alias :old_execute :execute 
    def execute(args=nil)
      Rake.application.current_task = @name  
      old_execute(args)
    end
  end #class Task
end #module Rake

class Logger
  alias :original_add :add

  def add(severity, message = nil, progname = nil)
    if (self.level == Logger::DEBUG)
      begin
        #try to get rake task name
        taskName = Rake.application.current_task
      rescue Exception => e
      end
      
      if message
        message = "#{taskName}|\t#{message}"
      else
        progname = "#{taskName}|\t#{progname}"
      end
    end
    original_add( severity, message, progname )
  end
end

def puts( s )
  if $logger
    $logger.info( s )
  else
    Kernel::puts( s )
  end
end

$logger = Logger.new(STDOUT)
if Rake.application.options.trace
  $logger.level = Logger::DEBUG
else
  $logger.level = Logger::INFO
end


Rake::FileUtilsExt.verbose(Rake.application.options.trace == true)


$logger.formatter = proc do |severity,datetime,progname,msg|
  "[#{severity}]\t#{msg}\n"
end

Jake.set_logger( $logger )


def print_timestamp(msg = 'just for info')  
  if $timestamp_start_milliseconds == 0
    $timestamp_start_milliseconds = (Time.now.to_f*1000.0).to_i
  end
  curmillis = (Time.now.to_f*1000.0).to_i - $timestamp_start_milliseconds

  $logger.debug '-$TIME$- message [ '+msg+' ] time is { '+Time.now.utc.iso8601+' } milliseconds from start ('+curmillis.to_s+')'
end




load File.join(pwd, 'lib/commonAPI/printing_zebra/ext/platform/wm/PrintingService/PrintingService/installer/Rakefile')
#load File.join(pwd, 'platform/bb/build/bb.rake')
load File.join(pwd, 'platform/android/build/android.rake')
load File.join(pwd, 'platform/iphone/rbuild/iphone.rake')
load File.join(pwd, 'platform/wm/build/wm.rake')
load File.join(pwd, 'platform/linux/tasks/linux.rake')
load File.join(pwd, 'platform/wp8/build/wp.rake')
load File.join(pwd, 'platform/uwp/build/uwp.rake')
load File.join(pwd, 'platform/osx/build/osx.rake')


#------------------------------------------------------------------------

def get_dir_hash(dir, init = nil)
  hash = init
  hash = Digest::SHA2.new if hash.nil?
  Dir.glob(dir + "/**/*").each do |f|
    hash << f
    hash.file(f) if File.file? f
  end
  hash
end

#------------------------------------------------------------------------

namespace "framework" do
  task :spec do
    loadpath = $LOAD_PATH.inject("") { |load_path,pe| load_path += " -I" + pe }

    rhoruby = ""

    if RUBY_PLATFORM =~ /(win|w)32$/
      rhoruby = 'res\\build-tools\\RhoRuby'
    elsif RUBY_PLATFORM =~ /darwin/
      rhoruby = 'res/build-tools/RubyMac'
    else
      rhoruby = 'res/build-tools/rubylinux'
    end

    puts `#{rhoruby}  -I#{File.expand_path('spec/framework_spec/app/')} -I#{File.expand_path('lib/framework')} -I#{File.expand_path('lib/test')} -Clib/test framework_test.rb`
  end
end

$application_build_configs_keys = ['encrypt_files_key', 'nodejs_application', 'security_token', 'encrypt_database', 'android_title', 'iphone_db_in_approot', 'iphone_set_approot', 'iphone_userpath_in_approot', "iphone_use_new_ios7_status_bar_style", "iphone_full_screen", "webkit_outprocess", "webengine", "iphone_enable_startup_logging"]

$winxpe_build = false

def make_application_build_config_header_file
  f = StringIO.new("", "w+")
  f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
  #f.puts "// Generated #{Time.now.to_s}"
  f.puts ""
  f.puts "#include <string.h>"
  f.puts "#include <common/RhoConf.h>"
  f.puts ""
  f.puts '//#include "app_build_configs.h"'
  if $rhosimulator_build
    f.puts '#include "common/RhoSimConf.h"'
  end
  f.puts ""

  f.puts 'static const char* keys[] = { ""'
  $application_build_configs.keys.each do |key|
    f.puts ',"'+key+'"'
  end
  f.puts '};'
  f.puts ''

  count = 1

  f.puts 'static const char* values[] = { ""'
  $application_build_configs.keys.each do |key|
    value = $application_build_configs[key].to_s().gsub('\\', "\\\\\\")
    value = value.gsub('"', "\\\"")
    f.puts ',"'+ value +'"'
    count = count + 1
  end
  f.puts '};'
  f.puts ''

  f.puts '#define APP_BUILD_CONFIG_COUNT '+count.to_s
  f.puts ''
  f.puts 'const char* get_app_build_config_item(const char* key) {'
  f.puts '  int i;'
  f.puts '  const char* szValue;'
  if $rhosimulator_build
    f.puts '  if (strcmp(key, "security_token") == 0) {'
    f.puts '    return rho_simconf_getString("security_token");'
    f.puts '  }'
  end
  f.puts ""
  f.puts '  szValue = rho_conf_getString(key);'
  f.puts '  if (strcmp(szValue, "") != 0)'
  f.puts '    return szValue;'
  f.puts ""
  f.puts '  for (i = 1; i < APP_BUILD_CONFIG_COUNT; i++) {'
  f.puts '    if (strcmp(key, keys[i]) == 0) {'
  f.puts '      return values[i];'
  f.puts '    }'
  f.puts '  }'
  f.puts '  return 0;'
  f.puts '}'
  f.puts ''

  Jake.modify_file_if_content_changed(File.join($startdir, "platform", "shared", "common", "app_build_configs.c"), f)
end

def make_application_build_capabilities_header_file
  $logger.debug "%%% Prepare capability header file %%%"

  f = StringIO.new("", "w+")
  f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
  #f.puts "// Generated #{Time.now.to_s}"
  f.puts ""

  caps = []

  capabilities = $app_config["capabilities"]

  if capabilities != nil && capabilities.is_a?(Array)
    capabilities.each do |cap|
      caps << cap
    end
  end

  caps.sort.each do |cap|
    f.puts '#define APP_BUILD_CAPABILITY_'+cap.upcase
  end

  f.puts ''

  if $js_application || $nodejs_application
    puts '#define RHO_NO_RUBY' if USE_TRACES
    f.puts '#define RHO_NO_RUBY'
    f.puts '#define RHO_NO_RUBY_API'
  else
    $logger.debug '//#define RHO_NO_RUBY'
  end

  Jake.modify_file_if_content_changed(File.join($startdir, "platform", "shared", "common", "app_build_capabilities.h"), f)
end

def make_application_build_config_java_file

  f = StringIO.new("", "w+")
  f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
  #f.puts "// Generated #{Time.now.to_s}"

  f.puts "package com.rho;"
  f.puts ""
  f.puts "public class AppBuildConfig {"

  f.puts 'static final String keys[] = { ""'
  $application_build_configs.keys.each do |key|
    f.puts ',"'+key+'"'
  end
  f.puts '};'
  f.puts ''

  count = 1

  f.puts 'static final String values[] = { ""'
  $application_build_configs.keys.each do |key|
    f.puts ',"'+$application_build_configs[key]+'"'
    count = count + 1
  end
  f.puts '};'
  f.puts ''

  f.puts 'static final int APP_BUILD_CONFIG_COUNT = '+count.to_s + ';'
  f.puts ''
  f.puts 'public static String getItem(String key){'
  f.puts '  for (int i = 1; i < APP_BUILD_CONFIG_COUNT; i++) {'
  f.puts '    if ( key.compareTo( keys[i]) == 0) {'
  f.puts '      return values[i];'
  f.puts '    }'
  f.puts '  }'
  f.puts '  return null;'
  f.puts '}'
  f.puts "}"

  Jake.modify_file_if_content_changed( File.join( $startdir, "platform/bb/RubyVM/src/com/rho/AppBuildConfig.java" ), f )
end

def update_rhoprofiler_java_file
  use_profiler = $app_config['profiler'] || ($app_config[$current_platform] && $app_config[$current_platform]['profiler'])
  use_profiler = use_profiler && use_profiler.to_i() != 0 ? true : false

  content = ""
  File.open( File.join( $startdir, "platform/bb/RubyVM/src/com/rho/RhoProfiler.java" ), 'rb' ){ |f| content = f.read() }
  is_find = nil

  if use_profiler
    is_find = content.sub!( 'RHO_STRIP_PROFILER = true;', 'RHO_STRIP_PROFILER = false;' )
  else
    is_find = content.sub!( 'RHO_STRIP_PROFILER = false;', 'RHO_STRIP_PROFILER = true;' )
  end

  if is_find
    puts "RhoProfiler.java has been modified: RhoProfiler is " + (use_profiler ? "enabled!" : "disabled!")
    File.open( File.join( $startdir, "platform/bb/RubyVM/src/com/rho/RhoProfiler.java" ), 'wb' ){ |f| f.write(content) }
  end
end

def update_rhodefs_header_file
  use_profiler = $app_config['profiler'] || ($app_config[$current_platform] && $app_config[$current_platform]['profiler'])
  use_profiler = use_profiler && use_profiler.to_i() != 0 ? true : false

  content = ""
  File.open( File.join( $startdir, "platform/shared/common/RhoDefs.h" ), 'rb' ){ |f| content = f.read() }
  is_find = nil

  if use_profiler
    is_find = content.sub!( '#define RHO_STRIP_PROFILER 1', '#define RHO_STRIP_PROFILER 0' )
  else
    is_find = content.sub!( '#define RHO_STRIP_PROFILER 0', '#define RHO_STRIP_PROFILER 1' )
  end

  if is_find
    puts "RhoDefs.h has been modified: RhoProfiler is " + (use_profiler ? "enabled!" : "disabled!")
    File.open( File.join( $startdir, "platform/shared/common/RhoDefs.h" ), 'wb' ){ |f| f.write(content) }
  end
end


namespace :dev do

  namespace :update do

    desc 'This command initializes original state files. It needs for correct execution of command partial update from CLI. The first partial update cannot find out source code changes if initialize didn\'t execute before it'
    task :initialize => ['config:common'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      mkdir_p RhoDevelopment::Configuration::development_directory
      updated_list_filename = File.join(RhoDevelopment::Configuration::application_root, 'upgrade_package_add_files.txt')
      removed_list_filename = File.join(RhoDevelopment::Configuration::application_root, 'upgrade_package_remove_files.txt')
      #WindowsMobile
      RhoDevelopment.setup(RhoDevelopment::Configuration::development_directory, 'wm')
      RhoDevelopment::check_changes_from_last_build(updated_list_filename, removed_list_filename)
      #iPhone
      RhoDevelopment.setup(RhoDevelopment::Configuration::development_directory, 'iphone')
      RhoDevelopment::check_changes_from_last_build(updated_list_filename, removed_list_filename)
      #Android
      RhoDevelopment.setup(RhoDevelopment::Configuration::development_directory, 'android')
      RhoDevelopment::check_changes_from_last_build(updated_list_filename, removed_list_filename)
    end

    desc 'If source code was changed - builds partial update for all platforms and notifies all subscribers'
    task :partial => ['config:common'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      unless RhoDevelopment::Configuration::has_enabled_subscribers?
        puts 'Enabled subscribers not found'.warning
        exit 1
      end
      RhoDevelopment::WebServer.ensure_running
      filename = RhoDevelopment::Configuration::next_filename_for_downloading()
      RhoDevelopment::WebServer.dispatch_task(RhoDevelopment::PartialUpdateTask.new(filename));
    end

    desc 'Builds full update bundle for all platforms and notifies all subscribers'
    task :full => ['config:common'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      unless RhoDevelopment::Configuration::has_enabled_subscribers?
        puts 'Enabled subscribers not found'.warning
        exit 1
      end
      RhoDevelopment::WebServer.ensure_running
      filename = RhoDevelopment::Configuration::next_filename_for_downloading()
      RhoDevelopment::WebServer::dispatch_task(RhoDevelopment::AllPlatformsFullBundleBuildingTask.new(filename))
      RhoDevelopment::WebServer::dispatch_task(RhoDevelopment::AllSubscribersFullUpdateNotifyingTask.new(filename))
    end

    desc 'It builds update with files from diff file list for all platforms and notifies all subscribers'
    task :build_and_notify => ['config:common'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      unless RhoDevelopment::Configuration::has_enabled_subscribers?
        puts 'Enabled subscribers not found'.warning
        exit 1
      end
      RhoDevelopment::WebServer.ensure_running
      filename = RhoDevelopment::Configuration::next_filename_for_downloading()
      RhoDevelopment::WebServer::dispatch_task(RhoDevelopment::AllPlatformsPartialBundleBuildingTask.new(filename))
      RhoDevelopment::WebServer::dispatch_task(RhoDevelopment::AllSubscribersPartialUpdateNotifyingTask.new(filename))
    end

    desc 'It launches watcher for source code and builds partial update and notifies all subscribers on each change'
    task :auto => ['config:common'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      RhoDevelopment::WebServer.ensure_running
      pid = RhoDevelopment::WebServer::get_auto_update_pid
      if pid
        puts 'Another auto updating process is already launched'.warning
        exit 1
      end
      updater = RhoDevelopment::AutoUpdater.new
      updater.add_directory(File.join($app_basedir, '/public'))
      updater.add_directory(File.join($app_basedir, '/app'))
      updater.add_directory(File.join($app_basedir, '/nodejs')) if File.exists? File.join($app_basedir, '/nodejs')
      updater.run
    end

    namespace 'auto' do
      desc 'It stops auto update process'
      task :stop => ['config:common'] do
        RhoDevelopment::Configuration::application_root = $app_basedir
        pid = RhoDevelopment::WebServer::get_auto_update_pid
        if pid
          RhoDevelopment::Platform::terminate_process(pid)
          RhoDevelopment::WebServer::set_auto_update_pid(0)
        else
          puts 'Auto updating is not launched'.warning
          exit 1
        end

      end

    end

  end

  namespace :webserver do

    desc 'It launches development web server. It is certain object which controls executing scheduling tasks, handles requests etc..'
    task :start => ['config:common'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      RhoDevelopment::WebServer.ensure_running
    end

    task :privateStart => ['config:initialize'] do
      RhoDevelopment::Configuration::application_root = $app_basedir
      server = RhoDevelopment::WebServer.new
      server.start
    end

    desc 'It shut down development web server'
    task :stop do
      RhoDevelopment::Configuration::application_root = $app_basedir
      RhoDevelopment::WebServer::stop
    end

  end

  namespace :network do

    desc 'Discover application on devices in local network - application should be executed on devices'
    task :discovery, [:mask] => ['config:initialize'] do |t, args|

      RhoDevelopment::Configuration::application_root = $app_basedir
      finder = RhoDevelopment::DeviceFinder.new

      if args[:mask] == nil
        finder.run
      else
        finder.discovery((args[:mask]).split('.')[0, 3].join('.'))
      end
    end

    desc 'Return string with available networks masks separated by semicolon. It needs for RhoStudio'
    task :list do
      addresses = RhoDevelopment::Network::available_addresses

      if (addresses.empty?)
        puts 'Network interfaces were not found.'.warning
        exit 1
      else
        addresses.each {
            |each|
          _mask = each.split('.')[0, 3].join('.')
          print "#{_mask}.*"
          print ';' if addresses.last != each
        }
      end
    end
  end

end

#------------------------------------------------------------------------

#TODO:  call clean from all platforms scripts
namespace "clean" do
  task :common => "config:common" do

    if $config["platform"] == "bb"
      return
    end

    rm_rf File.join($app_path, "bin/tmp") if File.exists? File.join($app_path, "bin/tmp")
  end

  task :generated => "config:common" do

    if $config["platform"] == "bb"
      return
    end

    rm_rf File.join($app_path, "bin/tmp") if File.exists? File.join($app_path, "bin/tmp")
    rm_rf File.join($app_path, "bin/RhoBundle") if File.exists? File.join($app_path, "bin/RhoBundle")

    extpaths = $app_config["extpaths"]

    $app_config["extensions"].each do |extname|
      puts 'ext - ' + extname

      extpath = nil
      extpaths.each do |p|
        ep = File.join(p, extname)
        if File.exists?( ep ) && is_ext_supported(ep)
          extpath = ep
          break
        end
      end

      if extpath.nil?
        extpath = find_ext_ingems(extname)
        if extpath
          extpath = nil unless is_ext_supported(extpath)
        end
      end

      if (extpath.nil?) && (extname != 'symbolapi')
        raise "Can't find extension '#{extname}'. Aborting build.\nExtensions search paths are:\n#{extpaths}"
      end

      unless extpath.nil?
        extyml = File.join(extpath, "ext.yml")
        #puts "extyml " + extyml

        if File.file? extyml
          extconf = Jake.config(File.open(extyml))
          type = Jake.getBuildProp( "exttype", extconf )
          #wm_type = extconf["wm"]["exttype"] if extconf["wm"]

          if type != "prebuilt" #&& wm_type != "prebuilt"
            rm_rf  File.join(extpath, "ext", "shared", "generated")
            rm_rf  File.join(extpath, "ext", "platform", "android", "generated")
            rm_rf  File.join(extpath, "ext", "platform", "iphone", "generated")
            rm_rf  File.join(extpath, "ext", "platform", "osx", "generated")
            rm_rf  File.join(extpath, "ext", "platform", "wm", "generated")
            rm_rf  File.join(extpath, "ext", "platform", "wp8", "generated")
            rm_rf  File.join(extpath, "ext", "platform", "uwp", "generated")
            rm_rf  File.join(extpath, "ext", "public", "api", "generated")
          end
        end
      end
    end
  end
end

#------------------------------------------------------------------------
def get_conf(key_path, default = nil)
  result = nil

  key_sections = key_path.split('/').reject { |c| c.empty? }

  [$app_config, $config, $shared_conf].each do |config|
    if !config.nil?
      curr = config
      key_sections.each_with_index do |section, i|
        if !curr[section].nil?
          curr = curr[section]
        else
          break
        end
        if (i == key_sections.length-1) && !curr.nil?
          result = curr
        end
      end
      break if !result.nil?
    end
  end

  result = nil if result.kind_of?(String) && result.strip.empty?

  result.nil? ? default : result
end

#------------------------------------------------------------------------
#token handling

def get_app_list()
  result = JSON.parse(Rhohub::App.list())
end

def from_boolean(v)
  v == true ? "YES" : "NO"
end

def time_to_str(time)
  d_h_m_s = [60,60,24].reduce([time]) { |m,o| m.unshift(m.shift.divmod(o)).flatten }
  best = []
  ["day","hour","minute","second"].each_with_index do |v, i|
    if d_h_m_s[i] > 0
      best << d_h_m_s[i].to_s + " " + v + ((d_h_m_s[i] > 1) ? "s" : "")
    end
  end
  best.empty? ? "now" : best.first(2).join(" ")
end

def sort_by_distance(array, template)
  template.nil? ? array : array.sort_by { |s| distance(template, s) }
end

def rhohub_make_request(srv)
  if block_given?
    build_was_proxy_problem = false

    begin
      yield

    rescue Timeout::Error, Errno::ETIMEDOUT, Errno::EINVAL, Errno::ECONNRESET,
        Errno::ECONNREFUSED, SocketError => e
      unless RestClient.proxy.nil? || RestClient.proxy.empty?
        BuildOutput.put_log(BuildOutput::WARNING,'Could not connect using proxy server, retrying without proxy','Connection problem')
        RestClient.proxy = ''
        build_was_proxy_problem = true
        retry
      else
        if build_was_proxy_problem
          BuildOutput.put_log(BuildOutput::WARNING,"Could not connect to server #{get_server(srv,'')}\n#{e.inspect}",'Network problem')
        else
          BuildOutput.put_log(BuildOutput::WARNING,"Could not connect to server #{get_server(srv,'')}. If you are behind proxy please set http(s)_proxy ENV variable",'Network problem')
        end
        exit 1
      end

    rescue EOFError, Net::HTTPBadResponse, Net::HTTPHeaderSyntaxError, Net::ProtocolError => e
      puts "Http request problem: #{e.inspect}"

    rescue RestClient::RequestFailed => e
      puts "Http request problem: #{e.message}"

    rescue RestClient::ExceptionWithResponse => e
      # do nothing, this is is 404 or something like that
    end

    if RestClient.proxy != $proxy
      $proxy = RestClient.proxy
    end
  end
end


def check_update_token_file(server_list, user_acc, token_folder, subscription_level = -1)
  is_valid = -2

  if user_acc.is_valid_token?()
    Rhohub.token = user_acc.token

    is_valid = user_acc.is_outdated() ? 0 : 2

    if (user_acc.is_outdated() || (subscription_level > user_acc.subscription_level))
      servers_sorted = sort_by_distance(server_list, user_acc.server)

      servers_sorted.each do |srv|
        Rhohub.url = srv

        if (subscription_level > user_acc.subscription_level)
          puts "Connecting to #{get_server(srv,'')}"

          rhohub_make_request(srv) do
            subscription = Rhohub::Subscription.check()
            user_acc.subscription = subscription
          end

          if user_acc.subscription_level >= subscription_level
            user_acc.server = srv
            break
          end
        end
      end

      is_valid = user_acc.subscription_level >= 0 ? 2 : 0

      if is_valid == 0
        servers_sorted.each do |srv|
          Rhohub.url = srv

          user_apps = nil
          begin
            user_apps = get_app_list()
          rescue Exception => e
            user_apps = nil
          end

          if user_apps.nil?
            user_acc.token = nil
            is_valid = -1
          else
            is_valid = 1
          end

          if is_valid > 0
            user_acc.server = srv
            break
          end
        end
      end
    end

    Rhohub.url = user_acc.server if is_valid > 0

    if (user_acc.is_valid_token?() && user_acc.changed)
      user_acc.save_token(token_folder)
    end
  else
    is_valid = -2
  end

  is_valid
end

def read_and_delete_files( file_list )
  result = []
  if file_list.kind_of?(String)
    file_list = [file_list]
  end

  if file_list.kind_of?(Array)
    file_list.each do |read_file|
      f_size = File.size?(read_file)
      if !f_size.nil? && f_size < 1024
        begin
          result << File.read(read_file)
          File.delete(read_file)
        rescue Exception => e
          puts "Reading file exception #{e.inspect}"
        end
      end
    end
  end

  result
end

$server_list = ['https://rms.rhomobile.com/api/v1']
$selected_server = $server_list.first

def get_server(url, default)
  url = default if url.nil? || url.empty?
  scheme, user_info, host, port, registry, path, opaque, query, fragment =  URI.split(url)
  case scheme
    when "http"
      URI::HTTP.build({:host => host, :port => port}).to_s
    when "https"
      URI::HTTPS.build({:host => host, :port => port}).to_s
    else
      ""
  end

end

def distance(a, b, case_insensitive = false)
  as = a.to_s
  bs = b.to_s

  if case_insensitive
    as = as.downcase
    bs = bs.downcase
  end

  rows = as.size + 1
  cols = bs.size + 1

  dist = [ Array.new(cols) {|k| k}, Array.new(cols) {0}, Array.new(cols) {0} ]

  (1...rows).each do |i|
    k = i % 3
    dist[k][0] = i

    (1...cols).each do |j|
      cost = as[i - 1] == bs[j - 1] ? 0 : 1

      d1 = dist[k - 1][j] + 1
      d2 = dist[k][j - 1] + 1
      d3 = dist[k - 1][j - 1] + cost

      d_now = [d1, d2, d3].min

      if i > 1 && j > 1 && as[i - 1] == bs[j - 2] && as[i - 2] == bs[j - 1]
        d1 = dist[k - 2][j - 2] + cost
        d_now = [d_now, d1].min;
      end

      dist[k][j] = d_now;
    end
  end
  dist[(rows - 1) % 3][-1]
end

#------------------------------------------------------------------------
def to_boolean(s)
  if s.kind_of?(String)
    !!(s =~ /^(true|t|yes|y|1)$/i)
  elsif s.kind_of?(TrueClass)
    true
  else
    false
  end
end

def cloud_url_git_match(str)
  server = nil
  user = ''
  app = nil
  #TODO: remove this to support any valid git repo url
  res = /git@(git.*?\.(?:rhomobile|rhohub|github)\.com):(.*?)\/(.*?).git/i.match(str)
  unless res.nil?
  #   res = /(git@|http\:\/\/|https\:\/\/)(.*?)\/(.*?).git/i.match(str)
  #   unless res.nil?
  #     server = res[2]
  #     proj_path = res[3]
  #     purl = proj_path.split('\\').compact
  #     if purl.length == 2
  #       user, app = purl[0], purl[1]
  #     else
  #       user, app = '', proj_path
  #     end
  #   end
  # else
    server = res[1]; user = res[2]; app = res[3]
  end

  (server.nil? || app.nil?) ? {} : { :str => "#{server}:#{user}/#{app}", :server => server,  :user => user, :app => app }
end

def split_number_in_groups(number)
  number.to_s.gsub(/(\d)(?=(\d\d\d)+(?!\d))/, "\\1'")
end

MAX_BUFFER_SIZE = 1024*1024

def fill_with_zeroes(file, size)
  buffer = "\0" * MAX_BUFFER_SIZE
  to_write = [size, 0].max
  while to_write > MAX_BUFFER_SIZE
    file.write(buffer)
    to_write -= buffer.length
  end
  if to_write > 0
    buffer = "\0" * to_write
    file.write(buffer)
  end
  file.flush
  file.seek(0)
end

def http_get(url, proxy, save_to)
  uri = URI.parse(url)

  if !(proxy.nil? || proxy.empty?)
    proxy_uri = URI.parse(proxy)
    http = Net::HTTP.new(uri.host, uri.port, proxy_uri.host, proxy_uri.port, proxy_uri.user, proxy_uri.password )
  else
    http = Net::HTTP.new(uri.host, uri.port)
  end

  server_file_name = uri.path[%r{[^/]+\z}]

  f_name = File.join(save_to, server_file_name)

  if uri.scheme == "https"  # enable SSL/TLS
    http.use_ssl = true
    http.verify_mode = OpenSSL::SSL::VERIFY_NONE
  end

  header_resp = nil

  http.start {
    header_resp = http.head(uri.path)
  }

  if !header_resp.kind_of?(Net::HTTPSuccess)
    if block_given?
      yield(header_resp.content_length, -1, "Server error #{header_resp.inspect}")
    end
    return false, "Server error: #{header_resp.inspect}"
  end

  if File.exists?(f_name)
    if File.stat(f_name).size == header_resp.content_length
      if block_given?
        yield(header_resp.content_length, header_resp.content_length, "File #{f_name} from #{url} is already in the cache")
      end

      return true, f_name
    end
  end

  size_delimited = split_number_in_groups(header_resp.content_length)

  if block_given?
    yield(0, header_resp.content_length, "Downloading #{size_delimited} bytes")
  end

  if save_to.nil?
    res = ""
    http.start {
      res = http.get(uri.path)
    }
    result = res.body
  else
    temp_name = File.join(save_to,File.basename(server_file_name,'.*')+'.tmp')
    f = File.open(temp_name, "wb")

    fill_with_zeroes(f, header_resp.content_length)
    done = 0
    begin
      result = false

      buffer = []
      buffer_size = 0

      http.request_get(uri.path) do |resp|
        last_p = 0
        length = resp['Content-Length'].to_i
        length = length > 1 ? length : 1

        resp.read_body do |segment|
          chunk_size = segment.length

          if buffer_size + chunk_size > MAX_BUFFER_SIZE
            f.write(buffer.join(''))
            buffer = [segment]
            buffer_size = chunk_size
          else
            buffer << segment
            buffer_size += chunk_size
          end

          if  block_given?
            done += chunk_size
            dot = (done * 100 / length).to_i
            if dot > 100
              dot = 100
            end
            if last_p < dot
              last_p = dot
              yield(done, header_resp.content_length, "Downloaded #{last_p}% from #{size_delimited} bytes")
            end
          end
        end
        unless buffer.empty?
          f.write(buffer.join(''))
          f.flush
        end

      end
      result = f_name
    ensure
      f.close()
    end
    FileUtils.mv(temp_name, f_name)
    yield(done, header_resp.content_length, "Download finished") if block_given?
  end

  return true, result
end

def get_build_platforms()
  build_caps = JSON.parse(Rhohub::Build.platforms())

  build_platforms = {}

  build_caps.each do |bc|
    bc.each do |k, v|
      res = /(.+?)-(.*)/.match(k)
      if !res.nil?
        platform = res[1]
        version = res[2]
      else
        platform = k
        version = ""
      end
      v.gsub!("iphone-","iphone:")
      id = {:ver => version, :tag => v}
      if build_platforms[platform].nil?
        build_platforms[platform] = [id]
      else
        build_platforms[platform] << id
      end
    end
  end

  build_platforms
end

def find_platform_by_command(platforms, command)
  result = "not found"

  command = "iphone:development" if command == "iphone:ad_hoc"
  command = "iphone:distribution" if command == "iphone:app_store"
  platforms.each do |platform, content|
    content.each do |el|
      if el[:tag] == command
        result = "#{platform} #{el[:ver]}".strip()
        break
      end
    end

  end

  result
end


def show_build_information(build_hash, platforms, opts = {})
  build_states = {
      "queued" => "queued".cyan,
      "started" => "started".blue,
      "completed" => "completed".green,
      "failed" => "failed".red
  }

  label = build_states[ build_hash["status"] ]

  message = ""
  target = ""
  if build_hash["target_device"]
    target = ", target platform: " + find_platform_by_command(platforms, build_hash["target_device"]).blue
  end
  if build_hash["rhodes_version"] && build_hash["version"]
    message = "Rhodes version: #{build_hash["rhodes_version"].cyan}, app version: #{build_hash["version"].cyan}"
  end

  puts "Build ##{build_hash["id"]}: #{label}#{target}"
  puts "  #{message}" unless message.nil? || message.empty? || opts[:hide_ver]

  dl = build_hash["download_link"]

  if !(dl.nil? || dl.empty?) && !(opts[:hide_link])
    puts "  Download link : #{dl.underline}"
  end
end

def show_build_messages(build_hash, proxy, save_to)
  if build_hash["status"] == "failed"
    if !(build_hash["download_link"].nil? || build_hash["download_link"].empty?)
      is_ok, error_file = http_get(build_hash["download_link"], proxy, save_to)

      if is_ok
        error = File.read(error_file)
        BuildOutput.put_log(BuildOutput::ERROR, error, "Build log")
      else
        BuildOutput.put_log(BuildOutput::ERROR, error_file, "Server error")
      end
    end
  end
end



def best_match(target, list, is_lex = false)
  best = list.first

  if !(target.nil?)
    if !is_lex
      sorted = list.sort{|a, b| String.natcmp(b, a)}
      best = sorted.first
      sorted.each do |item|
        if String.natcmp(target, item) <= 0
          best = item
        else
          break
        end
      end
    else
      best = list.min_by{ |el| distance(el, target, true) }
    end
  end

  best
end

def find_platform_version(platform, platform_list, default_ver, info, is_lex = false)
  platform_conf = platform_list[platform]
  if platform_conf.empty?
    raise Exception.new("Could not find any #{platform} sdk on cloud build server")
  end

  req_ver = nil

  req_ver = $app_config[platform]["version"] unless $app_config[platform].nil?
  req_ver = $config[platform]["version"] if req_ver.nil? and !$config[platform].nil?

  req_ver = default_ver if req_ver.nil?

  best = platform_conf.first

  if !(req_ver.nil? || req_ver.empty?)
    if !is_lex
      platform_conf.sort { |a, b| String.natcmp(b[:ver], a[:ver]) }.each do |ver|
        if String.natcmp(req_ver, ver[:ver]) < 0
          best = ver
        else
          break
        end
      end
    else
      best = platform_conf.min_by { |el| distance(el[:ver], req_ver, true) }
    end

    if info
      if best[:ver] != req_ver
        puts "WARNING! Could not find exact version of #{platform} sdk. Using #{best[:ver]} instead of #{req_ver}"
      else
        puts "Using requested #{platform} sdk version #{req_ver}"
      end
    end
  else
    if info
      puts "No #{platform} sdk version was specified, using #{best[:ver]}"
    end
  end
end


def check_cloud_build_result(result)
  if !(result["text"].nil?)
    e_msg = "Error running build: #{result["text"]}"
    BuildOutput.error(e_msg)
    raise Exception.new(e_msg)
  end
end

TIME_FORMAT = '%02d:%02d:%02d.%02d'

$start_time = Time.now

def put_message_with_timestamp( message, no_newline = false)
  seconds = ((Time.now - $start_time)*100).floor

  cleaner = ' ' * 80

  seconds, msecs = seconds.divmod(100)
  minutes, seconds = seconds.divmod(60)
  hours, minutes = minutes.divmod(3600)

  time = sprintf TIME_FORMAT, hours, minutes, seconds, msecs

  data = "[#{time}] #{message}#{cleaner}"
  if no_newline
    print " #{data}\r"
  else
    puts " #{data}"
  end

  STDOUT.flush
end

def valid_build_id(id)
  if id.kind_of?(NilClass)
    true
  elsif id.kind_of?(Integer)
    true
  elsif id.kind_of?(String)
    Integer(id) != nil rescue false
  else
    false
  end
end

def wait_and_get_build(app_id, build_id, proxy, save_to = nil, unzip_to = nil)
  puts("Application build progress: \n")

  app_request = {:app_id => app_id, :id => build_id}

  begin
    result = JSON.parse(Rhohub::Build.show(app_request))

    status = result["status"]

    case status
    when "queued"
      desc = "Build is queued. Please wait"
      sleep(2)
    when "started"
      desc = "Build is started. Please wait"
      sleep(1)
    when "completed"
      desc = "Build is ready to be downloaded."
    when "failed"
      desc = "Build failed."
    end

    build_complete = %w[completed failed].include?(status)

    put_message_with_timestamp("Current status: #{desc}", true)

  end while !build_complete

  puts

  result_link = result["download_link"]

  if !(save_to.nil? || save_to.empty?)
    is_ok, result_link = http_get(result["download_link"], proxy, save_to) do |current, total, msg|
      put_message_with_timestamp("Current status: #{msg}", true)
    end

    puts

    if is_ok
      if !(unzip_to.nil? || unzip_to.empty?)
        if (status == "completed")
          Jake.unzip(result_link, unzip_to) do |a,b,msg|
            put_message_with_timestamp("Current status: #{msg}", true)
          end

          puts
        end
      end
    else
      put_message_with_timestamp("Server error: #{result_link}", true)
    end
  end

  return (status == "completed"), result_link
end


def start_cloud_build(app_id, build_params)
  result = JSON.parse(Rhohub::Build.create({:app_id => app_id}, build_params))

  if !(result['status'].nil? && result['id'].nil?)
    build_id = result['id'].to_i

    if (!build_id.nil?)
      result = JSON.parse(Rhohub::Build.show({:app_id => $app_cloud_id, :id => build_id}))
    end
  else
    build_id = -1
  end

  return build_id, result
end

def get_builds_list(app_id)
  result = []
  begin
    result = JSON.parse(Rhohub::Build.list({:app_id => app_id})).sort{ |a, b| a['id'].to_i <=> b['id'].to_i}
  rescue Exception => e
    puts "Got exception #{e.inspect}"
  end

  result
end

MATCH_INVALID_ID = -1
MATCH_ANY_ID = 0
MATCH_EXACT_ID = 1
MATCH_WILDCARD_ID = 2
MATCH_LATEST_ID = 3
MATCH_HISTORY_ID = 4

RESULT_ANY = 0
RESULT_EXACT = 1
RESULT_NO_ANY = -1
RESULT_NO_STATE = -2
RESULT_NO_IDX = -3
RESULT_INVALID_ID = -4
RESULT_STATE_MISMATCH = -5

def find_build_id(build_id, builds, state_filter = nil)
  match_type = MATCH_INVALID_ID
  result = RESULT_INVALID_ID
  match = []
  match_id = nil
  regex = nil

  unless build_id.nil? || build_id.empty?
    if build_id.kind_of?(Integer) || !!(build_id =~ /^[-+]?[0-9]+$/)
      match_id = build_id.to_i
      if match_id <= 0
        match_type = (match_id == 0) ? MATCH_LATEST_ID : MATCH_HISTORY_ID
      else
        match_type = MATCH_EXACT_ID
      end
    elsif !!(build_id =~ /^[0-9\*\?]+$/)
      regex = '^' + Regexp.escape(build_id).gsub("\\*", '.*?').gsub("\\?", '.') + '$'
      match_type = MATCH_WILDCARD_ID
    end
  else
    match_type = MATCH_ANY_ID
  end

  if match_type != MATCH_INVALID_ID
    unless builds.empty?
      is_filtered = !state_filter.nil?

      filtered = is_filtered ? builds.select{ |el| state_filter.index(el['status']) != nil } : builds

      unless filtered.empty?
        case match_type
          when MATCH_ANY_ID
            match = filtered
            result = RESULT_ANY

          when MATCH_EXACT_ID, MATCH_WILDCARD_ID
            if match_type == MATCH_EXACT_ID
              found = filtered.select {|f| f['id'].to_s == match_id.to_s }
            else
              found = filtered.select {|f| !!f['id'].to_s.match(regex) }
            end

            if found.empty?
              if is_filtered
                if match_type == MATCH_EXACT_ID
                  more_found = builds.select {|f| f['id'] == match_id.to_s }
                else
                  more_found = []
                end

                if more_found.empty?
                  result = RESULT_NO_IDX
                else
                  result = RESULT_STATE_MISMATCH
                  match = more_found
                end
              else
                result = RESULT_NO_IDX
              end

              if result == RESULT_NO_IDX && match_type != MATCH_WILDCARD_ID
                match = filtered.collect{ |h| { :id => h['id'], :build => h, :dist => distance(match_id.to_s, h['id'].to_s) } }.reject{|a| a[:dist] > 1}.map{ |h| h[:build] }
              end
            else
              match = found
              result = found.length > 1 ? RESULT_ANY : RESULT_EXACT
            end

          when MATCH_LATEST_ID, MATCH_HISTORY_ID
            idx = filtered.size + match_id - 1
            if idx >= 0
              result = RESULT_EXACT
              match = [filtered[idx]]
            else
              result = RESULT_NO_IDX
            end
        end
      else
        result = RESULT_NO_STATE
      end
    else
      result = RESULT_NO_ANY
    end
  end

  return result, match_type, match
end


def match_build_id(build_id, builds, wildcards = true)
  if !build_id.nil? && (build_id.kind_of?(Integer) || !build_id.empty?)
    found_id = build_id.to_i

    found = nil

    if found_id <= 0 && wildcards
      if found_id.abs < builds.length
        found = builds[builds.length - (found_id).abs - 1]
      end
    else
      found = builds.find {|f| f['id'] == found_id }
    end

    return [found].compact
  end

  builds
end

def filter_by_status(builds, filter = [])
  result = builds

  unless filter.nil? || filter.empty?
    result = builds.select{ |el| filter.index(el['status']) != nil }
  end

  result
end

$rhodes_ver_default = '3.5.1.14'
$latest_platform = nil

def deploy_build(platform)
  files = Dir.glob(File.join($cloud_build_temp,'**','*')).select {|entry| File.file?(entry) }

  detected_bin = nil
  detected_platform = nil
  log_file = nil

  files.each do |fname|
    case fname
      when /\.ipa/
        detected_bin = fname
        detected_platform = 'iphone'
      when /\.cab/
        detected_bin = fname
        detected_platform = 'wm'
      when /\.apk/
        detected_bin = fname
        detected_platform = 'android'
      when /\.(exe|msi)/
        detected_bin = fname
        detected_platform = 'win32'
      when /log\.txt/i
        log_file = fname
    end
  end

  if detected_bin.nil?
    BuildOutput::error("Could not find executable for platform #{platform}\nYou should check #{$cloud_build_temp}",'Application deployment')
    unless log_file.nil?
      BuildOutput::error("Build log:\n#{File.read(log_file).gsub(/\e\[(\d+)(;\d+)*m/,'')}",'Application deployment')
    end
    fail 'Missing executable'
  elsif !platform.include?(detected_platform)
    BuildOutput::error("Executable platfrom missmatch, build for #{platform} has an executable #{detected_bin} that belongs to #{detected_platform}")
    fail 'Executable platform missmatch'
  end

  dest = File.join($cloud_build_bin, detected_platform)

  if !File.exists?(dest)
    FileUtils.mkpath(dest)
  else
    FileUtils.rm_rf(Dir.glob(File.join(dest,'*')), secure: true)
  end


  FileUtils.mv(detected_bin, dest)

  unless log_file.nil?
    FileUtils.mv(log_file, dest)
  end

  remaining = (files - [detected_bin, log_file])

  detected_bin = File.join(dest, File.basename(detected_bin))

  unless remaining.empty?
    misc = File.join(dest, 'misc')

    if !File.exists?(misc)
      FileUtils.mkpath(misc)
    end

    remaining.each do |src|
      FileUtils.mv(src, misc)
    end
  end

  unpacked_file_list = Dir.glob(File.join(dest,'**','*'))

  unless $cloud_build_temp.empty?
    FileUtils.rm_rf($cloud_build_temp, {:secure => true})
  end

  put_message_with_timestamp("Done, application files deployed to #{dest}")

  return unpacked_file_list, detected_platform, detected_bin
end

def get_build(build_id, show_info = false)
  result = false
  message = 'none'
  platform = 'none'

  builds = get_builds_list($app_cloud_id)

  if !builds.empty?
    if valid_build_id(build_id)
      matching_builds  = match_build_id(build_id, builds)

      if !matching_builds.empty?
        build_hash = matching_builds.last

        if !build_hash.nil?
          build_id = build_hash['id']

          $platform_list = get_build_platforms() unless $platform_list

          show_build_information(build_hash, $platform_list, {:hide_link => true}) if show_info

          platform = find_platform_by_command($platform_list, build_hash["target_device"])

          FileUtils.rm_rf(Dir.glob(File.join($cloud_build_temp,'*')))

          $start_time = Time.now
          successful, file = wait_and_get_build($app_cloud_id, build_id, $proxy, $cloud_build_home, $cloud_build_temp)

          if !file.nil?
            if successful
              result = true
              message = file
            else
              put_message_with_timestamp('Done with build errors')
              BuildOutput.put_log( BuildOutput::ERROR, File.read(file), 'build error')
            end
          else
            message = 'Could not get any result from server'
          end
        end
      else
        str = build_id.to_s
        match = builds.collect{ |h| { :id => h['id'], :dist => distance(str, h['id'].to_s) } }.min_by{|a| a[:dist]}
        if match[:dist] < 3
          message = "Could not find build ##{build_id}, did you mean #{match[:id]}?"
        else
          message = "Could not find build ##{build_id}"
        end
      end
    else
      message = "Invalid build_id: '#{build_id}'. Please provide integer number in range from #{(builds.first)['id']} to #{(builds.last)['id']}"
    end
  else
    message = 'Nothing to download'
  end

  return result, message, platform
end

def find_platform_version(platform, platform_conf, req_ver, info, is_lex = false)
  best = platform_conf.first

  unless req_ver.nil?
    req_ver_display = req_ver.empty? ? 'default' : req_ver
  else
    req_ver_display = 'default'
  end

  unless req_ver.nil?
    if !is_lex
      platform_conf.sort { |a, b| String.natcmp(b[:ver], a[:ver]) }.each do |ver|
        if String.natcmp(req_ver, ver[:ver]) < 0
          best = ver
        else
          break
        end
      end
    else
      best = platform_conf.min_by { |el| distance(el[:ver], req_ver, true) }
    end

    if info
      if best[:ver] != req_ver
        puts "WARNING! Could not find exact version of #{platform} sdk. Using #{best[:ver]} instead of #{req_ver_display}"
      else
        puts "Using requested #{platform} sdk version #{req_ver_display}"
      end
    end
  else
    if info
      if !best[:ver].empty?
        puts "No #{platform} sdk version was specified, using #{best[:ver]}"
      else
        puts "Using requested #{platform} sdk."
      end
    end
  end

  best
end

def try_get_platform_config(platform, platform_list)
  platform_conf = platform_list[platform]
  if platform_conf.empty?
    raise Exception.new("Could not find any #{platform} sdk on cloud build server")
  end
  platform_conf
end

def platform_ver_from_config(platform, default_ver, use_sdk = false)
  req_ver = nil

  unless $app_config[platform].nil?
    req_ver = $app_config[platform]["version"]
    req_ver = $app_config[platform]["sdk"] if req_ver.nil? && use_sdk
  end

  unless $config[platform].nil?
    req_ver = $config[platform]["version"] if req_ver.nil?
    req_ver = $config[platform]["sdk"] if req_ver.nil? && use_sdk
  end

  req_ver = default_ver if req_ver.nil?
  req_ver
end

def do_platform_build(platform_name, platform_list, is_lexicographic_ver, build_info = {}, config_override = nil)

  platform_conf = try_get_platform_config(platform_name, platform_list)
#TODO: WARNING: Ruby 1.9.3 is getting old day by day , please upgrade to ruby 2.3
  req_ver = platform_ver_from_config(platform_name, config_override, platform_name == 'wm').to_s

  if platform_name == 'wm'
    digits = nil

    if req_ver.downcase.include?('windows mobile'.downcase)
      digits = req_ver.match(/(?<![\d\w])(\d+(?:\.\d+(?:\.\d+)*)*)(?![\d\w])/i)
    elsif req_ver.match(/(?<![a-z])wm(?![a-z])/i)
      digits = req_ver.match(/(?<=wm)(\d+(?:\.\d+(?:\.\d+)*)*)(?![\d\w])/i)
    end

    req_ver = digits[0] unless digits.nil?
  end

  platform_version = find_platform_version(platform_name, platform_conf, req_ver, true, is_lexicographic_ver)

  puts "Running cloud build using #{platform_version[:tag]} command"

  build_hash = {
      'target_device' => platform_version[:tag],
      'version_tag' => 'master',
      'rhodes_version' => $rhodes_ver
  }

  build_info.each do |k, v|
    build_hash[k] = v
  end

  build_flags = { :build => build_hash }

  build_id, res = start_cloud_build($app_cloud_id, build_flags)

  if (!build_id.nil?)
    show_build_information(res, platform_list, {:hide_link => true})
  end

  check_cloud_build_result(res)

  build_id
end

def list_missing_files(files_array)
  failed = files_array.select{|file| !File.exists?(file)}

  failed
end

def get_iphone_options()
  profile_file = get_conf('iphone/production/mobileprovision_file')
  cert_file = get_conf('iphone/production/certificate_file')
  cert_pw = get_conf('iphone/production/certificate_password')

  if profile_file.nil? || cert_file.nil?
    raise Exception.new('You should specify mobileprovision_file and certificate_file in iphone:production section of your build.yml')
  end

  profile_file = File.expand_path(profile_file, $app_path)
  cert_file = File.expand_path(cert_file, $app_path)

  missing = list_missing_files([profile_file, cert_file])

  if !missing.empty?
    raise Exception.new("Could not load #{missing.join(', ')}")
  end

  options = {
      :upload_cert => Base64.urlsafe_encode64(File.open(cert_file, 'rb') { |io| io.read }),
      :upload_profile => Base64.urlsafe_encode64(File.open(profile_file, 'rb') { |io| io.read }),
      :bundle_identifier => get_conf('iphone/BundleIdentifier')
  }

  if !cert_pw.nil? && !cert_pw.empty?
    options[:cert_pw] = cert_pw
  end

  options
end

def run_binary_on(platform, package, devsim)
  if !package.nil?
    Rake::Task["run:#{platform}:#{devsim}:package"].invoke(package)
  else
    BuildOutput.error( "Could not find executable file for #{platform} project", 'No file')
  end
end

def get_build_and_run(build_id, run_target)
  is_ok, res, build_platform = get_build(build_id)
  if is_ok
    files, platform, package = deploy_build(build_platform)
    puts files, platform, package
    unless files.empty?
      if run_target
        run_binary_on(platform, package, run_target)
      end
    end
  else
    BuildOutput.put_log(BuildOutput::ERROR, res, 'build error')
  end
end

def build_deploy_run(target, run_target = nil)
  res =  target.split(':')

  platform = res.first

  data = nil

  if res.length > 1
    data = res.last
  end

  if platform == 'iphone'
    options = get_iphone_options()
  else
    options = {}
  end

  b_id = do_platform_build( platform, $platform_list, platform == 'iphone', options, data)

  get_build_and_run(b_id, run_target)
end


def includes(a, b)
  a.include?(b) || b.include?(a)
end

#------------------------------------------------------------------------
#config

def create_rhodes_home()
  home = File.join(Dir.home(),'.rhomobile')
  if !File.exist?(home)
    FileUtils::mkdir_p home
  end

  home
end

def find_proxy()
  # proxy url priority starting from maximum
  priority = [ "https_proxy", "http_proxy", "all_proxy" ].reverse

  proxy = nil

  best_proxy = ENV.max_by do |k, v|
    val = priority.index(k.downcase)
    if val.nil?
      -1
    else
      val
    end
  end

  # check for
  if !priority.index(best_proxy[0].downcase).nil?
    proxy = best_proxy[1]

    prefix = ""

    case best_proxy[0].downcase
    when "https_proxy"
      prefix = "https://"
    else
      prefix = "http://"
    end

    if !proxy.include?("://")
      proxy = prefix + proxy
    end
  end

  proxy
end

def get_ssl_cert_bundle_store(rhodes_home, proxy)
  crt_file = File.join(rhodes_home, "crt.pem")

  #lets get that file once a month
  if !(File.exists?(crt_file)) || ((Time.now - File.mtime(crt_file)).to_i > 30 * 24 * 60 * 60)
    puts "getting cert bundle"
    url = URI.parse("https://raw.githubusercontent.com/bagder/ca-bundle/master/ca-bundle.crt")

    if !(proxy.nil? || proxy.empty?)
      proxy_uri = URI.parse(proxy)
      http = Net::HTTP.new(url.host, url.port, proxy_uri.host, proxy_uri.port, proxy_uri.user, proxy_uri.password )
    else
      http = Net::HTTP.new(url.host, url.port)
    end

    if url.scheme == "https"  # enable SSL/TLS
      http.use_ssl = true
      # there is no way to verify connection here :/
      http.verify_mode = OpenSSL::SSL::VERIFY_NONE
    end

    begin
      http.start do |http|
        resp = http.get(url.path)
        if resp.code == "200"
          open(crt_file, "wb") do |file|
            file.write(resp.body)
          end
        else
          abort "\n\n>>>> A cacert.pem bundle could not be downloaded."
        end
      end
    rescue
      abort "\n\n>>>> A cacert.pem bundle could not be downloaded."
    end
  end

  cert_store = OpenSSL::X509::Store.new
  cert_store.set_default_paths
  cert_store.add_file crt_file

  return cert_store
end

namespace "config" do
  task :set_rho, [:rho_dir] do |t, args|
    RhoPackages.set_root(args[:rho_dir])
  end

  task :load do

    require 'deep_merge'

    print_timestamp('First timestamp')

    buildyml = 'rhobuild.yml'

    # read shared config
    $rhodes_home = create_rhodes_home()

    conf_file = File.join($rhodes_home,buildyml)
    $shared_conf = {}
    if File.exists?(conf_file)
      $logger.info "Shared config is available at #{File.join($rhodes_home,buildyml)}"
      $shared_conf = Jake.config(File.open(File.join($rhodes_home,buildyml)))
    end

    $current_platform_bridge = $current_platform unless $current_platform_bridge

    $logger.info( "Using Rhodes configuration from path: #{buildyml}" )

    # read gem folder build config
    buildyml = ENV["RHOBUILD"] unless ENV["RHOBUILD"].nil?
    $config = {}
    $config = Jake.config(File.open(buildyml)) if File.file?(buildyml)

    $config = $config.deep_merge($shared_conf) if $shared_conf

    $config["platform"] = $current_platform if $current_platform
    $config["env"]["app"] = "spec/framework_spec" if $rhosimulator_build

    $app_path = ENV["RHO_APP_PATH"] if ENV["RHO_APP_PATH"] && $app_path.nil?

    if $app_path.nil? #if we are called from the rakefile directly, this wont be set
      #load the apps path and config

      $app_path = $config["env"]["app"] unless $config["env"].nil?

      if $app_path.nil?
        b_y = File.join(Dir.pwd(),'build.yml')
        if File.exists?(b_y)
          $app_path = Dir.pwd()
        end
      end
    end

    $app_config = {}

    if (!$app_path.nil?)
      app_yml = File.join($app_path, "build.yml")

      if File.exists?(app_yml)
        # read application config
        $app_config = Jake.config(File.open(app_yml)) if $app_config_disable_reread != true

        if File.exists?(File.join($app_path, "app_rakefile"))
          load File.join($app_path, "app_rakefile")
          $app_rakefile_exist = true
          Rake::Task["app:config"].invoke
        end
      end
    end

    $proxy = get_conf('connection/proxy', find_proxy())

    if !($proxy.nil? || $proxy.empty?)
      puts "Using proxy: #{$proxy}"
    end


    # I hate that way of dealing with ssl, but we need to get working
    # set of certificates from somewhere for windows
    # so lets solve it less hacky, get mozilla's bundle of certs converted by cURL team
    # and use it for accessing via rest client
    if (/cygwin|mswin|mingw|bccwin/ =~ RUBY_PLATFORM) != nil
      Rhohub.cert_store = get_ssl_cert_bundle_store($rhodes_home, $proxy)
    end

    $re_app = ($app_config["app_type"] == 'rhoelements') || !($app_config['capabilities'].nil? || $app_config['capabilities'].index('shared_runtime').nil?)
  end

  task :initialize => [:load] do
    $binextensions = []
    $app_extensions_list = {}
    $app_extension_cfg = {}

    if RUBY_PLATFORM =~ /(win|w)32$/
      $all_files_mask = "*.*"
      $rubypath = "res/build-tools/RhoRuby.exe"
    else
      $all_files_mask = "*"
      if RUBY_PLATFORM =~ /darwin/
        $rubypath = "res/build-tools/RubyMac"
      else
        $rubypath = "res/build-tools/rubylinux"
      end
    end

    if $app_path.nil? || !(File.exists?($app_path))
      puts "Could not find rhodes application. Please verify your application setting in #{File.dirname(__FILE__)}/rhobuild.yml"
      exit 1
    end

    ENV["RHO_APP_PATH"] = $app_path.to_s
    ENV["ROOT_PATH"]    = $app_path.to_s + '/app/'
    ENV["APP_TYPE"]     = "rhodes"


    Jake.normalize_build_yml($app_config)

    Jake.set_bbver($app_config["bbver"].to_s)
  end

  task :common => [:initialize] do
    $logger.info "Starting rhodes build system using ruby version: #{RUBY_VERSION}"
    print_timestamp('config:common')

    if $app_config && !$app_config["sdk"].nil?
      BuildOutput.note('To use latest Rhodes gem, run migrate-rhodes-app in application folder or comment sdk in build.yml.','You use sdk parameter in build.yml')
    end

    #Rhodes gem/framework version from version file.
    ENV['rhodes_version'] = File.read(File.join($startdir,'version')).chomp

    $bindir = File.join($app_path, "bin")
    $tmpdir = File.join($bindir, "tmp")

    $skip_checking_XCode = false
    $skip_build_rhodes_main = false
    $skip_build_extensions = false
    $skip_build_xmls = false
    $skip_checking_Android_SDK = false
    $skip_build_js_api_files = false

    extpaths = []

    add_ext_path = lambda {|p| extpaths << File.absolute_path(p, $app_path)}

    add_ext_paths = Proc.new do |paths|
      add_ext_path.call(paths) if paths.is_a? String
      paths.each {|p| add_ext_path.call(p)} if paths.is_a? Array
    end

    if $app_config["paths"] and $app_config["paths"]["extensions"]
      add_ext_paths.call($app_config["paths"]["extensions"])
    end
    if $config["env"]
        if  $config["env"]["paths"]
            if $config["env"]["paths"]["extensions"]
                add_ext_paths.call($config["env"]["paths"]["extensions"])
            end
        end
    end
    extpaths << File.join($app_path, "extensions")
    extpaths << File.join($startdir, "lib","commonAPI")
    extpaths << File.join($startdir, "lib","extensions")
    extpaths << File.join($startdir, "extensions")
    extpaths << File.join($startdir, "../rho-tau-extensions-"+ENV['rhodes_version']+"/libs")
    $app_config["extpaths"] = extpaths

    if $app_config["build"] and $app_config["build"].casecmp("release") == 0
      $debug = false
    else
      $debug = true
    end

    # merge extensions from platform list to global one
    $app_config['extensions'] = [] unless $app_config['extensions'] and $app_config['extensions'].is_a? Array
    if $app_config[$config['platform']] and $app_config[$config['platform']]['extensions'] and $app_config[$config['platform']]['extensions'].is_a? Array
      $app_config['extensions'] = $app_config['extensions'] | $app_config[$config['platform']]['extensions']
    end

    # gather main extensions
    extensions = []
    extensions << "coreapi" #unless $app_config['re_buildstub']
    if ($current_platform == "iphone") || ($current_platform == "android")
        extensions << "decryptstub"
    else
        puts "do not checking for encrypt/decrypt because not iOS/Android 1"
    end
    extensions << "zlib" if $current_platform == "win32" # required by coreapi on win32 for gzip support in Network
    extensions += get_extensions
    extensions << "rhoconnect-client" if $rhosimulator_build
    extensions << "json"

    # filter list of extensions with main extensions list (regardless of case!)
    downcased = extensions.map(&:downcase)
    $app_config['extensions'].reject! { |ext| downcased.include?(ext.downcase) }

    $app_config['extensions'] = extensions + $app_config['extensions']
    $app_config['extensions'].uniq!

    capabilities = []
    capabilities += $app_config["capabilities"] if $app_config["capabilities"] and
                    $app_config["capabilities"].is_a? Array
    capabilities += $app_config[$config["platform"]]["capabilities"] if $app_config[$config["platform"]] and
                    $app_config[$config["platform"]]["capabilities"] and
                    $app_config[$config["platform"]]["capabilities"].is_a? Array
    $app_config["capabilities"] = capabilities

    application_build_configs = {}

    #Process rhoelements settings
    if $current_platform == "wm" || $current_platform == "android"
      if $app_config["app_type"] == 'rhoelements'

          $app_config["capabilities"] += ["symbol"] unless $app_config["capabilities"].index("symbol")
          if $current_platform == "wm"
			$app_config["extensions"] += ["webkit"]
          else
			$app_config["extensions"] += ["rhoelementsext"]
			$app_config["extensions"] += ["symbolapi"] #extension with plug-ins

			#check for RE2 plugins
			plugins = ""
			$app_config["extensions"].each do |ext|
				if ( ext.start_with?('moto-') )
				plugins += ',' if plugins.length() > 0
				plugins += ext[5, ext.length()-5]
				end
			end

			if plugins.length() == 0
				plugins = "ALL"
			end

			application_build_configs['moto-plugins'] = plugins if plugins.length() > 0
		end


        if !$app_config["capabilities"].index('native_browser') && $current_platform != "android"
          $app_config['extensions'] += ['webkit-browser'] unless $app_config['extensions'].index('webkit-browser')
        end
      end

      application_build_configs['shared-runtime'] = '1' if $app_config["capabilities"].index('shared_runtime')

      if $app_config["extensions"].index("webkit-browser")
        $app_config["capabilities"] += ["webkit_browser"]
        $app_config["extensions"].delete("webkit-browser")
      end

      if  $app_config["capabilities"].index("webkit_browser") || ($app_config["capabilities"].index("symbol") && $current_platform != "android")
        #contains wm and android libs for webkit browser
        if $current_platform == "android"
			$app_config["extensions"] += ["rhoelements"] unless $app_config['extensions'].index('rhoelements')
        end
      end
    end
    if $app_config['capabilities'].index("camera") && $current_platform != 'win32'
      $app_config['extensions'] = $app_config['extensions'] | ['mediacapture']
    end
    if $app_config["app_type"] == 'rhoelements'

      # add audiocapture extensions for rhoelements app
      if !$app_config['extensions'].index('rhoelementsext')
        if $current_platform == "iphone" || $current_platform == "android"
          $app_config['extensions'] = $app_config['extensions'] | ['audiocapture']
        end
      end

      if $current_platform == "wm"
        $app_config['extensions'] = $app_config['extensions'] | ['symboldevice']
        $app_config['extensions'] = $app_config['extensions'] | ['barcode']
        $app_config['extensions'] = $app_config['extensions'] | ['indicators']
        $app_config['extensions'] = $app_config['extensions'] | ['cardreader']
        $app_config['extensions'] = $app_config['extensions'] | ['signature']
        $app_config['extensions'] = $app_config['extensions'] | ['hardwarekeys']
        $app_config['extensions'] = $app_config['extensions'] | ['sensor']
      end

      if $current_platform == "iphone"
        $app_config['extensions'] = $app_config['extensions'] | ['barcode']
        $app_config['extensions'] = $app_config['extensions'] | ['signature']
        $app_config['extensions'] = $app_config['extensions'] | ['indicators']
        $app_config['extensions'] = $app_config['extensions'] | ['hardwarekeys']
        $app_config['extensions'] = $app_config['extensions'] | ['sensor']
      end

      if $current_platform == "android"
        $app_config['extensions'] = $app_config['extensions'] | ['symboldevice']
        $app_config['extensions'] = $app_config['extensions'] | ['emdk3-manager']
        $app_config['extensions'] = $app_config['extensions'] | ['barcode']
        $app_config['extensions'] = $app_config['extensions'] | ['signature']
        $app_config['extensions'] = $app_config['extensions'] | ['indicators']
        $app_config['extensions'] = $app_config['extensions'] | ['hardwarekeys']
        $app_config['extensions'] = $app_config['extensions'] | ['sensor']
      end

      if $current_platform == "wp8"
        $app_config['extensions'] = $app_config['extensions'] | ['barcode']
      end

      if $current_platform == "uwp"
        $app_config['extensions'] = $app_config['extensions'] | ['barcode']
      end
   end

   if $current_platform == "android"
        if $app_config['extensions'].index('rhoelementsext')
          $app_config['extensions'].delete('rhoelementsext')
          $app_config['extensions'].unshift('rhoelementsext')
        end
        if $app_config['extensions'].index('emdk3-manager')
          $app_config['extensions'].delete('emdk3-manager')
          $app_config['extensions'].unshift('emdk3-manager')
        elsif $app_config['extensions'].index('barcode') || $app_config['extensions'].index('indicators') || $app_config['extensions'].index('mobile_payment') || $app_config['extensions'].index('smartCradle')
		      #Barcode has dependency on emdk3-manager. So load it if barcode is present
		      $app_config['extensions'].unshift('emdk3-manager')
        end
        $app_config['extensions'].unshift('coreapi')
    end

    #if $app_config['extensions'].index('rhoelementsext')
    #    #$app_config["extensions"].delete("rawsensors")
    #    $app_config["extensions"].delete("audiocapture")
    #end

    $app_config['extensions'].delete('barcode') if ( $app_config['extensions'].index('enterprise-barcode') || $app_config['extensions'].index('allbarcode') )

    $hidden_app = $app_config["hidden_app"].nil?() ? "0" : $app_config["hidden_app"]

    #application build configs
    $application_build_configs_keys.each do |key|
      value = $app_config[key]
      if $app_config[$config["platform"]] != nil
        if $app_config[$config["platform"]][key] != nil
          value = $app_config[$config["platform"]][key]
        end
      end
      if value != nil
        application_build_configs[key] = value
      end
    end
    $application_build_configs = application_build_configs

    if $current_platform == "win32" && $winxpe_build
      $app_config['capabilities'] << 'winxpe'
    end

    $app_config['extensions'].uniq!() if $app_config['extensions']
    $app_config['capabilities'].uniq!() if $app_config['capabilities']

    $app_config['extensions'].delete("mspec") if !$debug && $app_config['extensions'].index('mspec')
    $app_config['extensions'].delete("rhospec") if !$debug && $app_config['extensions'].index('rhospec')
    $app_config['extensions'].delete("development") if !$debug && $app_config['extensions'].index('development')
    $app_config['extensions'].delete("Development") if !$debug && $app_config['extensions'].index('Development')

    $rhologhostport = $config["log_host_port"]
    $rhologhostport = 52363 unless $rhologhostport
    begin
      $rhologhostaddr = Jake.localip()
    rescue Exception => e
      puts "Jake.localip() error : #{e}"
    end

    $file_map_name     = "RhoBundleMap.txt"

    obfuscate_js       = Jake.getBuildBoolProp2("obfuscate", "js", $app_config, nil)
    obfuscate_css      = Jake.getBuildBoolProp2("obfuscate", "css", $app_config, nil)
    $obfuscate_exclude = Jake.getBuildProp2("obfuscate", "exclude_dirs" )

    minify_js       = Jake.getBuildBoolProp2("minify", "js", $app_config, nil)
    minify_css      = Jake.getBuildBoolProp2("minify", "css", $app_config, nil)
    $minify_exclude = Jake.getBuildProp2("minify", "exclude_dirs" )

    $minify_types = []

    if !$debug
      minify_js = true if minify_js == nil
      minify_css = true if minify_css == nil
    end

    $minify_types << "js" if minify_js or obfuscate_js
    $minify_types << "css" if minify_css or obfuscate_css

    $minifier          = File.join(File.dirname(__FILE__),'res/build-tools/yuicompressor-2.4.8-rhomodified.jar')

    $use_shared_runtime = Jake.getBuildBoolProp("use_shared_runtime")
    $js_application    = Jake.getBuildBoolProp("javascript_application")
    $nodejs_application    = Jake.getBuildBoolProp("nodejs_application")

    $logger.debug '%%%_%%% $js_application = '+$js_application.to_s
    $logger.debug '%%%_%%% $nodejs_application = '+$nodejs_application.to_s

    if !$js_application && !$nodejs_application && !Dir.exists?(File.join($app_path, "app"))
      BuildOutput.error([
                          "Add javascript_application:true to build.yml, since application does not contain app folder.",
                          "See: http://docs.rhomobile.com/guide/api_js#javascript-rhomobile-application-structure"
      ]);
      exit(1)
    end

    $shared_rt_js_appliction = ($js_application and $current_platform == "wm" and $app_config["capabilities"].index('shared_runtime'))
    $logger.debug "%%%_%%% $shared_rt_js_application = #{$shared_rt_js_appliction}"
    $app_config['extensions'] = $app_config['extensions'] | ['rubyvm_stub'] if $shared_rt_js_appliction

    # check for encrypting
    if ($current_platform == "iphone") || ($current_platform == "android")
        $encrypt_aes_key = nil
        encrypt_file_extensions = $app_config["encrypt_file_extensions"]
        encrypt_files_key = $app_config["encrypt_files_key"]
        if (encrypt_file_extensions != nil) && (encrypt_files_key != nil)
            if encrypt_file_extensions.is_a?(Array) && encrypt_files_key.is_a?(String)
                $encrypt_aes_key = encrypt_files_key
                $app_config['extensions'] = $app_config['extensions'] | ['openssl.so']
                $app_config['extensions'] = $app_config['extensions'] | ['decrypt']
                $app_config["extensions"].delete("decryptstub")
            end
        end
    else
        puts "do not checking for encrypt/decrypt because not iOS/Android"
    end

    if $current_platform == "bb"
      make_application_build_config_java_file()
      update_rhoprofiler_java_file()
    elsif $current_platform == "wp"
    else
      make_application_build_config_header_file
      make_application_build_capabilities_header_file
      update_rhodefs_header_file
    end

    $remote_debug = false
    $remote_debug = Jake.getBool(ENV['rho_remote_debug'])  if ENV['rho_remote_debug']

    if $remote_debug
      $app_config['extensions'] = $app_config['extensions'] | ['debugger']
      $app_config['extensions'] = $app_config['extensions'] | ['uri']
      $app_config['extensions'] = $app_config['extensions'] | ['timeout']
    end

    # it`s should be in the end of common:config task
    platform_task = "config:#{$current_platform}:app_config"
    Rake::Task[platform_task].invoke if Rake::Task.task_defined? platform_task

    $logger.debug "$app_config['extensions'] : #{$app_config['extensions'].inspect}"
    $logger.debug "$app_config['capabilities'] : #{$app_config['capabilities'].inspect}"

  end # end of config:common

  task :qt do
    $qtdir = ENV['QTDIR']
    unless (!$qtdir.nil?) and ($qtdir !~/^\s*$/) and File.directory?($qtdir)
      puts "\nPlease, set QTDIR environment variable to Qt root directory path"
      exit 1
    end
    $qmake = File.join($qtdir, 'bin/qmake')
    $macdeployqt = File.join($qtdir, 'bin/macdeployqt')
  end

  task :rhosimulator do
    $rhosimulator_build = true
  end

end

def copy_assets(asset, file_map)

  dest = File.join($srcdir,'apps/public')

  cp_r asset + "/.", dest, :preserve => true, :remove_destination => true, :verbose => USE_TRACES
end

def clear_linker_settings
  if $config["platform"] == "iphone"
    #    outfile = ""
    #    IO.read($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj").each_line do |line|
    #      if line =~ /EXTENSIONS_LDFLAGS = /
    #        outfile << line.gsub(/EXTENSIONS_LDFLAGS = ".*"/, 'EXTENSIONS_LDFLAGS = ""')
    #      else
    #        outfile << line
    #      end
    #    end
    #    File.open($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj","w") {|f| f.write outfile}
    #    ENV["EXTENSIONS_LDFLAGS"] = ""

    $ldflags = ""
  end

end

def add_linker_library(libraryname)
  #  if $config["platform"] == "iphone"
  #    outfile = ""
  #    IO.read($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj").each_line do |line|
  #      if line =~ /EXTENSIONS_LDFLAGS = /
  #        outfile << line.gsub(/";/, " $(TARGET_TEMP_DIR)/#{libraryname}\";")
  #      else
  #        outfile << line
  #      end
  #    end
  #    File.open($startdir + "/platform/iphone/rhorunner.xcodeproj/project.pbxproj","w") {|f| f.write outfile}
  #  end
  simulator = $sdk =~ /iphonesimulator/

  if ENV["TARGET_TEMP_DIR"] and ENV["TARGET_TEMP_DIR"] != ""
    tmpdir = ENV["TARGET_TEMP_DIR"]
  else
    tmpdir = File.join($app_path, 'project/iphone') + "/build/rhorunner.build/#{$configuration}-" +
      ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
  end
  $ldflags << "#{tmpdir}/#{libraryname}\n" unless $ldflags.nil?
end

def add_inker_library_absolute(fulllibraryfilepath)
    $ldflags << fulllibraryfilepath + "\n" unless $ldflags.nil?
end

def set_linker_flags
  if $config["platform"] == "iphone"
    simulator = $sdk =~ /iphonesimulator/
    if ENV["TARGET_RHODESLIBS_DIR"] and ENV["TARGET_RHODESLIBS_DIR"] != ""
      tmpdir = ENV["TARGET_RHODESLIBS_DIR"]
    else
      if ENV["TARGET_TEMP_DIR"] and ENV["TARGET_TEMP_DIR"] != ""
        tmpdir = ENV["TARGET_TEMP_DIR"]
      else
        tmpdir = File.join($app_path, 'project/iphone') + "/build/rhorunner.build/#{$configuration}-" +
          ( simulator ? "iphonesimulator" : "iphoneos") + "/rhorunner.build"
      end
    end
    mkdir_p tmpdir unless File.exist? tmpdir
    tmpdir = File.join($app_path.to_s, "project/iphone")
    mkdir_p tmpdir unless File.exist? tmpdir
    File.open(tmpdir + "/rhodeslibs.txt","w") { |f| f.write $ldflags }
    #    ENV["EXTENSIONS_LDFLAGS"] = $ldflags
    #    puts `export $EXTENSIONS_LDFLAGS`
  end

end

def add_extension(path,dest)
  puts 'add_extension - ' + path.to_s + " - " + dest.to_s

  start = pwd
  chdir path if File.directory?(path)
  puts 'chdir path=' + path.to_s

  if !$js_application && !$nodejs_application
    Dir.glob("*").each do |f|
      cp_r f,dest unless f =~ /^ext(\/|(\.yml)?$)/ || f =~ /^app/  || f =~ /^public/
    end
  end

  if $current_platform == "bb"
    FileUtils.cp_r 'app', File.join( dest, "apps/app" ) if File.exist? 'app'
    FileUtils.cp_r 'public', File.join( dest, "apps/public" ) if File.exist? 'public'
  else
    if !$nodejs_application
      FileUtils.cp_r('app', File.join( File.dirname(dest), "apps/app" ).to_s) if File.exist? 'app'
      FileUtils.cp_r('public', File.join( File.dirname(dest), "apps").to_s) if File.exist? 'public'
    end
  end

  chdir start
end

def find_ext_ingems(extname)
  extpath = nil
  begin
    $rhodes_extensions = nil
    $rhodes_join_ext_name = false

    require extname
    if $rhodes_extensions
      extpath = $rhodes_extensions[0]
      $app_config["extpaths"] << extpath
      if $rhodes_join_ext_name
        extpath = File.join(extpath,extname)
      end
    end
  rescue Exception => e
    puts "exception: #{e}"
  end

  extpath
end

def common_prefix(paths)
  return '' if paths.empty?
  return paths.first.split('/').slice(0...-1).join('/') if paths.length <= 1
  arr = paths.sort
  first = arr.first.split('/')
  last = arr.last.split('/')
  i = 0
  i += 1 while first[i] == last[i] && i <= first.length
  first.slice(0, i).join('/')
end

def write_orm_modules_js(folder, modules)
  modules.each { |f|
    fname = File.basename(f.to_s).gsub(/^(|.*[\\\/])(Rho\.|)([^\\\/]+)\.js$/, '\3').gsub(/\./, '-')
    cp f.to_s, File.join(folder, 'rhoapi-modules-' + fname + '.js')
  }
end

def write_modules_js(folder, filename, modules, do_separate_js_modules)
  f = StringIO.new("", "w+")
  f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"

  if modules
    modules.each do |m|
      module_name = m.gsub(/^(|.*[\\\/])([^\\\/]+)\.js$/, '\2')
      f.puts( "// Module #{module_name}\n\n" )
      f.write(File.read(m))
    end
  end

  Jake.modify_file_if_content_changed(File.join(folder,filename), f)

  if modules && do_separate_js_modules
    common = common_prefix(modules)

    # Glue all modules based on assumption that FrameworkName.ExtName.*.js should be in one file
    groups = modules.group_by do |mod|
      path, name = File.split(mod)
      name_parts = name.ext().split('.')
      if name_parts.size > 2
        res = name_parts.first(2).join('.')
      else
        res = name
      end
      res
    end

    groups.each do |k, v|
      f = StringIO.new("", "w+")

      if v.size > 1
        fname = k.end_with?(".js") ? k : k + ".js"
      else
        fname = v.first
      end

      module_name = fname.gsub(/^(|.*[\\\/])([^\\\/]+)\.js$/, '\2')

      f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
      f.puts( "// Module #{module_name}" )

      v.each do |fname|
        f.puts "\n// From file #{fname.gsub(common,'')}\n\n"
        f.write(File.read(fname))
      end

      Jake.modify_file_if_content_changed(File.join(folder, module_name.downcase+'.js'), f)
    end
  end
end

def is_ext_supported(extpath)
  extyml = File.join(extpath, "ext.yml")
  res = true
  if File.file? extyml
    extconf = Jake.config(File.open(extyml))
    if extconf["platforms"]
      res = extconf["platforms"].index($current_platform) != nil
    end
  end
  res
end

def init_extensions(dest, mode = "")


  print_timestamp('init_extensions( '+dest.to_s+', mode= ['+mode.to_s+'] ) START')

  extentries = []
  extentries_init = []
  nativelib = []
  extlibs = []
  extjsmodulefiles = []
  extjsmodulefiles_opt = []
  startJSModules = []
  startNodeJSModules = []
  startJSModules_opt = []
  endJSModules = []
  extcsharplibs = []
  extcsharpentries = []
  extcsharppaths = []
  extcsharpprojects = []
  extscsharp = nil
  ext_xmls_paths = []

  extpaths = $app_config["extpaths"]

  rhoapi_js_folder = nil
  rhoapi_nodejs_folder = nil
  if !dest.nil?
    if $nodejs_application
        rhoapi_js_folder = File.join( File.dirname(dest), "apps/nodejs/server/public/api" )
    else
        rhoapi_js_folder = File.join( File.dirname(dest), "apps/public/api" )
    end
    rhoapi_nodejs_folder = File.join( File.dirname(dest), "apps/nodejs/rhoapi" )
  elsif mode == "update_rho_modules_js"
      if $nodejs_application
          rhoapi_js_folder = File.join( $app_path, "nodejs/server/public/api" )
      else
          rhoapi_js_folder = File.join( $app_path, "public/api" )
      end
    rhoapi_nodejs_folder = File.join( $app_path, "nodejs/rhoapi" )
  end

  do_separate_js_modules = Jake.getBuildBoolProp("separate_js_modules", $app_config, false)

  $logger.debug "rhoapi_js_folder: #{rhoapi_js_folder}"

  $logger.info 'Init extensions'

  $logger.debug "Extensions list: #{$app_config['extensions'].to_s}"

  # TODO: checker init
  gen_checker = GeneratorTimeChecker.new
  gen_checker.init($startdir, $app_path)

  $app_config["extensions"].each do |extname|
    puts 'ext - ' + extname

    if extname == "webkit"
		$is_webkit_engine = true
    end

    extpath = nil
    extpaths.each do |p|
      ep = File.join(p, extname)
      if File.exists?( ep )

         if !is_ext_supported(ep)
          raise "Extension #{extname} is not supported for platform: #{$current_platform}"
         end

        extpath = ep
        break
      end
    end

    if extpath.nil?
      extpath = find_ext_ingems(extname)
      if extpath
        extpath = nil unless is_ext_supported(extpath)
      end
    end

    if ((extpath.nil?) && (extname != 'symbolapi') ) && ($skip_build_extensions == false)
      raise "Can't find extension '#{extname}'. Aborting build.\nExtensions search paths are:\n#{extpaths}"
    end

    $app_extensions_list[extname] = extpath

    unless extpath.nil?
      puts 'iter=' + extpath.to_s

      if $config["platform"] != "bb"
        extyml = File.join(extpath, "ext.yml")
        puts "extyml " + extyml

        if File.file? extyml
          extconf = Jake.config(File.open(extyml))

          $app_extension_cfg[extpath] = extconf

          entry           = extconf["entry"]
          nlib            = extconf["nativelibs"]
          type            = Jake.getBuildProp( "exttype", extconf )
          xml_api_paths   = extconf["xml_api_paths"]
          extconf_wp8     = $config["platform"] == "wp8" && (!extconf['wp8'].nil?) ? extconf['wp8'] : Hash.new
          extconf_uwp     = $config["platform"] == "uwp" && (!extconf['uwp'].nil?) ? extconf['uwp'] : Hash.new
          if ($config["platform"] == "wp8")
            csharp_impl_all = (!extconf_wp8['csharp_impl'].nil?) ? true : false
          else
            csharp_impl_all = (!extconf_uwp['csharp_impl'].nil?) ? true : false
          end

          if nlib != nil
            nlib.each do |libname|
              nativelib << libname
            end
          end

          # add additional libraries
          link_libraries = []
          if (!extconf["link_libraries"].nil?) && (extconf["link_libraries"].is_a? Array)
              link_libraries += extconf["link_libraries"]
          end
          if (!extconf[$config["platform"]].nil?) && (!extconf[$config["platform"]]["link_libraries"].nil?) && (extconf[$config["platform"]]["link_libraries"].is_a? Array)
              link_libraries += extconf[$config["platform"]]["link_libraries"]
          end
          link_libraries.each do |linklib|
              fulllibpath = File.join(extpath, linklib)
              add_inker_library_absolute(fulllibpath)
          end

          if entry && entry.length() > 0
            if xml_api_paths.nil? #&& !(("rhoelementsext" == extname || "dominjector" == extname ) && ($config["platform"] == "wm"||$config["platform"] == "android"))

              $ruby_only_extensions_list = [] unless $ruby_only_extensions_list
              $ruby_only_extensions_list << extname

              if (("rhoelementsext" == extname || "dominjector" == extname) && ($config["platform"] == "wm"||$config["platform"] == "android"))
                extentries << entry
                extentries_init << entry
            elsif !$js_application && !$nodejs_application
                extentries << entry
                entry =  "if (rho_ruby_is_started()) #{entry}"
                extentries_init << entry
              end
            else
              extentries << entry
              extentries_init << entry
            end

          end

          if type.to_s() != "nativelib"
            libs = extconf["libraries"]
            libs = [] unless libs.is_a? Array

            if (!extconf[$config["platform"]].nil?) && (!extconf[$config["platform"]]["libraries"].nil?) && (extconf[$config["platform"]]["libraries"].is_a? Array)
              libs = libs + extconf[$config["platform"]]["libraries"]
            end

            if $config["platform"] == "wm" || $config["platform"] == "win32" || $config["platform"] == "uwp"
              libs.each do |lib|
                if $config["platform"] == "uwp"
                  lib = lib.delete('.')
                end
                extconf_uwp_lib = !extconf_uwp[lib.downcase].nil? ? extconf_uwp[lib.downcase] : Hash.new
                csharp_impl = csharp_impl_all || (!extconf_uwp_lib['csharp_impl'].nil?)
                if extconf_uwp_lib['libname'].nil?
                  extlibs << lib + (csharp_impl ? "Lib" : "") + ".lib"
                end

                if csharp_impl
                    uwp_root_namespace = !extconf_uwp_lib['root_namespace'].nil? ? extconf_uwp_lib['root_namespace'] : 'rho'
                    extcsharplibs << (extconf_uwp_lib['libname'].nil? ? (lib + "Lib.lib") : (extconf_uwp_lib['libname'] + ".lib"))
                    extcsharppaths << "<#{lib.upcase}_ROOT>" + File.join(extpath, 'ext') + "</#{lib.upcase}_ROOT>"
                    extcsharpprojects << '<Import Project="$(' + lib.upcase + '_ROOT)\\platform\\uwp\\' + lib + 'Impl.targets" />'
                    extcsharpentries << "#{lib}FactoryComponent.setImpl(new #{uwp_root_namespace}.#{lib}Impl.#{lib}Factory())"
                end
              end
            else
              libs.map! { |lib| "lib" + lib + ".a" }
              extlibs += libs
            end
          end

          if (xml_api_paths && type != "prebuilt") && !($use_prebuild_data && (extname == 'coreapi')) && !$prebuild_win32 # && wm_type != "prebuilt"
            xml_api_paths    = xml_api_paths.split(',')

            xml_api_paths.each do |xml_api|
              xml_path = File.join(extpath, xml_api.strip())

              ext_xmls_paths <<  xml_path
              if mode != "get_ext_xml_paths"
                #api generator
                if File.exist? xml_path
                  if gen_checker.check(xml_path)
                    puts 'start running rhogen with api key for + ['+xml_path.to_s+']'
                    if !$skip_build_extensions
                      Jake.run32("\"#{$startdir}/bin/rhogen\" api \"#{xml_path}\"")
                    end
                  end
                end
              end
            end

          end
        end

        #if !$skip_build_extensions
          unless rhoapi_js_folder.nil?
            Dir.glob(extpath + "/public/api/*.js").each do |f|
              fBaseName = File.basename(f)
              if (fBaseName.start_with?("rhoapi-native") )
                endJSModules << f if fBaseName == "rhoapi-native.all.js"
                next
              end
              if (fBaseName == "rhoapi-force.ajax.js")
                add = Jake.getBuildBoolProp("ajax_api_bridge", $app_config, false)
                add = Jake.getBuildBoolProp2($current_platform, "ajax_api_bridge", $app_config, add)
                endJSModules << f if add
                next
              end
              if (fBaseName == "#{extname}-postDef.js")
                puts "add post-def module: #{f}"
                endJSModules << f
              end

              if f.downcase().end_with?("jquery-2.0.2-rho-custom.min.js")
                startJSModules.unshift(f)
              elsif f.downcase().end_with?("rhoapi.js")
                startJSModules << f
            elsif f.downcase().end_with?("rhonodejsapi.js")
                startNodeJSModules << f
              elsif f.downcase().end_with?("rho.application.js")
                endJSModules << f
              elsif f.downcase().end_with?("rho.database.js")
                endJSModules << f
              elsif f.downcase().end_with?("rho.newormhelper.js")
                endJSModules << f
              elsif /(rho\.orm)|(rho\.ruby\.runtime)|(rho\.rhosim\.fix)/i.match(f.downcase())
                puts "add #{f} to startJSModules_opt.."
                startJSModules_opt << f
              else
                extjsmodulefiles << f
              end
            end

            Dir.glob(extpath + "/public/api/generated/*.js").each do |f|
              if /(rho\.orm)|(rho\.ruby\.runtime)|(rho\.rhosim\.fix)/i.match(f.downcase())
                puts "add #{f} to extjsmodulefiles_opt.." if USE_TRACES
                extjsmodulefiles_opt << f
              else
                puts "add #{f} to extjsmodulefiles.." if USE_TRACES
                extjsmodulefiles << f
              end
            end
          end
        #end

      end

      add_extension(extpath, dest) if !dest.nil? && mode == ""
    end


  end

  stubs = File.join($startdir, "platform", "wm", "rhodes", "browser", "stubs.cpp")
  if $is_webkit_engine == false
	File.open(stubs, "w+") do |f|
        f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
        f.puts "#include \"StdAfx.h\""
        f.puts "#include \"common/RhoStd.h\""
        f.puts "#include \"IBrowserEngine.h\""
        f.puts "rho::IBrowserEngine* rho_wmimpl_get_webkitBrowserEngine(HWND hwndParent, HINSTANCE rhoAppInstance) {return 0;}"
        Jake.modify_file_if_content_changed( stubs, f )
    end
  else
	File.open(stubs, "w+") do |f|
        f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
        f.puts "#include \"StdAfx.h\""
        Jake.modify_file_if_content_changed( stubs, f )
    end
  end

  if ($ruby_only_extensions_list)
    BuildOutput.warning([
                          'The following extensions do not have JavaScript API: ',
                          $ruby_only_extensions_list.join(', '),
    'Use RMS 4.0 extensions to provide JavaScript API'])
  end

  return ext_xmls_paths if mode == "get_ext_xml_paths"

  #TODO: checker update
  gen_checker.update

  exts = File.join($startdir, "platform", "shared", "ruby", "ext", "rho", "extensions.c")

  if $config["platform"] == "uwp"
    extscsharp = File.join($startdir, "platform", "uwp", "rhodes", "CSharpExtensions.cs")
    extscsharptargets = File.join($startdir, "platform", "uwp", "rhodes", "CSharpExtensions.targets")
    extscsharpcpp = File.join($startdir, "platform", "uwp", "rhoruntime", "CSharpExtensions.cpp")
  end

  puts "exts " + exts

  # deploy Common API JS implementation
  extnodejsmodulefiles = startNodeJSModules.concat( extjsmodulefiles )
  extjsmodulefiles = startJSModules.concat( extjsmodulefiles )
  extjsmodulefiles = extjsmodulefiles.concat(endJSModules)
  extjsmodulefiles_opt = startJSModules_opt.concat( extjsmodulefiles_opt )
  #
  #if !$skip_build_extensions
    if extjsmodulefiles.count > 0 || extjsmodulefiles_opt.count > 0
      rm_rf rhoapi_js_folder if Dir.exist?(rhoapi_js_folder)
      mkdir_p rhoapi_js_folder
    end
    if extnodejsmodulefiles.count > 0
      rm_rf rhoapi_nodejs_folder if Dir.exist?(rhoapi_nodejs_folder)
      mkdir_p rhoapi_nodejs_folder
    end
    #
    if !$skip_build_js_api_files
      if extjsmodulefiles.count > 0
        puts 'extjsmodulefiles=' + extjsmodulefiles.to_s if USE_TRACES
        write_modules_js(rhoapi_js_folder, "rhoapi-modules.js", extjsmodulefiles, do_separate_js_modules)

        $ebfiles_shared_rt_js_appliction = ($js_application and ($current_platform == "wm" or $current_platform == "android") and $app_config["capabilities"].index('shared_runtime'))
        if $use_shared_runtime || $ebfiles_shared_rt_js_appliction
          start_path = Dir.pwd
          chdir rhoapi_js_folder

          Dir.glob("**/*").each { |f|
            $new_name = f.to_s.dup
            $new_name.sub! 'rho', 'eb'
            cp File.join(rhoapi_js_folder, f.to_s), File.join(rhoapi_js_folder, $new_name)
          }

          chdir start_path
        end
      end
      if extnodejsmodulefiles.count > 0
        puts 'extnodejsmodulefiles=' + extnodejsmodulefiles.to_s
        write_modules_js(rhoapi_nodejs_folder, "rhoapi.js", extnodejsmodulefiles, false)
      end
      # make rhoapi-modules-ORM.js only if not shared-runtime (for WM) build
      if !$shared_rt_js_appliction
        if extjsmodulefiles_opt.count > 0
          puts 'extjsmodulefiles_opt=' + extjsmodulefiles_opt.to_s if USE_TRACES
          #write_modules_js(rhoapi_js_folder, "rhoapi-modules-ORM.js", extjsmodulefiles_opt, do_separate_js_modules)
          write_orm_modules_js(rhoapi_js_folder, extjsmodulefiles_opt)
        end
      end
    end
  #end


  if mode == "update_rho_modules_js"
    print_timestamp('init_extensions() FINISH')
    return
  end

  if $config["platform"] != "bb"
    f = StringIO.new("", "w+")
    f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
    f.puts "int rho_ruby_is_started();"

    if $config["platform"] == "wm" || $config["platform"] == "win32" || $config["platform"] == "wp8" || $config["platform"] == "uwp"
      # Add libraries through pragma
      extlibs.each do |lib|
        f.puts "#pragma comment(lib, \"#{lib}\")"
      end

      nativelib.each do |lib|
        f.puts "#pragma comment(lib, \"#{lib}\")"
      end
    end

    extentries.each do |entry|
      f.puts "extern void #{entry}(void);"
    end

    f.puts "void Init_Extensions(void) {"
    extentries_init.each do |entry|
      f.puts "    #{entry}();"
    end
    f.puts "}"

    Jake.modify_file_if_content_changed( exts, f )

    if !extscsharp.nil? && !$skip_build_extensions
      # C# extensions initialization
      f = StringIO.new("", "w+")
      f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
      f.puts "using rhoruntime;"
      f.puts "namespace rhodes {"
      f.puts "    public static class CSharpExtensions {"
      f.puts "        public static void InitializeExtensions() {"
      extcsharpentries.each do |entry|
        f.puts "            #{entry};"
      end
      f.puts "        }"
      f.puts "    }"
      f.puts "}"
      Jake.modify_file_if_content_changed( extscsharp, f )

      # C++ runtime libraries linking
      f = StringIO.new("", "w+")
      f.puts "// WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT MANUALLY!"
      extcsharplibs.each do |lib|
        f.puts "#pragma comment(lib, \"#{lib}\")"
      end
      Jake.modify_file_if_content_changed( extscsharpcpp, f )

      f = StringIO.new("", "w+")
      f.puts '<?xml version="1.0" encoding="utf-8"?>'
      f.puts '<!-- WARNING! THIS FILE IS GENERATED AUTOMATICALLY! DO NOT EDIT IT! -->'
      f.puts '<Project ToolsVersion="4.0" DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">'
      f.puts '  <PropertyGroup>'
      extcsharppaths.each do |p|
        f.puts "    #{p}"
      end
      f.puts '  </PropertyGroup>'
      extcsharpprojects.each do |p|
        f.puts "  #{p}"
      end
      f.puts '</Project>'
      Jake.modify_file_if_content_changed( extscsharptargets, f )
    end

    extlibs.each { |lib| add_linker_library(lib) }
    nativelib.each { |lib| add_linker_library(lib) }

    set_linker_flags
  end

  unless $app_config["constants"].nil?
    File.open("rhobuild.rb","w") do |file|
      file << "module RhoBuild\n"
      $app_config["constants"].each do |key,value|
        value.gsub!(/"/,"\\\"")
        file << "  #{key.upcase} = \"#{value}\"\n"
      end
      file << "end\n"
    end
  end

  if $excludeextlib and (not dest.nil?)
    chdir dest
    $excludeextlib.each {|e| Dir.glob(e).each {|f| rm f}}
  end
  print_timestamp('init_extensions() FINISH')
  #exit
end

def public_folder_cp_r(src_dir, dst_dir, level, file_map, start_path)

  return if src_dir == dst_dir

  mkdir_p dst_dir, :verbose => USE_TRACES if not File.exists? dst_dir

  Dir.foreach(src_dir) do |filename|
    next if filename.eql?('.') || filename.eql?('..')
    next if filename.eql?('api') && level == 0

    filepath = File.join(src_dir, filename)
    dst_path = File.join(dst_dir, filename)

    if File.directory?(filepath)
      public_folder_cp_r(filepath, dst_path, (level+1), file_map, start_path)
    else
      map_items = file_map.select {|f| f[:path] == filepath[start_path.size+8..-1] }

      if map_items.size > 1
        puts "WARNING, duplicate file records."
      end

      if !map_items.nil? && map_items.size != 0
        content = File.readlines(filepath)

        new_time = Digest::MD5.hexdigest(content.to_s)
        old_time = map_items[0][:hash]

        next if new_time == old_time

        puts "map_items=" + map_items.to_s if Rake.application.options.trace
        puts "new_time=" + new_time.to_s if Rake.application.options.trace
        puts "old_time=" + old_time.to_s if Rake.application.options.trace
      end

      cp filepath, dst_path, :preserve => true, :verbose => USE_TRACES
    end
  end
end

def common_bundle_start( startdir, dest)

  print_timestamp('common_bundle_start( '+startdir.to_s+' , '+dest.to_s+' ) START')

  app = $app_path

  puts $srcdir
  puts dest
  puts startdir

  #rm_rf $srcdir
  mkdir_p $srcdir
  mkdir_p dest if not File.exists? dest
  mkdir_p File.join($srcdir,'apps')

  start = pwd

  if !$js_application && !$nodejs_application
    Dir.glob('lib/framework/*').each do |f|
      cp_r(f, dest, :preserve => true) unless f.to_s == 'lib/framework/autocomplete'
    end
  end

  chdir dest
  Dir.glob("**/rhodes-framework.rb").each {|f| rm f}
  Dir.glob("**/erb.rb").each {|f| rm f}
  Dir.glob("**/find.rb").each {|f| rm f}

  $excludelib.each {|e| Dir.glob(e).each {|f| rm f}}

  chdir start
  clear_linker_settings

  init_extensions(dest)

  chdir startdir

  if File.exists? app + '/app'
    cp_r app + '/app',File.join($srcdir,'apps'), :preserve => true
  end

  file_map = Jake.build_file_map(File.join($srcdir,'apps/public'), $file_map_name, true)

  if File.exists? app + '/public'
    public_folder_cp_r app + '/public', File.join($srcdir,'apps/public'), 0, file_map, app
  end

  file_map = Jake.build_file_map(File.join($srcdir,'apps/nodejs'), $file_map_name, true)

  if File.exists? app + '/nodejs'
    public_folder_cp_r app + '/nodejs', File.join($srcdir,'apps/nodejs'), 0, file_map, app
  end


  if $app_config["app_type"] == 'rhoelements'
    $config_xml = nil
    if $app_config[$config["platform"]] &&
        $app_config[$config["platform"]]["rhoelements"] &&
        $app_config[$config["platform"]]["rhoelements"]["config"] &&
        (File.exists? File.join(app, $app_config[$config["platform"]]["rhoelements"]["config"]))

      $config_xml = File.join(app, $app_config[$config["platform"]]["rhoelements"]["config"])
    elsif $app_config["rhoelements"] && $app_config["rhoelements"]["config"] && (File.exists? File.join(app, $app_config["rhoelements"]["config"]))
      $config_xml = File.join(app, $app_config["rhoelements"]["config"])
    end
    if $current_platform == "wm"
      if !($config_xml.nil?)
        cp $config_xml, File.join($srcdir,'apps/Config.xml'), :preserve => true
      end
    end
  end

  Jake.make_rhoconfig_txt

  unless $debug
    rm_rf $srcdir + "/apps/app/SpecRunner"
    rm_rf $srcdir + "/apps/app/spec"
    rm_rf $srcdir + "/apps/app/mspec.rb"
    rm_rf $srcdir + "/apps/app/spec_runner.rb"
  end

  copy_assets($assetfolder, file_map) if ($assetfolder and File.exists? $assetfolder)

  replace_platform = $config['platform']
  replace_platform = "bb6" if $bb6

  [File.join($srcdir,'apps'), ($current_platform == "bb" ? File.join($srcdir,'res') : File.join($srcdir,'lib/res'))].each do |folder|
    next unless Dir.exists? folder
    chdir folder

    Dir.glob("**/*.#{replace_platform}.*").each do |file|
      oldfile = file.gsub(Regexp.new(Regexp.escape('.') + replace_platform + Regexp.escape('.')),'.')
      rm oldfile if File.exists? oldfile
      mv file,oldfile
    end

    Dir.glob("**/*.wm.*").each { |f| rm f }
    Dir.glob("**/*.win32.*").each { |f| rm f }
    Dir.glob("**/*.wp.*").each { |f| rm f }
    Dir.glob("**/*.wp8.*").each { |f| rm f }
    Dir.glob("**/*.uwp.*").each { |f| rm f }
    Dir.glob("**/*.sym.*").each { |f| rm f }
    Dir.glob("**/*.iphone.*").each { |f| rm f }
    Dir.glob("**/*.bb.*").each { |f| rm f }
    Dir.glob("**/*.bb6.*").each { |f| rm f }
    Dir.glob("**/*.android.*").each { |f| rm f }
    Dir.glob("**/.svn").each { |f| rm_rf f }
    Dir.glob("**/CVS").each { |f| rm_rf f }
  end
  print_timestamp('common_bundle_start() FINISH')
end #end of common_bundle_start

def create_manifest
  require File.dirname(__FILE__) + '/lib/framework/rhoappmanifest'

  if Dir.exists? File.join($srcdir, 'apps/app')
    fappManifest = Rho::AppManifest.enumerate_models(File.join($srcdir, 'apps/app'))
    content = fappManifest.read();
  else
    content = ""
  end

  File.open( File.join($srcdir,'apps/app_manifest.txt'), "w"){|file| file.write(content)}
end

def process_exclude_folders(excluded_dirs=[])

  excl = excluded_dirs

  exclude_platform = $config['platform']
  exclude_platform = "bb6" if $bb6
  #exclude_platform = "wm" if exclude_platform == 'win32'

  if $app_config["excludedirs"]
    excl += $app_config["excludedirs"]['all'] if $app_config["excludedirs"]['all']
    excl += $app_config["excludedirs"][exclude_platform] if $app_config["excludedirs"][exclude_platform]
  end

  if  $config["excludedirs"]
    excl += $config["excludedirs"]['all'] if $config["excludedirs"]['all']
    excl += $config["excludedirs"][exclude_platform] if $config["excludedirs"][exclude_platform]
  end

  if excl.size() > 0
    chdir File.join($srcdir)#, 'apps')

    excl.each do |mask|
      Dir.glob(mask).each {|f| puts "f: #{f}"; rm_rf f}
    end

    chdir File.join($srcdir, 'apps')

    excl.each do |mask|
      Dir.glob(mask).each {|f| puts "f: #{f}"; rm_rf f}
    end

  end

end

def get_extensions
  value = ENV['rho_extensions']
  return value.split(',') if value
  return []
end

#------------------------------------------------------------------------

namespace "build" do
  task :set_version, :version do |t, args|
    version = args[:version]

    rhodes_dir = File.dirname(__FILE__)

    File.open(File.join(rhodes_dir, 'version'), 'wb') { |f| f.write(version) }
  end

  namespace "bundle" do


    desc "Generate AES key for bundle encryption"
    task :generate_AES_key do
        generated_key = Jake.generate_AES_key
        puts 'Generated AES key is : "'+generated_key+'"'
    end

    task :prepare_native_generated_files do

      currentdir = Dir.pwd()

      chdir $startdir

      Rake::Task["app:build_bundle"].invoke if $app_rakefile_exist

      app = $app_path
      rhodeslib = File.dirname(__FILE__) + "/lib/framework"
      compileERB = "lib/build/compileERB/default.rb"
      compileRB = "lib/build/compileRB/compileRB.rb"
      startdir = pwd
      dest = $srcdir + "/lib"

      common_bundle_start(startdir,dest)

      Rake::Task["app:after_build_bundle"].invoke if $app_rakefile_exist

      Dir.chdir currentdir
    end

    task :xruby do

      if $js_application || $nodejs_application
        return
      end

      print_timestamp('build:bundle:xruby START')

      #needs $config, $srcdir, $excludelib, $bindir
      app = $app_path
      jpath = $config["env"]["paths"]["java"]
      startdir = pwd
      dest =  $srcdir
      xruby =  File.dirname(__FILE__) + '/res/build-tools/xruby-0.3.3.jar'
      compileERB = "lib/build/compileERB/bb.rb"
      rhodeslib = File.dirname(__FILE__) + "/lib/framework"

      common_bundle_start(startdir, dest)

      process_exclude_folders()
      cp_r File.join(startdir, "platform/shared/db/res/db"), File.join($srcdir, 'apps')

      chdir startdir

      #create manifest
      create_manifest

      cp   compileERB, $srcdir
      puts "Running bb.rb"

      cmd_str = "#{$rubypath} -I#{rhodeslib} #{$srcdir}/bb.rb"
      if defined?(Bundler)
        Bundler.with_clean_env do
          puts `#{cmd_str}`
        end
      else
        puts `#{cmd_str}`
      end
      unless $? == 0
        puts "Error interpreting erb code"
        exit 1
      end

      rm "#{$srcdir}/bb.rb"

      chdir $bindir
      # -n#{$bundleClassName}
      output = `java -jar "#{xruby}" -v -c RhoBundle 2>&1`
      output.each_line { |x| puts ">>> " + x  }
      unless $? == 0
        puts "Error interpreting ruby code"
        exit 1
      end
      chdir startdir
      chdir $srcdir

      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }
=begin
      # RubyIDContainer.* files takes half space of jar why we need it?
      Jake.unjar("../RhoBundle.jar", $tmpdir)
      Dir.glob($tmpdir + "/**/RubyIDContainer.class") { |f| rm f }
      rm "#{$bindir}/RhoBundle.jar"
      chdir $tmpdir
      puts `jar cf #{$bindir}/RhoBundle.jar #{$all_files_mask}`
      rm_rf $tmpdir
      mkdir_p $tmpdir
      chdir $srcdir
=end

      Jake.build_file_map($srcdir, $file_map_name)

      puts `"#{File.join(jpath,'jar')}" uf ../RhoBundle.jar apps/#{$all_files_mask}`
      unless $? == 0
        puts "Error creating Rhobundle.jar"
        exit 1
      end
      chdir startdir

      print_timestamp('build:bundle:xruby FINISH')

    end

    # its task for compiling ruby code in rhostudio
    # TODO: temporary fix I hope. This code is copied from line 207 of this file
    task :rhostudio => ["config:wm"] do

      if RUBY_PLATFORM =~ /(win|w)32$/
        $all_files_mask = "*.*"
        $rubypath = "res/build-tools/RhoRuby.exe"
      else
        $all_files_mask = "*"
        if RUBY_PLATFORM =~ /darwin/
          $rubypath = "res/build-tools/RubyMac"
        else
          $rubypath = "res/build-tools/rubylinux"
        end
      end

      Rake::Task["build:bundle:noxruby"].invoke

      Jake.build_file_map( File.join($srcdir, "apps"), "rhofilelist.txt" )
    end

    def cp_if(src_dir, dst_dir)
      puts src_dir

      if !Dir.exist? src_dir
        return
      end

      chdir src_dir

      Dir.glob("**") { |f|
        #puts 'f=' + f.to_s

        if File.file? File.join(src_dir, f.to_s)
          f_path        = File.join(src_dir, f.to_s)
          src_file_time = File.mtime f_path
          dst_file      = File.join(dst_dir, f.to_s)

          if File.exist? dst_file
            dst_file_time = File.mtime dst_file.to_s

            if src_file_time < dst_file_time
              next
            end
          end

          cp f_path, dst_dir
        else
          new_dst_dir = File.join(dst_dir, f.to_s)

          if !Dir.exist? new_dst_dir
            mkdir new_dst_dir
          end

          cp_if(File.join(src_dir, f.to_s), new_dst_dir)
        end
      }
    end

    task :noxruby, :exclude_dirs do |t, args|
      exclude_dirs = args[:exclude_dirs]
      excluded_dirs = []
      if (!exclude_dirs.nil?) && (exclude_dirs !~ /^\s*$/)
        excluded_dirs = exclude_dirs.split(':')
      end

      Rake::Task["app:build_bundle"].invoke if $app_rakefile_exist

      app = $app_path
      rhodeslib = File.dirname(__FILE__) + "/lib/framework"
      compileERB = "lib/build/compileERB/default.rb"
      compileRB = "lib/build/compileRB/compileRB.rb"
      startdir = pwd
      dest = $srcdir + "/lib"

      common_bundle_start(startdir,dest)
      process_exclude_folders(excluded_dirs)
      chdir startdir

      if !$js_application && !$nodejs_application

        create_manifest
        cp compileERB, $srcdir
        puts "Running default.rb"
        cmd_str = "#{$rubypath} -E UTF-8 -I#{rhodeslib} #{$srcdir}/default.rb"
        puts cmd_str
        if defined?(Bundler)
          Bundler.with_clean_env do
            puts `#{cmd_str}`
          end
        else
          puts `#{cmd_str}`
        end        
        unless $? == 0
          puts "Error interpreting erb code"
          exit 1
        end

        rm "#{$srcdir}/default.rb"

        cp   compileRB, $srcdir
        puts "Running compileRB"
        cmd_str = "#{$rubypath} -E UTF-8 -I#{rhodeslib} #{$srcdir}/compileRB.rb"
        if defined?(Bundler)
          Bundler.with_clean_env do
            puts `#{cmd_str}`
          end
        else
          puts `#{cmd_str}`
        end
        unless $? == 0
          puts "Error interpreting ruby code"
          exit 1
        end
      end

      # change modification time for improvement of performance on WM platform
      Find.find($srcdir) do |path|
        atime = File.atime(path.to_s) # last access time
        mtime = File.mtime(path.to_s) # modification time
        fName   = nil

        if File.extname(path) == ".rb"
          newName = File.basename(path).sub('.rb','.iseq')
          fName = File.join(File.dirname(path), newName)
        end

        if File.extname(path) == ".erb"
          newName = File.basename(path).sub('.erb','_erb.iseq')
          fName = File.join(File.dirname(path), newName)
        end

        File.utime(atime, mtime, fName) unless fName.nil? || !File.exist?(fName)
      end

      chdir $srcdir
      Dir.glob("**/*.rb") { |f| rm f }
      Dir.glob("**/*.erb") { |f| rm f }

      if !$skip_build_extensions
        if not $minify_types.empty?
          minify_js_and_css($srcdir,$minify_types)
        end
      end

      #encrypt files
      if $encrypt_aes_key != nil
          Jake.encrypt_files_by_AES($srcdir, $encrypt_aes_key, $app_config["encrypt_file_extensions"])
      end


      chdir startdir
      cp_r "platform/shared/db/res/db", File.join($srcdir, "db")

      # create bundle map file with the final information
      Jake.build_file_map($srcdir, $file_map_name)

      Rake::Task["app:after_build_bundle"].invoke if $app_rakefile_exist
    end # end of noxruby

    def is_exclude_folder(excludes, filename)
      return false if !excludes || !filename

      excludes.each do |excl|
        return true if filename.index(excl)
      end

      return false
    end

    def minify_js_and_css(dir,types)
      pattern = types.join(',')

      files_to_minify = []

      Dir.glob( File.join(dir,'**',"*.{#{pattern}}") ) do |f|
        if File.file?(f) and !File.fnmatch("*.min.*",f)
          next if is_exclude_folder($obfuscate_exclude, f )
          next if is_exclude_folder( $minify_exclude, f )

          ext = File.extname(f)

          if (ext == '.js') or (ext == '.css') then
            files_to_minify << f
          end

        end
      end

      minify_inplace_batch(files_to_minify) if files_to_minify.length>0
    end

    def minify_inplace_batch(files_to_minify)
      puts "minifying file list: #{files_to_minify}" if USE_TRACES

      cmd = "java -jar #{$minifier} -o \"x$:x\""

      files_to_minify.each { |f| cmd += " #{f}" }

      require 'open3'

      status = nil
      error = nil

      begin
        Open3.popen3(cmd) do |stdin, stdout, stderr, wait_thr|
          output = stdout.read
          error = stderr.read
          status = wait_thr.value
        end
      rescue Exception => e
        puts "Minify error: #{e.inspect}"
        error = e.inspect
      end

      puts "Minification done: #{status}" if USE_TRACES

      if !status || !status.exitstatus.zero?
        puts "WARNING: Minification error!" if USE_TRACES
        error = output if error.nil?
        BuildOutput.warning(["Minification errors occured. Minificator stderr output: \n" + error], 'Minification error')
      end
    end

    def minify_inplace(filename,type)
      puts "minify file: #{filename}" if USE_TRACES

      f = StringIO.new("", "w+")
      f.write(File.read(filename))
      f.rewind()

      require 'open3'
      f.rewind()
      fc = StringIO.new("","w+")

      output = true
      status = nil
      error = nil
      begin

        Open3.popen3("java","-jar","#{$minifier}","--type","#{type}") do |stdin, stdout, stderr, wait_thr|
          stdin.binmode

          while buffer = f.read(4096)
            stdin.write(buffer)
          end
          f.close
          stdin.close

          output = stdout.read
          error = stderr.read

          status = wait_thr.value
        end

      rescue Exception => e
        puts "Minify error: #{e.inspect}"
        error = e.inspect
        #raise e
      end

      if !status || !status.exitstatus.zero?
        puts "WARNING: Minification error!" if USE_TRACES

        error = output if error.nil?

        BuildOutput.warning(['Failed to minify ' + filename, 'Output: ' + error], 'Minification error')

        output = File.read(filename)
        #exit 1
      end

      fc.puts(output)
      Jake.modify_file_if_content_changed(filename, fc)
      #File.open(filename, "w"){|file| file.write(f.read())}
    end

    task :upgrade_package do

      $bindir = File.join($app_path, 'bin')
      $current_platform = 'empty'
      $srcdir = File.join($bindir, 'RhoBundle')

      $targetdir = File.join($bindir, 'target')
      $excludelib = ['**/builtinME.rb', '**/ServeME.rb', '**/dateME.rb', '**/rationalME.rb']
      $tmpdir = File.join($bindir, 'tmp')
      $appname = $app_config['name']
      $appname = 'Rhodes' if $appname.nil?
      $vendor = $app_config['vendor']
      $vendor = 'rhomobile' if $vendor.nil?
      $vendor = $vendor.gsub(/^[^A-Za-z]/, '_').gsub(/[^A-Za-z0-9]/, '_').gsub(/_+/, '_').downcase
      $appincdir = (File.join $tmpdir, 'include')

      Rake::Task["config:common"].invoke

      Rake::Task["build:bundle:noxruby"].invoke

      new_zip_file = File.join($srcdir, 'apps', 'upgrade_bundle.zip')

      if RUBY_PLATFORM =~ /(win|w)32$/

        require 'rubygems'
        require 'zip'
        require 'find'
        require 'fileutils'
        include FileUtils

        root = $srcdir
        new_zip_file = File.join($srcdir, 'upgrade_bundle.zip')
        (Zip::ZipFile rescue Zip::File).open(new_zip_file, Zip::File::CREATE) do |zipfile|
          Find.find(root) do |path|
            Find.prune if File.basename(path)[0] == ?.
            dest = /apps\/(\w.*)/.match(path)
            if dest
              puts '     add file to zip : ' + dest[1].to_s
              zipfile.add(dest[1], path)
            end
          end
        end

      else
        chdir File.join($srcdir, 'apps')
        sh 'zip -r upgrade_bundle.zip .'
      end

      cp new_zip_file, $bindir
      rm new_zip_file
    end

    task :noiseq do
      app = $app_path
      rhodeslib = File.dirname(__FILE__) + "/lib/framework"
      compileERB = "lib/build/compileERB/bb.rb"
      startdir = pwd
      dest = $srcdir + "/lib"

      common_bundle_start(startdir,dest)
      process_exclude_folders
      chdir startdir

      create_manifest

      cp compileERB, $srcdir
      puts "Running bb.rb"

      puts `#{$rubypath} -I#{rhodeslib} "#{$srcdir}/bb.rb"`
      unless $? == 0
        puts "Error interpreting erb code"
        exit 1
      end

      rm "#{$srcdir}/bb.rb"

      chdir $srcdir
      Dir.glob("**/*.erb") { |f| rm f }

      chdir startdir
      cp_r "platform/shared/db/res/db", $srcdir
    end
  end
end

task :get_ext_xml_paths, [:platform] do |t,args|
  throw "You must pass in platform(win32, wm, android, iphone, wp8, uwp)" if args.platform.nil?

  $current_platform = args.platform
  $current_platform_bridge = args.platform

  Rake::Task["config:common"].invoke

  res_xmls = init_extensions( nil, "get_ext_xml_paths")

  puts res_xmls
end

desc "Generate rhoapi-modules.js file with coreapi and javascript parts of extensions"
task :update_rho_modules_js, [:platform] do |t,args|
  throw "You must pass in platform(win32, wm, android, iphone, wp8, uwp, all)" if args.platform.nil?

  $current_platform = args.platform
  $current_platform = 'wm' if args.platform == 'all'
  $current_platform_bridge = args.platform

  Rake::Task["config:common"].invoke

  init_extensions( nil, "update_rho_modules_js")

  minify_inplace( File.join( $app_path, "public/api/rhoapi-modules.js" ), "js" ) if $minify_types.include?('js')

  if !$shared_rt_js_appliction
    minify_inplace( File.join( $app_path, "public/api/rhoapi-modules-ORM.js" ), "js" ) if $minify_types.include?('js')
    minify_inplace( File.join( $app_path, "public/api/rhoapi-modules-ORMHelper.js" ), "js" ) if $minify_types.include?('js')
    minify_inplace( File.join( $app_path, "public/api/rhoapi-modules-Ruby-RunTime.js" ), "js" ) if $minify_types.include?('js')
    minify_inplace( File.join( $app_path, "public/api/rhoapi-modules-Rhosim-Fix.js" ), "js" ) if $minify_types.include?('js')
  end
end

# Simple rakefile that loads subdirectory 'rhodes' Rakefile
# run "rake -T" to see list of available tasks

#desc "Get versions"
task :get_version do

  #genver = "unknown"
  iphonever = "unknown"
  #symver = "unknown"
  wmver = "unknown"
  androidver = "unknown"

  # File.open("res/generators/templates/application/build.yml","r") do |f|
  #     file = f.read
  #     if file.match(/version: (\d+\.\d+\.\d+)/)
  #       genver = $1
  #     end
  #   end

  File.open("platform/iphone/Info.plist","r") do |f|
    file = f.read
    if file.match(/CFBundleVersion<\/key>\s+<string>(\d+\.\d+\.*\d*)<\/string>/)
      iphonever =  $1
    end
  end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") do |f|
    file = f.read
    if file.match(/versionName="(\d+\.\d+\.*\d*)"/)
      androidver =  $1
    end
  end

  gemver = "unknown"
  rhodesver = "unknown"
  frameworkver = "unknown"

  File.open("lib/rhodes.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      gemver =  $1
    end
  end

  File.open("lib/framework/rhodes.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      rhodesver =  $1
    end
  end

  File.open("lib/framework/version.rb","r") do |f|
    file = f.read
    if file.match(/VERSION = '(\d+\.\d+\.*\d*)'/)
      frameworkver =  $1
    end
  end

  puts "Versions:"
  #puts "  Generator:        " + genver
  puts "  iPhone:           " + iphonever
  #puts "  WinMo:            " + wmver
  puts "  Android:          " + androidver
  puts "  Gem:              " + gemver
  puts "  Rhodes:           " + rhodesver
  puts "  Framework:        " + frameworkver
end

#desc "Set version"
task :set_version, [:version] do |t,args|
  throw "You must pass in version" if args.version.nil?
  ver = args.version.split(/\./)
  major = ver[0]
  minor = ver[1]
  build = ver[2]

  throw "Invalid version format. Must be in the format of: major.minor.build" if major.nil? or minor.nil? or build.nil?

  verstring = major+"."+minor+"."+build
  origfile = ""

  # File.open("res/generators/templates/application/build.yml","r") { |f| origfile = f.read }
  #   File.open("res/generators/templates/application/build.yml","w") do |f|
  #     f.write origfile.gsub(/version: (\d+\.\d+\.\d+)/, "version: #{verstring}")
  #   end

  File.open("platform/iphone/Info.plist","r") { |f| origfile = f.read }
  File.open("platform/iphone/Info.plist","w") do |f|
    f.write origfile.gsub(/CFBundleVersion<\/key>(\s+)<string>(\d+\.\d+\.*\d*)<\/string>/, "CFBundleVersion</key>\n\t<string>#{verstring}</string>")
  end

  File.open("platform/android/Rhodes/AndroidManifest.xml","r") { |f| origfile = f.read }
  File.open("platform/android/Rhodes/AndroidManifest.xml","w") do |f|
    origfile.match(/versionCode="(\d+)"/)
    vercode = $1.to_i + 1
    origfile.gsub!(/versionCode="(\d+)"/,"versionCode=\"#{vercode}\"")
    origfile.gsub!(/versionName="(\d+\.\d+\.*\d*)"/,"versionName=\"#{verstring}\"")

    f.write origfile
  end
  ["lib/rhodes.rb","lib/framework/rhodes.rb","lib/framework/version.rb"].each do |versionfile|

    File.open(versionfile,"r") { |f| origfile = f.read }
    File.open(versionfile,"w") do |f|
      origfile.gsub!(/^(\s*VERSION) = '(\d+\.\d+\.*\d*)'/, '\1 = \''+ verstring + "'")
      f.write origfile
    end
  end

  Rake::Task[:get_version].invoke
end

#------------------------------------------------------------------------

namespace "buildall" do
  namespace "bb" do
    #    desc "Build all jdk versions for blackberry"
    task :production => "config:common" do
      $config["env"]["paths"].each do |k,v|
        if k.to_s =~ /^4/
          puts "BUILDING VERSION: #{k}"
          $app_config["bbver"] = k
          #          Jake.reconfig($config)

          #reset all tasks used for building
          Rake::Task["config:bb"].reenable
          Rake::Task["build:bb:rhobundle"].reenable
          Rake::Task["build:bb:rhodes"].reenable
          Rake::Task["build:bb:rubyvm"].reenable
          Rake::Task["device:bb:dev"].reenable
          Rake::Task["device:bb:production"].reenable
          Rake::Task["device:bb:rhobundle"].reenable
          Rake::Task["package:bb:dev"].reenable
          Rake::Task["package:bb:production"].reenable
          Rake::Task["package:bb:rhobundle"].reenable
          Rake::Task["package:bb:rhodes"].reenable
          Rake::Task["package:bb:rubyvm"].reenable
          Rake::Task["device:bb:production"].reenable
          Rake::Task["clean:bb:preverified"].reenable

          Rake::Task["clean:bb:preverified"].invoke
          Rake::Task["device:bb:production"].invoke
        end
      end

    end
  end
end

task :gem do
  puts "Removing old gem"
  rm_rf Dir.glob("rhodes*.gem")
  puts "Copying Rakefile"
  cp "Rakefile", "rakefile.rb"

  puts "Building manifest"
  out = ""
  Dir.glob("**/*") do |fname|
    # TODO: create exclusion list
    next unless File.file? fname
    next if fname =~ /rhoconnect-client/
    next if fname =~ /^spec\/api_generator_spec/
    next if fname =~ /ruby-standalone/

    out << fname + "\n"
  end
  File.open("Manifest.txt",'w') {|f| f.write(out)}

  puts "Building gem"
  Jake.run3('gem build rhodes.gemspec')
end

namespace "rhomobile-debug" do
  task :gem do
    puts "Removing old gem"
    rm_rf Dir.glob("rhomobile-debug*.gem")
    rm_rf "rhomobile-debug"

    mkdir_p "rhomobile-debug"
    mkdir_p "rhomobile-debug/lib"
    cp 'lib/extensions/debugger/debugger.rb', "rhomobile-debug/lib", :preserve => true
    cp 'lib/extensions/debugger/README.md', "rhomobile-debug", :preserve => true
    cp 'lib/extensions/debugger/LICENSE', "rhomobile-debug", :preserve => true
    cp 'lib/extensions/debugger/CHANGELOG', "rhomobile-debug", :preserve => true

    cp 'rhomobile-debug.gemspec', "rhomobile-debug", :preserve => true

    startdir = pwd
    chdir 'rhomobile-debug'

    puts "Building gem"
    Jake.run3('gem build rhomobile-debug.gemspec')

    Dir.glob("rhomobile-debug*.gem").each do |f|
      cp f, startdir, :preserve => true
    end

    chdir startdir
    rm_rf "rhomobile-debug"
  end
end

task :tasks do
  Rake::Task.tasks.each {|t| puts t.to_s.ljust(27) + "# " + t.comment.to_s}
end

task :switch_app do
  puts "Preparing rhobuild.yml"
  rhobuildyml = File.dirname(__FILE__) + "/rhobuild.yml"
  if File.exists? rhobuildyml
    config = YAML::load_file(rhobuildyml)
  else
    puts "Cant find rhobuild.yml"
    exit 1
  end
  config["env"]["app"] = $app_path.gsub(/\\/,"/")
  File.open(  rhobuildyml, 'w' ) do |out|
    YAML.dump( config, out )
  end
end

#Rake::RDocTask.new do |rd|
#RDoc::Task.new do |rd|
#  rd.main = "README.textile"
#  rd.rdoc_files.include("README.textile", "lib/framework/**/*.rb")
#end
#Rake::Task["rdoc"].comment=nil
#Rake::Task["rerdoc"].comment=nil

#task :rdocpush => :rdoc do
#  puts "Pushing RDOC. This may take a while"
#  `scp -r html/* dev@dev.rhomobile.com:dev.rhomobile.com/rhodes/`
#end

#------------------------------------------------------------------------

namespace "build" do
  #    desc "Build rhoconnect-client package"
  task :rhoconnect_client do

    ver = File.read("rhoconnect-client/version").chomp #.gsub(".", "_")
    zip_name = "rhoconnect-client-"+ver+".zip"

    bin_dir = "rhoconnect-client-bin"
    src_dir = bin_dir + "/rhoconnect-client-"+ver #"/src"
    shared_dir = src_dir + "/platform/shared"
    rm_rf bin_dir
    rm    zip_name if File.exists? zip_name
    mkdir_p bin_dir
    mkdir_p src_dir

    cp_r 'rhoconnect-client', src_dir, :preserve => true

    mv src_dir+"/rhoconnect-client/license", src_dir
    mv src_dir+"/rhoconnect-client/README.textile", src_dir
    mv src_dir+"/rhoconnect-client/version", src_dir
    mv src_dir+"/rhoconnect-client/changelog", src_dir

    Dir.glob(src_dir+"/rhoconnect-client/**/*").each do |f|
      #puts f

      rm_rf f if f.index("/build/") || f.index(".DS_Store")

    end

    mkdir_p shared_dir

    Dir.glob("platform/shared/*").each do |f|
      next if f == "platform/shared/ruby" || f == "platform/shared/rubyext" || f == "platform/shared/xruby" || f == "platform/shared/shttpd" ||
        f == "platform/shared/stlport"  || f == "platform/shared/qt"
      #puts f
      cp_r f, shared_dir #, :preserve => true
    end
    startdir = pwd
    chdir bin_dir
    puts `zip -r #{File.join(startdir, zip_name)} *`

    chdir startdir

    rm_rf bin_dir
  end
end

#------------------------------------------------------------------------

namespace "run" do

  task :rhoconnect_push_spec do
    require 'mspec'

    Dir.chdir( File.join(File.dirname(__FILE__),'spec','server_spec'))
    MSpec.register_files [ 'rhoconnect_push_spec.rb' ]

    MSpec.process
    MSpec.exit_code
  end

  namespace :rhosimulator do

    task :build => ["config:common"] do

      RhoPackages.request 'rhosimulator'

      puts "rho_reload_app_changes : #{ENV['rho_reload_app_changes']}"

      $appname = $app_config["name"].nil? ? "Rhodes" : $app_config["name"]

      sim_conf = "rhodes_path='#{$startdir}'\r\n"
      sim_conf += "app_version='#{$app_config["version"]}'\r\n"
      sim_conf += "app_name='#{$appname}'\r\n"
      if ( ENV['rho_reload_app_changes'] )
        sim_conf += "reload_app_changes=#{ENV['rho_reload_app_changes']}\r\n"
      else
        sim_conf += "reload_app_changes=1\r\n"
      end

      if $config['debug']
        sim_conf += "debug_port=#{$config['debug']['port']}\r\n"
      else
        sim_conf += "debug_port=\r\n"
      end

      if $config['debug'] && $config['debug']['host'] && $config['debug']['host'].length() > 0
        sim_conf += "debug_host='#{$config['debug']['host']}'\r\n"
      else
        sim_conf += "debug_host='127.0.0.1'\r\n"
      end

      sim_conf += $rhosim_config if $rhosim_config

      #check gem extensions
      config_ext_paths = ""
      extpaths = $app_config["extpaths"]
      extjsmodulefiles = []
      extjsmodulefiles_opt = []
      startJSModules = []
      startJSModules_opt = []
      endJSModules = []

      rhoapi_js_folder = File.join( $app_path, "public/api" )
      puts "rhoapi_js_folder: #{rhoapi_js_folder}" if USE_TRACES

      do_separate_js_modules = Jake.getBuildBoolProp("separate_js_modules", $app_config, false)

      # TODO: checker init
      gen_checker = GeneratorTimeChecker.new
      gen_checker.init($startdir, $app_path)

      $app_config["extensions"].each do |extname|

        extpath = nil
        extpaths.each do |p|
          ep = File.join(p, extname)
          if File.exists? ep
            extpath = ep
            break
          end
        end

        if extpath.nil?
          begin
            $rhodes_extensions = nil
            $rhodes_join_ext_name = false
            require extname

            if $rhodes_extensions
              extpath = $rhodes_extensions[0]
              if $rhodes_join_ext_name
                extpath = File.join(extpath,extname)
              end
            end

            config_ext_paths += "#{extpath};" if extpath && extpath.length() > 0
          rescue Exception => e
          end
        else

          # if $config["platform"] != "bb"
          #     extyml = File.join(extpath, "ext.yml")
          #     next if File.file? extyml
          # end

          config_ext_paths += "#{extpath};" if extpath && extpath.length() > 0
        end

        if extpath && extpath.length() > 0
          extyml = File.join(extpath, "ext.yml")
          puts "extyml " + extyml

          if File.file? extyml
            extconf = Jake.config(File.open(extyml))
            type = Jake.getBuildProp( "exttype", extconf )
            xml_api_paths  = extconf["xml_api_paths"]
            templates_path = File.join($startdir, "res", "generators", "templates")

            if xml_api_paths && type != "prebuilt"
              xml_api_paths = xml_api_paths.split(',')

              xml_api_paths.each do |xml_api|
                xml_path = File.join(extpath, xml_api.strip())

                #TODO checker check
                if gen_checker.check(xml_path)
                  #                      puts 'ruuuuuuuun generatooooooooooooor'
                  cmd_line = "#{$startdir}/bin/rhogen api #{xml_path}"
                  puts "cmd_line: #{cmd_line}"
                  system "#{cmd_line}"
                end
              end
            end
          end

          # TODO: RhoSimulator should look for 'public' at all extension folders!
          unless rhoapi_js_folder.nil?
            Dir.glob(extpath + "/public/api/*.js").each do |f|
              fBaseName = File.basename(f)
              if (fBaseName.start_with?("rhoapi-native") )
                endJSModules << f if fBaseName == "rhoapi-native.all.js"
                next
              end
              if (fBaseName == "rhoapi-force.ajax.js")
                add = Jake.getBuildBoolProp("ajax_api_bridge", $app_config, false)
                add = Jake.getBuildBoolProp2($current_platform, "ajax_api_bridge", $app_config, add)
                endJSModules << f if add
                next
              end
              if (fBaseName == "#{extname}-postDef.js")
                puts "add post-def module: #{f}"
                endJSModules << f
              end

              if f.downcase().end_with?("jquery-2.0.2-rho-custom.min.js")
                startJSModules.unshift(f)
              elsif f.downcase().end_with?("rhoapi.js")
                startJSModules << f
              elsif f.downcase().end_with?("rhonodejsapi.js")
                puts "ignore honodejsapi.js"
              elsif f.downcase().end_with?("rho.application.js")
                endJSModules << f
              elsif f.downcase().end_with?("rho.database.js")
                endJSModules << f
              elsif f.downcase().end_with?("rho.newormhelper.js")
                endJSModules << f #if $current_platform == "android" || $current_platform == "iphone" || $current_platform == "wm"
              elsif /(rho\.orm)|(rho\.ruby\.runtime)|(rho\.rhosim\.fix)/i.match(f.downcase())
                puts "add #{f} to startJSModules_opt.."
                startJSModules_opt << f
              else
                extjsmodulefiles << f
              end
            end
            Dir.glob(extpath + "/public/api/generated/*.js").each do |f|
              if /(rho\.orm)|(rho\.ruby\.runtime)|(rho\.rhosim\.fix)/i.match(f.downcase())
                puts "add #{f} to extjsmodulefiles_opt.." if USE_TRACES
                extjsmodulefiles_opt << f
              else
                puts "add #{f} to extjsmodulefiles.." if USE_TRACES
                extjsmodulefiles << f
              end
            end

          end

        end
      end

      #TODO: checker update
      gen_checker.update

      # deploy Common API JS implementation
      extjsmodulefiles = startJSModules.concat( extjsmodulefiles )
      extjsmodulefiles = extjsmodulefiles.concat(endJSModules)
      extjsmodulefiles_opt = startJSModules_opt.concat( extjsmodulefiles_opt )
      #
      if extjsmodulefiles.count > 0 || extjsmodulefiles_opt.count > 0
        rm_rf rhoapi_js_folder if Dir.exist?(rhoapi_js_folder)
        mkdir_p rhoapi_js_folder
      end
      #
      if extjsmodulefiles.count > 0
        puts "extjsmodulefiles: #{extjsmodulefiles}" if USE_TRACES
        write_modules_js(rhoapi_js_folder, "rhoapi-modules.js", extjsmodulefiles, do_separate_js_modules)
      end
      #
      if extjsmodulefiles_opt.count > 0
        puts "extjsmodulefiles_opt: #{extjsmodulefiles_opt}" if USE_TRACES
        #write_modules_js(rhoapi_js_folder, "rhoapi-modules-ORM.js", extjsmodulefiles_opt, do_separate_js_modules)
        write_orm_modules_js(rhoapi_js_folder, extjsmodulefiles_opt)
      end

      sim_conf += "ext_path=#{config_ext_paths}\r\n" if config_ext_paths && config_ext_paths.length() > 0

      security_token = $app_config["security_token"]
      sim_conf += "security_token=#{security_token}\r\n" if !security_token.nil?

      fdir = File.join($app_path, 'rhosimulator')
      mkdir fdir unless File.exist?(fdir)

      Jake.get_config_override_params.each do |key, value|
        if key != 'start_path'
          puts "Override '#{key}' is not supported."
          next
        end
        sim_conf += "#{key}=#{value}\r\n"
      end

      if !(sim_conf.include? "platform=") || (sim_conf.include? "platform='UNKNOWN'")
        sim_conf += "platform=#{RUBY_PLATFORM}\r\n"
      end

      fname = File.join(fdir, 'rhosimconfig.txt')
      File.open(fname, "wb") do |fconf|
        fconf.write( sim_conf )
      end
    end

    task :run, [:wait] => ["config:common"] do |t, args|
      wait = args[:wait] == 'wait'

      RhoPackages.request 'rhosimulator'

      if $js_application
        args = ["-jsapproot='#{$app_path}'", "-rhodespath='#{$startdir}'"]
      else
        args = ["-approot='#{$app_path}'", "-rhodespath='#{$startdir}'"]
      end

      args << "-security_token=#{ENV['security_token']}" if !ENV['security_token'].nil?

      path = ''
      cmd = ''
      if RUBY_PLATFORM =~ /(win|w)32$/
        if $config['env']['paths']['rhosimulator'] and $config['env']['paths']['rhosimulator'].length() > 0
          path = File.join( $config['env']['paths']['rhosimulator'], "rhosimulator.exe" )
        else
          path = File.join( $startdir, "platform/win32/RhoSimulator/rhosimulator.exe" )
        end

        oldDir = File.join( $startdir, "platform/win32/RhoSimulator" )
        newDir = oldDir
        #newDir = File.join( $startdir, "platform/win32/RhoSimulatorRunnable" )

        #rm_rf newDir if Dir.exist?(newDir)
        #FileUtils.mkpath newDir

        #cp_r File.join(oldDir, "."), newDir

        qtdir = ENV['QTDIR']
        if !qtdir.nil?
          cp File.join(qtdir, "bin/Qt5Core.dll"), newDir
          args << "-remote-debugging-port=9090"
        end

        cmd = File.join(newDir, 'rhosimulator.exe')
      elsif RUBY_PLATFORM =~ /darwin/
        if $config['env']['paths']['rhosimulator'] and $config['env']['paths']['rhosimulator'].length() > 0
          path = File.join( $config['env']['paths']['rhosimulator'], "RhoSimulator.app" )
        else
          path = File.join( $startdir, "platform/osx/bin/RhoSimulator/RhoSimulator.app" )
        end
        cmd = 'open'
        args.unshift(path, '--args')
        args.unshift('-W') if wait
      else
        args << ">/dev/null"
        args << "2>/dev/null"
      end

      if !File.exists?(path)
        puts "Cannot find RhoSimulator: '#{path}' does not exists"
        puts "Check sdk path in build.yml - it should point to latest rhodes (run 'set-rhodes-sdk' in application folder) OR"

        if $config['env']['paths']['rhosimulator'] and $config['env']['paths']['rhosimulator'].length() > 0
          puts "Check 'env:paths:rhosimulator' path in '<rhodes>/rhobuild.yml' OR"
        end

        puts "Install Rhodes gem OR"
        puts "Install RhoSimulator and modify 'env:paths:rhosimulator' section in '<rhodes>/rhobuild.yml'"
        exit 1
      end

      puts 'start rhosimulator'
      xwait = false # waiting on Mac OS X is controlled other way. Examine this task.
      xwait = wait if RUBY_PLATFORM =~ /(win|w)32$/
      Jake.run2 cmd, args, {:nowait => !xwait}
    end

  end


  #desc "Run application on RhoSimulator"
  task :rhosimulator => ["run:rhosimulator:build", "run:rhosimulator:run"]
end

#------------------------------------------------------------------------
namespace "wm_gem" do
  namespace "build" do
   task :printing_service do
      printing_service_target = File.join(File.dirname(__FILE__), 'libs', 'printing-service')
      rm_r printing_service_target if File.exists?(printing_service_target)
      mkdir_p printing_service_target
      Rake::Task['printing:build:all'].invoke(printing_service_target)
   end
   task :config  do
      puts "Configure build"
      buildyml = "rhoelements.yml"

      $config = Jake.config(File.open(buildyml))

      $cabwiz = $config["env"]["paths"]["cabwiz"]
      $cabwiz = "" if $cabwiz.nil?
      $cabwiz = File.join($cabwiz, "cabwiz.exe")

      $devenv = $config["env"]["paths"]["devenv"]
      $devenv = "devenv.exe" if $devenv.nil?

      $vcbuild = $config["env"]["paths"]["vcbuild"]
      $vcbuild = "vcbuild" if $vcbuild.nil?
      raise "VCBUILD is not set" if $vcbuild.nil?

      $sdk = $config["sdk"]
      raise "SDK is not set" if $sdk.nil?

      $app_config  = Hash.new
      $app_config["capabilities"] = $config["capabilities"]

      make_application_build_capabilities_header_file
      $startdir = File.dirname(__FILE__)

      #ENV['RHO_ROOT']  = File.expand_path("../rhodes")
      #puts "RHO_ROOT - " + ENV['RHO_ROOT'].to_s

      puts "$startdir - " + $startdir

      $neon_root = $config["env"]["paths"]["neon"] unless $config["env"]["paths"]["neon"].nil?
      $rhoelementsext_main_dir = File.join($startdir, 'extensions/rhoelementsext/ext')
      $rhoelementsext_wm_dir = File.join($rhoelementsext_main_dir, "rhoelementsext/platform/wm")

      $re_version = $config['version']
    end

	namespace 'wm' do
		 task :copy_webkit => ["wm_gem:build:config"] do
				chdir File.dirname(__FILE__)

				dest_dir = "./libs/data"
				mkdir_p dest_dir

				src_dir = File.join($startdir, "neon/R2D1/Build" )

				cp_r File.join(src_dir, "Config"), dest_dir, :preserve => true
				cp_r File.join(src_dir, "Windows/NPAPI"), dest_dir, :preserve => true
				cp_r File.join(src_dir, "Windows/Plugins"), File.join(dest_dir, 'Plugin'), :preserve => true

				Dir.glob(File.join(src_dir, "Windows/Binaries/*.*")).each { |f| cp_r f, dest_dir }
				Dir.glob(File.join(src_dir, "Windows/WebKit/*.*")).each { |f| cp_r f, dest_dir }

				chdir File.dirname(__FILE__)
				chmod_R 0777, "./libs/data"
			  end
			end
		end
end
namespace "build" do
  task :set_neon, :neon_dir do |t, args|
    Jake.edit_yml(File.join(File.dirname(__FILE__), 'rhoelements.yml')) do |yml|
      yml['env']['paths']['neon'] = args[:neon_dir]
    end
  end

  task :rhosimulator do
    if RUBY_PLATFORM =~ /(win|w)32$/
      Rake::Task["build:win32:rhosimulator"].invoke
    elsif RUBY_PLATFORM =~ /darwin/
      Rake::Task["build:osx:rhosimulator"].invoke
    elsif RUBY_PLATFORM =~ /linux/
      Rake::Task["build:linux:rhosimulator"].invoke
    else
      puts "Sorry, at this time RhoSimulator can be built for Windows, Mac OS X or Linux only"
      exit 1
    end
  end

  task :rhosimulator_version do
    $rhodes_version = File.read(File.join($startdir,'version')).chomp
    File.open(File.join($startdir, 'platform/shared/qt/rhodes/RhoSimulatorVersion.h'), "wb") do |fversion|
      fversion.write( "#define RHOSIMULATOR_VERSION \"#{$rhodes_version}\"\n" )
    end
  end
end

#------------------------------------------------------------------------

namespace :run do
  desc "start rholog(webrick) server"
  task :webrickrhologserver, :app_path  do |t, args|
    require 'webrick'

    puts "Args were: #{args}"
    $app_path = args[:app_path]

    Rake::Task["config:wm"].invoke

    $rhologserver = WEBrick::HTTPServer.new :Port => $rhologhostport

    puts "LOCAL SERVER STARTED ON #{$rhologhostaddr}:#{$rhologhostport}"
    started = File.open($app_path + "/started", "w+")
    started.close

    #write host and port 4 log server
    $rhologfile = File.open(getLogPath, "w+")

    $rhologserver.mount_proc '/' do |req,res|
      if ( req.body == "RHOLOG_GET_APP_NAME" )
        res.status = 200
        res.chunked = true
        res.body = $app_path
      elsif ( req.body == "RHOLOG_CLOSE" )
        res.status = 200
        res.chunked = true
        res.body = ""

        $rhologserver.shutdown
      else
        $rhologfile.puts req.body
        $rhologfile.flush
        res.status = 200
        res.chunked = true
        res.body = ""
      end
    end

    ['INT', 'TERM'].each {|signal|
      trap(signal) {$rhologserver.shutdown}
    }

    $rhologserver.start
    $rhologfile.close

  end
end

namespace :dev do

  namespace :genenerate do
    desc "Regenerate all extensions in search paths"
    task :extensions => ["config:common"] do
      puts $app_config["extpaths"].join(', ')
      ($app_config["extpaths"] - [$app_path]).each do |path|
        puts "Search path #{path}"
        xmls = Dir.glob(File.join(path,'**','*.xml'))
        exts = xmls.reject{|f| !File.exists?(File.join(File.split(f)[0],'..','ext.yml'))}
        exts.each do |ext|
          puts "Processing #{ext}"
          result = Jake.run2('"'+File.join($startdir,'bin','rhogen')+'"',['api',"\"#{ext}\""],{:hide_output=>true})
          if $?.exitstatus != 0
            puts result.red
            fail "probem with extension #{ext}"
          end
        end
      end
    end
  end

end


namespace :config do
  namespace :common do
    task :ymlsetup do
      #Setting build.yml configs in rhoconfig.txt for cloud
      File.open("#{$app_path}/rhoconfig.txt", "a") do |f|
        f << "\n\n# Settings for cloud from build.yml\n"
        $application_build_configs.each do |k,v|
          f << "#{k}=\"#{v}\"\n"
        end
        f << "\n"
      end
    end
  end
end


$running_time = []

module Rake
  class Task
    alias :old_invoke :invoke
    def invoke(*args)
      start_time = Time.now
      old_invoke(*args)
      end_time = Time.now
      $running_time << [@name, ((end_time.to_f - start_time.to_f)*1000).to_i]
    end
  end
end

#------------------------------------------------------------------------

at_exit do
  BuildOutput.note(RequiredTime.generate_benchmark_report,"Reqire loading time") if RequiredTime.hooked?
  BuildOutput.note($running_time.map {|task| "Task '#{task[0]}' - #{task[1]} ms" }, "Task exicution time") if $task_execution_time

  print BuildOutput.getLogText
end
