require 'socket'
require 'uri'
require 'net/http'
require 'json'
require 'yaml'
require_relative 'ExtendedString'


class OneTimeServer


  def run
    if !self.webServerWorking?
      self.startServer
    end

    if self.sourceChanged?
      self.notifySubscribers
    end

  end

  def webServerWorking?
    true
  end

  def sourceChanged?
    true
  end

  def notifySubscribers
    self.subscribers.each { |subscriber|
      self.notifySubscriber(subscriber)
    }
  end

  def notifySubscriber(aSubscriber)

  end

  def subscribers
    if (defined(@subscribers)).nil?
      self.loadConfig
    end

  end

  def loadConfig
    if File.exist?(self.configFilename)
      config = YAML.load_file(self.configFilename)
      config['devices'].each { |each|
        subscriber = Subscriber.new
        subscriber.ip = "#{each['ip']}"
        subscriber.port = "#{each['port']}"
        subscriber.platform = each['platform']
        subscriber.name = each['name']
        subscriber.application = each['application']
        @subscribers << subscriber
        puts "#{subscriber} added"
      }
    else
      puts "Devices configuration file #{configFilename} not found".warning
    end
  end

  def applicationRoot=(aString)
    @applicationRoot = aString
  end

  def configFilename
    File.join(@applicationRoot, 'dev-config.yml')
  end
end


class Subscriber

  def ip=(aString)
    @ip = aString
  end

  def ip
    @ip
  end

  def port=(aString)
    @port = aString
  end

  def port
    @port
  end

  def platform=(aString)
    @platform = aString
  end

  def platform
    @platform
  end

  def name=(aString)
    @name = aString
  end

  def name
    @name
  end

  def application=(aString)
    @application = aString
  end

  def application
    @application
  end

  def normalizedPlatformName
    if ['apple', 'iphone'].include?(@platform.downcase)
      return 'iphone'
    end
    @platform
  end

  def buildTask
    "build:#{self.normalizedPlatformName}:upgrade_package_partial"
  end

  def to_s
    "#{self.class}(uri=#{@uri}, name=#{@name}, platform=#{@platform}, app=#{@application})"
  end

  def to_hash
    hash = {}
    instance_variables.each { |var| hash[var.to_s.delete("@")] = instance_variable_get(var) }
    hash
  end

end

class Discovery
  def initialize(appDir)
    @applicationDirectory = appDir
  end

  def ownIP
    IPSocket.getaddress(Socket.gethostname)
  end

  def run
    subscribers = self.renameMe
    self.save(subscribers)
  end

  #TODO: Rename this method
  def renameMe
    puts "Start discovering".primary
    subscribers = []
    mask = self.ownIP.split('.')[0, 3].join('.')
    1.upto(254) { |each|
      url = URI("http://#{mask}.#{each}:37579/development/get_info")
      begin
        http = Net::HTTP.new(url.host, url.port)
        http.open_timeout = 0.1
        http.start() { |http|
          response = http.get(url.path)
          puts "#{url} answered: #{response.body}".info
          data = JSON.parse(response.body)
          subscriber = {}
          subscriber['uri'] = "#{data['ip']}:#{data['port']}"
          subscriber['name'] = data['deviceFriendlyName']
          subscriber['platform'] = data['platform']
          subscriber['application'] = data['applicationName']
          subscribers << subscriber
        }
      rescue Errno::ECONNREFUSED, Net::OpenTimeout => e
        #TODO may be it is necessary remove subscriber from list?
        puts "#{url} is not accessible. error: #{e.class}".info
      end
    }
    return subscribers
  end

  def save(anArray)
    if anArray.empty?
      puts 'No devices found'.warning
      return
    end
    filename = File.join(@applicationDirectory, 'dev-config.yml')
    if File.exist?(filename)
      config = YAML.load_file(filename)
    else
      config = {"devices" => []}
    end
    config['devices'] = anArray
    yml = config.to_yaml
    File.open(filename, 'w') { |file| file.write yml }

  end

end