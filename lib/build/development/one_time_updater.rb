module RhoDevelopment

  class OneTimeUpdater
    def run
      unless Configuration::has_subscribers?
        puts 'Subscribers not found'.warning
        return
      end

      development_directory = File.join(Configuration::application_root, '.development')
      updated_list_filename = File.join(Configuration::application_root, 'upgrade_package_add_files.txt')
      removed_list_filename = File.join(Configuration::application_root, 'upgrade_package_remove_files.txt')
      mkdir_p development_directory
      Configuration::subscriber_platforms.each { |each|
        RhoDevelopment.setup(development_directory, each)
        changed = RhoDevelopment.check_changes_from_last_build(updated_list_filename, removed_list_filename)
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