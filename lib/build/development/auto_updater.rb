module RhoDevelopment

  class AutoUpdater

    def initialize
      @listeners = []
    end

    def run
      @listeners.each { |each|
        each.start }
      WebServer::set_auto_update_pid(Process.pid)
      puts 'Live update auto process started'.primary
      begin
        sleep 1
      end while self.has_active_listeners
    end

    def has_active_listeners
      @listeners.any? { |each| each.processing? }
    end

    def add_directory(string)
      listener = Listen.to(string, {:debug => true}) { | modified, added, removed |
        puts 'Files were changed...'.info
        puts "Files were added: #{added}".info
        puts "Files were modified: #{modified}".info
        puts "Files were removed: #{removed}".info
      exclude_items = (Jake.getBuildProp2('rhobundle', 'exclude_items') || []).collect { |each| %r{#{each}} }
      added_files = added.reject { |each| (exclude_items.any? { |each_item| each.index(each_item) }) }
      modified_files = modified.reject { |each| (exclude_items.any? { |each_item| each.index(each_item) }) }
      removed_files = removed.reject { |each| (exclude_items.any? { |each_item| each.index(each_item) }) }

      if ((added_files + modified_files +removed_files).empty?)
        puts 'Changes in file system were skipped because they were rejected by rules described at build.yml > rhobundle > exclude_items'.warning
      else
        self.on_file_changed(added_files, modified_files, removed_files)
      end
      }
      @listeners << listener
    end

  def on_file_changed(added_files, changed_files, removed_files)
    puts 'The following files will be included in update bundle:'.info
    puts "As added: #{added_files}".info
    puts "As modified: #{changed_files}".info
    puts "As removed: #{removed_files}".info
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