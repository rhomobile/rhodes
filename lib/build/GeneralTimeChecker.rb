require 'pathname'
require 'yaml'

#TODO check generate api
class GeneralTimeChecker

  @@app_path           = nil
  @@is_run_always      = false
  @@cached_time        = nil
  @@time_cache_path    = nil

  def find_latest_modified_date(path)
    #puts 'find_latest_modified_date.path=' + path.to_s

    latest_mod_time = nil
    templates_dir   = Dir.new(path)

    templates_dir.each { |item|
      if item == '.' || item == '..'
        next
      end

      full_path = File.join(path, item)
      mod_time  = nil

      if File.directory?(full_path)
        mod_time = find_latest_modified_date(full_path)

        if mod_time.nil?
          next
        end
      else
        mod_time = File.mtime(full_path)
      end

      if latest_mod_time.nil?
        latest_mod_time = mod_time
      else
        if latest_mod_time < mod_time
          latest_mod_time = mod_time
        end
      end
    }

    return latest_mod_time
  end

  def init(startdir, app_path, timename)
    @@app_path       = app_path
    @@time_cache_path  = File.join(@@app_path, "bin", "tmp", timename+".time")

    if File.exist? @@time_cache_path
      time_cache_file = File.new(@@time_cache_path)
      @@cached_time   = Time.parse(time_cache_file.gets)
      time_cache_file.close

      puts "cached_time : #{@@cached_time}"

    else
      @@is_run_always = true
    end

  end

  def check(xmlpath)
    generate_xml = false

    xml_time = File.mtime(File.new(xmlpath))

    #puts "xmlpath: #{xmlpath}; xml_time : #{xml_time}"
    # for generate in first time
    if @@is_run_always
      @@do_cache = true
      generate_xml = true
    elsif @@cached_time < xml_time
      puts "!!!"
      generate_xml = true
    end

    generate_xml
  end

  def update()

    FileUtils.mkdir(File.join(@@app_path, "bin") ) unless File.exist? File.join(@@app_path, "bin")
    FileUtils.mkdir(File.join(@@app_path, "bin", "tmp") ) unless File.exist? File.join(@@app_path, "bin", "tmp")

    time_cache_file = File.new(@@time_cache_path, "w+")
    time_cache_file.puts Time.new
    time_cache_file.close()
  end
end
