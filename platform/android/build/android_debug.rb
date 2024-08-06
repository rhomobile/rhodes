require 'open3'

class AndroidDebug
    
    def isWindows?
        OS.windows?
    end

    def initialize(android_package, a_p, p, p2, t)
        @package = android_package

        if(p == nil || p.empty?) 
            @port = 5039
        else
            @port = p.to_i
        end

        @jdwp_port = p2
        @app_path = a_p

        if(t == nil || t.empty?) 
            @type = "-d"
        else
            if t == "device"
                @type = "-d"
            elsif t == "emul"
                @type = "-e"
            else
                @type = "-d"
            end
        end
    end
  
    def execute_blocked(cmd)
        Open3.popen3(cmd) do |stdin, stdout, stderr, wait_thr|
            if wait_thr.value != 0 
                raise "Shell return error: #{wait_thr.value.to_s}\n STDOUT: #{stdout.read}" 
            end
            output = stdout.read
            return output
        end
    end

    def execute_non_blocked(cmd)
        stdin, stdout, stderr, wait_thr = Open3.popen3(cmd)
        sleep(3.0)

        stdin.close
        stderr.close
        stdout.close
    end

    def print_gdbserver_out
        puts "GDBSERVER says: "
        text=File.open("gdbserver.log").read
        text.each_line('/r/n') do |line|
            puts line
        end
    end

    def get_pid_process(process_name)
        adb_command_ps = "adb #{@type} shell \"ps | grep #{process_name}\""
        output_ps = execute_blocked(adb_command_ps)

        if output_ps.empty?
            return -1
        else
            return output_ps.split(' ')[1].to_i
        end
    end


    def StartGdbServer

        Dir.chdir(@app_path)
        @pid = get_pid_process("gdbserver")
        if get_pid_process("gdbserver") > 0
            execute_blocked("adb #{@type} shell \"run-as #{@package} kill #{@pid}\"")
        end

        output_ps = execute_blocked("adb #{@type} shell \"ps | grep #{@package}\"")
        if output_ps.empty?
            raise "Debug process not found!"
        else
            @pid = output_ps.split(' ')[1]
        end

        adb_command_start = "adb #{@type} shell \"run-as #{@package} ./lib/gdbserver :#{@port} --attach #{@pid}\" > gdbserver.log"
        execute_non_blocked(adb_command_start)
        print_gdbserver_out()

        execute_blocked("adb #{@type} forward tcp:#{@port} tcp:#{@port}")
    end

    def StartAppOnDebug
        @pid = get_pid_process(@package)
        if @pid != -1
            execute_blocked("adb #{@type} shell \"run-as #{@package} kill #{@pid}\"")
        end

        execute_blocked("adb #{@type} shell \"am start -D #{@package}/com.rhomobile.rhodes.RhodesActivity\"")
        @pid = get_pid_process(@package)
        puts "App wait debuger (#{@pid})"
    end

    def InitBreakPointList
        bps = []
        breakpoints = File.join @app_path, "breakpoints.bps"
        if File.size?(breakpoints)
            text = File.open(breakpoints).read
            text.each_line do |line|
                bps << line
            end
            return bps
        else
            return bps
        end
    end


    def StartGdb(path_to_gdb)
        if isWindows?()
            path_to_gdb = path_to_gdb + '.exe'
        end
        if !File.exist?(path_to_gdb) 
            raise "GDB not found!"
        end
        if !File.executable?(path_to_gdb) 
            raise "It's not GDB!!!"
        end

        bps = InitBreakPointList()

        Dir.chdir(@app_path)
        File.open("start_debug", "w+") do |f| 
            #f.write("set interactive-mode on\n")
            f.write("set breakpoint pending on\n")
            f.write("handle SIG33 nostop\n")
            f.write("target remote :#{@port}\n")
            bps.each do |item| 
                f.write("b #{item}\n")
            end
            f.write("handle SIG33 nostop\n")
            f.write("c\n")
        end    
        
        system(path_to_gdb)
    end

    def StartJdb
        @pid = get_pid_process(@package)
        if @pid == -1
            raise "Application not running!"
        end
        execute_blocked("adb forward tcp:#{@jdwp_port} jdwp:#{@pid}")
        system("jdb -connect com.sun.jdi.SocketAttach:hostname=localhost,port=#{@jdwp_port}")
    end

end