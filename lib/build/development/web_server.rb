require 'webrick'
require 'net/http'
require_relative '../ExtendedString'
require_relative 'configuration'
require_relative 'task'

module RhoDevelopment


#TODO: create DocumentRoot on start and remove it on stop
  class WebServer

    @webserver
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
        http.start() { |http|
          http.get(url.path)
        }
      rescue Errno::ECONNREFUSED,
          Net::OpenTimeout => e
        result = false
      end
      result
    end

    def self.stop
      url = Configuration::shut_down_webserver_request
      http = Net::HTTP.new(url.host, url.port)
      http.open_timeout = 5
      begin
        http.start() { |http|
          response = http.get(url.path)
          if response.code == 200
            puts 'Web server was shut down'.primary
          else
            puts "#{response.body}".warning
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
      super
      host = Configuration::own_ip_address
      port = Configuration::webserver_port
      documentRoot = Configuration::document_root
      puts "Path '#{documentRoot}' will be used as web server document root".primary
      print 'Cleaning document root directory... '.primary
      FileUtils.rm_rf("#{documentRoot}/.", secure: true)
      puts 'done'.success
      @tasks = Queue.new
      @webserver = WEBrick::HTTPServer.new(
          :Port => port,
          :DocumentRoot => documentRoot,
          :ServerType => WEBrick::SimpleServer,
          :BindAddress => host
      )
      self.configure
    end

    def configure
      @webserver.mount('/alive', Alive)
      @webserver.mount('/tasks/new', NewTask, self)
      @webserver.mount('/shutdown', Shutdown)
      @webserver.mount('/response_from_device', ResponseFromDevice, self)
    end

    def start
      @runThread = Thread.new do
        loop do
          unless @tasks.empty?
            task = @tasks.pop
            task.execute
          end
          sleep 1
        end
      end
      @webserver.start
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
      taskName = request.query['name']
      case taskName
        when BuildPartialBundleForAllPlatformsTask.symbol
          task = BuildPartialBundleForAllPlatformsTask.new

        when NotifyAllSubscribersTask.symbol
          task = NotifyAllSubscribersTask.new

        when BuildPlatformPartialUpdateTask.symbol
          task = BuildPlatformPartialUpdateTask.new(request.query['platform'])

        when NotifySubscriberAboutPartialUpdateTask.symbol
          subscriber = Configuration::subscriber_by_ip(request.query['ip'])
          task = NotifySubscriberAboutPartialUpdateTask.new(subscriber)

        else
          puts request.query.to_s.warning
          raise "Task #{taskName} not found".warning
      end
      @instance.add_task(task)
      response.status = 200
      response.body = taskName
    end
  end

  class ResponseFromDevice < WEBrick::HTTPServlet::AbstractServlet
    def initialize server, instance
      super server
      @instance = instance
    end

    def do_POST request, response
      puts request.query.to_s.primary
=begin
      if request.query['status'] == 'need_full_update'
        subscriber = LiveUpdatingConfig::subscriber_by_ip(request.query["ip"])
        puts "#{subscriber} is requesting full update bundle".info
        (BuildServer.new).build_full_bundle_for_subscriber(subscriber)
      end
      if request.query['status'] == 'ok'
        subscriber = LiveUpdatingConfig::subscriber_by_ip(request.query["ip"])
        puts "#{subscriber} applied update bundle successfully".info
      end
      if request.query['status'] == 'error'
        subscriber = LiveUpdatingConfig::subscriber_by_ip(request.query["ip"])
        puts "#{subscriber} got an error while updating bundle: #{request.query["status"].message}".info
      end
=end
      response.status = 200
      response.body = ''
    end
  end


end