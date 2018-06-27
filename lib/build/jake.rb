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
require 'socket'
require 'logger'

require 'openssl'
require 'base64'


SYNC_SERVER_BASE_URL = 'http://rhoconnect-spec-exact_platform.heroku.com'
SYNC_SERVER_CONSOLE_LOGIN = 'rhoadmin'
SYNC_SERVER_CONSOLE_PASSWORD = ''

BULK_SYNC_SERVER_URL = 'http://store-bulk.rhohub.com'
BULK_SYNC_SERVER_CONSOLE_LOGIN = 'rhoadmin'
BULK_SYNC_SERVER_CONSOLE_PASSWORD = ''

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



module AES
  class << self


    def encrypt(plain_text, key_64)

        alg = "AES-256-CBC"

        key = Base64.decode64(key_64)

        iv = OpenSSL::Cipher::Cipher.new(alg).random_iv
        iv_64 = Base64.encode64(iv).chomp

        aes = OpenSSL::Cipher::Cipher.new(alg)
        aes.encrypt
        aes.key = key
        aes.iv = iv

		sha1 = OpenSSL::Digest::SHA1.new
		digest = sha1.digest(plain_text)

        cipher = aes.update(plain_text)
        cipher << aes.final

        cipher_64 = Base64.encode64(cipher).chomp

        #res = iv_64 + "$" + cipher_64
        res = []
        res << iv
		res << digest
        res << cipher
        return res
    end



    def decrypt(data, key_64)

        alg = "AES-256-CBC"

        key = Base64.decode64(key_64)

        iv_and_ctext = []
        data.split('$').each do |part|
          iv_and_ctext << Base64.decode64(part)
        end

        iv = iv_and_ctext[0]
        cipher = iv_and_ctext[1]

        de_aes = OpenSSL::Cipher::Cipher.new(alg)
        de_aes.decrypt
        de_aes.key = key
        de_aes.iv = iv

        decrypted_text = de_aes.update(cipher)
        decrypted_text << de_aes.final

        return decrypted_text
    end

    def key

        alg = "AES-256-CBC"

        key = OpenSSL::Cipher::Cipher.new(alg).random_key
        key_64 = Base64.encode64(key).chomp
        return key_64
    end
  end

end




# Class with around building things
class Jake

  @@logger = nil

  def self.set_logger(logger)
    @@logger = logger
  end

  def self.log( severity, message )
    if @@logger
      @@logger.log(severity, message)
    else
      puts message
    end
  end

  def self.config(configfile)
    require 'yaml'

    conf = YAML::load(configfile)
    res = self.config_parse(conf)
    res
  end

  def self.normalize_build_yml(yml = $app_config)
    yml['wm'] = {} unless yml['wm'].is_a?(Hash)
    yml['wm']['webkit_outprocess'] = '0' if yml['wm']['webkit_outprocess'].nil?
  end

  def self.set_bbver(bbver)
    @@bbver = bbver
  end

  def self.get_absolute(path)
    get_absolute_ex(path, Dir.pwd())
  end

  def self.get_absolute_ex(path, currentdir)
    ret_path = File.expand_path(path, currentdir)
    return ret_path  if File.exists?(ret_path)

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
    require 'webrick'

    addr = localip                   #:BindAddress => addr,
    server = WEBrick::HTTPServer.new :Port => port
    port = server.config[:Port]
    log Logger::INFO, "LOCAL SERVER STARTED ON #{addr}:#{port}"
    Thread.new { server.start }
    return server, addr, port
  end

  def self.run_local_server_with_logger(port, log_file)
    require 'webrick'

    addr = localip
    log = WEBrick::Log.new log_file
    access_log = [[log_file, WEBrick::AccessLog::COMBINED_LOG_FORMAT]]
    server = WEBrick::HTTPServer.new :Port => port, :Logger => log, :AccessLog => access_log
    port = server.config[:Port]
    # puts "LOCAL SERVER STARTED ON #{addr}:#{port}"
    Thread.new { server.start }
    return server, addr, port
  end

  def self.reset_spec_server(platform)
    require 'rest_client'
    require 'json'

    begin
        platform = platform
        exact_url = SYNC_SERVER_BASE_URL.gsub(/exact_platform/, platform)
        log Logger::INFO, "going to reset server: #{exact_url}"
        # login to the server
        unless @srv_token
		  @srv_token = RestClient.post("#{exact_url}/rc/v1/system/login", { :login => SYNC_SERVER_CONSOLE_LOGIN, :password => SYNC_SERVER_CONSOLE_PASSWORD }.to_json, :content_type => :json)
        end
        # reset server
        RestClient.post("#{exact_url}/api/reset", {:api_token => @srv_token}.to_json, :content_type => :json)
		log Logger::INFO, "reset OK"
    rescue Exception => e
      log Logger::ERROR, "reset_spec_server failed: #{e}"
    end
  end

  def self.reset_bulk_server()
	require 'rest_client'
	require 'json'

	begin
		platform = platform
		exact_url = BULK_SYNC_SERVER_URL
		log Logger::INFO, "going to reset server: #{exact_url}"
		# login to the server
		unless @bulk_srv_token
			@bulk_srv_token = RestClient.post("#{exact_url}/rc/v1/system/login", { :login => BULK_SYNC_SERVER_CONSOLE_LOGIN, :password => BULK_SYNC_SERVER_CONSOLE_PASSWORD }.to_json, :content_type => :json)
		end
		RestClient.post("#{exact_url}/api/reset", {:api_token => @bulk_srv_token}.to_json, :content_type => :json)
		log Logger::INFO, "reset OK"
    rescue Exception => e
		log Logger::ERROR, "reset_bulk_server failed: #{e}"
	end
  end

  def self.decorate_spec
    $app_spec_decorator.before_spec unless $app_spec_decorator.nil?
    begin
      yield
    ensure
      $app_spec_decorator.after_spec unless $app_spec_decorator.nil?
    end
  end

  def self.before_run_spec()
    $total ||= 0
    $passed ||= 0
    $failed ||= 0
    $notsupported ||= 0
    $failed_lines ||= 0
    $passed_lines ||= 0
    $mspec_lines ||= 0
    $jasmine_lines ||= 0
    $total_lines_printed ||= 0
    $latest_test_line = ""
    $faillog = []
    @default_file_name = "junit.xml"
    $junitname = ''
    $junitlogs = {@default_file_name => []}
    $getdump = false
  end

  def self.print_statistic_in_progress
      new_total = ($mspec_lines + $jasmine_lines) / 10
      if new_total*10 > $total_lines_printed
          $total_lines_printed = new_total*10
          puts " "+$total_lines_printed.to_s+" tests / "+($passed_lines+$failed_lines).to_s+" checks prоcessed. Latest test is ["+$latest_test_line.to_s+"]"
      end
  end

  def self.process_spec_output(line)
      # Print MSpec example description
      log(Logger::INFO,line) if line =~ /\| - it/ or line =~ /\| describe/ or line =~ /\|   - /
      line = $1 if line =~ /^I\/APP\s+\(\s+[0-9]+\)\:\s+(.*)/
      if $getdump
        if line =~ /^I/
          $getdump = false
        else
          if !$faillog.include?(line)
            $faillog << line
          end
        end
      end

      if line =~ /JUNIT\| (.*)/          # JUNIT| XML
        $junitlogs[@default_file_name] << $1 if $junitlogs[@default_file_name] != nil
      elsif line =~ /JUNITNAME\|\s+(.*)/          # JUNITNAME| name
        $junitname = File.basename($1.strip,'.xml')
        $junitlogs[$junitname] = [] if $junitlogs[$junitname] != nil
      elsif line =~ /JUNITBLOB\| (.*)/
        if $junitname && $1
          $junitlogs[$junitname] << $1 if $junitlogs[$junitname] != nil
        end
      end

      ###
      # Here we are looking for the following pattern of spec stats:
      # ...   APP| ***Total:  ...
      # ...   APP| ***Passed: ...
      # ...   APP| ***Failed: ...
      # ...
      # ...   APP| ***Terminated
      # Bail out as soon as prev. line is found
      ###
      if line =~ /\| \*\*\*Total:\s+(.*)/  # | ***Total:
        $total += $1.to_i
      elsif line =~ /\| \*\*\*Passed:\s+(.*)/ # | ***Passed:
        $passed += $1.to_i
      elsif line =~ /\| \*\*\*Failed:\s+(.*)/    # | ***Failed:
        $failed += $1.to_i
      elsif line =~ /\| \*\*\*Not supported by Rhodes:\s+(.*)/    # | ***Failed:
        $notsupported += $1.to_i
      elsif line =~ /\| \*\*\*Terminated\s+(.*)/ # | ***Terminated
        return false
      end
      #passed Jasmine
      #Jasmine specRunner| <ORM Db Reset specs> : VT302-0054 | should delete all records only from selected models propertyBag databaseFullResetEx : Passed.
      if line =~ /I.* Jasmine specRunner\| .*Passed\./
        $passed_lines = $passed_lines +1
      end

      #Jasmine test lines
      #Jasmine specRunner| <ORM Db Reset specs> : VT302-0054 | should delete all records only from selected models propertyBag databaseFullResetEx started
      if line =~ /I.* Jasmine specRunner\| (.*) started/
        $jasmine_lines = $jasmine_lines +1
        $latest_test_line = $1.chomp
      end

      # tests for MSpec
      if line =~ /\| MSPEC run spec: \[(.*)\]/
        $mspec_lines = $mspec_lines +1
        $latest_test_line = $1.chomp
      end
      # Passed for MSpec
      if line =~ /\| PASSED:/
        $passed_lines = $passed_lines +1
      end
      # Faillog for MSpec
      if line =~ /\| FAIL:/
        line = line.gsub(/I.*APP\|/,"\n\n***")
        if !$faillog.include?(line)
          $faillog << line
        end
        $failed_lines = $failed_lines +1
        $getdump = true
      end
      # Faillog for Jusmine
      if line =~ /I.* Jasmine specRunner\| .*Failed\./
        line = line.gsub(/I.*Jasmine specRunner\|/,"\n\n***")
        if !$faillog.include?(line)
          $faillog << line
        end
        $failed_lines = $failed_lines +1
        $getdump = true
      end

      print_statistic_in_progress

      return true
  end

  def self.process_spec_results(start)
    finish = Time.now

    jpath = File.join($app_path,'junitrep')

    # remove old spec results
    test_patterns = ['Test*.xml', '*_spec_results.xml']
    base_path = File.join($app_path,'**')
    Dir.glob( test_patterns.map{ |pat| File.join(base_path, pat) } ).each { |file_name| File.delete(file_name) }

    FileUtils.rm_rf jpath

    FileUtils.mkdir_p jpath

    $junitlogs.each do |name, log|
      if log.length > 0
        File.open(File.join(jpath,"#{name}.xml"), "w") { |io| io << log.join().gsub('~~',$/) }
      end
    end

    FileUtils.rm_rf $app_path + "/faillog.txt"

    if $failed.to_i > 0
      log Logger::ERROR, "************************"
      log Logger::ERROR, "\n\n"
      $faillog.each {|x| log Logger::ERROR, x }
      File.open($app_path + "/faillog.txt", "w") { |io| $faillog.each {|x| io << x }  }
    end

    log(Logger::INFO, "\n")
    log(Logger::INFO,"************************")
    log(Logger::INFO,"Tests completed in #{"%.1f" % (finish - start)} seconds")
    log(Logger::INFO,"Total: #{$total}")
    log(Logger::INFO,"Passed: #{$passed}")
    log(Logger::INFO,"Failed: #{$failed}")
    log(Logger::INFO,"Not supported by Rhodes: #{$notsupported}")
    log(Logger::INFO,"Failures stored in faillog.txt") if $failed.to_i > 0
    log(Logger::INFO,"MSpec tests: #{$mspec_lines}")
    log(Logger::INFO,"Jasmine tests: #{$jasmine_lines}")
    log(Logger::INFO,"Passed checks: #{$passed_lines}")
    log(Logger::INFO,"Failed checks: #{$failed_lines}")
    log(Logger::INFO,"************************")
    log(Logger::INFO,"\n")
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

    if options[:string_for_add_to_command_line] != nil
      cmdstr = cmdstr + options[:string_for_add_to_command_line]
    end

    $stdout.flush
    unless options[:hide_output]
      log Logger::DEBUG,"PWD: #{Dir.pwd()}"
      log Logger::DEBUG,"CMD: #{cmdstr}"
      $stdout.flush
    end

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
      # this check was commented by crash on 1.9.2 Ruby on Mac OS
      argv = cmdstr #if RUBY_VERSION =~ /^1\.8/
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
                    log Logger::DEBUG,"RET: #{line}"
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

  def self.run3_dont_fail(command, cd = nil, env = {}, use_run2 = false)
    set_list = []
    env.each_pair do |k, v|
      if RUBY_PLATFORM =~ /(win|w)32$/
        set_list << "set \"#{k}=#{v}\"&&"
      else
        set_list << "export #{k}=#{v}&&"
      end
    end

    to_print = "CMD: #{command}"
    to_run = set_list.join('') + command
    if !cd.nil?
      to_print = "PWD: #{cd}\n#{to_print}"

      if RUBY_PLATFORM =~ /(win|w)32$/
        cd_ = cd.gsub('/', "\\")
        to_run = "cd /d \"#{cd_}\"&&#{to_run}"
      else
        to_run = "cd '#{cd}'&&#{to_run}"
      end
    end

    if !env.nil?
      to_print = "ENV: #{env}\n#{to_print}"
    end

    log Logger::DEBUG,to_print
    STDOUT.flush

    if use_run2
        self.run2(to_run, []) do |line|
            log Logger::DEBUG,line
        end
        return $?.exitstatus == 0
    else
        res = system(to_run)
        return res
    end
  end

  def self.run32(command, cd = nil, env = {})
    fail "[#{command}]" unless self.run3_dont_fail(command, cd, env, true)
  end


  def self.run3(command, cd = nil, env = {})
    fail "[#{command}]" unless self.run3_dont_fail(command, cd, env)
  end

  def self.run4(command)
      out = `#{command}`
      fail "[#{command}]" if $?.exitstatus != 0
      out
  end

  #will exec command and return stdout and stderr. options are to be extended for updated functionality
  def self.run_with_output( command, options = {} )
    require 'open3'
    poutres = ''
    perrres = ''
    wtr = nil
    Open3.popen3(command) do |pin,pout,perr,wait_thr|
      while line = pout.gets
        poutres << line
      end

      while line = perr.gets
        perrres << line
      end

      wtr = wait_thr
    end

    return poutres, perrres, wtr
  end

  def self.edit_yml(file, out_file = nil)
    out_file = file if out_file.nil?

    require 'yaml'

    yml = YAML::load_file(file)
    yield yml
    File.open(out_file, 'w') {|f| f.write yml.to_yaml}
  end

  def self.edit_xml(file, out_file = nil)
    out_file = file if out_file.nil?

    doc = REXML::Document.new(File.new(file).read)
    yield doc
    File.open(out_file, 'w') {|f| f << doc}
  end

  def self.edit_lines(file, out_file = nil)
    out_file = file if out_file.nil?

    lines = File.readlines(file)
    File.open(out_file, 'w') do |f|
      lines.each { |line| f.puts(yield line) }
    end
  end

  def self.clean_vsprops(file)
    changed = false
    edit_xml(file) do |doc|
      ['RHO_ROOT', 'TEMP_FILES_DIR'].each do |var|
        REXML::XPath.each(doc, "//UserMacro[@Name='#{var}']") do |node|
          changed = true
          node.remove
        end
      end
    end
  end

  def self.unjar(src,targetdir)
    jpath = $config["env"]["paths"]["java"]
    cmd = jpath && jpath.length()>0 ? File.join(jpath, "jar" ) : "jar"

#    if RUBY_PLATFORM =~ /(win|w)32$/
#      cmd =  $config["env"]["paths"]["java"] + "/jar.exe"
#    else
#      cmd =  $config["env"]["paths"]["java"] + "/jar"
#    end

      p = Pathname.new(src)
    src = p.realpath
    currentdir = Dir.pwd()
    src = src.to_s.gsub(/"/,"")

    args = Array.new

    args << "xf"
    args << src.to_s

    Dir.chdir targetdir
    log Logger::DEBUG,run(cmd,args)
    Dir.chdir currentdir
  end

  def self.jarfilelist(target)
    jpath = $config["env"]["paths"]["java"]
    cmd = jpath && jpath.length()>0 ? File.join(jpath, "jar" ) : "jar"

#    if RUBY_PLATFORM =~ /(win|w)32$/
#      cmd =  $config["env"]["paths"]["java"] + "/jar.exe"
#    else
#      cmd =  $config["env"]["paths"]["java"] + "/jar"
#    end
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
    #cmd +=  ".exe" if RUBY_PLATFORM =~ /(win|w)32$/

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

    log Logger::DEBUG,run(cmd,args)


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
        log Logger::DEBUG,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! service_enabled: #{$service_enabled}"
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
    log(Logger::DEBUG,outputstring) unless $? == 0
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
    log Logger::INFO,run("ant.bat",args,dir)
  end

  def self.modify_file_if_content_changed(file_name, f)
    f.rewind
    content = f.read()
    old_content = File.exists?(file_name) ? File.read(file_name) : ""

    if old_content != content
        log Logger::DEBUG, "!!!MODIFY #{file_name}"
        File.open(file_name, "w"){|file| file.write(content)}
    end

    f.close
  end

  def self.generate_AES_key
      return AES.key
  end

  def self.encrypt_files_by_AES(dir, key, extensions_list)

      puts "Jake.encrypt_files( dir:"+dir.to_s+", key:"+key.to_s+", ext_list:"+extensions_list.to_s+") BEGIN"
      if extensions_list == nil
          puts "extensions_list is NIL !"
          return
      end
      if !(extensions_list.is_a?(Array))
          puts "extensions_list is not Array !"+extensions_list.class.to_s
          return
      end

      psize    = dir.size + 1

      Dir.glob(File.join(dir, '**/*')).sort.each do |f|
        relpath = f[psize..-1]

        if File.directory?(f)
          type = 'dir'
        elsif File.file?(f)
          type = 'file'
        else
          next
        end

        #check file
        extension = File.extname(f).delete(".")
        if !extensions_list.include?(extension)
            next
        end

        puts "    encrypt file: "+f.to_s+" ..."

        #load file
        content = File.binread(f)
        #File.rename(f, f+".original")
        File.delete(f)



        #encrypt file
        #encrypted_content = public_key.public_encrypt( content )
        encrypted_content = AES.encrypt(content, key)


        #decrypted_content = AES.decrypt(encrypted_content, key)
        #File.open(f+".decrypted","wb") do |f|
        #    f.write(decrypted_content)
        #end

        #save file
        #output_f = File.new(f+".encrypted", "w")
        #output_f.puts encrypted_content
        #output_f.close

        File.open(f+".encrypted","wb") do |f|
            f.write(encrypted_content[0])
            f.write(encrypted_content[1])
			f.write(encrypted_content[2])
        end


        puts "        DONE"

      end
      puts "Jake.encrypt_files( dir:"+dir.to_s+", key:"+key.to_s+", ext_list:"+extensions_list.to_s+") END"

  end




  def self.build_file_map(dir, file_name, in_memory = false)
    require 'digest/md5'

    psize    = dir.size + 1
    file_map = Array.new
    file_map_name = File.join(dir, file_name)
    dat      = nil

    if in_memory == false
      dat = File.open(file_map_name, 'w')
    end

    Dir.glob(File.join(dir, '**/*')).sort.each do |f|
      relpath = f[psize..-1]

      if File.directory?(f)
        type = 'dir'
      elsif File.file?(f)
        type = 'file'
      else
        next
      end

      if File.basename(f) == file_name
        next
      end

      md5 = (type == 'file' ? (Digest::MD5.file(f)).to_s : '')
      size    = File.stat(f).size
      tm      = File.stat(f).mtime.to_i

      if in_memory == true
        map_item = { :path => relpath, :size => size, :time => tm, :hash => md5}
        file_map << map_item
      else
        dat.puts "#{relpath}|#{type}|#{size.to_s}|#{tm.to_s}|#{md5}"
      end
    end

    if in_memory == false
      dat.close
    end

    return file_map
  end

  # Unzips archive to specified directory
  # @param src_zip [String] absolute path to archive
  # @param dest_dir [String] path to directory when archive will be unzipped. If it not exists it will be created. It could contain nested directories
  # @param block [block, optional] Block code will be called before each file entry extracting and it's parameters are: file entry size in bytes, archive total size in bytes, string like "Unpacking files: NN%" where NN% - unzipping progress in percents
  def self.unzip(src_zip, dest_dir)
    require 'zip'
    require 'fileutils'

    unless File.exist?(dest_dir)
      FileUtils.mkdir_p(dest_dir)
    end

    Zip::File.open(src_zip) do |zip_file|

      unzipped_bytes = 0
      total_bytes = zip_file.inject(0) { |result, each| result  + each.size }

      zip_file.each do |entry|

        if block_given? and entry.size != 0
          unzipped_bytes = unzipped_bytes + entry.size
          yield(unzipped_bytes, total_bytes, "Unpacking files: #{(unzipped_bytes * 100) / total_bytes}%")
        end
        file_dir_name = File.join(dest_dir,File.dirname(entry.name))
        FileUtils::mkdir_p file_dir_name unless Dir.exists?(file_dir_name)
        entry.extract(File.join(dest_dir, entry.name))

      end
    end

  end

  # Zips specified files from directory
  # @param where [String] absolute path to base directory with files fir zipping
  # @param what [Array] Array of file path of files to zipping. Each file path is relative for where argument
  # @param dest [String] File path to created archive. If file already exists it will be removed before archive creation
  def self.zip(where, what, dest)
    require 'zip'

    if File.exist?(dest)
      FileUtils.rm(dest);
    end

    Zip::File.open(dest, Zip::File::CREATE) do |zipfile|
      what.each do |filename|
        zipfile.add(filename, File.join(where, filename))
      end
    end
  end

  def self.zip_upgrade_bundle(folder_path, zip_file_path)

    require 'zip'

    current_dir = Dir.pwd()
    begin
      Dir.chdir(folder_path)
      File.delete(zip_file_path) if File.exists?(zip_file_path)

      items_to_zip = []

      Zip::File.open(zip_file_path, Zip::File::CREATE) do |zip_file|
        (Dir['RhoBundle/**/*']).each { |path|
          exclude_items = (Jake.getBuildProp2('rhobundle', 'exclude_items') || []).collect { |each| %r{#{each}} }
          begin
            log Logger::INFO,"Excluded: #{path}".warning
            next
          end if (exclude_items.any? { |each| path.index(each) })
          log Logger::INFO,"added to zip : #{path}"
          zip_file.add(path, path)
        }
      end
    ensure
      Dir.chdir(current_dir)
    end
  end

=begin
        Find.find('RhoBundle') do |path|
          Find.prune if File.basename(path)[0] == ?.
          next if path.start_with?('RhoBundle/lib') || path.start_with?('RhoBundle/db') || path == 'RhoBundle/hash' || path == 'RhoBundle/name'
          puts "add to zip : #{path}"
          zip_file.add(path, path)
        end
=end


  def self.modify_rhoconfig_for_debug
    confpath_content = File.read($srcdir + "/apps/rhoconfig.txt") if File.exists?($srcdir + "/apps/rhoconfig.txt")
    log(Logger::INFO,"confpath_content=" + confpath_content.to_s)

    confpath_content += "\r\n" + "remotedebug=1"  if !confpath_content.include?("remotedebug=")
    confpath_content += "\r\n" + "debughosturl=" + $rhologhostaddr  if !confpath_content.include?("debughosturl=")

 #   puts "confpath_content=" + confpath_content.to_s
 #   puts  "$srcdir=" + $srcdir.to_s

    File.open($srcdir + "/apps/rhoconfig.txt", "w") { |f| f.write(confpath_content) }  if confpath_content && confpath_content.length()>0
  end

  def self.get_config_override_params
    override = {}
    ENV.each do |key, value|
      key.match(/^rho_override_(.+)$/) do |match|
        override[match[1]] = value
      end
    end
    return override
  end

  def self.copy_rhoconfig(source, target)
    override = get_config_override_params
    mentioned = Set.new

    lines = []

    # read file and edit overriden parameters
    File.open(source, 'r') do |file|
      while line = file.gets
        match = line.match(/^(\s*)(\w+)(\s*=\s*)/)
        if match
          name = match[2]
          if override.has_key?(name)
            lines << "#{match[1]}#{name}#{match[3]}#{override[name]}"
            mentioned << name
            next
          end
        end
        lines << line
      end
    end

    # append rest of overriden parameters to text
    override.each do |key, value|
      if !mentioned.include?(key)
        lines << ''
        lines << "#{key} = #{value}"
      end
    end

    # write text to target file
    File.open(target, 'w') do |file|
      lines.each { |l| file.puts l }
    end
  end

  def self.make_rhoconfig_txt
    copy_rhoconfig(File.join($app_path, 'rhoconfig.txt'), File.join($srcdir, 'apps', 'rhoconfig.txt'))

    modify_rhoconfig_for_debug if $remote_debug

    app_version = "\r\napp_version='#{$app_config["version"]}'"
    app_version += "\r\napp_name='#{$app_config["name"]}'"
    app_version += "\r\ntitle_text='#{$app_config["name"]}'"  if $current_platform == "win32"
    app_version += "\r\norg_name='#{$app_config["vendor"]}'"  if $current_platform == "win32"
    app_version += "\r\nrho_app_id='#{$app_config["rho_app_id"]}'" if $app_config['rho_app_id']

    #store Rhodes version to rhoconfig
    version_path = File.join($startdir, 'version')
    version = ""
    File.open( version_path, 'rb' ){ |f| version = f.read() }
    app_version += "\r\nrhodes_gem_version='#{version.strip}'"

    if $is_webkit_engine == true
		  File.open(File.join($srcdir,'apps/rhoconfig.txt'), "a"){ |f| f.write("\r\nwebengine=webkit") }
	  end

    File.open(File.join($srcdir,'apps/rhoconfig.txt'), "a"){ |f| f.write(app_version) }
    File.open(File.join($srcdir,'apps/rhoconfig.txt.timestamp'), "w"){ |f| f.write(Time.now.to_f().to_s()) }
  end

  def self.run_rho_log_server(app_path)
    require 'webrick'

    confpath_content = File.read($srcdir + "/apps/rhoconfig.txt") if File.exists?($srcdir + "/apps/rhoconfig.txt")
    confpath_content += "\r\n" + "rhologurl=http://" + $rhologhostaddr + ":" + $rhologhostport.to_s() if !confpath_content.include?("rhologurl=")
    confpath_content += "\r\n" + "LogToSocket=1" if !confpath_content.include?("LogToSocket=")
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

    system("START rake run:webrickrhologserver[\"#{app_path}\"]")
  end

  def self.get_process_list
    cmd = nil
    args = nil
    proc_list = []

    if RUBY_PLATFORM =~ /(win|w)32$/
      cmd = 'WMIC'
      args = ['path', 'win32_process', 'get', 'Processid,Parentprocessid,Commandline']
    else
      cmd = 'ps'
      args = ['axww', '-o', 'pid', '-o', 'ppid', '-o', 'command']
    end

    output = run2 cmd, args, {:hide_output=>true}

    output.each_line do |line|
      #puts "[[#{line}]]"
      if RUBY_PLATFORM =~ /(win|w)32$/
        match_data = /^(.*)\s+(\d+)\s+(\d+)\s*$/.match line
        proc_list << {:pid=>match_data[3], :ppid=>match_data[2], :cmd=>match_data[1]} if match_data
      else
        match_data = /(\d+)\s+(\d+)\s+(.*)/.match line
        proc_list << {:pid=>match_data[1], :ppid=>match_data[2], :cmd=>match_data[3]} if match_data
      end
    end
    proc_list
  end

  def self.setBuildProp(propName, propValue, config_yml=$app_config)
    config_yml[propName] = propValue
  end

  def self.getBuildProp(propName, config_yml=$app_config)
    res = nil

    res = config_yml[propName] if config_yml[propName]
    res = config_yml[$current_platform][propName] if config_yml[$current_platform] && config_yml[$current_platform][propName]

    res
  end

  def self.getBool(propObject, def_value=false)
    res = propObject

    return def_value unless res

    return true if res && (res.to_i() != 0 || res.casecmp("true") == 0 || res.casecmp("yes") == 0 )

    false
  end

  def self.getBuildBoolProp(propName, config_yml=$app_config, def_value=false)
    res = getBuildProp(propName)

    return def_value unless res

    return true if res && (res.to_i() != 0 || res.casecmp("true") == 0 || res.casecmp("yes") == 0 )

    false
  end

  def self.getBuildProp2(propName, propName2, config_yml=$app_config)
    res = nil
    if config_yml[propName]
        res1 = config_yml[propName]
        res = res1[propName2] if res1 && res1[propName2]
    end

    if config_yml[$current_platform]
        res1 = config_yml[$current_platform][propName]
        res = res1[propName2] if res1 && res1[propName2]
    end

    res
  end

  def self.getBuildBoolProp2(propName, propName2, config_yml=$app_config, def_value=false)
    res = getBuildProp2(propName, propName2)

    return def_value unless res

    return true if res && (res.to_i() != 0 || res.casecmp("true") == 0 || res.casecmp("yes") == 0 )

    false
  end

  def self.copyIfNeeded src, dst
    if File.directory? dst and !File.directory? src
      dst_path = File.join dst, File.basename(src)
    else
      dst_path = dst
    end
    unless File.exists? dst_path and FileUtils.identical? src, dst_path
      FileUtils.rm dst_path if File.exists? dst_path
      FileUtils.cp src, dst
    end
  end

  def self.enquote str
    "\"#{str}\""
  end

end
