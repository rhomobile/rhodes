module RhoDevelopment

  class Task

    def execute
      puts "Executing #{self.class.name} at #{Time::now}".primary
      begin
        self.action
      rescue => e
        puts e.inspect
        puts e.backtrace
      end
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'name' => self.class.symbol})
    end

  end

  class BuildTask < Task

    def copy_platform_bundle_to_webserver_root(platform, source)
      to = File.join(Configuration::document_root, 'download', platform, Configuration::partial_bundle_name)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(source, to)
      puts "Bundle #{source} for platform #{platform} was copied to #{to}".primary
    end

  end

  class BuildPartialBundleForAllPlatformsTask < BuildTask

    def self.symbol
      'BuildPartialBundleForAllSubscribers'
    end

    def action
      puts 'Start building partial bundles for all subscribers'.primary
      Configuration::subscriber_platforms.each { |each|
        puts "Start building partial bundle for #{each}".primary
        RhoDevelopment.setup(File.join(Configuration::applicationRoot, '.development'), each)
        RhoDevelopment.make_partial_bundle
        puts "Partial bundle for #{each} was built".primary
        self.copy_platform_bundle_to_webserver_root(each.downcase, File.join($targetdir, 'upgrade_bundle_partial.zip'))
      }
      puts 'Partial bundles for all subscribers were built'.primary
    end

  end

  class BuildPlatformPartialUpdateTask < BuildTask

    def self.symbol
      'BuildPlatformPartialUpdate'
    end

    def initialize(aPlatform)
      @platform = aPlatform
    end

    def action
      server = BuildServer.new
      server.build_partial_bundle_for_platform(@platform)
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'name' => self.class.symbol, 'platform' => @platform})
    end
  end

  class NotifyAllSubscribersTask < Task

    def self.symbol
      'NotifyAllSubscribers'
    end

    def action
      Configuration::subscribers.each { |subscriber|
        subscriber.notify
      }
    end

  end

  class NotifySubscriberAboutPartialUpdateTask < Task

    def self.symbol
      'NotifySubscriberAboutPartialUpdate'
    end

    def initialize(aSubscriber)
      @subscriber = aSubscriber
    end

    def action
      @subscriber.notify
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'name' => self.class.symbol, 'ip' => @subscriber.ip})
    end

  end

end