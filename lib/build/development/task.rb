module RhoDevelopment

  class Task

    def self.taskName
      self.name
    end

    def self.descendants
      ObjectSpace.each_object(Class).select { |each| each < self }
    end

    def self.fromHash(aHash)
      self.new
    end

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
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName})
    end

  end

  class BuildPartialBundleForAllPlatformsTask < Task

    def self.taskName
      'BuildPartialBundleForAllSubscribers'
    end

    def action
      server = BuildServer.new
      server.build_partial_bundles_for_all_subscribers
    end

  end

  class BuildFullBundleUpdateForSubscriberTask < Task

    def self.taskName
      'BuildFullBundleUpdateForSubscriber'
    end

    def self.fromHash(aHash)
      subscriber = Configuration::subscriber_by_ip(aHash['ip'])
      self.new(subscriber)
    end

    def initialize(aSubscriber)
      @subscriber = aSubscriber
    end

    def action
      server = BuildServer.new
      server.build_full_bundle_for_subscriber(@subscriber)
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'ip' => @subscriber.ip})
    end

  end

  class BuildPlatformPartialUpdateTask < Task

    def self.taskName
      'BuildPlatformPartialUpdate'
    end

    def self.fromHash(aHash)
      self.new(aHash['platform'])
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
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'platform' => @platform})
    end
  end

  class NotifyAllSubscribersTask < Task

    def self.taskName
      'NotifyAllSubscribers'
    end

    def action
      Configuration::subscribers.each { |subscriber|
        subscriber.notify
      }
    end

  end

  class NotifySubscriberAboutPartialUpdateTask < Task

    def self.taskName
      'NotifySubscriberAboutPartialUpdate'
    end

    def self.fromHash(aHash)
      subscriber = Configuration::subscriber_by_ip(aHash['ip'])
      self.new(subscriber)
    end

    def initialize(aSubscriber)
      @subscriber = aSubscriber
    end

    def action
      @subscriber.notify
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'ip' => @subscriber.ip})
    end

  end



  class NotifySubscriberAboutFullBundleUpdateTask < Task

    def self.taskName
      'NotifySubscriberAboutFullBundleUpdate'
    end

    def self.fromHash(aHash)
      subscriber = Configuration::subscriber_by_ip(aHash['ip'])
      self.new(subscriber)
    end

    def initialize(aSubscriber)
      @subscriber = aSubscriber
    end

    def action
      @subscriber.notify
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'ip' => @subscriber.ip})
    end

  end

end