require_relative 'configuration'
require_relative 'network'
require_relative 'subscriber'

module RhoDevelopment

  class DeviceFinder
    def run
      addresses = Network::available_addresses
      if (addresses.empty?)
        puts 'Network interfaces were not found.'.info
        return
      end
      if addresses.length != 1
        puts
        puts 'There are several network interfaces with following masks: '.primary
        addresses.each { |each|
          _mask = each.split('.')[0, 3].join('.')
          puts "#{addresses.index(each) + 1}. #{_mask}.*" }
        puts
        puts 'Please choose one of them: '
        input = STDIN.gets.strip.to_i
        selected_address = addresses[input - 1]
      else
        selected_address = addresses.last
      end
      mask = selected_address.split('.')[0, 3].join('.')
      self.discovery(mask)
    end

    def discovery(aString)
      puts "Network mask #{aString}.* will be used".primary
      print 'Discovering...'
      subscribers = self.parallelDiscovery(aString)
      if subscribers.empty?
        puts 'no devices found'.warning
        exit 1
      else
        puts 'done'.success
        subscribers.each { |each|
          puts each.to_s.info }
        print 'Storing subscribers...'.primary
        Configuration::store_subscribers(subscribers)
        puts 'done'.success
      end
    end

    def parallelDiscovery(aString)
      subscribers = []
      threads = []
      1.upto(254) { |each|
        threads << Thread.new {
          url = URI("http://#{aString}.#{each}:37579/development/get_info")
          begin
            http = Net::HTTP.new(url.host, url.port)
            http.open_timeout = 5
            response = http.get(url.path)
            data = JSON.parse(response.body)
            subscriber = {}
            subscriber['uri'] = "#{data['ip']}:#{data['port']}"
            subscriber['name'] = data['deviceFriendlyName']
            subscriber['platform'] = data['platform']
            subscriber['application'] = data['applicationName']
            subscribers << subscriber
          rescue *Configuration::handledNetworkExceptions => e
            #TODO may be it is necessary to remove subscriber from list?
            #puts "#{url} is not accessible. error: #{e.class}".info
          end
        }
      }
      threads.each { |thread| thread.join }
      return subscribers
    end

  end

end
