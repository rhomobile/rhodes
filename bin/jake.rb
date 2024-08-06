# encoding: utf-8
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

require 'pathname'
require 'yaml'
require 'socket'
require 'webrick'
  
SYNC_SERVER_BASE_URL = 'http://rhoconnect-spec-exact_platform.heroku.com'
SYNC_SERVER_CONSOLE_LOGIN = 'rhoadmin'
SYNC_SERVER_CONSOLE_PASSWORD = ''

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
    get_absolute_ex(path, Dir.pwd())
  end
    
  def self.get_absolute_ex(path, currentdir)
    ret_path = File.expand_path(path, currentdir)
    return ret_path  if File.exist?(ret_path)

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
          if File.exist? s
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
          if File.exist? s
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
    addr = localip                   #:BindAddress => addr,
    server = WEBrick::HTTPServer.new :Port => port
    port = server.config[:Port]
    puts "LOCAL SERVER STARTED ON #{addr}:#{port}"
    Thread.new { server.start }
    return server, addr, port
  end

  def self.reset_spec_server(platform)
    require 'rest_client'
    require 'json'

    platform = platform
    exact_url = SYNC_SERVER_BASE_URL.gsub(/exact_platform/, platform)
    puts "going to reset server: #{exact_url}"
    # login to the server
    unless @srv_token
      result = RestClient.post("#{exact_url}/login",
                           {:login => SYNC_SERVER_CONSOLE_LOGIN, :password => SYNC_SERVER_CONSOLE_PASSWORD}.to_json,
                           :content_type => :json)
      srv_session_cookie = 'rhoconnect_session=' + result.cookies['rhoconnect_session']
      @srv_token = RestClient.post("#{exact_url}/api/get_api_token", '', {'Cookie' => srv_session_cookie})
    end
    # reset server
    RestClient.post("#{exact_url}/api/reset", {:api_token => @srv_token}.to_json, :content_type => :json)
  end

  def self.run_spec_app(platform,appname)
    reset_spec_server(platform) if appname =~ /phone_spec/

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
    if File.exist?(File.join($app_path, 'server.rb'))
      $local_server = server
      require File.join($app_path, 'server.rb')
    end
    begin
      Rake::Task.tasks.each { |t| t.reenable }
      Rake::Task['run:' + platform + ':spec'].invoke
    ensure
      server.shutdown
    end
    
    $failed.to_i
  end

  def self.before_run_spec()
    $total ||= 0
    $passed ||= 0
    $failed ||= 0
    $faillog = []
    $getdump = false
  end
  
  def self.process_spec_output(line)
      puts line if line =~ /\| - it/ or line =~ /\| describe/ or line =~ /\|   - /

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
      
      return true
  end
  
  def self.process_spec_results(start)
    finish = Time.now
  
    FileUtils.rm_rf $app_path + "/faillog.txt"
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
      currentdir = Dir.pwd()
      Dir.chdir wd
    end

    cmdstr = argv.map { |x| x =~ / |\|/ ? '"' + x + '"' : x }.join(' ')

    unless options[:hide_output]
      puts "PWD: " + Dir.pwd()
      puts "CMD: " + cmdstr
      $stdout.flush
    end

    hideerrors = options[:hideerrors]
    if hideerrors
      if OS.windows?
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
                retval += line
		unless options[:hide_output]
                    puts "RET: " + line
                    $stdout.flush
		end
            end    
          end
        end
      end
    end

    if not wd.nil?
      Dir.chdir currentdir
    end

    retval
  end
  
  def self.run(command, args, wd=nil,system = false, hideerrors = false)
    self.run2(command, args, {:directory => wd, :system => system, :hiderrors => hideerrors})
  end
  
  def self.unjar(src,targetdir)
    jpath = $config["env"]["paths"]["java"]   
    cmd = jpath && jpath.length()>0 ? File.join(jpath, "jar" ) : "jar"  

      p = Pathname.new(src)
    src = p.realpath
    currentdir = Dir.pwd()
    src = src.to_s.gsub(/"/,"")
  
    args = Array.new
  
    args << "xf"
    args << src.to_s
  
    Dir.chdir targetdir
    puts run(cmd,args)
    Dir.chdir currentdir
  end
  
  def self.jarfilelist(target)
    jpath = $config["env"]["paths"]["java"]   
    cmd = jpath && jpath.length()>0 ? File.join(jpath, "jar" ) : "jar"  

    target.gsub!(/"/,"")

    args = []
    args << "tf"
    args << target

    filelist = []
    run(cmd,args).each { |file| filelist << file if not file =~ /\/$/ }

    filelist
  end

  def self.jar(target,manifest,files,isfolder=false)
    jpath = $config["env"]["paths"]["java"]   
    cmd = jpath && jpath.length()>0 ? File.join(jpath, "jar" ) : "jar"

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
    
    currentdir = Dir.pwd()
  
  
    Dir.chdir destdir
  
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
        
          if $service_enabled      
            if $hidden_app == "0"
                f.write "RIM-MIDlet-Flags-1: 1\n" 
            else
                f.write "RIM-MIDlet-Flags-1: 3\n" 
            end    
          else
            if $hidden_app == "0"
                f.write "RIM-MIDlet-Flags-1: 0\n" 
            else
                f.write "RIM-MIDlet-Flags-1: 2\n" 
            end    
          end
        
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
    Dir.chdir currentdir
  
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
  
  def self.modify_file_if_content_changed(file_name, f)
    f.rewind
    content = f.read()
    old_content = File.exist?(file_name) ? File.read(file_name) : ""

    if old_content != content  
        puts "Modify #{file_name}"      
        File.open(file_name, "w"){|file| file.write(content)}
    end
    
    f.close
  end

  def self.build_file_map(dir, file_name)
    psize = dir.size + 1

    File.open(File.join(dir, file_name), 'w') do |dat|
        Dir.glob(File.join(dir, '**/*')).sort.each do |f|
          relpath = f[psize..-1]

          if File.directory?(f)
            type = 'dir'
          elsif File.file?(f)
            type = 'file'
          else
            next
          end
          size = File.stat(f).size
          tm = File.stat(f).mtime.to_i

          dat.puts "#{relpath}\t#{type}\t#{size.to_s}\t#{tm.to_s}"
        end
    end 
  end

  def self.zip_upgrade_bundle(folder_path, zip_file_path)

      File.delete(zip_file_path) if File.exist?(zip_file_path)

      currentdir = Dir.pwd()
      Dir.chdir folder_path
      
      if OS.windows?
        begin
      
          require 'rubygems'
          require 'zip/zip'
          require 'find'
          require 'fileutils'
          include FileUtils

          Zip::ZipFile.open(zip_file_path, Zip::ZipFile::CREATE)do |zipfile|
            Find.find("RhoBundle") do |path|
              Find.prune if File.basename(path)[0] == ?.
              next if path.start_with?("RhoBundle/lib") || path.start_with?("RhoBundle/db") || path == 'RhoBundle/hash' || path == 'RhoBundle/name'
              
              puts "add to zip : #{path}"
              zipfile.add(path, path)
            end 
          end
        rescue Exception => e
          puts "ERROR : #{e}"
          puts 'Require "rubyzip" gem for make zip file !'
          puts 'Install gem by "gem install rubyzip"'
        end        
      else
        #chdir folder_path
        temp_dir = folder_path + '_tmp'
        mkdir_p temp_dir
        cp_r 'RhoBundle', temp_dir
        chdir temp_dir         
        rm_rf File.join(temp_dir, 'RhoBundle/lib')
        rm_rf File.join(temp_dir, 'RhoBundle/db')
        rm_rf File.join(temp_dir, 'RhoBundle/hash')
        rm_rf File.join(temp_dir, 'RhoBundle/name')
        sh %{zip -r temporary_archive.zip .}
        cp_r 'temporary_archive.zip', zip_file_path
        rm_rf 'temporary_archive.zip'
        rm_rf temp_dir
      end
      
      Dir.chdir currentdir

  end
  
  def self.run_rho_log_server(app_path)

	confpath_content = File.read($srcdir + "/apps/rhoconfig.txt") if File.exist?($srcdir + "/apps/rhoconfig.txt")
	confpath_content += "\r\n" + "rhologurl=http://" + $rhologhostaddr + ":" + $rhologhostport.to_s() if !confpath_content.include?("rhologurl=")
	File.open($srcdir + "/apps/rhoconfig.txt", "w") { |f| f.write(confpath_content) }  if confpath_content && confpath_content.length()>0
  
    begin
        require 'net/http'
        
        res = Net::HTTP.start(Jake.localip(), $rhologhostport) {|http|
             http.post('/', "RHOLOG_GET_APP_NAME")
        }
        puts "res : #{res}"
        puts "body : #{res.body}"
        
        if ( res && res.body == app_path)
            puts "Log server is already running. Reuse it."
            
	        started = File.open($app_path + "/started", "w+")
	        started.close
            
            return
        else
            puts "Close Log server for another app."            
            res = Net::HTTP.start(Jake.localip(), $rhologhostport) {|http|
                 http.post('/', "RHOLOG_CLOSE")
            }
            
        end
    rescue Exception => e
        puts "EXC: #{e}"
    end    
  
    system("START rake run:webrickrhologserver[#{app_path}]")
  end
    
end
  
