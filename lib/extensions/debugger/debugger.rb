require 'timeout'

def debug_read_cmd(io,wait)
  begin
    if wait
      if ($_cmd !~/[\n\r]/)
        $_cmd << io.readpartial(4096)
      end
    else
      $_cmd << io.read_nonblock(4096)
    end
  rescue
    # puts $!.inspect
  end
end

def execute_cmd(cmd) 
  puts "[Debugger] Executing: #{cmd}"
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
  cmd = $_cmd.match(/^([^\n\r]*)([\n\r]+|$)/)[0]
  $_cmd = $_cmd.sub(/^([^\n\r]*)([\n\r]+|$)(.*$)/, "\\3")
  wait = true
  if cmd != ""
    puts "[Debugger] Received command: #{cmd}"
    if cmd =~/^CONNECTED/
      puts "[Debugger] Connected to debugger"
      wait = false
    elsif cmd =~/^(BP|RM):/
      ary = cmd.split(":")
      bp = ary[1].gsub(/\|/,':') + ':' + ary[2].chomp
      if (cmd =~/^RM:/)
        $_breakpoint.delete(bp)
        puts "[Debugger] Breakpoint removed: #{bp}"
      else
        $_breakpoint.store(bp,1)
        puts "[Debugger] Breakpoint added: #{bp}"
      end
    elsif cmd =~ /^RMALL/
      $_breakpoint.clear
      puts "[Debugger] All breakpoints removed"
    elsif cmd =~ /^ENABLE/
      $_breakpoints_enabled = true
      puts "[Debugger] Breakpoints enabled"
    elsif cmd =~ /^DISABLE/
      $_breakpoints_enabled = false
      puts "[Debugger] Breakpoints disabled"
    elsif cmd =~ /^STEP/
      $_step = true
      wait = false
      puts "[Debugger] Step"
    elsif cmd =~ /^CONT/
      wait = false
      $_step = false
      puts "[Debugger] Resuming"
    elsif cmd =~ /^EV:/
      execute_cmd cmd.sub(/^EV:/,"")
    else
      puts "[Debugger] Unknown command"
    end
  end
  wait
end

$_tracefunc = lambda{|event, file, line, id, bind, classname|
  $_self = self;
  $_binding = bind;
  if event =~ /line/

    if ($_step or ($_breakpoints_enabled and (not $_breakpoint.empty?)))
      filename = file.sub(/^(.*?[\\\/]|)app[\\\/](.*)$/,'\\2')
      ln = line.to_i.to_s
      if ($_step or ($_breakpoints_enabled and ($_breakpoint.has_key?(filename + ':' + ln))))
        fn = filename.gsub(/:/, '|')
        $_s.write(($_step ? "STEP" : "BP") + ":#{fn}:#{ln}\n")
        puts "[Debugger] " + ($_step ? "Stop" : "Breakpoint") + " in #{fn} at #{ln}"

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
  puts "[Debugger] Opening connection"
  debug_host = (Rho::RhoConfig.debug_host.nil? or Rho::RhoConfig.debug_host == "") ? '127.0.0.1' : Rho::RhoConfig.debug_host
  debug_port = (Rho::RhoConfig.debug_port.nil? or Rho::RhoConfig.debug_port == "") ? 9000 : Rho::RhoConfig.debug_port
  $_s = timeout(30) { TCPSocket.open(debug_host, debug_port) }

  puts "[Debugger] Connected: " + $_s.to_s
  $_s.write("CONNECT\n")

  $_breakpoint = Hash.new
  $_breakpoints_enabled = true
  $_step = false
  $_cmd = ""

  set_trace_func $_tracefunc
rescue
  puts "[Debugger] Unable to open connection to debugger: " + $!.inspect
  $_s = nil
end

END {
  $_s.write("QUIT\n") if (not $_s.nil?)
}
