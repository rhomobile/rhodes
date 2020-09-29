require 'set'
require 'uri'
require 'timeout'

DEBUGGER_STEP_TYPE = ['STEP', 'STOVER', 'STRET', 'SUSP']
DEBUGGER_STEP_COMMENT = ['Stepped into', 'Stepped over', 'Stepped return', 'Suspended']

DEBUGGER_LOG_LEVEL_DEBUG = 0
DEBUGGER_LOG_LEVEL_INFO = 1
DEBUGGER_LOG_LEVEL_WARN = 2
DEBUGGER_LOG_LEVEL_ERROR = 3

class BreakPoints

  def initialize
    @break_points = Hash.new
    @enabled = true
  end

  def enabled?
    @enabled
  end

  def be_enabled
    @enabled = true
  end

  def be_disabled
    @enabled = false
  end


  def set_on?(file, line)
    puts "file: #{file}, @break_points: #{@break_points}, #{@break_points.has_key?(file)}"
    unless @break_points.has_key?(file)
      return false
    end
    @break_points[file].include?(line)
  end

  def set_break_point_on(file, line)
    puts "set_break_point_on file: #{file}, line: #{line}"
    unless @break_points.has_key?(file)
      @break_points[file] = Set.new
    end
    @break_points[file].add(line)
  end

  def set_break_points_on(file, lines)
    lines.each do |line|
      set_break_point_on(file, line)
    end
  end

  def delete_all_break_points
    @break_points.clear
  end

  def delete_break_point_on(file, line)
    if @break_points.has_key?(file)
      @break_points[file].delete(line)
    end
  end

  def delete_all_break_points_on(file)
    if @break_points.has_key?(file)
      @break_points[file].clear
    end
  end

  def empty?
    return @break_points.empty?
  end


end

def debugger_log(level, msg)
  if (level >= DEBUGGER_LOG_LEVEL_DEBUG) #DEBUGGER_LOG_LEVEL_WARN)
    puts "[Debugger] #{msg}"
  end
end

def log_command(cmd)
  debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Received command: #{cmd}")
end

def convert_to_relative_path(path, app_path)
  if path.include?("./")
    relative_path = "/" + path[path.index("./") + 2, path.length]
  elsif path.include?("lib")
    relative_path = "framework" + path[path.index("lib") + 3, path.length]
  elsif path.include?("framework")
    relative_path = path[path.index("framework"), path.length]
  elsif path.include?("extensions")
    relative_path = path[path.index("extensions"), path.length]
  else
    relative_path = path[app_path.length, path.length - app_path.length]
  end
  puts "convert path '#{path}' with app_path '#{app_path}' to relative_path '#{relative_path}'"
  return relative_path
end

def debug_read_cmd(io, wait)
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
  debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Executing: #{cmd.inspect}, #{advanced}")
  cmd = URI.unescape(cmd.gsub(/\+/, ' ')) if advanced
  result = ""
  error = '0';
  begin
    result = eval(cmd.to_s, $_binding).inspect
  rescue Exception => exc
    error = '1';
    result = "#{$!}".inspect
  end

  cmd = URI.escape(cmd.sub(/[\n\r]+$/, ''), Regexp.new("[^#{URI::PATTERN::UNRESERVED}]")) if advanced
  $_s.write("EV" + (advanced ? "L:#{error}:#{cmd}:" : ':' + (error.to_i != 0 ? 'ERROR: ' : '')) + result + "\n")
end

def get_variables_obsolete(scope)
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
    if ($_classname =~ /^\s*$/) || ($_methodname =~ /^\s*$/)
      return
    end
    cmd = "instance_variables"
    prefix = "self."
    vartype = "I"
  end

  vars = eval(prefix + cmd, $_binding)
  variables = vars.map { |eachVar|
    value = eval(eachVar.to_s, $_binding)
    {
        :name => eachVar,
        :type => value.class.name,
        :value => value.inspect,
        :variablesReference => 0
    }
  }

  message = {:event => 'variables', :variables => variables}
  $_s.write("#{message.to_json}\n")

=begin
  begin
    $_s.write("VSTART:#{vartype}\n")
    vars.each do |v|
      if v !~ /^\$(=|KCODE|-K)$/
        begin
          result = eval(v.to_s, $_binding).inspect
        rescue Exception => exc
          $_s.write("get var exception\n")
          result = "#{$!}".inspect
        end
        $_s.write("V:#{vartype}:#{v}:#{result}\n")
      end
    end
    $_s.write("VEND:#{vartype}\n")
  rescue
  end
=end
end

def get_local_variables() end

def get_variables(scope)
  if (scope === 'global')
    cmd = "global_variables"
    prefix = ""
  elsif (scope === 'local')
    cmd = "local_variables"
    prefix = ""
  elsif (scope === 'instance')
    if $_classname =~ /^\s*$/
      return
    end
    cmd = "class_variables"
    prefix = "#{$_classname}."
  elsif (scope === 'classInstance')
    if ($_classname =~ /^\s*$/) || ($_methodname =~ /^\s*$/)
      return
    end
    cmd = "instance_variables"
    prefix = "self."
  end

  vars = eval(prefix + cmd, $_binding)
  variables = vars.map { |eachVar|
    value = eval(eachVar.to_s, $_binding)
    {
        :name => eachVar,
        :type => value.class.name,
        :value => value.inspect,
        :variablesReference => 0
    }
  }

  {:event => 'variables', :variables => variables}
end

def get_treads
  threads = Thread.list.map { |each| {:id => each.object_id, :name => each} }
  {:event => :threads, :threads => threads}
end

def get_stacktrace(thread_id, launched_on_rhosim, windows_platform)
  puts "[get_stacktrace] thread: #{thread_id}, launched_on_rhosim: #{launched_on_rhosim}, windows: #{windows_platform}"
  thread = Thread.list.find { |each| each.object_id == thread_id }

  backtrace = thread.backtrace
  cutted_backtrace = backtrace.slice(2, backtrace.size - 1)
  frames = cutted_backtrace.map.with_index { |each, idx|
    parts = each.split(':')
    if launched_on_rhosim
      if windows_platform
        # C:/Users/mva/projects/rhomobile/rhodes/lib/extensions/debugger/debugger.rb:633:
        file = convert_to_relative_path(parts[0] + ':' + parts[1], $_app_path)
        line = parts[2].to_i
        name = parts[3]
      else
        file = convert_to_relative_path(parts[0], $_app_path)
        line = parts[1].to_i
        name = parts[2]
      end
    else
      file = convert_to_relative_path(parts[0], $_app_path)
      line = parts[1].to_i
      name = parts[2]
    end

    puts "parts #{parts}, file: #{file}"

    {
        :index => idx,
        :name => name, #parts[2]
        :file => file, #launched_on_rhosim ? parts[0] : convert_to_relative_path(parts[0], $_app_path),
        :line => line, #parts[1].to_i,
        :column => 0
    }
  }

  {:event => :stacktrace, :frames => frames, :count => frames.size}
end

def debug_handle_cmd(inline)
  if ($_cmd.size != 0)
    debugger_log(DEBUGGER_LOG_LEVEL_INFO, "$_cmd: #{$_cmd}")
  end

  cmd = $_cmd.match(/^([^\n\r]*)([\n\r]+|$)/)[0]
  processed = false
  wait = inline

  if cmd != ""
    begin
      debugger_log(DEBUGGER_LOG_LEVEL_INFO, "incoming command #{cmd}")
      json = Rho::JSON.parse(cmd)
      debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Command json #{json}")
      unless json.nil?
        if json['type'] === 'connected'
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Connected to debugger")
          processed = true
        end

        if json['type'] === 'setBreakPoints'
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Setting BreakPoints command")
          source = json['source'].gsub('\\', '/')
          $_breakpoint.set_break_points_on(source, json['lines'])
          processed = true
        end

        if json['type'] === 'threads'
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Threads are requested")
          response = get_treads
          $_s.write("#{response.to_json}\n")
          processed = true
        end

        if json['type'] === 'stacktrace'
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Stacktrace is requested")
          response = get_stacktrace(json['thread'], $is_rhosim, $is_windows)
          $_s.write("#{response.to_json}\n")
          processed = true
        end

        if inline && (json['type'] === 'variables')
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Variables #{json['scope']} are requested")
          response = get_variables(json['scope'])
          $_s.write("#{response.to_json}\n")
          processed = true
        end

        if inline && (json['type'] === 'stepIn')
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "StepIn command")
          $_step = 1
          $_step_level = -1
          $_resumed = true
          wait = false
          processed = true
        end

        if inline && (json['type'] === 'stepOut')
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "StepOut command")
          if $_call_stack < 1
            $_step = 0
            comment = ' (continue)'
          else
            $_step = 3
            $_step_level = $_call_stack - 1
            comment = ''
          end
          $_resumed = true
          wait = false
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Step return" + comment)
          processed = true
        end

        if inline && (json['type'] === 'stepOver')
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "StepOver command")
          $_step = 2
          $_step_level = $_call_stack
          $_resumed = true
          wait = false
          processed = true
        end

        if inline && (json['type'] === 'continue')
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Continue command")
          wait = false
          $_step = 0
          $_resumed = true
          processed = true
        end

        if json['type'] === 'kill'
          debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Kill command")
          processed = true
          System.exit
        end
      end
    rescue StandardError => e
      puts "Rescued: #{e.inspect}"
    end
  end


=begin
  if cmd != ""

    debugger_log(DEBUGGER_LOG_LEVEL_INFO, "cmd: #{cmd}")

    if cmd =~/^CONNECTED/
      log_command(cmd)
      debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Connected to debugger")
      processed = true
    elsif cmd =~/^BACKTRACE/
      thread_id = cmd.split(':').last.to_i
      thread = Thread.list.find { |each| each.object_id == thread_id }
      $_s.write("BACKTRACE:START\n")
      backtrace = thread.backtrace
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, thread.backtrace)
      backtrace.slice(2, backtrace.size - 1).each do |each|
        backtrace_line = each
        unless $is_rhosim
           parts = each.split(':')
           parts[0] = convert_to_relative_path(parts[0], $_app_path)
           backtrace_line = parts.join(':')
        end
        $_s.write("#{backtrace_line}\n")
      end
      $_s.write("BACKTRACE:END\n")
      processed = true
    elsif cmd =~/^THREADS/
      threads = Thread.list.map {|each|  {:id => each.object_id, :name => each}}
      message = {:event => :threads, :threads => threads}
      $_s.write("#{message.to_json}\n")
      processed = true
    elsif cmd =~/^(BP|RM):/
      log_command(cmd)
      ary = cmd.split(":")
      file = ary[1]
      line = ary[2].to_i
      if (cmd =~/^RM:/)
        $_breakpoint.delete_break_point_on(file, line)
        debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Breakpoint removed: #{bp}")
      else
        $_breakpoint.set_break_point_on(file, line)
        debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Breakpoint added: #{file}:#{line}")
      end
      processed = true
    elsif cmd =~ /^RMALL:/
      log_command(cmd)
      path = cmd.split(':')[1]
      file = path[$_app_path.size, path.size]
      $_breakpoint.delete_all_break_points_on(file)
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "All breakpoints removed")
      processed = true
    elsif cmd =~ /^ENABLE/
      log_command(cmd)
      $_breakpoint.be_enabled
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Breakpoints enabled")
      processed = true
    elsif cmd =~ /^DISABLE/
      log_command(cmd)
      $_breakpoint.be_disabled
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Breakpoints disabled")
      processed = true
    elsif inline && (cmd =~ /^STEPOVER/)
      $_s.write("STEPOVER start\n")
      log_command(cmd)
      $_step = 2
      $_step_level = $_call_stack
      $_resumed = true
      wait = false
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Step over")
      processed = true
    elsif inline && (cmd =~ /^STEPRET/)
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
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Step return" + comment)
      processed = true
    elsif inline && (cmd =~ /^STEP/)
      log_command(cmd)
      $_step = 1
      $_step_level = -1
      $_resumed = true
      wait = false
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Step into")
      processed = true
    elsif inline && (cmd =~ /^CONT/)
      log_command(cmd)
      wait = false
      $_step = 0
      $_resumed = true
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Resuming")
      processed = true
    elsif cmd =~ /^SUSP/
      log_command(cmd)
      $_step = 4
      $_step_level = -1
      wait = true
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Suspend")
      processed = true
    elsif cmd =~ /^KILL/
      log_command(cmd)
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Terminating...")
      processed = true
      $_s.write("KILL start\n")
      System.exit
      $_s.write("KILL end\n")
    elsif inline && (cmd =~ /^EVL?:/)
      log_command(cmd)
      processed = true
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Calc evaluation...")
      execute_cmd cmd.sub(/^EVL?:/,""), (cmd =~ /^EVL:/ ? true : false)
    elsif inline && (cmd =~ /^[GLCI]VARS/)
      log_command(cmd)
      debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, "Get variables...")
      get_variables cmd
      processed = true
    elsif inline
      log_command(cmd)
      debugger_log(DEBUGGER_LOG_LEVEL_WARN, "Unknown command")
      processed = true
    end
  end
=end

  if processed
    $_cmd = $_cmd.sub(/^([^\n\r]*)([\n\r]+(.*)|)$/, "\\3")
    $_wait = wait if inline
  end

  processed
end

$_tracefunc = lambda { |event, file, line, id, bind, classname|

  # puts "event: #{event}"
  # puts "file: #{file}"
  # puts "line: #{line}"
  # puts "id: #{id}"
  # puts "bind: #{bind}"
  # puts "classname: #{classname}"


  return if eval('::Thread.current != ::Thread.main', bind)
  $_binding = bind;
  $_classname = classname;
  $_methodname = id;
  file = file.to_s.gsub('\\', '/')

  if (file[0, $_app_path.length] == $_app_path) || (!(file.index("./").nil?)) || (!(file.index("framework").nil?)) || (!(file.index("extensions").nil?))

    if event =~ /^line/
      unhandled = true
      step_stop = ($_step > 0) && (($_step_level < 0) || ($_call_stack <= $_step_level))

      if (step_stop || ($_breakpoint.enabled? && (!($_breakpoint.empty?))))
        filename = ""

=begin
        
        if !(file.index("./").nil?)
          filename = "/" + file[file.index("./") + 2, file.length]
        elsif !(file.index("lib").nil?)
          filename = "framework/" + file[file.index("lib") + 3, file.length]
        elsif !(file.index("framework").nil?)
          filename = file[file.index("framework"), file.length]
        elsif !(file.index("extensions").nil?)
          filename = file[file.index("extensions"), file.length]
        else
          filename = file[$_app_path.length, file.length - $_app_path.length]
        end
=end


=begin
        if file.include?("./")
          filename = "/" + file[file.index("./") + 2, file.length]
        elsif file.include?("lib")
          filename = "framework/" + file[file.index("lib") + 3, file.length]
        elsif file.include?("framework")
          filename = file[file.index("framework"), file.length]
        elsif file.include?("extensions")
          filename = file[file.index("extensions"), file.length]
        else
          filename = file[$_app_path.length, file.length - $_app_path.length]
        end
=end

        filename = convert_to_relative_path(file, $_app_path)
        puts "step_stop #{step_stop}, $_breakpoint.enabled? #{$_breakpoint.enabled?}, $_breakpoint.set_on?(filename, line.to_i) #{$_breakpoint.set_on?(filename, line.to_i)}"

        if step_stop || ($_breakpoint.enabled? && ($_breakpoint.set_on?(filename, line.to_i)))
=begin
          $_s.write("[Debugger][3] stop on bp #{file}:#{line}\n")
=end

          fn = filename.gsub(/:/, '|')
          cl = classname.to_s.gsub(/:/, '#')
          thread_id = Thread.current.object_id

          message = {:event => :stopOnBreakpoint, :threadId => Thread.current.object_id}
          $_s.write("#{message.to_json}\n")

=begin
          $_s.write((step_stop ? DEBUGGER_STEP_TYPE[$_step-1] : "BP") + ":#{fn}:#{line - 1}:#{cl}:#{id}:#{thread_id}\n")
=end
          debugger_log(DEBUGGER_LOG_LEVEL_DEBUG, (step_stop ? DEBUGGER_STEP_COMMENT[$_step - 1] : "Breakpoint") + " in #{fn} at #{line}")
          $_step = 0
          $_step_level = -1

          app_type = ENV["APP_TYPE"]
          $_wait = true
          while $_wait
            while debug_handle_cmd(true) do
            end

            if app_type.eql? "rhodes"
              if Rho::System.main_window_closed
                $_s.write("QUIT\n") if !($_s.nil?)
                $_wait = false
              end
            end

            sleep(0.1) if $_wait
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
  debugger_log(DEBUGGER_LOG_LEVEL_INFO, "Opening connection")
  debug_host_env = ENV['RHOHOST']
  debug_port_env = ENV['rho_debug_port']
  debug_path_env = ENV['ROOT_PATH']

  debug_host = ((debug_host_env.nil?) || (debug_host_env == "")) ? '127.0.0.1' : debug_host_env
  debug_port = ((debug_port_env.nil?) || (debug_port_env == "")) ? 9000 : debug_port_env
  debug_path = ((debug_path_env.nil?) || (debug_path_env == "")) ? "" : debug_path_env

  $is_rhosim = false
  puts("RUBY_PLATFORM #{RUBY_PLATFORM}")
  $is_windows = (/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil

  if defined?(RHOSTUDIO_REMOTE_DEBUG) && RHOSTUDIO_REMOTE_DEBUG == true
    puts "[debugger] RHOSTUDIO_REMOTE_HOST=" + RHOSTUDIO_REMOTE_HOST.to_s
    debug_host = ((RHOSTUDIO_REMOTE_HOST.to_s != nil) || (RHOSTUDIO_REMOTE_HOST.to_s != "")) ? RHOSTUDIO_REMOTE_HOST.to_s : ""
    debug_path = "apps/app/"
  else
    $is_rhosim = true
    debug_path = ((debug_path_env.nil?) || (debug_path_env == "")) ? "" : debug_path_env
  end

  debugger_log(DEBUGGER_LOG_LEVEL_INFO, "host=" + debug_host.to_s)
  debugger_log(DEBUGGER_LOG_LEVEL_INFO, "port=" + debug_port.to_s)
  debugger_log(DEBUGGER_LOG_LEVEL_INFO, "path=" + debug_path.to_s)

  $_s = timeout(30) { TCPSocket.open(debug_host, debug_port) }

  debugger_log(DEBUGGER_LOG_LEVEL_WARN, "Connected: " + $_s.to_s)

  message = {:event => :connect, :host => debug_host, :port => debug_port, :debugPath => $_app_path}
  $_s.write("#{message.to_json}\n")

  $_s.write("CONNECT\nHOST=" + debug_host.to_s + "\nPORT=" + debug_port.to_s + "\n")

  #$_breakpoint = Hash.new
  $_breakpoint = BreakPoints.new()
  #$_breakpoints_enabled = true
  $_step = 0
  $_step_level = -1
  $_call_stack = 0
  $_resumed = false
  $_cmd = ""
  $_app_path = debug_path
  $_s.write("DEBUG PATH=" + $_app_path.to_s + "\n")

  at_exit {
    $_s.write("QUIT\n") if !($_s.nil?)
  }

  set_trace_func $_tracefunc

  Thread.new {
    while true
      debug_read_cmd($_s, true)
      while debug_handle_cmd(false) do
      end
      if ($_cmd !~ /^\s*$/) && (Thread.main.stop?)
        $_wait = true
        Thread.main.wakeup
      end
    end
  }

rescue
  debugger_log(DEBUGGER_LOG_LEVEL_ERROR, "Unable to open connection to debugger: " + $!.inspect)
  $_s = nil
end
