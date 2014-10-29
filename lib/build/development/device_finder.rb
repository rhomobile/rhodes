require_relative 'configuration'
require_relative 'subscriber'

module RhoDevelopment
  class DeviceFinder
    def run

      adresses = Configuration::own_ip_addresses
      if adresses.length != 0
        puts
        puts 'There are several network interfaces with following IP addresses: '.primary
        adresses.each { |each| puts "#{adresses.index(each) + 1}. #{each}" }
        puts
        puts 'Please choose one of them: '
        input = STDIN.gets.strip.to_i
        choosenAddress = adresses[input - 1]
      else
        choosenAddress = adresses.last
      end
      mask = choosenAddress.split('.')[0, 3].join('.')

      print "Discovering #{mask}.(1..254) ... ".primary
      subscribers = self.parallelDiscovery(mask)
      if subscribers.empty?
        puts 'no devices found'.warning
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
            puts "#{url} answered: #{response.body}".info
            data = JSON.parse(response.body)
            subscriber = {}
            subscriber['uri'] = "#{data['ip']}:#{data['port']}"
            subscriber['name'] = data['deviceFriendlyName']
            subscriber['platform'] = data['platform']
            subscriber['application'] = data['applicationName']
            subscribers << subscriber
          rescue Errno::ECONNREFUSED, Errno::EHOSTDOWN, Errno::EHOSTUNREACH, Net::OpenTimeout => e
            #rescue  => e
            #TODO may be it is necessary remove subscriber from list?
            #puts "#{url} is not accessible. error: #{e.class}".info
          end
        }
      }
      threads.each { |thread| thread.join }
      return subscribers
    end

  end

end