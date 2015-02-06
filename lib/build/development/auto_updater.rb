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

    def add_directory(string)
      listener = Listen.to(string, debug: true) do |modified, added, removed|
        self.on_file_changed(added, modified, removed)
      end

      @listeners << listener
    end

    def on_file_changed(added_files, changed_files, removed_files)
      puts 'Files changed...'
      puts "File added: #{added_files}"
      puts "File changed: #{changed_files}"
      puts "File removed: #{removed_files}"
      begin
        self.create_diff_files(added_files, changed_files, removed_files)
        WebServer::dispatch_task(AllPlatformsPartialBundleBuildingTask.new)
        WebServer::dispatch_task(AllSubscribersPartialUpdateNotifyingTask.new)
      rescue => e
        puts 'Exception...'.warning
        puts e.inspect.warning
        puts e.backtrace.to_s.warning
      end
    end

    def create_diff_files(added_files, changed_files, removed_files)
      self.write_list_of_updated_files(added_files, changed_files)
      self.write_lis_of_removed_files(removed_files)
    end

    def write_list_of_updated_files(added_files, changed_files)
      self.write_array_to_file('upgrade_package_add_files.txt', added_files + changed_files)
    end

    def write_lis_of_removed_files(removed_files)
      self.write_array_to_file('upgrade_package_remove_files.txt', removed_files)
    end

    def write_array_to_file(filename, array)
      path = File.join(Configuration::application_root, filename)
      File.open(path, 'w') { |file|
        array.each { |each| file.puts(self.relative_path(each)) }
      }
    end

    def relative_path(string)
      first = Pathname Configuration::application_root
      second = Pathname string
      second.relative_path_from first
    end

  end

end