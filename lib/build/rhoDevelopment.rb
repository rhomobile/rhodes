require 'fileutils'
require 'pathname'
require 'listen'
require 'webrick'
require 'net/http'
require_relative 'ExtendedString'

require_relative 'development/configuration'
require_relative 'development/device_finder'
require_relative 'development/subscriber'
require_relative 'development/build_server'
require_relative 'development/live_update_task'
require_relative 'development/web_server'
require_relative 'development/one_time_updater'
require_relative 'development/auto_updater'

module RhoDevelopment

$RhoDevelopmentSettingsFolder = nil
$RhoDevelopmentPlatform = nil


def setup(settings_folder_path, platform)
  $RhoDevelopmentSettingsFolder = settings_folder_path
  $RhoDevelopmentPlatform = platform
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


def extract_changes(old_file_array, new_file_array, add_file_array, remove_file_array)
  is_real_changes = false

  old_file_paths = []
  old_file_array.each do |item|
    old_file_paths << extract_item_path(item)
  end
  new_file_paths = []
  new_file_array.each do |item|
    new_file_paths << extract_item_path(item)
  end


  #extract new and changes
  new_file_array.each do |item|
    item_path = extract_item_path(item)
    item_fixed = item_path
    item_fixed = item_fixed.gsub('_erb.iseq', '.erb')
    item_fixed = item_fixed.gsub('.iseq', '.rb')

    if !(old_file_array.include?(item))
       #check for folder
       if extract_item_type(item) == 'dir'
          #check for this is new dir !
          if !(old_file_paths.include?(item_path))
             add_file_array << item_fixed
             is_real_changes = true
          end
       else
         if (item_path != 'rhoconfig.txt') &&
            (item_path != 'rhoconfig.txt.timestamp') &&
            (item_path.index('public/api') != 0) &&
            (item_path != 'app_manifest.txt')
           add_file_array << item_fixed
           is_real_changes = true
         end
       end
    end
  end

  #make remove list
  old_file_array.each do |item|
    item_path = extract_item_path(item)
    item_fixed = item_path
    item_fixed = item_fixed.gsub('_erb.iseq', '.erb')
    item_fixed = item_fixed.gsub('.iseq', '.rb')
    if !(new_file_paths.include?(item_path))
       remove_file_array << item_fixed
       is_real_changes = true
    end
  end

  return is_real_changes
end
module_function :extract_changes





def extract_state_file_from_bundle_zip(zip_path, path_for_save_state_file)

  currentdir = Dir.pwd()

  tmp_folder = File.join($tmpdir, 'development', 'tmp')
  mkdir_p tmp_folder

  chdir tmp_folder

  Jake.run('unzip', [zip_path, '-d', tmp_folder])

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
        old_array << line
     end
  end

  File.open(current_state_file, "r") do |f|
     while line = f.gets
        current_array << line
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