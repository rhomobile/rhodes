module RhoDevelopment

  # Abstract class for all live update tasks
  class LiveUpdateTask

    # Returns unique task name
    # @return [String] Unique task name
    def self.taskName
      self.name
    end

    # Returns LiveUpdateTask subclasses
    # @return [Array] array of classes
    def self.descendants
      ObjectSpace.each_object(Class).select { |each| each < self }
    end

    # Creates an object, parametrises and returns it
    # @param aHash [Hash] values for new instance parameterization
    # @return [Object] object
    def self.fromHash(aHash)
      self.new
    end

    # Execute specific task action
    def execute
      puts "Executing #{self.class.taskName} at #{Time::now}".primary
      begin
        self.action
      rescue => e
        puts "Executing #{self.class.taskName} failed".warning
        puts e.inspect.to_s.info
        puts e.backtrace.to_s.info
      end
    end

    # Method serializes itself to a hash and sends post request with the hash to specified URI
    # @param anUri [URI] URI for post request
    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName})
    end

  end

  class AllPlatformsPartialBundleBuildingTask < LiveUpdateTask

    def self.taskName
      'BuildPartialBundleForAllSubscribers'
    end

    def action
      server = BuildServer.new
      server.build_partial_bundles_for_all_subscribers
    end

  end

  class AllPlatformsFullBundleBuildingTask < LiveUpdateTask

    def self.taskName
      'BuildFullBundleForAllSubscribers'
    end

    def action
      server = BuildServer.new
      server.build_full_bundles_for_all_subscribers
    end

  end

  class SubscriberFullBundleUpdateBuildingTask < LiveUpdateTask

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

  # Class builds partial update bundle for specified platform
  class PlatformPartialUpdateBuildingTask < LiveUpdateTask

    # Returns unique task name
    # @return [String] Unique task name
    def self.taskName
      'BuildPlatformPartialUpdate'
    end

    # Creates instance of class and parametrises it with values from passed argument
    # @param aHash [Hash] It needs to contain key named as 'platform' with platform name.
    # @return [Object] instance of PlatformPartialUpdateBuildingTask
    def self.fromHash(aHash)
      self.new(aHash['platform'])
    end

    # Initialize instance with platform name
    # @param aPlatform [String] platform name.
    def initialize(aPlatform)
      @platform = aPlatform
    end

    # Builds partial update bundle
    def action
      server = BuildServer.new
      server.build_partial_bundle_for_platform(@platform)
    end

    # Method serializes itself to a hash and sends post request with the hash to specified URI
    # @param anUri [URI] URI for post request
    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'platform' => @platform})
    end
  end

  class AllSubscribersPartialUpdateNotifyingTask < LiveUpdateTask

    def self.taskName
      'NotifyAllSubscribersAboutPartialUpdate'
    end

    def action
      Configuration::enabled_subscribers.each { |subscriber|
        subscriber.partial_notify
      }
    end

  end

  class AllSubscribersFullUpdateNotifyingTask < LiveUpdateTask

    def self.taskName
      'NotifyAllSubscribersAboutFullUpdate'
    end

    def action
      Configuration::enabled_subscribers.each { |subscriber|
        subscriber.full_notify
      }
    end

  end

  # Notifies specified subscriber about a new partial update
  class SubscriberPartialUpdateNotifyingTask < LiveUpdateTask

    # Returns unique task name
    # @return [String] Unique task name
    def self.taskName
      'NotifySubscriberAboutPartialUpdate'
    end

    # Creates instance of class and parametrises it with values from passed argument
    # @param aHash [Hash] It needs to contain key named as 'ip' with subscriber IP.
    # @return [Object] instance of SubscriberPartialUpdateNotifyingTask
    def self.fromHash(aHash)
      subscriber = Configuration::subscriber_by_ip(aHash['ip'])
      self.new(subscriber)
    end

    # Initialize instance with instance of Subscriber
    # @param aSubscriber [Object] a Subscriber.
    def initialize(aSubscriber)
      @subscriber = aSubscriber
    end

    # Notify the subscriber about partial update bundle
    def action
      @subscriber.partial_notify
    end

    # Method serializes itself to a hash and sends post request with the hash to specified URI
    # @param anUri [URI] URI for post request
    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'ip' => @subscriber.ip})
    end

  end

  class SubscriberFullUpdateNotifyingTask < LiveUpdateTask

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
      @subscriber.full_notify
    end

    def dispatchToUrl(anUri)
      uri = URI.join(anUri, 'tasks/new')
      Net::HTTP.post_form(uri, {'taskName' => self.class.taskName, 'ip' => @subscriber.ip})
    end

  end

  # Checks is source code for each subscriber platform changed
  # and starts building and notifying process for platform
  class PartialUpdateTask < LiveUpdateTask

    def self.taskName
      'PartialUpdateTask'
    end

    # Checks has source code changes for each platform
    def action
      development_directory = File.join(Configuration::application_root, '.development')
      updated_list_filename = File.join(Configuration::application_root, 'upgrade_package_add_files.txt')
      removed_list_filename = File.join(Configuration::application_root, 'upgrade_package_remove_files.txt')
      mkdir_p development_directory
      Configuration::enabled_subscriber_platforms.each { |each|
        RhoDevelopment.setup(development_directory, each)
        changed = RhoDevelopment.check_changes_from_last_build(updated_list_filename, removed_list_filename)
        if changed
          puts "Source code for platform #{each} was changed".primary
          WebServer.dispatch_task(PlatformPartialUpdateBuildingTask.new(each));
          Configuration::enabled_subscribers_by_platform(each).each { |subscriber|
            WebServer.dispatch_task(SubscriberPartialUpdateNotifyingTask.new(subscriber));
          }
        else
          puts "Source code for platform #{each} wasn't changed".primary
        end
      }
    end

  end


end