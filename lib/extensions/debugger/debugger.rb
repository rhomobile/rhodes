require 'timeout'

def debug_read_cmd(io,wait)
  begin
    if wait
      $_cmd << io.readpartial(4096)
    else
      $_cmd << io.read_nonblock(4096)
    end
  rescue
    # puts $!.inspect
  end
end

def execute_cmd(cmd) 
  puts "executing: #{cmd}"
  result = ""
  begin
    result = eval(cmd, $_binding).inspect
  rescue
    result = "ERROR: #{$!}"
  end
  
  result = result.gsub(/\n/,"\\n")
  $_s.write("EV:" + result + "\n")
end

def debug_handle_cmd()
  cmd = $_cmd.match(/^([^\n\r])*([\n\r]+|$)/)[0]
  $_cmd = $_cmd.sub(/^([^\n\r])*([\n\r]+|$)(.*$)/, "\\3")
  if cmd != ""
    puts "Received command: #{cmd}"
    wait = true
    if cmd =~/^CONNECTED/
      puts "Connected to debugger"
      wait = false
    elsif cmd =~/^BP/
      ary = cmd.split(":")
      bp = ary[1].gsub(/\|/,':') + ':' + ary[2].chomp
      $_breakpoint.store(bp,1)
      wait = true
      puts "Breakpoint received: #{bp}"
    elsif cmd =~ /^STEP/
      $_step = true
      wait = false
      puts "Received STEP"
    elsif cmd =~ /^CONT/
      wait = false
      $_step = false
    elsif cmd =~ /^EV:/
      cmd = cmd.gsub(/^EV:/,"")
      execute_cmd cmd
    end
  end
  wait
end

$_tracefunc = lambda{|event, file, line, id, bind, classname|
  $_self = self;
  $_binding = bind;
  if event =~ /line/

    if (!($_breakpoint.empty?))
      filename = file.sub(/^(.*?[\\\/]|)app[\\\/](.*)$/,'\\2')
      ln = line.to_i.to_s
      if (($_breakpoint.has_key?(filename + ':' + ln)) or $_step)
        fn = filename.gsub(/:/, '|')
        $_s.write("BP:#{fn}:#{ln}\n")

        wait = true
        while wait
          debug_read_cmd($_s,wait)
          wait = debug_handle_cmd()
        end
      end
    end

    debug_read_cmd($_s,false)
    debug_handle_cmd()
  end
}


$_s = nil

begin
  puts "opening connection"
  debug_host = (Rho::RhoConfig.debug_host.nil? or Rho::RhoConfig.debug_host == "") ? '127.0.0.1' : Rho::RhoConfig.debug_host
  debug_port = (Rho::RhoConfig.debug_port.nil? or Rho::RhoConfig.debug_port == "") ? 9000 : Rho::RhoConfig.debug_port
  $_s = timeout(30) { TCPSocket.open(debug_host, debug_port) }

  puts "connected: " + $_s.to_s
  $_s.write("CONNECT\n")

  $_breakpoint = Hash.new
  $_step = false
  $_cmd = ""

  set_trace_func $_tracefunc
rescue
  puts "Unable to open connection to debugger:" + $!.inspect
  $_s = nil
end
