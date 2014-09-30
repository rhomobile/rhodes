module RhoDevelopment

  class OneTimeUpdater
    def run
      unless Configuration::has_subscribers?
        puts 'Subscribers not found'.warning
        return
      end

      WebServer::ensure_running


      devDir = File.join(Configuration::applicationRoot, '.development')
      updatedListFilename = File.join(Configuration::applicationRoot, 'upgrade_package_add_files.txt')
      removedListFilename = File.join(Configuration::applicationRoot, 'upgrade_package_remove_files.txt')
      mkdir_p devDir
      Configuration::subscriber_platforms.each { |each|
        RhoDevelopment.setup(devDir, each)
        #is_require_full_update = RhoDevelopment.is_require_full_update
        changed = RhoDevelopment.check_changes_from_last_build(updatedListFilename, removedListFilename)
        if changed
          puts "Source code for platform #{each} was changed".primary
          WebServer.dispatch_task(PlatformPartialUpdateBuildingTask.new(each));
          Configuration::subscribers_by_platform(each).each { |subscriber|
            WebServer.dispatch_task(SubscriberPartialUpdateNotifyingTask.new(subscriber));
          }
        else
          puts "Source code for platform #{each} wasn't changed".primary
        end
      }
    end


  end

end