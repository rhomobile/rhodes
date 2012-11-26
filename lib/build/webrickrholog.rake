namespace :run do
  desc "start rholog(webrick) server"
  task :webrickrhologserver, :app_path  do |t, args|
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
