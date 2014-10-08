module RhoDevelopment

  class AutoUpdater

    def initialize
      @listeners = []
    end

    def run
      @listeners.each { |each| each.start }

      begin
        sleep 1
      end while self.has_active_listeners
    end

    def has_active_listeners
      @listeners.any? { |each| each.processing? }
    end

    def add_directory(aString)
      listener = Listen.to(aString, debug: true) do |modified, added, removed|
        self.on_file_changed(added, modified, removed)
      end

      @listeners << listener
    end

    def on_file_changed(addedFiles, changedFiles, removedFiles)
      puts 'Files changed...'
      puts "File added: #{addedFiles}"
      puts "File changed: #{changedFiles}"
      puts "File removed: #{removedFiles}"
      begin
        self.create_diff_files(addedFiles, changedFiles, removedFiles)
        WebServer::dispatch_task(AllPlatformsPartialBundleBuildingTask.new)
        WebServer::dispatch_task(AllSubscribersPartialUpdateNotifyingTask.new)
      rescue => e
        puts 'Exception...'.warning
        puts e.inspect.warning
        puts e.backtrace.to_s.warning
      end
    end

    def create_diff_files(addedFiles, changedFiles, removedFiles)
      self.write_list_of_updated_files(addedFiles, changedFiles)
      self.write_lis_of_removed_files(removedFiles)
    end

    def write_list_of_updated_files(addedFiles, changedFiles)
      self.write_array_to_file('upgrade_package_add_files.txt', addedFiles + changedFiles)
    end

    def write_lis_of_removed_files(removedFiles)
      self.write_array_to_file('upgrade_package_remove_files.txt', removedFiles)
    end

    def write_array_to_file(filename, anArray)
      path = File.join(Configuration::applicationRoot, filename)
      File.open(path, 'w') { |file|
        anArray.each { |each| file.puts(self.relative_path(each)) }
      }
    end

    def relative_path(aString)
      first = Pathname Configuration::applicationRoot
      second = Pathname aString
      second.relative_path_from first
    end

  end

end