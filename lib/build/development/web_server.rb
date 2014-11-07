module RhoDevelopment


#TODO: create DocumentRoot on start and remove it on stop
  class WebServer

    @web_server
    @tasks

# class instance methods

    def self.ensure_running
      unless self.alive?
        case RbConfig::CONFIG['host_os']
          when /mswin|mingw32|windows/i
            cmd = "start \"development webserver\" /d \"#{Configuration::application_root}\" rake dev:webserver:privateStart"
          when /darwin/i
            cmd = "osascript -e 'tell app \"Terminal\" \n do script \" cd #{Configuration::application_root}; rake dev:webserver:privateStart\" \n end tell'"
        end
        system cmd
        until self.alive? do
          sleep(1)
        end
      end
    end

    def self.alive?
      url = Configuration::webserver_alive_request
      begin
        http = Net::HTTP.new(url.host, url.port)
        http.open_timeout = 5
        response = http.get(url.path)
        return response.code == '200'
      rescue *Configuration::handledNetworkExceptions => e
        return false
      end
    end

    def self.stop
      url = Configuration::shut_down_webserver_request
      begin
        http = Net::HTTP.new(url.host, url.port)
        http.open_timeout = 5
        response = http.get(url.path)
        if response.code == 200
          puts 'Web server was shut down'.primary
        else
          puts "#{response.body}".warning
        end
      rescue *Configuration::handledNetworkExceptions => e
        puts 'Web server is not answering'.warning
      end
    end

    def self.dispatch_task(aTask)
      aTask.dispatchToUrl(Configuration::webserver_uri)
    end

# instance methods

    def initialize
      document_root = Configuration::document_root

      puts "Webserver URL: #{Configuration::webserver_ip}:#{Configuration::webserver_port}".primary
      puts "Webserver document root: #{document_root}".primary
      print 'Cleaning document root directory... '.primary
      FileUtils.rm_rf("#{document_root}/.", secure: true)
      puts 'done'.success
      @tasks = Queue.new
      @web_server = WEBrick::HTTPServer.new(
          :BindAddress => Configuration::webserver_ip,
          :Port => Configuration::webserver_port,
          :DocumentRoot => document_root,
          :ServerType => WEBrick::SimpleServer
      )
      self.configure
    end

    def configure
      @web_server.mount('/alive', Alive)
      @web_server.mount('/tasks/new', NewTask, self)
      @web_server.mount('/shutdown', Shutdown)
      @web_server.mount('/response_from_device', ResponseFromDevice, self)
      @web_server.mount('/responce_from_device', ResponseFromDevice, self)
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
      task_name = request.query['taskName']
      task = LiveUpdateTask.descendants.detect { |each| each.taskName == task_name }
      if task != nil
        @instance.add_task(task.fromHash(request.query))
        response.status = 200
        response.body = "Task #{task_name} was added"
      else
        puts request.query.to_s.warning
        raise "Task #{task_name} not found".warning
      end
    end

  end

  class ResponseFromDevice < WEBrick::HTTPServlet::AbstractServlet
    def initialize server, instance
      super server
      @instance = instance
    end

    def do_POST request, response
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