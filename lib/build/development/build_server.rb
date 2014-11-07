module RhoDevelopment

  class BuildServer

    def build_partial_bundles_for_all_subscribers
      Configuration::subscribers.each { |each|
        self.build_partial_bundle_for_platform(each.normalized_platform_name)
        self.copy_platform_bundle_to_web_server_root(each.normalized_platform_name, Configuration::partial_bundle_name)
      }
    end

    def build_partial_bundle_for_platform(aPlatform)
      RhoDevelopment.setup(File.join(Configuration::application_root, '.development'), aPlatform)
      RhoDevelopment.make_partial_bundle
      self.copy_platform_bundle_to_web_server_root(aPlatform, Configuration::partial_bundle_name)
    end

    def build_full_bundle_for_subscriber(aSubscriber)
      RhoDevelopment.setup(File.join(Configuration::application_root, '.development'), aSubscriber.normalized_platform_name)
      RhoDevelopment.make_full_bundle
      self.copy_platform_bundle_to_web_server_root(aSubscriber.normalized_platform_name, Configuration::full_bundle_name)
    end

    def build_full_bundles_for_all_subscribers
      Configuration::subscribers.each { |each|
        self.build_full_bundle_for_subscriber(each)
        self.copy_platform_bundle_to_web_server_root(each.normalized_platform_name, Configuration::full_bundle_name)
      }
    end

    def copy_platform_bundle_to_web_server_root(platform, filename)
      from = File.join($targetdir, filename)
      to = File.join(Configuration::document_root, 'download', platform, filename)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
    end

  end

end