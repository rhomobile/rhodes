require 'pathname'
require 'yaml'
require 'socket'
require 'webrick'
  
class Hash
  def fetch_r(key)
    if self.has_key?(key) and not self[key].is_a?(Hash)
      return self[key]
    else
      self.each do |val|
	value = false 
	if val.is_a?(Array)
          val.each do |x|
            value = x.fetch_r(key) if x.is_a?(Hash)
  	    return value if value
  	  end
 	end
        value = val.fetch_r(key) if val.is_a?(Hash)
  	return value if value
      end
    end
    return false  
  end
end
  
class Jake


  def self.config(configfile)
    conf = YAML::load(configfile)
    res = self.config_parse(conf)
    res
  end
  def self.set_bbver(bbver)
    @@bbver = bbver
  end

  def self.get_absolute(path)
    return File.expand_path(path) if File.exists?(path)
    currentdir = pwd
    
    path = currentdir + "/" + path
  
    patharray = path.split(/\//)
  
    while idx = patharray.index("..") do
      if idx == 0
        raise "error getting absolute"
      end
     
      if patharray[idx-1] != ".."
        patharray.delete_at(idx)
        patharray.delete_at(idx-1)
      end
    end
    return patharray.join("/")  
  end	
  
  def self.config_parse(conf)
    if conf.is_a?(Array)
      conf.collect! do |x|
        if x.is_a?(Hash) or x.is_a?(Array)
          x = config_parse(x)
          x
        else
          if x =~ /%(.*?)%/
            x.gsub!(/%.*?%/, conf.fetch_r($1).to_s)
          end
          s = x.to_s
          if File.exists? s
            s.gsub!(/\\/, '/')
  	      end
  	      s
        end
      end
    elsif conf.is_a?(Hash)
      newhash = Hash.new

      conf.each do |k,x|
        if x.is_a?(Hash) or x.is_a?(Array)
          newhash[k.to_s] = config_parse(x)
        else
          s = x.to_s
          if File.exists? s
            s.gsub!(/\\/, '/')
          end
          newhash[k.to_s] = s
        end
      end
      conf = newhash

      conf
    end

    conf
  end

  def self.localip
    orig, Socket.do_not_reverse_lookup = Socket.do_not_reverse_lookup, true  # turn off reverse DNS resolution temporarily
    UDPSocket.open do |s|
      s.connect '174.142.8.58', 1
      s.addr.last
    end
  ensure
    Socket.do_not_reverse_lookup = orig
  end

  def self.run_local_server(port = 0)
    addr = localip
    server = WEBrick::HTTPServer.new :BindAddress => addr, :Port => port
    port = server.config[:Port]
    puts "LOCAL SERVER STARTED ON #{addr}:#{port}"
    Thread.new { server.start }
    return server, addr, port
  end

  def self.run_spec_app(platform,appname)
    rhobuildyml = File.join(basedir,'rhobuild.yml')
    rhobuild = YAML::load_file(rhobuildyml)
    rhobuild['env']['app'] = app_expanded_path(appname)
    File.open(rhobuildyml,'w') {|f| f.write rhobuild.to_yaml}
    $app_path = File.expand_path(File.join(basedir,'spec',appname))
    $app_config = Jake.config(File.open(File.join($app_path, "build.yml")))
    $config = Jake.config(File.open(rhobuildyml,'r'))

    server, addr, port = run_local_server
    File.open(File.join($app_path, 'app', 'local_server.rb'), 'w') do |f|
      f.puts "SPEC_LOCAL_SERVER_HOST = '#{addr}'"
      f.puts "SPEC_LOCAL_SERVER_PORT = #{port}"
    end
    if File.exists?(File.join($app_path, 'server.rb'))
      $local_server = server
      require File.join($app_path, 'server.rb')
    end
    begin
      Rake::Task.tasks.each { |t| t.reenable }
      Rake::Task['run:' + platform + ':spec'].invoke
    ensure
      server.shutdown
    end
  end

  def self.before_run_spec()
    $total ||= 0
    $passed ||= 0
    $failed ||= 0
    $faillog = []
    $getdump = false
  end
  
  def self.process_spec_output(line)
      puts line if line =~ /\| - it/ or line =~ /\| describe/

      if $getdump
        if line =~ /^I/
          $getdump = false
        else
          if !$faillog.include?(line)
            $faillog << line
          end
        end
      end

      if line =~ /\*\*\*Failed:\s+(.*)/
        $failed += $1.to_i
        return false
      elsif line =~ /\*\*\*Total:\s+(.*)/
        $total += $1.to_i
      elsif line =~ /\*\*\*Passed:\s+(.*)/
        $passed += $1.to_i
      end

      if line =~ /\| FAIL:/
        line = line.gsub(/I.*APP\|/,"\n\n***")
        if !$faillog.include?(line)
          $faillog << line
        end
        $getdump = true
      end
      
      return ($failed == 0)
  end
  
  def self.process_spec_results(start)
    finish = Time.now
  
    rm_rf $app_path + "/faillog.txt"
    File.open($app_path + "/faillog.txt", "w") { |io| $faillog.each {|x| io << x }  } if $failed.to_i > 0
    
    puts "************************"
    puts "\n\n"
    puts "Tests completed in #{finish - start} seconds"
    puts "Total: #{$total}"
    puts "Passed: #{$passed}"
    puts "Failed: #{$failed}"
    puts "\n"
    puts "Failures stored in faillog.txt" if $failed.to_i > 0
  end

  def self.run2(command, args, options = {}, &block)
  	argv = []
  	currentdir = ""
  	retval = ""
  	argv << command
  	argv += args
  	argv.map! { |x| x.to_s }.map! { |x| x =~ /^".*"$/ ? x[1..-2] : x }

  	wd = options[:directory]
  	if not wd.nil?
      currentdir = pwd()
      chdir wd
    end

    cmdstr = argv.map { |x| x =~ / |\|/ ? '"' + x + '"' : x }.join(' ')

    puts "PWD: " + pwd
    puts "CMD: " + cmdstr
    $stdout.flush

    hideerrors = options[:hideerrors]
    if hideerrors
      if RUBY_PLATFORM =~ /(win|w)32$/
        nul = "nul"
      else
        nul = "/dev/null"
      end
    end

    if options[:system]
      system(cmdstr)
      retval = ""
    else
      argv = cmdstr if RUBY_VERSION =~ /^1\.8/
      if options[:nowait]
        retval = IO.popen(argv)
      else
        IO.popen(argv) do |f|
          while line = f.gets
            if block_given?
                res = yield(line)
                if !res
                    #puts "f.pid : #{f.pid}"
                    Process.kill( 9, f.pid ) 
                end    
            else
                puts line
                $stdout.flush
            end    
          end
        end
      end
    end

    if not wd.nil?
      chdir currentdir
    end

    retval
  end
  
  def self.run(command, args, wd=nil,system = false, hideerrors = false)
    self.run2(command, args, {:directory => wd, :system => system, :hiderrors => hideerrors})
  end
  
  def self.unjar(src,targetdir)
    if RUBY_PLATFORM =~ /(win|w)32$/
      cmd =  $config["env"]["paths"]["java"] + "/jar.exe"
    else
      cmd =  $config["env"]["paths"]["java"] + "/jar"
    end

      p = Pathname.new(src)
    src = p.realpath
    currentdir = pwd()
    src = src.to_s.gsub(/"/,"")
  
    args = Array.new
  
    args << "xf"
    args << src.to_s
  
    chdir targetdir
    puts run(cmd,args)
    chdir currentdir
  end
  def self.jarfilelist(target)
    if RUBY_PLATFORM =~ /(win|w)32$/
      cmd =  $config["env"]["paths"]["java"] + "/jar.exe"
    else
      cmd =  $config["env"]["paths"]["java"] + "/jar"
    end
    target.gsub!(/"/,"")

    args = []
    args << "tf"
    args << target

    filelist = []
    run(cmd,args).each { |file| filelist << file if not file =~ /\/$/ }

    filelist
  end

  def self.jar(target,manifest,files,isfolder=false)
    if RUBY_PLATFORM =~ /(win|w)32$/
      cmd =  $config["env"]["paths"]["java"] + "/jar.exe"
    else
      cmd =  $config["env"]["paths"]["java"] + "/jar"
    end
    target.gsub!(/"/,"")
    
    args = []
    args << "cfm"
    args << target
    args << manifest
    if isfolder
      args << "-C"
      args << files
      args << "."
    else
      args << files
    end
  
    puts run(cmd,args)
  
  
  end
  
  def self.rapc(output,destdir,imports,files,title=nil,vendor=nil,version=nil,icon=nil,library=true,cldc=false,quiet=true, nowarn=true)
    #cmd = $config["env"]["paths"][$config["env"]["bbver"]]["java"] + "/java.exe"
#   cmd = "java.exe"
    
    jdehome = $config["env"]["paths"][@@bbver]["jde"]
    javabin = $config["env"]["paths"]["java"]
    cmd = jdehome + "/bin/rapc.exe"
    
    currentdir = pwd()
  
  
    chdir destdir
  
    if output and title and version and vendor
      f = File.new(output + ".rapc", "w")
      f.write "MicroEdition-Profile: MIDP-2.0\n"
      f.write "MicroEdition-Configuration: CLDC-1.1\n"
      f.write "MIDlet-Name: " + title + "\n"
      f.write "MIDlet-Version: " + version.to_s + "\n"
      f.write "MIDlet-Vendor: " + vendor.to_s + "\n"
      f.write "MIDlet-Jar-URL: " + output + ".jar\n"
      f.write "MIDlet-Jar-Size: 0\n"
      f.write "RIM-Library-Flags: 2\n" if library

      if cldc and icon
        f.write "MIDlet-1: " + title + "," + icon + ",\n"
        puts "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! service_enabled: #{$service_enabled}"
        $stdout.flush
        f.write "RIM-MIDlet-Flags-1: 1\n" if $service_enabled
      end

      f.close
    end
  
  
    args = []
    #args << "-classpath"
  #  args << "-jar"
    #args << jdehome + "/bin/rapc.jar"
    #args << "net.rim.tools.compiler.Compiler"
    
    args << "-javacompiler=" + javabin + "/javac.exe"
    args << "-quiet" if quiet
    args << "-nowarn" if nowarn
    args << 'import=' + imports
    args << 'codename=' + output
    args << 'library=' + output if library
    args << output + '.rapc'
    args << files
  
    cmd.gsub!(/\//,"\\")
    outputstring = run(cmd, args)
    puts outputstring unless $? == 0
    chdir currentdir
  
  end
  
  def self.ant(dir,target)
  
    srcdir = $config["build"]["srcdir"]
    rubypath = $config["build"]["rubypath"]
    excludelib = $config["build"]["excludelib"]
    excludeapps = $config["build"]["excludeapps"]
    compileERB = $config["build"]["compileERB"]
    
  
    args = []
    args << "-buildfile"
    args << dir + "/build.xml"
    args << '-Dsrc.dir=' + get_absolute(srcdir)
#    args << '-Druby.path=' + get_absolute(rubypath)
    args << '-Dexclude.lib=' + excludelib
    args << '-Dexclude.apps=' + excludeapps
    args << '-DcompileERB.path=' + get_absolute(compileERB)
    args << '-Dsrclib.dir=' + get_absolute(srcdir)
  
  
    args << target
    #puts args.to_s
    puts run("ant.bat",args,dir)
  end
end
  
