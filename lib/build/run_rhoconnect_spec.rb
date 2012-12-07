require File.join(File.dirname(__FILE__),'jake.rb')

	def reset_rhoconnect_server(host,port)
		require 'rest_client'
		require 'json'
		
		begin
			platform = platform
			exact_url = "http://#{host}:#{port}"
			puts "going to reset server: #{exact_url}"
			# login to the server
			unless @bulk_srv_token
				@bulk_srv_token = RestClient.post("#{exact_url}/rc/v1/system/login", { :login => "rhoadmin", :password => "" }.to_json, :content_type => :json)
			end
			RestClient.post("#{exact_url}/api/reset", {:api_token => @bulk_srv_token}.to_json, :content_type => :json)
			puts "reset OK"
			rescue Exception => e
			puts "reset_spec_server failed: #{e}"
		end
	end

	require 'socket'

	def run_rhoconnect_spec(platform)
		appname = "rhoconnect_spec"

		puts "run_spec_app(#{platform},#{appname})"

		host = Jake.localip
		port = 9292
		server_pid = nil
		resque_pid = nil

		rhobuildyml = File.join(basedir,'rhobuild.yml')
		$app_path = File.expand_path(File.join(basedir,'spec',appname))
		$app_config = Jake.config(File.open(File.join($app_path, "build.yml")))
		config = Jake.config(File.open(rhobuildyml,'r'))

		server_path = File.expand_path(File.join(basedir,'spec',appname,'server'))
		puts "server path: #{server_path}"

		rc_out = File.open( File.join($app_path, "rhoconnect.log" ), "w")
		redis_out = File.open( File.join($app_path, "redis.log" ), "w")
		resque_out = File.open( File.join($app_path, "resque.log" ), "w")


		puts "stop resque"
		Process.kill('INT', resque_pid) if resque_pid
		sleep(10)

		puts "stop rhoconnect"
#		Kernel.spawn("rhoconnect","stop",:chdir => server_path, :out => rc_out )
		Kernel.spawn("rhoconnect","stop",:chdir => server_path)
		sleep(10)

		puts "stop redis"
#		Kernel.spawn("rhoconnect","redis-stop",:chdir => server_path, :out => redis_out )
		Kernel.spawn("rhoconnect","redis-stop",:chdir => server_path)
		sleep(10)




		puts "run redis"
#		Kernel.spawn("rhoconnect","redis-start",:chdir => server_path, :out => redis_out )
		Kernel.spawn("rhoconnect","redis-start",:chdir => server_path)
		sleep(10)

		puts "run rhoconnect"
#		server_pid = Kernel.spawn("rhoconnect","start",:chdir => server_path, :out => rc_out )
		server_pid = Kernel.spawn("rhoconnect","start",:chdir => server_path)
		sleep(10)

		puts "reset rhoconnect"
		reset_rhoconnect_server(host,port)
		sleep(10)

		puts "run resque"
#		resque_pid = Kernel.spawn({ "QUEUE" => "1" }, "rake","resque:work",:chdir => server_path, :out => resque_out )
		resque_pid = Kernel.spawn({ "QUEUE" => "1" }, "rake","resque:work",:chdir => server_path)
		sleep(10)

		puts "set server parameters, resque_pid = #{resque_pid}, pid = #{server_pid}. setting address = #{host.inspect}, port = #{port}"

		File.open(File.join($app_path, 'app', 'sync_server.rb'), 'w') do |f|
			f.puts "SYNC_SERVER_HOST = '#{host}'"
			f.puts "SYNC_SERVER_PORT = #{port}"
		end


		puts "run specs"
		Rake::Task.tasks.each { |t| t.reenable }
		Rake::Task['run:' + platform + ':spec'].invoke

		sleep(10)


ensure
		puts "stop resque"
		Process.kill('INT', resque_pid) if resque_pid
		sleep(5)

		puts "stop rhoconnect"
		Process.spawn("rhoconnect","stop",:chdir => server_path, :out => rc_out )
		sleep(5)

		puts "stop redis"
		Process.spawn("rhoconnect","redis-stop",:chdir => server_path, :out => redis_out )
		sleep(5)

		puts "run_spec_app(#{platform},#{appname}) done"
	end
