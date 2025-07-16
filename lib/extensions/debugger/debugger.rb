require "socket"
require "json/pure"
require "set"

class DAPServer
  def initialize(host, port)
    @breakpoints = {}  # ключ: путь, значение: Set строк
    @current_binding = nil
    @stack_frames = []
    @frame_bindings = {}
    @next_var_ref = 1
    @variables_map = {} # variablesReference => [binding, :local | :instance | :global]
    @step_mode = nil
    @server = TCPServer.new(host, port)
    puts "DAP server listening on #{host}:#{port}"
  end

  def start
    loop do
      @client = @server.accept
      Thread.new { handle_client() }
    end
  end

  def handle_client()
    loop do
      header = read_header()
      break unless header

      content_length = header["Content-Length"].to_i
      json_data = @client.read(content_length)
      request = Rho::JSON.parse(json_data)
      handle_request(request)
    end
  rescue => e
    puts "[Error] #{e.message}"
  ensure
    @client&.close
  end

  def read_header()
    header = {}
    while (line = @client.gets)
      line = line.strip
      break if line.empty?
      key, value = line.split(/:\s*/, 2)
      header[key] = value
    end
    header.empty? ? nil : header
  end

  def send_response(req, body: {}, success: true, message: nil)
    response = {
      type: "response",
      request_seq: req["seq"],
      success: success,
      command: req["command"],
    }
    response[:body] = body if body.any?
    response[:message] = message if message
    write_message(response)
  end

  def send_event(event_name, body = {})
    event = {
      type: "event",
      event: event_name,
      body: body,
    }
    write_message(event)
  end

  def write_message(message)
    body = message.to_json
    header = "Content-Length: #{body.bytesize}\r\n\r\n"
    puts "app -> vsc: #{body}"
    @client.write(header + body)
  end

  def handle_request(req)
    puts "vsc -> app: #{req}"

    return unless req["type"] == "request"

    case req["command"]
    when "initialize"
      handle_initialize(req)
    when "attach"
      handle_attach(req)
    when "threads"
      handle_threads(req)
    when "stackTrace"
      handle_stack_trace(req)
    when "scopes"
      handle_scopes(req)
    when "variables"
      handle_variables(req)
    when "disconnect"
      handle_disconnect(req)
    when "setBreakpoints"
      handle_set_breakpoints(req)
    when "evaluate"
      handle_evaluate(req)
    when "continue"
      handle_continue(req)
    when "next"
      handle_next(req)
    else
      send_response(req, success: false, message: "Command not implemented")
    end
  end

  def trace_callback(event, file, line, method_name, binding, klass)
    normalized_file = File.expand_path(file)
    return unless normalized_file.start_with?(@root_path)

    msg = "[#{event}] #{file}:#{line} #{klass} #{method_name}"
    send_output_event("console", msg)

    return unless event == "line"

    stop_reason = nil
    if @step_mode == :next
      stop_reason = "step"
      @step_mode = nil
    elsif @breakpoints[normalized_file]&.include?(line)
      stop_reason = "breakpoint"
    end

    return unless stop_reason

    unless @stopped
      @stopped = true
      puts "[BREAK] Hit #{stop_reason} at #{normalized_file}:#{line}"

      @current_binding = binding
      update_stack_frames(binding)
      handle_stop(stop_reason, normalized_file, line)
    end
  end

  # === Command Handlers ===

  def handle_continue(req)
    @wait_for_continue = false
    send_response(req, body: { allThreadsContinued: true })
  end

  def handle_next(req)
    @step_mode = :next
    @wait_for_continue = false
    send_response(req)
  end

  def handle_initialize(req)
    send_response(req, body: { supportsEvaluateForHovers: true })
    send_event("initialized")
  end

  def handle_attach(req)
    args = req["arguments"] || {}

    @root_path = File.expand_path(args["appRoot"] || Dir.pwd)
    puts "APP ROOT: #{@root_path}"

    set_trace_func method(:trace_callback).to_proc
    send_response(req)
  end

  def handle_threads(req)
    threads = Thread.list.map { |each| { :id => each.object_id, :name => each } }
    send_response(req, body: { threads: threads })
  end

  def handle_set_breakpoints(req)
    source = req.dig("arguments", "source", "path")
    breakpoints = req.dig("arguments", "breakpoints") || []

    lines = breakpoints.map { |bp| bp["line"] }.to_set
    @breakpoints[source] = lines

    # Подтверждение установки
    send_response(req, body: {
                         breakpoints: lines.map { |line| { verified: true, line: line } },
                       })
  end

  def handle_stack_trace(req)
    thread_id = req.dig("arguments", "threadId")

    #Для MVP: поддержка только главного потока
    if Thread.main.object_id != thread_id
      send_response(req, body: { stackFrames: [], totalFrames: 0 })
      return
    end

    frames = @stack_frames || []
    send_response(req, body: {
                         stackFrames: frames,
                         totalFrames: frames.size,
                       })
  end

  def handle_disconnect(req)
    send_response(req)
    # Доп. логика: остановить обработку, закрыть соединение и т. д.
  end

  def handle_evaluate(req)
    expr = req.dig("arguments", "expression") || ""
    result = "Echo: #{expr}"
    send_response(req, body: {
                         result: result,
                         variablesReference: 0,
                       })
  end

  def send_output_event(category, message)
    send_event("output", {
      category: category,
      output: message + "\n",
    })
  end

  def update_stack_frames(binding)
    locations = caller_locations(1) # skip this method itself
    @stack_frames = []
    @frame_bindings = {}

    filtered = []

    # Обрезаем стек выше trace_callback
    locations.each do |loc|
      path = File.expand_path(loc.path)
      next if path.include?(__FILE__) # пропускаем вызовы из этого файла (DAP-сервер)
      filtered << loc
    end

    filtered.each_with_index do |loc, i|
      @stack_frames << {
        id: i,
        name: loc.label,
        source: { name: File.basename(loc.path), path: File.expand_path(loc.path) },
        line: loc.lineno,
        column: 1,
      }

      # пока только верхний frame имеет binding
      @frame_bindings[i] = binding if i == 0
    end
  end

  def handle_scopes(req)
    frame_id = req.dig("arguments", "frameId")
    binding = @frame_bindings[frame_id]

    scopes = []

    if binding
      local_ref = @next_var_ref
      @next_var_ref += 1
      @variables_map[local_ref] = [binding, :local]

      scopes << {
        name: "Local",
        variablesReference: local_ref,
        expensive: false,
      }

      instance_ref = @next_var_ref
      @next_var_ref += 1
      @variables_map[instance_ref] = [binding, :instance]

      scopes << {
        name: "Instance",
        variablesReference: instance_ref,
        expensive: false,
      }
    end

    global_ref = @next_var_ref
    @next_var_ref += 1
    @variables_map[global_ref] = [nil, :global]

    scopes << {
      name: "Global",
      variablesReference: global_ref,
      expensive: true,
    }

    send_response(req, body: { scopes: scopes })
  end

  def handle_variables(req)
    ref = req.dig("arguments", "variablesReference")
    binding, scope_type = @variables_map[ref]

    vars = case scope_type
      when :local
        binding.local_variables.map do |name|
          build_var(name, binding.local_variable_get(name))
        end
      when :instance
        self_obj = binding.eval("self")
        self_obj.instance_variables.map do |name|
          build_var(name, self_obj.instance_variable_get(name))
        end
      when :global
        global_variables.map do |name|
          build_var(name, eval(name.to_s))
        end
      else
        []
      end

    send_response(req, body: { variables: vars })
  end

  def build_var(name, value)
    {
      name: name.to_s,
      value: value.inspect,
      type: value.class.to_s,
      variablesReference: 0, # позже можно расширить
    }
  rescue => e
    {
      name: name.to_s,
      value: "[error: #{e.message}]",
      type: "Error",
      variablesReference: 0,
    }
  end

  def handle_stop(reason, file, line)
    send_stopped_event(reason: reason, thread_id: Thread.current.object_id)
    @wait_for_continue = true
    sleep 0.05 while @wait_for_continue
    @stopped = false
  end

  def send_stopped_event(reason:, thread_id:)
    send_event("stopped", {
      reason: reason,
      threadId: thread_id,
      allThreadsStopped: true,
    })
  end
end

# === Запуск ===

debug_host = ENV["DEBUG_HOST"] || "127.0.0.1"
debug_port = (ENV["DEBUG_PORT"] || 9000).to_i

Thread.new do
  DAPServer.new(debug_host, debug_port).start
end
