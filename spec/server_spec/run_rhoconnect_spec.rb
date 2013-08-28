require 'fileutils'
require 'rake'

require File.join($rho_root,'lib','build','rhoconnect_helper.rb')
require File.join($rho_root,'lib','build','jake.rb')
require File.join($rho_root,'platform','android','build','android_tools.rb')
load File.join($rho_root,'Rakefile')
load File.join($rho_root,'platform','android','build','android.rake')

    def stop_apps
		RhoconnectHelper.stop_rhoconnect_stack
	end

def run_apps(platform)
	appname = "rhoconnect_push_client"
	test_appname = "testapp"

	puts "run_spec_app(#{platform},#{appname})"

	rhobuildyml = File.join($rho_root,'rhobuild.yml')
	$app_path = File.expand_path(File.join(File.dirname(__FILE__),appname))
	puts "app path: #{$app_path}"

	$app_config = Jake.config(File.open(File.join($app_path, "build.yml")))
	config = Jake.config(File.open(rhobuildyml,'r'))

	$source_path = File.expand_path(File.join($app_path,'server'))
	$tmp_path = File.join(File.dirname(__FILE__),'tmp')

	cleanup_apps

	FileUtils.mkdir_p File.expand_path($tmp_path)
	$server_path = File.expand_path(File.join($tmp_path,'testapp'))



	RhoconnectHelper.set_rhoconnect_bin $rhoconnect_root
	puts "rhoconnect_bin: #{RhoconnectHelper.rhoconnect_bin}"

	RhoconnectHelper.set_rc_out File.open( File.join($app_path, "rhoconnect.log" ), "w")
	RhoconnectHelper.set_redis_out File.open( File.join($app_path, "redis.log" ), "w")
	RhoconnectHelper.set_resque_out File.open( File.join($app_path, "resque.log" ), "w")
	RhoconnectHelper.set_rc_push_out File.open( File.join($app_path, "rc_push.log" ), "w")

	RhoconnectHelper.set_enable_resque(false)
	RhoconnectHelper.set_enable_redis($rhoconnect_use_redis)

	stop_apps

	puts "generate app"
	res = RhoconnectHelper.generate_app($tmp_path,test_appname)

	puts "patching Gemfile with correct rhoconnect path"
	target_gemfile = File.join($server_path, 'Gemfile')
	file = File.read(target_gemfile)
	file.gsub!(/(gem 'rhoconnect'*.*)/, "gem 'rhoconnect', :path => '#{$rhoconnect_root}'")
	File.open(target_gemfile, 'w') {|f| f.write file}

	puts "patching Gemfile with aws-s3"
	File.open(target_gemfile, 'a') {|f| f.puts "gem 'aws-s3', '>= 0.6.3'" }

	puts "patching Gemfile with sqlite3"
	File.open(target_gemfile, 'a') {|f| f.puts "gem 'sqlite3', '>= 1.3.3'" }

	puts "bundle install"
	Kernel.system("bundle","install",:chdir => $server_path)

	puts "adding source files"
	FileUtils.cp_r ["#{$source_path}/settings"], $server_path
    FileUtils.cp_r ["#{$source_path}/controllers"], $server_path

	puts "cleanup rhoconnect data"
	FileUtils.rm_r(File.join($server_path,"data")) if File.directory?(File.join($server_path,"data"))

	RhoconnectHelper.start_rhoconnect_stack($server_path,true)

	File.open(File.join($app_path, 'app', 'sync_server.rb'), 'w') do |f|
		f.puts "SYNC_SERVER_HOST = '#{RhoconnectHelper.host}'"
		f.puts "SYNC_SERVER_PORT = #{RhoconnectHelper.port}"
	end

	File.open(File.join($app_path, 'app', 'push_server.rb'), 'w') do |f|
		f.puts "PUSH_SERVER_HOST = '#{RhoconnectHelper.push_host}'"
		f.puts "PUSH_SERVER_PORT = #{RhoconnectHelper.push_port}"
	end
	cfgfile = File.join($app_path, 'rhoconfig.txt')
	cfg = File.read(cfgfile)
	cfg.gsub!(/(rhoconnect_push_server.*)/, "rhoconnect_push_server = 'http://#{RhoconnectHelper.push_host}:#{RhoconnectHelper.push_port}'")
	File.open(cfgfile, 'w') {|f| f.write cfg}

	puts "configure android"
	Rake::Task["config:android:emulator"].invoke
	AndroidTools.run_emulator( :wipe => true )

	push_service_apk = File.join($rhoelements_root,'libs','rhoconnect-push-service','rhoconnect-push-service.apk')

	puts "install rhoconnect push service"
	AndroidTools.load_app_and_run("-e", push_service_apk, "")

	puts 'Building and starting application...'
	FileUtils.chdir File.join($spec_path, 'rhoconnect_push_client')
	system("rake run:#{$platform}").should == true

rescue Exception => e
	puts "exception caught: #{e.inspect}"
	puts e.backtrace

ensure
	puts "run_spec_app(#{$platform},#{$appname}) done"
end

    def cleanup_apps
		puts "cleanup"
		FileUtils.rm_r File.expand_path($tmp_path) if File.directory?($tmp_path)
    end
