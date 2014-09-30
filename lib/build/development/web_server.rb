module RhoDevelopment


#TODO: create DocumentRoot on start and remove it on stop
  class WebServer

    @web_server
    @tasks

# class instance methods
#TODO: Use it from rake command
    def self.ensure_running
      unless self.alive?
        process = ChildProcess.build('rake', 'dev:webserver:privateStart')
        process.io.inherit!
        process.cwd = $app_basedir
        process.start
      end
    end

    def self.alive?
      result = true
      url = URI("http://#{Configuration::own_ip_address}:#{Configuration::webserver_port}/alive")
      http = Net::HTTP.new(url.host, url.port)
      http.open_timeout = 5
      begin
        http.start { |_http|
          _http.get(url.path)
        }
      rescue Errno::ECONNREFUSED,
          Net::OpenTimeout => e
        result = false
      end
      result
    end

    def self.stop
      _url = Configuration::shut_down_webserver_request
      _http = Net::HTTP.new(_url.host, _url.port)
      _http.open_timeout = 5
      begin
        _http.start() { |http|
          _response = http.get(_url.path)
          if _response.code == 200
            puts 'Web server was shut down'.primary
          else
            puts "#{_response.body}".warning
          end
        }
      rescue Errno::ECONNREFUSED => e
        puts 'Web server is not answering'.warning
      end
    end

    def self.dispatch_task(aTask)
      aTask.dispatchToUrl(Configuration::webserver_uri)
    end


# instance methods

    def initialize
      _host = Configuration::own_ip_address
      _port = Configuration::webserver_port
      _document_root = Configuration::document_root
      puts "Path '#{_document_root}' will be used as web server document root".primary
      print 'Cleaning document root directory... '.primary
      FileUtils.rm_rf("#{_document_root}/.", secure: true)
      puts 'done'.success
      @tasks = Queue.new
      @web_server = WEBrick::HTTPServer.new(
          :Port => _port,
          :DocumentRoot => _document_root,
          :ServerType => WEBrick::SimpleServer,
          :BindAddress => _host
      )
      self.configure
    end

    def configure
      @web_server.mount('/alive', Alive)
      @web_server.mount('/tasks/new', NewTask, self)
      @web_server.mount('/shutdown', Shutdown)
      @web_server.mount('/response_from_device', ResponseFromDevice, self)
    end

    def start
      @run_thread = Thread.new do
        loop do
          unless @tasks.empty?
            _task = @tasks.pop
            _task.execute
          end
          sleep 1
        end
      end
      @web_server.start
    end

    def add_task(aTask)
      @tasks << aTask
    end

  end

  class Alive < WEBrick::HTTPServlet::AbstractServlet
    def do_GET request, response
      response.status = 200
      response.body = 'Server is alive'
    end
  end

  class Shutdown < WEBrick::HTTPServlet::AbstractServlet
    def initialize server
      super server
      @server = server
    end

    def do_GET request, response
      response.status = 200
      response.body = 'Web server was shut down'
      @server.shutdown
    end
  end

  class NewTask < WEBrick::HTTPServlet::AbstractServlet
    def initialize server, instance
      super server
      @instance = instance
    end

    def do_POST request, response
      _task_name = request.query['taskName']
      _task = LiveUpdateTask.descendants.detect { |each| each.taskName == _task_name }
      if task != nil
        @instance.add_task(_task.fromHash(request.query))
        response.status = 200
        response.body = "Task #{_task_name} was added"
      else
        puts request.query.to_s.warning
        raise "Task #{_task_name} not found".warning
      end
    end

  end

  class ResponseFromDevice < WEBrick::HTTPServlet::AbstractServlet
    def initialize server, instance
      super server
      @instance = instance
    end

    def do_POST request, response
      puts request.query.to_s.primary
      subscriber = Configuration::subscriber_by_ip(request.query['ip'])
      if request.query['status'] == 'need_full_update'
        puts "#{subscriber} is requesting full update bundle".info
        WebServer::dispatch_task(SubscriberFullBundleUpdateBuildingTask.new(subscriber))
        WebServer::dispatch_task(SubscriberFullUpdateNotifyingTask.new(subscriber))
      end
      if request.query['status'] == 'ok'
        puts "#{subscriber} applied update bundle successfully".info
      end
      if request.query['status'] == 'error'
        puts "#{subscriber} got an error while updating bundle: #{request.query['message']}".info
      end

      response.status = 200
      response.body = ''
    end
  end


end