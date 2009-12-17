
require 'net/http'

def debug_read_cmd(io)
    outbuf = ""
    begin
      r,w,e = IO.select([io],nil,nil,0)
       while r
         outbuf << io.read(1)
         r,w,e = IO.select([io],nil,nil,0)
       end
    rescue
      puts $!.inspect
    end
    outbuf
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

def debug_handle_cmd(cmd)
  puts "Received command: #{cmd}"
  wait = true
  if cmd =~/CONNECTED/
    puts "Connected to debugger"
    wait = false
  elsif cmd =~/^BP/
    ary = cmd.split(":")
    $_file = ary[1]
    $_line = ary[2].chomp
    wait = true
    puts "Breakpoint received:#{$_file}:#{$_line}"
  elsif cmd =~ /STEP/
    $_step = true
    wait = false
    puts "Received STEP"
  elsif cmd =~ /CONT/
    wait = false
    $_step = false
  elsif cmd =~ /^EV:/
    cmd = cmd.gsub(/^EV:/,"")
    execute_cmd cmd
  end


  wait
end

$_tracefunc = lambda{|event, file, line, id, bind, classname|
  $_self = self;
  $_binding = bind;
  if event =~ /line/

    if ($_file or $_step)
      if ((file =~ Regexp.new(Regexp.escape($_file) + '$') and line.to_i == $_line.to_i) or $_step)
        $_s.write("BP:#{file}:#{line}\n")

        wait = true
        while wait
          cmd = debug_read_cmd $_s
          wait = debug_handle_cmd(cmd) if cmd != ""
        end
      end
    end

    cmd = debug_read_cmd $_s
    debug_handle_cmd(cmd) if cmd != ""
  end
}


$_s = nil

begin
  puts "opening connection"
  raise "debug host not set" if (Rho::RhoConfig.debug_host.nil? or Rho::RhoConfig.debug_host == "")
  $_s = timeout(30) { TCPSocket.open(Rho::RhoConfig.debug_host, 9000) }

  puts "connected: " + $_s.to_s
  $_s.write("CONNECT\n")

  $_file = nil
  $_line = nil
  $_step = false

  set_trace_func $_tracefunc
rescue
  puts "Unable to open connection to debugger:" + $!.inspect
  $_s = nil
end





