require 'fileutils'
require 'pathname'
require 'listen'
require 'webrick'
require 'net/http'
require_relative 'ExtendedString'

require_relative 'development/configuration'
require_relative 'development/network'
require_relative 'development/device_finder'
require_relative 'development/subscriber'
require_relative 'development/build_server'
require_relative 'development/live_update_task'
require_relative 'development/web_server'
require_relative 'development/auto_updater'
require_relative 'development/platform'

module RhoDevelopment

$RhoDevelopmentSettingsFolder = nil
$RhoDevelopmentPlatform = nil


class RhofilelistItem

  attr_accessor :path, :file, :size, :datetime, :hash

  def self.fromString(aString)
    instance = self.new
    instance.initializeFromString(aString)
    instance
  end

  def initializeFromString(aString)
    elements = aString.split('|')
    @path = elements[0]
    @file = elements[1] == 'file'
    @size = elements[2].to_i
    @datetime = elements[3].to_i
    @hash = @file ? elements[4] : ''
  end

  def directory?
    (not self.file?)
  end

  def file?
    @file
  end

  def fixed_path
    fixed_path = @path.gsub('_erb.iseq', '.erb')
    fixed_path = fixed_path.gsub('.iseq', '.rb')
    return fixed_path
  end

  def ==(object)
    if self.class != object.class
      return false
    end

    if self.directory? and object.directory?
      return self.path == object.path
    end

    if self.file? and object.file?
      return self.hash == object.hash
    end

    return false

  end

  def inspect
    "#{@path}|#{self.file? ? 'file' : 'dir'}|#{self.size}|#{self.datetime}|#{self.file? ? self.hash : ''}"
  end

end


def setup(settings_folder_path, platform)

  puts 'RhoDevelopment.setup('+platform.to_s+')'

  $RhoDevelopmentSettingsFolder = settings_folder_path
  $RhoDevelopmentPlatform = platform

  $current_platform = platform

  Rake::Task['config:common'].reenable
  Rake::Task['config:common'].invoke

  #disable checking XCode in config:iphone - it required for working on Win platfrom or on MAc OS without XCode
  $skip_checking_XCode = true

  Rake::Task["config:#{$RhoDevelopmentPlatform}"].reenable
  Rake::Task["config:#{$RhoDevelopmentPlatform}"].invoke

end
module_function :setup

def get_settings_folder
  return $RhoDevelopmentSettingsFolder
end
module_function :get_settings_folder

def get_platform
  return $RhoDevelopmentPlatform
end
module_function :get_platform

def get_last_state_file_path
  return File.join($RhoDevelopmentSettingsFolder, "last_build_content_state.#{$RhoDevelopmentPlatform}.txt")
end
module_function :get_last_state_file_path

def make_full_bundle
  system("rake build:#{$RhoDevelopmentPlatform}:upgrade_package")

=begin
  s_skip_build_rhodes_main = $skip_build_rhodes_main
  s_skip_build_extensions = $skip_build_extensions
  s_skip_build_xmls = $skip_build_xmls
  s_use_prebuild_data = $use_prebuild_data
  $skip_build_rhodes_main = true
  $skip_build_extensions = true
  $skip_build_xmls = true
  $use_prebuild_data = true

  #Rake::Task['config:common'].reenable
  #Rake::Task['config:#{$RhoDevelopmentPlatform}'].reenable
  Rake::Task["build:#{$RhoDevelopmentPlatform}:rhobundle"].reenable
  Rake::Task["build:bundle:noxruby"].reenable
  Rake::Task["build:#{$RhoDevelopmentPlatform}:upgrade_package"].reenable

  Rake::Task["build:#{$RhoDevelopmentPlatform}:upgrade_package"].invoke


  $skip_build_rhodes_main = s_skip_build_rhodes_main
  $skip_build_extensions = s_skip_build_extensions
  $skip_build_xmls = s_skip_build_xmls
  $use_prebuild_data = s_use_prebuild_data
=end
end
module_function :make_full_bundle

def make_partial_bundle
  system("rake build:#{$RhoDevelopmentPlatform}:upgrade_package_partial")
=begin
  s_skip_build_rhodes_main = $skip_build_rhodes_main
  s_skip_build_extensions = $skip_build_extensions
  s_skip_build_xmls = $skip_build_xmls
  s_use_prebuild_data = $use_prebuild_data
  $skip_build_rhodes_main = true
  $skip_build_extensions = true
  $skip_build_xmls = true
  $use_prebuild_data = true

  Rake::Task["build:#{$RhoDevelopmentPlatform}:rhobundle"].reenable
  Rake::Task["build:bundle:noxruby"].reenable
  Rake::Task["build:#{$RhoDevelopmentPlatform}:upgrade_package_partial"].reenable

  Rake::Task["build:#{$RhoDevelopmentPlatform}:upgrade_package_partial"].invoke

  $skip_build_rhodes_main = s_skip_build_rhodes_main
  $skip_build_extensions = s_skip_build_extensions
  $skip_build_xmls = s_skip_build_xmls
  $use_prebuild_data = s_use_prebuild_data
=end
end
module_function :make_partial_bundle

#file,dir,nil
def extract_item_type(item)
  if item == nil
     return nil
  end
  item_tokenized = item.split('|')
  if item_tokenized.size >= 2
    return item_tokenized[1]
  end
  return nil
end
module_function :extract_item_type

def extract_item_path(item)
  if item == nil
     return nil
  end
  item_tokenized = item.split('|')
  if item_tokenized.size >= 1
    return item_tokenized[0]
  end
  return nil
end
module_function :extract_item_path

  def extract_changes(old_items, new_items, added_files, removed_files)

    new_items.each do |new_item|

      if new_item.path == "app/sdfssdf.css"
        puts 'new'
      end

      if (new_item.path != 'rhoconfig.txt') &&
          (new_item.path != 'rhoconfig.txt.timestamp') &&
          (new_item.path.index('public/api') != 0) &&
          (new_item.path != 'app_manifest.txt')

        if !(old_items.any? { |each| each == new_item })
          added_files << new_item.fixed_path
        end
      end
    end

    old_items.each do |old_item|
      if !(new_items.any? { |each| each.path == old_item.path })
        removed_files << old_item.fixed_path
      end
    end

    return ((not added_files.empty?) or (not removed_files.empty?))

  end

  module_function :extract_changes

def extract_state_file_from_bundle_zip(zip_path, path_for_save_state_file)

  currentdir = Dir.pwd()

  tmp_folder = File.join($tmpdir, 'development', 'tmp')
  mkdir_p tmp_folder

  chdir tmp_folder

  Jake.unzip(zip_path, tmp_folder)

  state_path = File.join(tmp_folder, 'RhoBundle','apps', 'rhofilelist.txt')

  rm_rf path_for_save_state_file if File.exist? path_for_save_state_file

  cp state_path, path_for_save_state_file

  rm_rf tmp_folder

  Dir.chdir currentdir
end
module_function :extract_state_file_from_bundle_zip

def get_full_bundle_zip_path
  return File.join($targetdir, "upgrade_bundle.zip")
end
module_function :get_full_bundle_zip_path

def get_partial_bundle_zip_path
  return File.join($targetdir, "upgrade_bundle_partial.zip")
end
module_function :get_partial_bundle_zip_path

def check_changes_from_last_build(add_file_path, remove_file_path)

  is_real_changes = false

  currentdir = Dir.pwd()

  make_full_bundle

  #extract file with hashes
  tmp_folder = File.join($tmpdir, 'development')
  rm_rf tmp_folder
  mkdir_p tmp_folder

  current_state_file = File.join(tmp_folder, 'current_state_file.txt')

  full_bundle_path = get_full_bundle_zip_path

  extract_state_file_from_bundle_zip(full_bundle_path, current_state_file)

  old_state_file = get_last_state_file_path

  add_array = []
  remove_array = []

  old_array = []
  current_array = []


  if !(File.exist? get_last_state_file_path)
    is_real_changes = nil
  end

  if is_real_changes == nil
    rm_rf get_last_state_file_path if File.exist? get_last_state_file_path
    cp current_state_file, get_last_state_file_path
    return is_real_changes
  end

  File.open(old_state_file, "r") do |f|
     while line = f.gets
        #old_array << line
        old_array << RhofilelistItem.fromString(line)
     end
  end

  File.open(current_state_file, "r") do |f|
     while line = f.gets
        #current_array << line
        current_array << RhofilelistItem.fromString(line)
     end
  end


  #cp get_last_state_file_path, '/Users/MOHUS/ReloadBundleDemo/old_state.txt'
  #cp current_state_file, '/Users/MOHUS/ReloadBundleDemo/new_state.txt'

  rm_rf get_last_state_file_path if File.exist? get_last_state_file_path
  cp current_state_file, get_last_state_file_path


  #puts 'AAAAA old_state = '+old_array.to_s
  #puts 'BBBBB new_state = '+current_array.to_s

  is_real_changes = extract_changes(old_array, current_array, add_array, remove_array)

  if is_real_changes
     # save add and remove


     rm_rf add_file_path if File.exist? add_file_path
     rm_rf remove_file_path if File.exist? remove_file_path

     File.open(add_file_path, 'w') { |file|
        add_array.each { |each| file.puts(each) }
     }
     File.open(remove_file_path, 'w') { |file|
        remove_array.each { |each| file.puts(each) }
     }

  end

  rm_rf tmp_folder

  Dir.chdir currentdir

  return is_real_changes
end
module_function :check_changes_from_last_build

def is_require_full_update
  return !(File.exist? get_last_state_file_path)
end
module_function :is_require_full_update

end










