
require 'net/http'

puts "opening connection"
$_s = timeout(5000) { TCPSocket.open("192.168.1.110", 9000) }
puts "connected"
$_s.write("CONNECT\n")

$_file = nil
$_line = nil
$_step = false

def read_cmd(io)
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



$_tracefunc = lambda{|event, file, line, id, bind, classname|
  $_self = self;
  $_binding = bind;
  if event =~ /line/ 

    if ($_file or $_step)
      if ((file =~ Regexp.new(Regexp.escape($_file) + '$') and line.to_i == $_line.to_i) or $_step)
        $_s.write("BP:#{file}:#{line}\n")

        wait = true
        while wait
          cmd = read_cmd $_s
          if cmd =~ /CONT/
            wait = false
            $_step = false
          elsif cmd =~ /STEP/
            puts "STEP"

            wait = false
            $_step = true
          
          elsif cmd =~/^BP/
            ary = cmd.split(":")
            $_file = ary[1]
            $_line = ary[2].chomp
            puts "Breakpoint received:#{$_file}:#{$_line}"
          end
        end
      end
    end

    cmd = read_cmd $_s
    if cmd =~/CONNECTED/
      puts "Connected to debugger"
    elsif cmd =~/^BP/
      ary = cmd.split(":")
      $_file = ary[1]
      $_line = ary[2].chomp
      puts "Breakpoint received:#{$_file}:#{$_line}"
    elsif cmd =~ /STEP/
      $_step = true
      puts "Received STEP"
    end
  end
}

set_trace_func $_tracefunc