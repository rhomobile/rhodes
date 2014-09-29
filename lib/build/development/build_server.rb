module RhoDevelopment

  class BuildServer

    def build_partial_bundles_for_all_subscribers
      puts "Start building partial bundles for all subscribers".primary
      builtPlatforms = []
      Configuration::subscribers.each { |each|
        builtPlatforms << each.platform
        self.build_partial_bundle_for_subscriber(each)
        self.copy_platform_bundle_to_webserver_root(each.platform, "upgrade_bundle_partial.zip")
      }
      puts "Partial bundles for all subscribers were built".primary
    end

    def build_partial_bundle_for_subscriber(aSubscriber)
      puts "Start building partial bundle for #{aSubscriber.platform}".primary
      RhoDevelopment.setup(File.join(Configuration::applicationRoot, '.development'), aSubscriber.normalized_platform_name)
      RhoDevelopment.make_partial_bundle
      puts "Partial bundle for #{aSubscriber.platform} was built".primary
    end

    def build_partial_bundle_for_platform(aPlatform)
      puts "Start building partial bundle for #{aPlatform}".primary
      RhoDevelopment.setup(File.join(Configuration::applicationRoot, '.development'), aPlatform)
      RhoDevelopment.make_partial_bundle
      puts "Partial bundle for #{aPlatform} was built".primary
    end


    def build_full_bundle_for_subscriber(aSubscriber)
      puts "Start building full bundle for #{aSubscriber.platform}".primary
      RhoDevelopment.setup(File.join(Configuration::applicationRoot, '.development'), aSubscriber.normalized_platform_name)
      RhoDevelopment.make_full_bundle
      self.copy_platform_bundle_to_webserver_root(aSubscriber.platform, "upgrade_bundle.zip")
      puts "Full bundle for #{aSubscriber.platform} was built".primary
    end

    def build_full_bundle_for_all_subscribers
      Configuration::subscribers.each { |subscriber|
        self.build_full_bundle_for_subscriber(subscriber)
      }
    end

    def copy_platform_bundle_to_webserver_root(platform, filename)
      from = File.join($targetdir, filename)
      to = File.join(Configuration::document_root, 'download', platform, Configuration::download_bundle_name)
      FileUtils.mkpath(File.dirname(to))
      FileUtils.cp(from, to)
      puts "Bundle #{filename} for platform #{platform} was copied to web server document root".primary
    end

  end

end