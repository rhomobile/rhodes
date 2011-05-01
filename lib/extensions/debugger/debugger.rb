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
      $_file = ary[1].gsub(/\|/,':')
      $_line = ary[2].chomp
      wait = true
      puts "Breakpoint received: #{$_file}:#{$_line}"
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

    if ($_file or $_step)
      if ((file =~ Regexp.new(Regexp.escape($_file) + '$') and line.to_i == $_line.to_i) or $_step)
        filename = file.gsub(/:/, '|')
        $_s.write("BP:#{filename}:#{line}\n")

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

  $_file = nil
  $_line = nil
  $_step = false
  $_cmd = ""

  set_trace_func $_tracefunc
rescue
  puts "Unable to open connection to debugger:" + $!.inspect
  $_s = nil
end
