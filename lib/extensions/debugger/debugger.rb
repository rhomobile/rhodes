require 'uri'
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

def execute_cmd(cmd, include_code)
  puts "[Debugger] Executing: #{cmd}"
  result = ""
  begin
    result = eval(cmd, $_binding).inspect
  rescue Exception => exc
    result = "ERROR: #{$!}"
  end
  
  result = result.gsub(/\n/,"\\n")
  cmd = URI.escape(cmd.sub(/[\n\r]+$/, ''), Regexp.new("[^#{URI::PATTERN::UNRESERVED}]"))
  $_s.write("EV" + (include_code ? "L:#{cmd}:" : ':') + result + "\n")
end

def get_variables(scope)
  if (scope =~ /^GVARS/)
   cmd = "global_variables"
   prefix = ""
   vartype = "G"
  elsif (scope =~ /^LVARS/)
   cmd = "local_variables"
   prefix = ""
   vartype = "L"
  elsif (scope =~ /^CVARS/)
   if $_classname =~ /^\s*$/
     return
   end
   cmd = "class_variables"
   prefix = "#{$_classname}."
   vartype = "C"
  elsif (scope =~ /^IVARS/)
   if ($_classname =~ /^\s*$/) or ($_methodname =~ /^\s*$/)
     return
   end
   cmd = "instance_variables"
   prefix = "self."
   vartype = "I"
  end
  begin
    # puts "[Debugger] Watching list: '#{prefix}#{cmd}'"
    vars = eval(prefix + cmd, $_binding)
    vars.each do |v|
      # puts "[Debugger] Watching '#{v}'"
      $_s.write("V:#{vartype}:#{v}:#{eval(v,$_binding).inspect}\n")
    end
  rescue
  end
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
    elsif cmd =~ /^STEPOVER/
      $_step = true
      $_step_level = $_call_stack
      wait = false
      puts "[Debugger] Step over"
    elsif cmd =~ /^STEPRET/
      $_step = true
      $_step_level = $_call_stack-1
      wait = false
      puts "[Debugger] Step return"
    elsif cmd =~ /^STEP/
      $_step = true
      $_step_level = -1
      wait = false
      puts "[Debugger] Step into"
    elsif cmd =~ /^CONT/
      wait = false
      $_step = false
      $_resumed = true
      puts "[Debugger] Resuming"
    elsif cmd =~ /^SUSP/
      $_step = true
      $_step_level = -1
      wait = true
      puts "[Debugger] Suspend"
    elsif cmd =~ /^KILL/
      puts "[Debugger] Terminating..."
      System.exit
    elsif cmd =~ /^EVL?:/
      execute_cmd cmd.sub(/^EVL?:/,""), (cmd =~ /^EVL:/ ? true : false)
    elsif cmd =~ /^[GLCI]VARS/
      get_variables cmd
    else
      puts "[Debugger] Unknown command"
    end
  end
  wait
end

$_tracefunc = lambda{|event, file, line, id, bind, classname|
  $_self = self;
  $_binding = bind;
  $_classname = classname;
  $_methodname = id;
  if event =~ /^line/

    unhandled = true
    step_stop = $_step and (($_step_level < 0) or ($_call_stack <= $_step_level))
    if (step_stop or ($_breakpoints_enabled and (not $_breakpoint.empty?)))
      filename = file.sub(/^(.*?[\\\/]|)app[\\\/](.*)$/,'\\2')
      ln = line.to_i.to_s
      if (step_stop or ($_breakpoints_enabled and ($_breakpoint.has_key?(filename + ':' + ln))))
        fn = filename.gsub(/:/, '|')
        cl = classname.to_s.gsub(/:/,'#')
        $_s.write((step_stop ? "STEP" : "BP") + ":#{fn}:#{ln}:#{cl}:#{id}\n")
        puts "[Debugger] " + (step_stop ? "Stop" : "Breakpoint") + " in #{fn} at #{ln}"

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

    if $_resumed
      $_resumed = false
      $_s.write("RESUMED\n")
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
  $_resumed = false
  $_cmd = ""

  at_exit {
   $_s.write("QUIT\n") if (not $_s.nil?)
  }

  set_trace_func $_tracefunc
rescue
  puts "[Debugger] Unable to open connection to debugger: " + $!.inspect
  $_s = nil
end
