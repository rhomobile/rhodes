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
    elsif cmd =~ /^STEPINTO/
      $_step = true
      $_step_level = -1
      wait = false
      puts "[Debugger] Step into"
    elsif cmd =~ /^STEPOVER/
      $_step = true
      $_step_level = $_call_stack
      wait = false
      puts "[Debugger] Step over"
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
  if event =~ /^line/

    unhandled = true
    step_stop = $_step and (($_step_level < 0) or ($_call_stack <= $_step_level))
    if (step_stop or ($_breakpoints_enabled and (not $_breakpoint.empty?)))
      filename = file.sub(/^(.*?[\\\/]|)app[\\\/](.*)$/,'\\2')
      ln = line.to_i.to_s
      if (step_stop or ($_breakpoints_enabled and ($_breakpoint.has_key?(filename + ':' + ln))))
        fn = filename.gsub(/:/, '|')
        $_s.write((step_stop ? "STEP" : "BP") + ":#{fn}:#{ln}\n")
        puts "[Debugger] " + (step_stop ? "Stop" : "Breakpoint") + " in #{fn} at #{ln}"

        local = eval("local_variables", bind)
        local.each do |l|
          $_s.write("AW:#{l}:#{eval(l,bind).inspect}\n")
        end

        wait = true
        while wait
          debug_read_cmd($_s,wait)
          wait = debug_handle_cmd()
        end
        unhandled = false
      end
    end

    if unhandled
      debug_read_cmd($_s,false)
      debug_handle_cmd()
    end

  elsif event =~ /^call/
    ++$_call_stack
  elsif event =~ /^return/
    --$_call_stack
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
  $_step_level = -1
  $_call_stack = 0
  $_cmd = ""

  set_trace_func $_tracefunc
rescue
  puts "[Debugger] Unable to open connection to debugger: " + $!.inspect
  $_s = nil
end

END {
  $_s.write("QUIT\n") if (not $_s.nil?)
}
