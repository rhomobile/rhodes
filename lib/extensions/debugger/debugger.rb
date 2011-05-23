require 'uri'
require 'timeout'

DEBUGGER_STEP_TYPE = ['STEP','STOVER','STRET','SUSP']
DEBUGGER_STEP_COMMENT = ['Stepped into','Stepped over','Stepped return','Suspended']

def debug_read_cmd(io,wait)
  begin
    if wait
      cmd = io.readpartial(4096)
      $_cmd << cmd if cmd !~ /^\s*$/
    else
      cmd = io.read_nonblock(4096)
      $_cmd << cmd if cmd !~ /^\s*$/
    end
  rescue
    # puts $!.inspect
  end
end

def execute_cmd(cmd, advanced)
  cmd = URI.unescape(cmd.gsub(/\+/,' ')) if advanced
  puts "[Debugger] Executing: #{cmd.inspect}"
  result = ""
  error = '0';
  begin
    result = eval(cmd, $_binding).inspect
  rescue Exception => exc
    error = '1';
    result = "#{$!}".inspect
  end
  
  cmd = URI.escape(cmd.sub(/[\n\r]+$/, ''), Regexp.new("[^#{URI::PATTERN::UNRESERVED}]")) if advanced
  $_s.write("EV" + (advanced ? "L:#{error}:#{cmd}:" : ':'+(error.to_i != 0 ? 'ERROR: ':'')) + result + "\n")
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
    vars = eval(prefix + cmd, $_binding)
    $_s.write("VSTART:#{vartype}\n")
    vars.each do |v|
      if v !~ /^\$(=|KCODE)$/
        begin
          result = eval(v,$_binding).inspect
        rescue Exception => exc
          result = "#{$!}".inspect
        end
        $_s.write("V:#{vartype}:#{v}:#{result}\n")
      end
    end
    $_s.write("VEND:#{vartype}\n")
  rescue
  end
end

def log_command(cmd)
  # puts "[Debugger] Received command: #{cmd}"
end

def debug_handle_cmd(inline)
  cmd = $_cmd.match(/^([^\n\r]*)([\n\r]+|$)/)[0]
  processed = false
  wait = inline
  if cmd != ""
    if cmd =~/^CONNECTED/
      log_command(cmd)
      puts "[Debugger] Connected to debugger"
      processed = true
    elsif cmd =~/^(BP|RM):/
      log_command(cmd)
      ary = cmd.split(":")
      bp = ary[1].gsub(/\|/,':') + ':' + ary[2].chomp
      if (cmd =~/^RM:/)
        $_breakpoint.delete(bp)
        puts "[Debugger] Breakpoint removed: #{bp}"
      else
        $_breakpoint.store(bp,1)
        puts "[Debugger] Breakpoint added: #{bp}"
      end
      processed = true
    elsif cmd =~ /^RMALL/
      log_command(cmd)
      $_breakpoint.clear
      puts "[Debugger] All breakpoints removed"
      processed = true
    elsif cmd =~ /^ENABLE/
      log_command(cmd)
      $_breakpoints_enabled = true
      puts "[Debugger] Breakpoints enabled"
      processed = true
    elsif cmd =~ /^DISABLE/
      log_command(cmd)
      $_breakpoints_enabled = false
      puts "[Debugger] Breakpoints disabled"
      processed = true
    elsif inline and (cmd =~ /^STEPOVER/)
      log_command(cmd)
      $_step = 2
      $_step_level = $_call_stack
      $_resumed = true
      wait = false
      puts "[Debugger] Step over"
      processed = true
    elsif inline and (cmd =~ /^STEPRET/)
      log_command(cmd)
      if $_call_stack < 1
        $_step = 0
        comment = ' (continue)'
      else
        $_step = 3
        $_step_level = $_call_stack-1;
        comment = ''
      end
      $_resumed = true
      wait = false
      puts "[Debugger] Step return" + comment
      processed = true
    elsif inline and (cmd =~ /^STEP/)
      log_command(cmd)
      $_step = 1
      $_step_level = -1
      $_resumed = true
      wait = false
      puts "[Debugger] Step into"
      processed = true
    elsif inline and (cmd =~ /^CONT/)
      log_command(cmd)
      wait = false
      $_step = 0
      $_resumed = true
      puts "[Debugger] Resuming"
      processed = true
    elsif cmd =~ /^SUSP/
      log_command(cmd)
      $_step = 4
      $_step_level = -1
      wait = true
      puts "[Debugger] Suspend"
      processed = true
    elsif cmd =~ /^KILL/
      log_command(cmd)
      puts "[Debugger] Terminating..."
      processed = true
      System.exit
    elsif inline and (cmd =~ /^EVL?:/)
      log_command(cmd)
      processed = true
      execute_cmd cmd.sub(/^EVL?:/,""), (cmd =~ /^EVL:/ ? true : false)
    elsif inline and (cmd =~ /^[GLCI]VARS/)
      log_command(cmd)
      get_variables cmd
      processed = true
    elsif inline
      log_command(cmd)
      puts "[Debugger] Unknown command"
      processed = true
    end
  end
  if processed
    $_cmd = $_cmd.sub(/^([^\n\r]*)[\n\r]+(.*$)/, "\\2")
    $_wait = wait if inline
  end
  processed
end

$_tracefunc = lambda{|event, file, line, id, bind, classname|
  return if eval('Thread.current!=Thread.main', bind)
  $_binding = bind;
  $_classname = classname;
  $_methodname = id;
  file = file.to_s.gsub('\\', '/')
  if file[0, $_app_path.length] == $_app_path
    if event =~ /^line/

      unhandled = true
      step_stop = ($_step > 0) and (($_step_level < 0) or ($_call_stack <= $_step_level))
      if (step_stop or ($_breakpoints_enabled and (not $_breakpoint.empty?)))
        filename = file[$_app_path.length, file.length-$_app_path.length]
        ln = line.to_i.to_s
        if (step_stop or ($_breakpoints_enabled and ($_breakpoint.has_key?(filename + ':' + ln))))
          fn = filename.gsub(/:/, '|')
          cl = classname.to_s.gsub(/:/,'#')
          $_s.write((step_stop ? DEBUGGER_STEP_TYPE[$_step-1] : "BP") + ":#{fn}:#{ln}:#{cl}:#{id}\n")
          puts "[Debugger] " + (step_stop ? DEBUGGER_STEP_COMMENT[$_step-1] : "Breakpoint") + " in #{fn} at #{ln}"
          $_step = 0
          $_step_level = -1

          $_wait = true
          while $_wait
            while debug_handle_cmd(true) do end
            if System::get_property('main_window_closed')
              $_wait = false
            end
            sleep if $_wait
          end
          unhandled = false
        end
      end

      if unhandled
        debug_handle_cmd(true)
      end

    elsif event =~ /^call/
      $_call_stack += 1
    elsif event =~ /^return/
      $_call_stack -= 1
    end
  end

  if $_resumed
    $_resumed = false
    $_s.write("RESUMED\n")
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
  $_step = 0
  $_step_level = -1
  $_call_stack = 0
  $_resumed = false
  $_cmd = ""
  $_app_path = File.join(Rho::RhoApplication::get_base_app_path(), 'app/').gsub('\\', '/')

  at_exit {
    $_s.write("QUIT\n") if (not $_s.nil?)
  }

  set_trace_func $_tracefunc

  Thread.new {
    while true
      debug_read_cmd($_s,true)
      while debug_handle_cmd(false) do end
      if ($_cmd !~ /^\s*$/) and (Thread.main.stop?)
        $_wait = true
        Thread.main.wakeup
      end
    end
  }

rescue
  puts "[Debugger] Unable to open connection to debugger: " + $!.inspect
  $_s = nil
end
