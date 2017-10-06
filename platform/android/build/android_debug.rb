require 'Open3'

class AndroidDebug
    
    def isWindows?
        if /cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM
            return true
        else
            return false
        end
    end

    def initialize(android_package, p)
        @package = android_package
        @port = p
    end
  
    def execute_blocked(cmd)
        Open3.popen3(cmd) do |stdin, stdout, stderr, wait_thr|
            if wait_thr.value != 0 
                raise "Shell return error: #{wait_thr.value.to_s}\n STDERR: #{stdout.read}" 
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
        adb_command_ps = "adb shell \"ps | grep #{process_name}\""
        output_ps = execute_blocked(adb_command_ps)

        if output_ps.empty?
            return -1
        else
            return output_ps.split(' ')[1].to_i
        end
    end


    def StartGdbServer

        @pid = get_pid_process("gdbserver")
        if get_pid_process("gdbserver") > 0
            execute_blocked("adb shell \"run-as #{@package} kill #{@pid}\"")
        end

        output_ps = execute_blocked("adb shell \"ps | grep #{@package}\"")
        if output_ps.empty?
            raise "Debug process not found!"
        else
            @pid = output_ps.split(' ')[1]
        end

        adb_command_start = "adb shell \"run-as #{@package} ./lib/gdbserver :#{@port} --attach #{@pid}\" > gdbserver.log"
        execute_non_blocked(adb_command_start)
        print_gdbserver_out()

        execute_blocked("adb forward tcp:#{@port} tcp:#{@port}")
    end

    def StartGdb(path_to_gdb, path_to_lib)
        if isWindows?()
            path_to_gdb = path_to_gdb + '.exe'
        end
        if !File.exist?(path_to_gdb) 
            raise "GDB not found!"
        end
        if !File.executable?(path_to_gdb) 
            raise "It's not GDB!!!"
        end
        system(path_to_gdb + " \"target remote :5039 \"")
    end

end